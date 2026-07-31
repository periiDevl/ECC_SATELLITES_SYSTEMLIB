#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
	char* key;
	char* value;
	struct Node* next;
};

struct HashMap
{
	int numOfElements, capacity;
	struct Node** arr;
};

struct HASHMAP_Entry
{
	char* key;
	char* value;
};

void initNode(struct Node* node, char* key, char* value);
void HASHMAP_initHashMap(struct HashMap* mp);
int hashFunction(struct HashMap* mp, char* key);
void HASHMAP_insert(struct HashMap* mp, char* key, char* value);
void HASHMAP_delete(struct HashMap* mp, char* key);
char* HASHMAP_search(struct HashMap* mp, char* key);
struct HASHMAP_Entry* HASHMAP_getAllElements(struct HashMap* mp, int* outCount);