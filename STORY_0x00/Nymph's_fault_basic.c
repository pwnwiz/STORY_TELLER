#include <stdio.h>
#include <unistd.h>
#include <string.h>


void hint()
{
        __asm__ __volatile__
        (
        "pop %rdi \n\t"
        "pop %rsi \n\t"
        "pop %rdx \n\t"
        "ret \n\t"
        );
 
}

void hent()
{
	__asm__ __volatile__
	(
	"pop %rdi \n\t"
	"ret \n\t"
	);
}

void producer()
{
        printf("+++++++++++++++++++++++++++++++++++\n");
        printf("  prod by pwnWiz, Taewoo, OIEHSOO\n");  
        printf("+++++++++++++++++++++++++++++++++++\n\n");
}

int select()
{
        int choice;
        printf("There are three routes. You have to choose the only way.\n\n");
        printf("1. Pray to god\n");
        printf("2. Steal the potion from Poseidon\n");
        printf("3. Persuade Nymph to give up something at the expense of life of lotus\n\n");
        printf("What is your decision??\n\n");
        scanf("%d", &choice);
        printf("\n");
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
        printf("The lotus which protects city from devils finally withered and humans lost their light.\n\n");
        printf("To solve this problem, the Nymph has to be forgiven by the only god.\n\n");
        printf("Luckily, You have revealed that only way for her to be granted amnesty is that revival of the lotus.\n\n");
}

void ending()
{
        printf("You have saved the lotus with potion and the world became peaceful as before.\n\n");
 printf("And the Nymph will stay forever filled with regrets waiting to die alone\n\n");
        producer();

}

void pray() // choice 1
{
        printf("Well. It will be the best way.. I thought.\n\n");
        printf("However, I have no idea that where should I go to meet the only god not even know the face..\n\n");
        printf("Let me just pray from here..\n\n");
        printf("my Lord, forgive her sins, she regrets her every single faults.\n\n");
        printf("But god said NOTHING :<\n\n");
        printf("This was the not the BEST choice.\n\n");
        producer();
}

int canStealPotion(char *stealPotion)
{
        int len = strlen(stealPotion);
        char *potionName[4] = {"Red", "Blue", "Purple", "Yellow"};

        for(int i=0; i<4; i++)
                if(!strncmp(stealPotion, potionName[i], len))
                        return 1;
        return 0;
}

void potion() // choice 2
{
        char str[7];

        printf("You have decided to go to Poseion's palace to steal the potion\n\n");
        printf("Oh!! There are various potions. Red, Blue, Purple, Yellow....\n\n");
        printf("Which one will you choose???\n\n");

        // 여기서 버퍼 오버플로우 취약점 안터짐 (크기는 yellow까지..)
        // 만약에 Red, Blue, Purple, Yellow 가 입력되면 아래 실행

        int potionStrLen = read(0, str, sizeof(str));
        str[potionStrLen - 1] = '\0';

        if(canStealPotion(str)){
                printf("I have chosen ( %s )!\n\n", str);
                printf("Let me get out quickly before being caught.\n\n");
          printf("YOU! THIEF!!! ROP!!!\n\n"); // hint
                ending();
        }
        else{
                printf("YOU! THIEF!!!\n\n");
                printf("You have been put into the jail\n\n");
                producer();
        }

}


void persuade() // choice 3
{
        char str[256]={0};

        printf("Hey Human! What the hell are you doing here?\n\n");
        printf("Your highness. Please save us.\n\n");
        printf("Only with your help, we can solve this disaster.\n\n");
        printf("Hmm.. I have to give up my beautiful eyes if I help you. That's not what I want.\n\n");
        printf("If I do that, What will you give?\n\n");
        // 자유 입력 (여기서 버퍼 오버플로우 취약점 터짐) 

        read(0, str, 500);

        write(1, str, strlen(str + 1));

        printf("\nHMM... Nope\n\n");
        producer();
}

int main()
{
        int choice;

        intro();
        choice=select();

        switch (choice)
        {
                case 1:
                        pray(); break;
                case 2:
                        potion(); break;
                case 3:
                        persuade(); break;
                default:
                        printf("Wrong input!!!\n");
        }

        return 0;
}
                                
