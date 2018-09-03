#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <dlfcn.h>
#include <stdint.h>

#define FULL 7
#define debug 1

struct cavalry{
    char *name;
    char *salary;
    unsigned long popularity[2]; // [0] -> strlen, [1] -> in_use
};

struct root_cavalry{
    char *name;
    char *salary;
    char *secret;
    unsigned long popularity[3]; // [0] -> strlen salary, [1] -> strlen secret, [1] -> in use
};

long overflow = 0;
long root_limit = 0;
char unlink_here=NULL;
struct root_cavalry *global_root_cavalry;
struct cavalry **global_cavalry;
unsigned long info_root_cavalry[6];


void allocate_root()
{
    global_root_cavalry = info_root_cavalry;
}

void allocate_global(struct cavalry **global_cavalry, int *cavalry_count)
{
    if(*cavalry_count > FULL)
    {
        return;
    }

    if(*cavalry_count == 0)
    {
        global_cavalry[*cavalry_count] = global_root_cavalry;
        *cavalry_count += 1;
        return;
    }

    global_cavalry[*cavalry_count] = (struct cavalry*)calloc(1, sizeof(struct cavalry));
    *cavalry_count += 1;
}

void allocate_cavalry(struct cavalry **global_cavalry, int *cavalry_count, int *info_cavalry_count, int *fastbin)
{
    char *allocate_name_buf[8] = {"root", "knight1", "knight2", "knight3", "knight4", "knight5", "knight6", "knight7"};
    char salary[4096];
    int popularity=0;

    memset(salary, 0, 4096);

    if(*cavalry_count != *info_cavalry_count+1 || *cavalry_count > FULL)
    {
        return;
    }

    if(*cavalry_count == 1) // root 할당
    {
        global_root_cavalry->name = malloc(16);
        strcpy(global_root_cavalry->name, allocate_name_buf[*info_cavalry_count]);
        printf(">>");
        read(0, salary, 4096);
        if(strlen(salary)<=15)
        {
            popularity=strlen(salary);
        }
        else
        {
            popularity = strlen(salary)-strlen(salary)%16;
        }

        if(popularity <= 120)
            *fastbin = 1;

        global_root_cavalry->salary = malloc(popularity);
        memset(global_root_cavalry->salary, 0, popularity);
        memcpy(global_root_cavalry->salary, salary, popularity);

        global_root_cavalry->secret = malloc(4000);
        memset(global_root_cavalry->secret, 0 , 4000);

        global_root_cavalry->popularity[0] = popularity;
        global_root_cavalry->popularity[1] = 4000;
        global_root_cavalry->popularity[2] = 0x1;

        *info_cavalry_count += 1;
        return;
    }


    global_cavalry[*info_cavalry_count]->name=malloc(16);
    strcpy(global_cavalry[*info_cavalry_count]->name, allocate_name_buf[*info_cavalry_count]);
    printf(">>");
    read(0, salary, 4096);
    if(strlen(salary)<16)
    {
        popularity = strlen(salary);
    }
    else
    {
        popularity = strlen(salary)-strlen(salary)%16;
    }

    if(popularity <= 120)
    {
        if(*fastbin > 0) // fastbin이면 0x100으로 임의 할당
        {
            global_cavalry[*info_cavalry_count]->salary = malloc(0x100);
            memset(global_cavalry[*info_cavalry_count]->salary, 0, 0x100);
            memcpy(global_cavalry[*info_cavalry_count]->salary, salary, popularity);
            global_cavalry[*info_cavalry_count]->popularity[0] = 0x100;
            global_cavalry[*info_cavalry_count]->popularity[1] = 1;
            *info_cavalry_count += 1;
            return;
        }
    }

    if(popularity <= 120)
          *fastbin = 1;

    global_cavalry[*info_cavalry_count]->salary = malloc(popularity);
    memset(global_cavalry[*info_cavalry_count]->salary, 0, popularity);
    memcpy(global_cavalry[*info_cavalry_count]->salary, salary, popularity);
    global_cavalry[*info_cavalry_count]->popularity[0] = popularity;
    global_cavalry[*info_cavalry_count]->popularity[1] = 1;

    *info_cavalry_count += 1;
}

void show_name(struct cavalry **global_cavalry, int *info_cavalry_count)
{
    int choice;
    int change_bit;
    printf(">>");
    scanf("%d", &choice);
    getchar();

    if(choice >= *info_cavalry_count || choice<0)
    {
        return;
    }

    if(choice!=0 && global_cavalry[choice]->popularity[1] != 1)
    {
        return;
    }

    if(choice == 0)
    {
        printf(">>%s\n", global_root_cavalry->name);

        if(strcmp(global_root_cavalry->name, "root")==0)
        {
                printf(">>");
                scanf("%d", &change_bit);
                getchar();

            if(change_bit!=19950610)
            {
                epilogue();
            }

            if(overflow == 0)
            {
                if(global_root_cavalry->popularity[0]<=0x50)
                    global_root_cavalry->popularity[0] += 0x8;
                overflow += 1;
            }

            if(global_root_cavalry->popularity[2]) // root in_use set -> double free
                global_root_cavalry->popularity[2] = 0;
            else
                global_root_cavalry->popularity[2] = 1; 
        }

        return;
    }

    printf(">>");
    printf("%s\n", global_cavalry[choice]->name);

}

void free_cavalry(struct cavalry **global_cavalry, int *info_cavalry_count, int *fastbin)
{
    int choice;
    printf(">>");
    scanf("%d", &choice);
    getchar();

    if(choice >= *info_cavalry_count || choice<0)
    {
        return;
    }

    if(choice == 0)
    {
        if(global_root_cavalry->popularity[2] != 1)
        {
            return;
        }

        if(global_root_cavalry->popularity[0]<=120)
        {
            *fastbin = 0;
        }

        free(global_root_cavalry->salary);
        global_root_cavalry->popularity[2] = 0;

    }

    if(global_cavalry[choice]->popularity[1] != 1)
    {
        return;
    }

    else
    {
        free(global_cavalry[choice]->salary); // free
        global_cavalry[choice]->popularity[1] = 0; // in_use set to 0
    }
}

void modify_salary(struct cavalry **global_cavalry, int *info_cavalry_count)
{
   int choice;
   int size;
   char buf[4096];

   memset(buf, 0, 4096);

   printf(">>");
   scanf("%d", &choice);
   getchar();

    if(choice >= *info_cavalry_count || choice<0)
    {
        return;
    } 

    if(choice == 0)
    {
        if(global_root_cavalry->popularity[2] != 1)
        {
            return;
        }

        size = global_root_cavalry->popularity[0];
        if(size>=4096)
            size=4095;
        read(0, buf, size+1);
        memcpy(global_root_cavalry->salary, buf, size);

        return;
    }

    if(global_cavalry[choice]->popularity[1] != 1)
    {
        return;
    }

    size = global_cavalry[choice]->popularity[0];
    if(size>=4096)
        size = 4095;
    read(0, buf, size+1);
    memcpy(global_cavalry[choice]->salary, buf, size);
}

void allocate_salary(struct cavalry **global_cavalry, int *info_cavalry_count)
{
    int choice;
    printf(">>");
    scanf("%d", &choice);
    getchar();

    if(choice != 0 )
        epilogue();

    if(root_limit == 1)
    {
        free(global_root_cavalry->secret);
    }

    if(root_limit > 1)
    {
        epilogue();
    }

    root_limit += 1;

    if(global_root_cavalry->popularity[2] == 0)
    {
        global_root_cavalry->salary = malloc(48);
        global_root_cavalry->popularity[2] = 1;
    }

}


void epilogue()
{
    printf("+++++++++++++++++++++++++++++++++++\n");
    printf("         prod by pwnWiz\n");
    printf("+++++++++++++++++++++++++++++++++++\n\n");
    exit(1);
}

void prologue()
{
    printf("\n");
    printf("===================================\n");
    printf("   STORY 0x02.   Viva La Vida\n");
    printf("===================================\n\n");
    printf("I used to rule the world,\n");
    printf("Seas would rise when I gave the word.\n\n");
}

void selection(int *choice, int *breadwinner)
{
    int temp;
    if(!*breadwinner)
    {
        printf("|*| I hear Jerusalem bells are ringing.\n"); // choice==8
        printf("|*| Roman Cavalry choirs are singing.\n");   // choice==13
        printf("|*| Be my mirror, my sword and shield.\n");  // choice==54
        printf("|*| My missionaries in a foreign field.\n"); // choice==24
        printf("|*| I know Saint Peter won't call my name.\n"); // choice ==21
        printf("|*| But that was when I rule the world.\n"); // choice=1
    }
    printf(">>");
    scanf("%d", &temp);
    *choice=temp;
}

int main()
{   
    global_cavalry = (struct cavalry**)calloc((FULL+1), sizeof(struct cavalry*));
    allocate_root();
    int fastbin=0; // only one fastbin allocation
    int cavalry_count = 0; 
    int info_cavalry_count = 0;
    int choice = 0;
    int breadwinner = 0; // menu choice 한 번만 출력
    int exit_menu = 0;

    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    prologue();

    while(choice != 101)
    {
        selection(&choice, &breadwinner);
        getchar();
        breadwinner++;
        switch(choice)
        {
            case 8:     // global_cavalry 할당
                allocate_global(global_cavalry, &cavalry_count);
                break;
            case 13:    // cavalry 정보 할당
                allocate_cavalry(global_cavalry, &cavalry_count, &info_cavalry_count, &fastbin);
                break;
            case 54:    // leak name and change bit
                show_name(global_cavalry, &info_cavalry_count); 
                break;
            case 24:    // free -> double free trigger
                free_cavalry(global_cavalry, &info_cavalry_count, &fastbin); 
                break;    
            case 21:    // unlink here
                modify_salary(global_cavalry, &info_cavalry_count);
                break;
            case 1:     // unlink trigger method here
                allocate_salary(global_cavalry, &info_cavalry_count);
                break;
            default:    // 10 wrong input
                exit_menu++; 
                if(exit_menu>10)
                    epilogue();
                break;
        }

    }

    epilogue();

    return 0;

}

