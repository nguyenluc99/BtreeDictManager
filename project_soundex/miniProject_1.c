#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<termio.h>
#include "btree.h"

#define SIZE_WORDS 50
#define SIZE_OF_SOUNDEX 1000

typedef struct
{
    char *s;
} string;

char getch(void);                                           // Lay 1 ki tu nhap tu ban phim
int searchRelatedWords(BTA *root, char *word, string *str); //tim kiem tu bat dau bang word trong root
void searchWords(BTA *root);
void clearInput(int n);  
char *toLowerString(char *str);                                                   //chuyen doi chuoi chu hoa thanh chuoi chu thuong
int SoundEx(char *SoundEx, char *WordString, int LengthOption, int CensusOption); //ham soundex
void SoundexInsert(char *word, BTA *btSoundex);
BTA *recordData(BTA *root, FILE *file, char fDataName[30], int isSoundexInsert);

BTA *recordData(BTA *root, FILE *file, char fDataName[30], int isSoundexInsert)
{
    root = btcrt(fDataName, 0, 0);
    file = fopen("/usr/share/dict/words", "r");
    char *temp = (char *)malloc(SIZE_WORDS);
    while (fgets(temp, SIZE_WORDS, file) != NULL)
    {
        temp = toLowerString(temp);
        temp[strlen(temp)] = '\0';
        if (!isSoundexInsert)
            binsky(root, temp, 1);
        else if (isSoundexInsert == 1)
        {
            SoundexInsert(temp, root);
        }
    }
    free(temp);
    fclose(file);
    return root;
}

void clearInput(int n)
{
    int i = 0;
    while (i < n)
    {
        putchar('\b');
        printf("%c[0K", 27);
        i++;
    }
}

char *toLowerString(char *str)
{
    char *temp = (char *)malloc(SIZE_WORDS);
    int n = strlen(str);
    temp[n - 1] = '\0';
    str[n - 1] = '\0';
    for (int i = 0; i < n; i++)
        temp[i] = tolower(str[i]);
    free(str);
    return temp;
}

char getch(void)
{
    char ch;
    int fd = fileno(stdin);
    struct termio old_tty, new_tty;
    ioctl(fd, TCGETA, &old_tty);
    new_tty = old_tty;
    new_tty.c_lflag &= ~(ICANON | ECHO | ISIG);
    ioctl(fd, TCSETA, &new_tty);
    fread(&ch, 1, sizeof(ch), stdin);
    ioctl(fd, TCSETA, &old_tty);
    return ch;
}

void searchWords(BTA *root)
{
    char c;
    char *words = (char *)malloc(SIZE_WORDS);
    string *str = (string *)malloc(1000 * sizeof(string));
    int i = 0, j = 0;
    int value;
    int numwords = 0;
    printf("***Enter the word (use \"tab\" to be auto-complete): ");
    while (1)
    {
        c = tolower(getch());
        if ((c != '\t') && (c != '\n') && (c != 27) && (c != 127))
        {
            words[j++] = c;
            putchar(c);
            i = 0;
            numwords = 0;
        }
        if (c == 127)
        {
            if (j > 0)
            {
                putchar('\b');
                printf("%c[0K", 27);
                words[j--] = '\0';
                i = 0;
            }
        }
        if (c == '\t')
        {
            if (j != 0)
            {
                if (i == 0)
                {
                    free(str);
                    str = (string *)malloc(1000 * sizeof(string));
                    int n = searchRelatedWords(root, words, str);
                    numwords = n;
                    if (n != 0)
                    {
                        clearInput(j);
                        free(words);
                        words = (char *)malloc(SIZE_WORDS * sizeof(char));
                        strcpy(words, str[i].s);
                        j = strlen(words);
                        for (int k = 0; k < j; k++)
                            putchar(words[k]);
                        i++;
                    }
                }
                else
                {
                    clearInput(j);
                    if (i == numwords)
                        i = 0;
                    free(words);
                    words = (char *)malloc(SIZE_WORDS);
                    strcpy(words, str[i].s);
                    j = strlen(words);
                    for (int k = 0; k < j; k++)
                        putchar(words[k]);
                    i++;
                }
            }
        }
        if (c == '\n')
        {
            printf("\n");
            free(str);
            str = (string *)malloc(1000 * sizeof(str));
            numwords = searchRelatedWords(root, words, str);
            if (numwords != 0)
            {
                printf("-- The words have profix which you enter: \n");
                for (int k = 0; k < numwords; k++)
                {
                    printf("%s\n", str[k].s);
                }
            }
            else
            {
                printf("-- Not exist the word have profix which you enter.\n");
            }
            break;
        }
    }
}

int searchRelatedWords(BTA *root, char *word, string *str)
{
    int i, total = 0, value;
    char key[50];
    i = bfndky(root, word, &value);
    while (1)
    {
        i = bnxtky(root, key, &value);
        if (i != 0 || strncmp(key, word, strlen(word)) != 0)
            break;
        str[total++].s = strdup(key);
        if (total == 1000)
            break;
    }
    return total;
}

int SoundEx(char *SoundEx, char *WordString, int LengthOption, int CensusOption)
{
    int InSz = 31;
    char WordStr[32];
    int SoundExLen, WSLen, i;
    char FirstLetter, *p, *p2;

    SoundExLen = WSLen = 0;
    SoundEx[0] = 0;

    if (CensusOption)
    {
        LengthOption = 4;
    }

    if (LengthOption)
    {
        SoundExLen = LengthOption;
    }
    if (SoundExLen > 10)
    {
        SoundExLen = 10;
    }
    if (SoundExLen < 4)
    {
        SoundExLen = 4;
    }

    if (!WordString)
    {
        return (0);
    }
    for (p = WordString, p2 = WordStr, i = 0; (*p); p++, p2++, i++)
    {
        if (i >= InSz)
            break;
        (*p2) = (*p);
    }
    (*p2) = 0;
    for (p = WordStr; (*p); p++)
    {
        if ((*p) >= 'a' && (*p) <= 'z')
        {
            (*p) -= 0x20;
        }
    }
    for (p = WordStr; (*p); p++)
    {
        if ((*p) < 'A' || (*p) > 'Z')
        {
            (*p) = ' ';
        }
    }
    for (i = 0, p = p2 = WordStr; (*p); p++)
    {
        if (!i)
        {
            if ((*p) != ' ')
            {
                (*p2) = (*p);
                p2++;
                i++;
            }
        }
        else
        {
            (*p2) = (*p);
            p2++;
        }
    }
    (*p2) = 0;
    for (i = 0, p = WordStr; (*p); p++)
        i++;
    for (; i; i--)
    {
        if (WordStr[i] == ' ')
        {
            WordStr[i] = 0;
        }
        else
        {
            break;
        }
    }
    for (WSLen = 0, p = WordStr; (*p); p++)
        WSLen++;

    if (!WSLen)
    {
        return (0);
    }
    if (!CensusOption)
    {
        if (WordStr[0] == 'P' && WordStr[1] == 'S')
        {
            WordStr[0] = '_';
        }

        if (WordStr[0] == 'P' && WordStr[1] == 'F')
        {
            WordStr[0] = '_';
        }
        if (WordStr[0] == 'G' && WordStr[1] == 'H')
        {
            WordStr[1] = '_';
        }

        for (i = 0; i < WSLen; i++)
        {
            if (WordStr[i] == 'D' && WordStr[i + 1] == 'G')
            {
                WordStr[i] = '_';
                i++;
                continue;
            }
            if (WordStr[i] == 'G' && WordStr[i + 1] == 'H')
            {
                WordStr[i] = '_';
                i++;
                continue;
            }
            if (WordStr[i] == 'K' && WordStr[i + 1] == 'N')
            {
                WordStr[i] = '_';
                i++;
                continue;
            }
            if (WordStr[i] == 'G' && WordStr[i + 1] == 'N')
            {
                WordStr[i] = '_';
                i++;
                continue;
            }
            if (WordStr[i] == 'M' && WordStr[i + 1] == 'B')
            {
                WordStr[i + 1] = '_';
                i++;
                continue;
            }

            if (WordStr[i] == 'P' && WordStr[i + 1] == 'H')
            {
                WordStr[i] = 'F';
                WordStr[i + 1] = '_';
                i++;
                continue;
            }
            if (WordStr[i] == 'T' &&
                WordStr[i + 1] == 'C' &&
                WordStr[i + 2] == 'H')
            {

                WordStr[i] = '_';
                i++;
                i++;
                continue;
            }
            if (WordStr[i] == 'M' && WordStr[i + 1] == 'P' && (WordStr[i + 2] == 'S' || WordStr[i + 2] == 'T' || WordStr[i + 2] == 'Z'))
            {
                WordStr[i + 1] = '_';
                i++;
            }
        }
    }
    for (p = p2 = WordStr; (*p); p++)
    {
        (*p2) = (*p);
        if ((*p2) != '_')
        {
            p2++;
        }
    }
    (*p2) = 0;
    FirstLetter = WordStr[0];
    if (FirstLetter == 'H' || FirstLetter == 'W')
    {
        WordStr[0] = '-';
    }
    if (CensusOption == 1)
    {
        for (p = &(WordStr[1]); (*p); p++)
        {
            if ((*p) == 'H' || (*p) == 'W')
            {
                (*p) = '.';
            }
        }
    }

    for (p = WordStr; (*p); p++)
    {
        if ((*p) == 'A' ||
            (*p) == 'E' ||
            (*p) == 'I' ||
            (*p) == 'O' ||
            (*p) == 'U' ||
            (*p) == 'Y' ||
            (*p) == 'H' ||
            (*p) == 'W')
        {
            (*p) = '0';
        }
        if ((*p) == 'B' ||
            (*p) == 'P' ||
            (*p) == 'F' ||
            (*p) == 'V')
        {
            (*p) = '1';
        }
        if ((*p) == 'C' ||
            (*p) == 'S' ||
            (*p) == 'G' ||
            (*p) == 'J' ||
            (*p) == 'K' ||
            (*p) == 'Q' ||
            (*p) == 'X' ||
            (*p) == 'Z')
        {
            (*p) = '2';
        }
        if ((*p) == 'D' ||
            (*p) == 'T')
        {
            (*p) = '3';
        }
        if ((*p) == 'L')
        {
            (*p) = '4';
        }

        if ((*p) == 'M' ||
            (*p) == 'N')
        {
            (*p) = '5';
        }
        if ((*p) == 'R')
        {
            (*p) = '6';
        }
    }

    if (CensusOption == 1)
    {
        for (p = p2 = &WordStr[1]; (*p); p++)
        {
            (*p2) = (*p);
            if ((*p2) != '.')
            {
                p2++;
            }
        }
        (*p2) = 0;
    }
    for (p = p2 = &(WordStr[0]); (*p); p++)
    {
        (*p2) = (*p);
        if ((*p2) != p[1])
        {
            p2++;
        }
    }
    (*p2) = 0;
    for (p = p2 = &WordStr[1]; (*p); p++)
    {
        (*p2) = (*p);
        if ((*p2) != ' ' && (*p2) != '0')
        {
            p2++;
        }
    }
    (*p2) = 0;
    for (WSLen = 0, p = WordStr; (*p); p++)
        WSLen++;
    for (i = WSLen; i < SoundExLen; i++)
    {
        WordStr[i] = '0';
    }
    WordStr[SoundExLen] = 0;
    WordStr[0] = FirstLetter;
    for (p2 = SoundEx, p = WordStr; (*p); p++, p2++)
    {
        (*p2) = (*p);
    }
    (*p2) = 0;
    return (SoundExLen);
}
void SoundexInsert(char *word, BTA *btSoundex)
{
    int i, rsize;
    char s[SIZE_OF_SOUNDEX], soundEx[10];
    SoundEx(soundEx, word, 4, 4);
    soundEx[4] = '\0';
    i = btsel(btSoundex, soundEx, s, SIZE_OF_SOUNDEX, &rsize);
    if (i != 0)
        btins(btSoundex, soundEx, word, SIZE_OF_SOUNDEX);
    else
    {
        strcat(s, "\n");
        strcat(s, word);
        s[strlen(s)] = '\0';
        btupd(btSoundex, soundEx, s, SIZE_OF_SOUNDEX);
    }
}

int main(void){
char *file ="init_data.db";
    BTA *init_data;
    char *buf = (char*)malloc(SIZE_WORDS);
    btinit();

    init_data = btopn(file,0,0);

    if(!init_data)
        init_data= btcrt(file, 0,0);
    FILE *f = fopen("/usr/share/dict/words","r");

    char word[20];
    int rsize;
    BTint value;
    while(!feof(f)){
        fgets(buf, 50,f);
        binsky(init_data,buf,value); //insert a key
        //btins(init_data,buf,(char*)&value, sizeof(int));
            }
    
    int n, size;
    do{
        printf("===========MiniProject==========\n");
        printf("|Options:                      |\n");
        printf("|1.Print all Dictionary        |\n");
        printf("|2.Add a word                  |\n");
        printf("|3.Search a word (Normal)      |\n");
        printf("|4.Delete a word               |\n");
        printf("|5.Search tab                  |\n");
        printf("|6.Exit                        |\n");
        printf("================================\n");
        printf("Enter your choice:              \n");
        scanf("%d", &n);
        while(getchar()!='\n');
        switch(n){
            case 1:
            
        btsel(init_data,"",(char*)&value,sizeof(int),&rsize); // locating data for an existing key
        while(!btseln(init_data, buf,(char*) &value, sizeof(int), &rsize))
        {
            printf("%s", buf);
            }
    
            break;
            case 2:
            printf("Add a word:");
            gets(buf);
            //while(getchar()!='\n')
            //btins(init_data,buf,(char*)&value, sizeof(int));
            int res = binsky(init_data, buf, value);
            if(! res)
                printf("The word is successfully stored in DictBtree\n");
                else
                {
                    printf("Error!\n");
                }    
            break;
            case 3:
            printf("Search a word:");
            gets(buf);
            int res1 =bfndky(init_data,buf,&value);
            if (!res1) {
                printf("%s\n", buf);
            }
             else 
             printf("Not found %s!\n", buf);
            break;
            case 4:
            printf("Delete a word:");
            gets(buf);
            int res2=bdelky(init_data,buf);
            if (res2==QNOKEY) printf("Error!\n");
            else {
            if(!res2)
                printf("The word is successfully deleted in DictBtree\n");
            else
                printf("Error!\n");
            }
            break;
            case 5:
            btinit();
            char *dict ="dict.db";
            BTA *root = btopn(dict, 0, 0);
            FILE *file1 = fopen("/usr/share/dict/words", "r");
            if (root == NULL)
            root = recordData(root, file1, dict, 0);


            searchWords(root);
            btcls(root);
            fclose(file1);
            break;
            }
    }while(n!=6);
    fclose(f);
    free(buf);
}