#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <dlfcn.h>
#include <stdint.h>

typedef struct information{
    char name[28];
    int quantity;
}information;

void producer()
{
    printf("+++++++++++++++++++++++++++++++++++\n");
    printf("         prod by pwnWiz\n");
    printf("+++++++++++++++++++++++++++++++++++\n\n");
}

void intro()
{
    printf("\n");
    printf("===================================\n");
    printf("   STORY 0x01.   Halloween Day\n");
    printf("===================================\n\n");
    printf("Today is the halloween day.\n\n");
    printf("My family prepared many sweets for young visitors who will visit tonight.\n\n");
    printf("Will they knock on the door for candy??\n\n");
    printf("REMEMBER !! Among them, there is possibility of exsistence of ghost...\n\n");
    printf("Will I able to find out whether he or she is ghost or not?\n\n");
    printf("It's time..\n\n");
}

void menu(int *choice)
{
    int temp;
    printf("-----------------\n");
    printf("TRICK OR TREAT !!\n");
    printf("-----------------\n");
    printf("1.ACCEPT\n");
    printf("2.DENY\n");
    printf("3.SEE VISITOR\n");
    printf("4.FOUND GHOST\n");
    printf("5.REST\n");
    printf(">>");
    scanf("%d", &temp);
    *choice=temp;
}

void accept(information **human, int *index) // malloc을 사용하여 할당을 받는 부분 크기는 fastbin으로 한정
{
    int size;
    printf("You have accepted visitors.\n");
    printf("How many candies will you give??\n>>");
    scanf("%d", &size); // 사이즈를 받는데 의미없음.
    
    if(*index>=10){ // index check;
        printf("Nope!!\n");
        exit(1);
    }
    
    if(size>32) { human[*index]=malloc(96); }
    else { human[*index]=malloc(size); }
    human[*index]->quantity=size;
    printf("What is your name? New visitor??\n");
    read(0, human[*index]->name, 28);
    printf("Hi, %s\n", human[*index]->name);
    
    (*index)++;
}

void deny(information **human, int *index) // malloc된 부분을 free 해주는 부분
{
    int select;
    if(*index<=0)
    {
        printf("You Can not Deny\n");
    }
    else
    {
        printf("You want to reset the memory??\n");
        printf("Select the Member\n");
        scanf("%d", &select);

        if(select <=0 || select > *index)
        {
            printf("You Don't even have memory\n");
            return;
        }

        free(human[select-1]);
        printf("OK.\n");
    }
}

void visitor(information **human, int *index) // list부분 malloc한 값들을 확인할 수 있음
{
    int choice;
    if(*index<=0)
    {
        printf("No visitor\n");
    } 
    else
    {
        printf("Enter the NO you want to see\n>");
        scanf("%d", &choice);
        if(choice>=*index || choice<0)
        {
            printf("No visitor\n");
        }
        else
        {
            printf("%s\n", human[choice]->name);
        }
    }
    
}

void check_ghost(information **ghost, int *ghost_index) // libc leak pumpkin (smallbin 할당 및 프리)
{
    char name_ghost[20];
    char xas[8]="pumpkin";
    char xsa[10]="nightmare";
    char xsee[10]="darkness!";
    for(int i=0;i<20;i++)
    {
	 name_ghost[i]=NULL;
    }
    int choice;

    printf("You really think it is ghost???\n");
    printf("What is the name??\n");

    read(0, name_ghost, 20);

    if(strncmp(xas, name_ghost, 7)==0)
    {
	printf("You got me!!! haha\n");
        ghost[*ghost_index]=malloc(200);
        (*ghost_index)++;
    }

    else if(strncmp(xsa, name_ghost, 9)==0)
    {
        printf("how many candies you want to give??\n>");
        scanf("%d", &choice);
        if(choice<0 || choice>*ghost_index)
        {
            printf("Nope!!!\n");
            exit(1);
        }

        free(ghost[choice]);
    }
    else if(strncmp(xsee, name_ghost, 9)==0)
    {
	printf("Here's the secret you want to know..\n");
	scanf("%d", &choice);
	if(choice<0 || choice>*ghost_index)
	{
		printf("But you are not ready..\n");
		exit(1);
	}
	else
	{
		printf("%s\n", ghost[choice]->name);
	}	
    }
	
}

void rest() // exit
{
    printf("You should go and sleep\n");  
    producer();
    exit(1);   
}

void bonus(information **human, int *index)
{
	int select;
	char tempname[28];
	printf("Angel has visited your house, and you may change one thing now.\n>>");
	scanf("%d", &select);
	if(select<0 || select > *index)
	{
		printf("You Failed gg..\n");
	}
	else
	{
		printf("OK. Write!\n>>");
		read(0, tempname, 28);
		if(strlen(tempname)>28)
		{
			printf("gg..\n");
		}
		strncpy(human[select]->name, tempname, strlen(tempname));
		printf("OK!!\n");
	}
}

int main()
{
    information *human[10]; // 최대 인덱스 개수
    information *ghost[5];
    int index=0; // 인덱스
    int ghost_index=0;
    
    setvbuf(stdout, 0, 2, 0); // for nc
    
    int choice; // case 문
    
    intro();
    while(1)
    {
        menu(&choice);
        switch(choice)
        {
            case 1:
                accept(&human, &index);
                break;
            case 2:
                deny(&human, &index);
                break;
            case 3:
                visitor(&human, &index);
                break;
            case 4:
                check_ghost(&ghost, &ghost_index);
                break;
            case 5:
                rest();
                break;
	    case 19950610:
		printf("You got the bonus!!!");
		bonus(&human, &index);
		break;
            default:
                printf("YOU GEEK!!\n");
        }
    }
    return 0; 
}

