.global _start
_start:
    bl main      @ Call C main
halt:
    mov r0, #0  @ Optional: set r0
    b halt      @ Infinite halt loop, CPU stops here

