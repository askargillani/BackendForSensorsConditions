#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *r = "askar";
	char buff[100];
	
	int i = snprintf(buff,20,"%s\n",r);
	printf("%s\n",buff);
	memset(buff,'\0',1);
	printf("buff:%s\n",buff);
}
