#include<stdio.h>

int main()
{
	FILE *fp = fopen("./Nymph\'s_fault_encrypt", "r");
	FILE *fp2 = fopen("./Nymph\'s_fault_decrypt", "w");
	char ch=0x7F, ch1;

	while(0 < fread(&ch1, 1, 1, fp))
	{
		fputc(ch, fp2);
        ch ^= ch1;
	}
    fputc(ch, fp2);

	fclose(fp);
	fclose(fp2);

	return 0;
}
