#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// #include<ncurses.h>
#include<unistd.h> 
#include<math.h>
#include<ctype.h>
#include <termios.h>
#include"btree.h"
#define MAX_LEN 4
#define MAX_NUM_OF_WORD 2000
#define INI_WORD 1000
#include <termios.h>

static struct termios old, new;
void initTermios(int echo);
void resetTermios(void);
char getch_(int echo);
char getch(void);
 
// gcc -o min miniProject.c -Lbt-5.0.0/lib -lbt -Ibt-5.0.0/inc -lncurses  
char* toString(long, char*);
void generateData(BTA* , char*);
void addAWord(BTA* );
void menu();

int main(){
    btinit();
    char ch;
    long tmpVal, *tmpPointer;
    int n, size, size2, i, j, found;
    BTA * myDict;
    n = 0;
    char *nextKey = (char*)malloc(10 * sizeof(char));
    char *key = (char*)malloc(10 * sizeof(char));
    char *iniStr = (char*)malloc(MAX_LEN * sizeof(char));
    myDict = btopn("dict", 0, 0); 
    if (!myDict) {
        printf("create a book\n");
        myDict = btcrt("dict", 0, 0);  
        generateData(myDict, iniStr);
    }
    while (n != 6 ) {
        menu();
        scanf("%d%*c", &n);
        strcpy(key,"");
        strcpy(nextKey,"");
        switch(n){
            case 1: // Add a word
                printf("add word");
                addAWord(myDict);
                break;
            case 2: // 2. Search a word
                // printf("Insert a key to find\t");
                // scanf("%[^\n]%*c", key);
                // if (btsel(myDict, key, (char*)&tmpVal, sizeof(long), &size))
                //     printf("Key not found");
                // else printf("key %s has value %ld\n", key, tmpVal);
                printf("%s","Insert a key to search\t");
                j = 0;
                i = 0;
                found = 0;
                while (1){
                    char c = getch();
                    if ((c != 9) && (c != 10) && (c != 27) && (c != 127)){
                        key[i++] = c;
                        putchar(c);
                        j++;
                    }
                    if (c == 127 && j > 0){
                        putchar('\b');
                        printf("%c[0K", 27);
                        key[i--] = '\0';
                        j--;
                    }
                    if (c==9) { printf("\n");break;}
                    else if (c==10) break;
                }
                key[i] = '\0';
                printf("\n");
                // printf("key to search is '%s'\n", key);
                if (strlen(key)){
                    btsel(myDict, key, (char*)&tmpVal, sizeof(long), &size);
                    while (!btseln(myDict, nextKey, (char*)&tmpVal, sizeof(long), &size2))
                    {
                        if (memcmp(key, nextKey, strlen(key)) != 0) break;
                        printf("'%s' has value %ld \t", nextKey, tmpVal);
                        found = 1;
                    }	
                    if (!found) printf("keyword not found\n");
                } else printf("Please insert somethin to search\n");
                break;
            case 3: // 3. Delete a word
                printf("Insert a key to delete\t");
                scanf("%[^\n]%*c", key);
                if (btdel(myDict, key))
                    printf("Error delete the key %s\n", key);
                else printf("key %s deleted", key);
                break;
            case 4: // 4. Print the dict
                strcpy(key, "");
                btsel(myDict, key, (char*)&tmpVal, sizeof(long), &size);
                while (!btseln(myDict, nextKey, (char*)&tmpVal, sizeof(long), &size2))
                {
                    printf("'%s' has value %ld \t", nextKey, tmpVal);
                }	
                break;
            case 5 :

                break;
            case 6 :
                printf("You type to out\n");
                break;
        }
    }
    btcls(myDict);
    return 0;
}

void initTermios(int echo) {
  tcgetattr(0, &old); //grab old terminal i/o settings
  new = old; //make new settings same as old settings
  new.c_lflag &= ~ICANON; //disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}
 
void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}
char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
char getch(void) {
  return getch_(0);
}
char* toString(long num, char* iniStr){
    char ch;
    long re;
    char tmpStr[MAX_LEN];
    long iniPos = 0;
    long i;
    do {
        re = num % 27;
        if (re != 0){
            tmpStr[iniPos] = re + 'a' - 1;
            iniPos ++;
        }
        num /= 27;
        if (num == 0) break;
    }  while (num > 0); 
    for (i = 0; i < iniPos; i++) {
        iniStr[i] = tmpStr[iniPos - i - 1];
    }
    iniStr[iniPos] = '\0';
    return iniStr;
}
void generateData(BTA* dict, char* iniStr){
    long len = pow(26, MAX_LEN), val;
    long i;
    for (i = 0; i < INI_WORD; i ++) {
        val = len / INI_WORD * i;
        btins(dict, (char*)toString(val, iniStr), (char*)&val,  sizeof(long));
    }
}
void addAWord(BTA* dict){
    char key[10];
    long val;
    printf("Insert a key\t");
    scanf("%[^\n]%*c", key);
    printf("Insert a long value\t");
    scanf("%ld", &val);
    if (btins(dict, key, (char*)&val,  sizeof(long)))
        printf("Error insert key at value = %ld\n", val);
    else printf("Success at value = %ld\n", val);
}
void menu() {
    printf("\n===========MiniProject==========\n"); 
    printf("|Options:                       |\n"); 
    printf("|1. Add a word                  |\n"); 
    printf("|2. Search a word               |\n"); 
    printf("|3. Deleta a word               |\n"); 
    printf("|4. Print the dict              |\n"); 
    printf("|5. Suggesstion search          |\n"); 
    printf("|6. Quit                        |\n"); 
    printf("================================\n"); 
    printf("Enter your choice:              \n"); 
}