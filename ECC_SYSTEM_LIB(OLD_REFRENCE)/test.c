#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include"ALGORITHMS/RAW.h"
int main()
{
	printf("TEST STARTED\n");
	printf("------------\n");

	ECCALGO_RAW("dataaaaa", sizeof("dataaaaa"));
	printf("%s",ECCLAGORAW_debug);

	return 0;
}
