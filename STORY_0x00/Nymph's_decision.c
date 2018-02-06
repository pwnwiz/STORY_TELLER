#include <stdio.h>


int select()
{
	int choice;
	printf("There are three routes. You have to choose the only way.\n\n");
	printf("1. Pray to god\n");
	printf("2. Steal the potion from poseidon\n");
	printf("3. Break the nymph's leg\n");
	printf("What is your decision??\n\n");
	scanf("%d", &choice);
	return choice;
}

void intro()
{
	printf("\n");
	printf("===================================\n");
	printf("   STORY 0x00.   Nymph's Fault\n");	
	printf("===================================\n\n");
	printf("Once upon a time in Venice, there was a lonely nymph who wanted to become an angel.\n\n");
	printf("However, she didn’t concern for the safety of the lotus on the water nearby given as her duty.\n\n");
	printf("The lotus finally withered and she had been punished by the only god.\n\n");
	printf("You have revealed that only way for her to be granted amnesty is that revival of the lotus.\n\n");
}

int main()
{
	int choice;

	intro();
	choice=select();
	return 0;

}
