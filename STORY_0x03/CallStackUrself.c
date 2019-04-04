#include <stdio.h>

void intro()
{
	printf("=======================================\n");
	printf("     STORY 0x03.   CallStackUrself\n");
	printf("=======================================\n");
	printf("Hello Newbie right here!\n");
	printf("I teach u one short lesson for your improvement\n");
	printf("Have you ever heard about ROP not using pop3/ret gadget?\n");
	printf("Anyway, pwn it and get the flag !!!\n");
}

void producer()
{
	printf("+++++++++++++++++++++++++++++++++++++++\n");
	printf("            prod by pwnWiz\n");
	printf("+++++++++++++++++++++++++++++++++++++++\n");
}

void vuln(){
	char buf[48];
	read(0, buf, 512);
}

int main(){
	setvbuf(stdout, 0, 2, 0);
	intro();

	vuln();

	producer();
}
