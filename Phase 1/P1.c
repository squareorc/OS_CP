#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[40];
char Memory[100][4];
char IR[4];
char R[4];
int IC;
int SI;
FILE *infile;
FILE *outfile;

void init()
{
    memset(buffer, '\0', sizeof(buffer));
    memset(Memory, '\0', sizeof(Memory));
    memset(IR, '\0', sizeof(IR));
    memset(R, '\0', sizeof(R));
    IC = 0;
    SI = 0;
}

void resetBuffer()
{
    memset(buffer, '\0', sizeof(buffer));
}

void READ()
{
    printf("Read function called\n");
    
    if (fgets(buffer, sizeof(buffer), infile) == NULL)
        return;
    
    printf("\nIR[2] %c IR[2]-'0' %d\n", IR[2], IR[2] - '0');
    int buff = 0, mem_ptr = (IR[2] - '0') * 10;
    while (buff < 40 && buffer[buff] != '\0')
    {
        for (int i = 0; i < 4; i++)
        {
            Memory[mem_ptr][i] = buffer[buff];
            buff++;
        }
        mem_ptr++;
    }
    resetBuffer();
}

void WRITE()
{
    printf("Write function called\n");
    outfile = fopen("output.txt", "a");
    if (!outfile)
        return;
    
    for (int i = (IR[2] - '0') * 10; i < (IR[2] - '0' + 1) * 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (Memory[i][j] != '\0')
            {
                fputc(Memory[i][j], outfile);
            }
        }
    }
    fputc('\n', outfile);
    fclose(outfile);
}

void TERMINATE()
{
    printf("Terminate called\n\n");
    outfile = fopen("output.txt", "a");
    if (!outfile)
        return;
    fputs("\n\n", outfile);
    fclose(outfile);
}

void MOS()
{
    printf("\nPrinting IR : \n");
    for (int i = 0; i < 4; i++)
    {
        printf("%c ", IR[i]);
    }
    printf("\n");
    
    switch (SI)
    {
    case 1:
        READ();
        break;
    case 2:
        WRITE();
        break;
    case 3:
        TERMINATE();
        break;
    }
    SI = 0;
}

void EXECUTEUSERPROGRAM()
{
    while (IC < 99 && Memory[IC][0] != '\0')
    {
        printf("In ExeUserProg: IC :-  %d\n", IC);
        
        for (int i = 0; i < 4; i++)
        {
            IR[i] = Memory[IC][i];
        }
        IC++;
        
        if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
            return;
        }
    }
}

void STARTEXE()
{
    IC = 0;
    EXECUTEUSERPROGRAM();
}

void LOAD()
{
    char s[41];
    if (!infile)
        return;
    
    while (fgets(s, sizeof(s), infile))
    {
        if (strncmp(s, "$AMJ", 4) == 0)
        {
            init();
            printf("\nNew Job started\n");
        }
        else if (strncmp(s, "$DTA", 4) == 0)
        {
            printf("Data card loading\n");
            resetBuffer();
            STARTEXE();
        }
        else if (strncmp(s, "$END", 4) == 0)
        {
            printf("END of Job\n");
        }
        else
        {
            printf("Program Card loading\n");
            resetBuffer();
            strncpy(buffer, s, sizeof(buffer) - 1);
            
            int buff = 0;
            while (buff < 40 && buffer[buff] != '\0')
            {
                for (int j = 0; j < 4; j++)
                {
                    if (buffer[buff] == 'H')
                    {
                        Memory[IC][j] = 'H';
                        buff++;
                        break;
                    }
                    Memory[IC][j] = buffer[buff];
                    buff++;
                }
                IC++;
            }
        }
        for (int i = 0; i < 100; i++)
        {
            printf("M[%d]\t", i);
            for (int j = 0; j < 4; j++)
            {
                printf("%c", Memory[i][j]);
            }
            printf("\n");
        }
    }
    fclose(infile);
}

int main()
{
    infile = fopen("input.txt", "r");
    if (!infile)
    {
        printf("Error opening input file\n");
        return 1;
    }
    init();
    LOAD();
    return 0;
}
