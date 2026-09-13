#include "HASHMAP_IMPL.h"

void initNode(struct Node* node, char* key, char* value)
{
	node->key = key;
	node->value = value;
	node->next = NULL;
};

void HASHMAP_initHashMap(struct HashMap* mp)
{
	mp->capacity = 100;
	mp->numOfElements = 0;
	mp->arr = (struct Node**)calloc(mp->capacity, sizeof(struct Node*));
}

int hashFunction(struct HashMap* mp, char* key)
{
	int bucketIndex;
	int sum = 0, factor = 31;
	for (int i = 0; i < strlen(key); i++)
	{
		sum = ((sum % mp->capacity) + (((int)key[i]) * factor) % mp->capacity) % mp->capacity;
		factor = ((factor % __INT16_MAX__) * (31 % __INT16_MAX__)) % __INT16_MAX__;
	}
	bucketIndex = sum;
	return bucketIndex;
}

void HASHMAP_insert(struct HashMap* mp, char* key, char* value)
{
	int bucketIndex = hashFunction(mp, key);
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	initNode(newNode, key, value);
	if (mp->arr[bucketIndex] == NULL)
	{
		mp->arr[bucketIndex] = newNode;
	}
	else
	{
		newNode->next = mp->arr[bucketIndex];
		mp->arr[bucketIndex] = newNode;
	}
}

void HASHMAP_delete(struct HashMap* mp, char* key)
{
	int bucketIndex = hashFunction(mp, key);
	struct Node* prevNode = NULL;
	struct Node* currNode = mp->arr[bucketIndex];
	while (currNode != NULL)
	{
		if (strcmp(key, currNode->key) == 0)
		{
			if (currNode == mp->arr[bucketIndex])
			{
				mp->arr[bucketIndex] = currNode->next;
			}
			else
			{
				prevNode->next = currNode->next;
			}
			free(currNode);
			break;
		}
		prevNode = currNode;
		currNode = currNode->next;
	}
	return;
}

char* HASHMAP_search(struct HashMap* mp, char* key)
{
	int bucketIndex = hashFunction(mp, key);
	struct Node* bucketHead = mp->arr[bucketIndex];
	while (bucketHead != NULL)
	{
		if (strcmp(bucketHead->key, key) == 0)
		{
			return bucketHead->value;
		}
		bucketHead = bucketHead->next;
	}
	return "Error";
}

struct HASHMAP_Entry* HASHMAP_getAllElements(struct HashMap* mp, int* outCount)
{
	struct HASHMAP_Entry* entries = (struct HASHMAP_Entry*)malloc(sizeof(struct HASHMAP_Entry) * mp->numOfElements);
	int idx = 0;
	for (int i = 0; i < mp->capacity; i++)
	{
		struct Node* curr = mp->arr[i];
		while (curr != NULL)
		{
			entries[idx].key = curr->key;
			entries[idx].value = curr->value;
			idx++;
			curr = curr->next;
		}
	}
	*outCount = idx;
	return entries;
}