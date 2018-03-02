#include<stdio.h>

int main()
{
	FILE *fp = fopen("./Nymph\'s_fault", "r");
	FILE *fp2 = fopen("./Nymph\'s_fault2", "w");
	char ch, ch1, ch2;

	fread(&ch, 1, 1, fp);
	printf("%c",ch);
	while(0 < fread(&ch1, 1, 1, fp))
	{
		printf("%c",ch1);
		fputc(ch ^ ch1, fp2);
		ch = ch1;
	}

	fclose(fp);
	fclose(fp2);

	return 0;
}
