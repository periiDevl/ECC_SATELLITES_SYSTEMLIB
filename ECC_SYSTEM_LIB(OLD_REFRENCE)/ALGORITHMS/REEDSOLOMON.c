#include "REEDSOLOMON.h"
#include <string.h>

static uint8_t gf_exp[512];
static uint8_t gf_log[256];

void rs_init(void) {
    int x = 1;
    for (int i = 0; i < 255; i++) {
        gf_exp[i] = x;
        gf_exp[i + 255] = x;
        gf_log[x] = i;
        x <<= 1;
        if (x & 0x100) x ^= 0x11d;
    }
}

static inline uint8_t gf_mul(uint8_t a, uint8_t b) {
    return (a && b) ? gf_exp[gf_log[a] + gf_log[b]] : 0;
}

static inline uint8_t gf_div(uint8_t a, uint8_t b) {
    return (a && b) ? gf_exp[gf_log[a] + 255 - gf_log[b]] : 0;
}

/*
 * Encode: systematic Reed-Solomon.
 *
 * Codeword layout (high-degree first): [data[0]..data[k-1] | parity[0]..parity[nsym-1]]
 *
 * Generator polynomial: g(x) = prod_{i=0}^{nsym-1} (x - alpha^i)
 * Parity = remainder of (data * x^nsym) / g(x)
 */
void rs_encode(void* data, size_t data_len, uint8_t* parity, size_t ecc_len) {
    uint8_t *msg = (uint8_t*)data;
    int k = (int)data_len;
    int nsym = (int)ecc_len;

    /* Build generator polynomial, coefficients stored low-to-high (gen[0] = const term) */
    uint8_t gen[256] = {0};
    gen[0] = 1;
    for (int i = 0; i < nsym; i++) {
        /* Multiply gen by (x + alpha^i) — subtraction = addition in GF(2) */
        for (int j = i + 1; j > 0; j--)
            gen[j] = gen[j-1] ^ gf_mul(gf_exp[i], gen[j]);
        gen[0] = gf_mul(gf_exp[i], gen[0]);
    }

    /* Polynomial long division: remainder goes into parity */
    memset(parity, 0, nsym);
    for (int i = 0; i < k; i++) {
        uint8_t feedback = msg[i] ^ parity[0];
        if (feedback != 0) {
            for (int j = 0; j < nsym - 1; j++)
                parity[j] = parity[j+1] ^ gf_mul(feedback, gen[nsym-1-j]);
            parity[nsym-1] = gf_mul(feedback, gen[0]);
        } else {
            memmove(parity, parity + 1, nsym - 1);
            parity[nsym-1] = 0;
        }
    }
}

/*
 * Decode: detect and correct errors in-place using:
 *   1. Syndrome computation (Horner's method)
 *   2. Berlekamp-Massey (error locator polynomial)
 *   3. Chien search (find error positions)
 *   4. Forney algorithm (compute error magnitudes)
 *
 * Returns:
 *   0  = no errors (data was clean)
 *   1  = errors found and corrected
 *  -1  = uncorrectable (too many errors)
 */
int rs_decode(void* data, size_t data_len, uint8_t* parity, size_t ecc_len) {
    uint8_t *msg = (uint8_t*)data;
    int k    = (int)data_len;
    int nsym = (int)ecc_len;
    int n    = k + nsym;

    if (n > 255) return -1; /* GF(2^8) limit */

    /* Full codeword: data first, parity at the end */
    uint8_t full[255];
    memcpy(full,     msg,    k);
    memcpy(full + k, parity, nsym);

    /* ── Step 1: Syndromes ──────────────────────────────────────────────────
     * S[i] = codeword(alpha^i), for i = 0..nsym-1.
     * The codeword polynomial is: full[0]*x^(n-1) + full[1]*x^(n-2) + ... + full[n-1]
     * Evaluated at alpha^i using Horner's method.
     */
    uint8_t S[64] = {0};
    int has_errors = 0;
    for (int i = 0; i < nsym; i++) {
        uint8_t xi = gf_exp[i], s = 0;
        for (int j = 0; j < n; j++) s = gf_mul(s, xi) ^ full[j];
        S[i] = s;
        if (s) has_errors = 1;
    }
    if (!has_errors) return 0;

    /* ── Step 2: Berlekamp-Massey ───────────────────────────────────────────
     * Find the error locator polynomial sigma(x) = 1 + sigma[1]*x + ... + sigma[L]*x^L
     * such that sigma(x) * S(x) ≡ 0  (mod x^nsym).
     * Coefficients stored low-to-high: sigma[0] = 1.
     */
    uint8_t sigma[64] = {1}, prev[64] = {1};
    int L = 0, steps = 1;
    uint8_t last_d = 1;

    for (int r = 0; r < nsym; r++) {
        /* Discrepancy delta = S[r] + sum_{i=1}^{L} sigma[i]*S[r-i] */
        uint8_t delta = S[r];
        for (int i = 1; i <= L; i++) delta ^= gf_mul(sigma[i], S[r-i]);

        if (!delta) { steps++; continue; }

        uint8_t tmp[64];
        memcpy(tmp, sigma, 64);
        uint8_t coef = gf_div(delta, last_d);
        for (int i = steps; i < 64; i++) sigma[i] ^= gf_mul(coef, prev[i - steps]);
        if (2 * L <= r) {
            L = r + 1 - L;
            memcpy(prev, tmp, 64);
            last_d = delta;
            steps = 1;
        } else {
            steps++;
        }
    }

    /* ── Step 3: Chien Search ───────────────────────────────────────────────
     * Find byte positions where sigma has a root.
     *
     * Codeword byte at index i corresponds to the coefficient of x^(n-1-i),
     * so its "locator" is X_i = alpha^(n-1-i).
     * An error at byte i means sigma(X_i^{-1}) = 0,
     * i.e., sigma(alpha^{-(n-1-i)}) = sigma(alpha^{i-(n-1)}) = 0.
     */
    int err_pos[64], err_count = 0;
    for (int i = 0; i < n; i++) {
        int power = (i - (n - 1) + 255 * 10) % 255; /* alpha^power = X_i^{-1} */
        uint8_t xi = gf_exp[power];

        /* Evaluate sigma at xi */
        uint8_t v = 0;
        for (int j = 0; j <= L; j++)
            v ^= gf_mul(sigma[j], gf_exp[(gf_log[xi] * j) % 255]);

        if (v == 0) {
            if (err_count >= L) return -1; /* More roots than degree — overflow */
            err_pos[err_count++] = i;
        }
    }
    if (err_count != L) return -1; /* Chien mismatch — uncorrectable */

    /* ── Step 4: Error Evaluator Polynomial (Omega) ─────────────────────────
     * omega(x) = S(x) * sigma(x)  mod x^nsym
     * Stored low-to-high.
     */
    uint8_t omega[64] = {0};
    for (int i = 0; i < nsym; i++)
        for (int j = 0; j <= L && j <= i; j++)
            omega[i] ^= gf_mul(S[i - j], sigma[j]);

    /* ── Step 5: Formal Derivative of sigma ────────────────────────────────
     * sigma'(x): in GF(2), derivative of x^k is k*x^(k-1).
     * Even-power terms vanish (their derivative coefficient is even = 0).
     * Only odd-indexed coefficients survive, shifted down by one degree.
     */
    uint8_t dsigma[64] = {0};
    for (int i = 1; i <= L; i += 2) dsigma[i - 1] = sigma[i];

    /* ── Step 6: Forney Algorithm ───────────────────────────────────────────
     * Error magnitude at byte i:
     *   e_i = X_i * omega(X_i^{-1}) / sigma'(X_i^{-1})
     * where X_i = alpha^(n-1-i).
     */
    for (int k2 = 0; k2 < err_count; k2++) {
        int idx = err_pos[k2];
        int pw_Xi     = (n - 1 - idx + 255 * 10) % 255; /* X_i     = alpha^pw_Xi */
        int pw_Xi_inv = (idx - (n - 1) + 255 * 10) % 255; /* X_i^-1 = alpha^pw_Xi_inv */
        uint8_t Xi     = gf_exp[pw_Xi];
        uint8_t Xi_inv = gf_exp[pw_Xi_inv];

        /* Evaluate omega and dsigma at Xi_inv using Horner (low-to-high) */
        uint8_t om = 0;
        for (int i = nsym - 1; i >= 0; i--) om = gf_mul(om, Xi_inv) ^ omega[i];
        uint8_t dv = 0;
        for (int i = L - 1; i >= 0; i--) dv = gf_mul(dv, Xi_inv) ^ dsigma[i];

        if (!dv) return -1; /* Derivative is zero — uncorrectable */

        uint8_t magnitude = gf_mul(Xi, gf_div(om, dv));
        full[idx] ^= magnitude;
    }

    /* Write corrected bytes back */
    memcpy(msg,    full,     k);
    memcpy(parity, full + k, nsym);
    return 1;
}