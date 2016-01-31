#include<stdio.h>
#include<stdlib.h>

struct item
{
	int b;
	char c;
};

char main(void)
{  
	struct item an_item = {.b = 1};
	printf("%d\n", an_item.b);
	an_item.c = 'a';
	printf("%d, %c\n", an_item.b, an_item.c);
	return 0;
}
