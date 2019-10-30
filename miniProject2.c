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
    BTA * myDict;
    char nextKey[10], iniStr[MAX_LEN], key[10], ch;
    long tmpVal, *tmpPointer;
    int n = 0, size, i, j;
    // myDict = btopn("myDict", 0, 0); 
    // if (!myDict) {
        printf("create a book");
        myDict = btcrt("myDict", 0, 0);  
        generateData(myDict, iniStr);
    // }
    while (n != 5 ) {
        menu();
        scanf("%d%*c", &n);
        printf("got %d \n", n);
        switch(n){
            case 1: // Add a word
                printf("add word");
                addAWord(myDict);
                break;
            case 2: // 2. Search a word
                printf("Insert a key to find\t");
                scanf("%[^\n]%*c", key);
                if (btsel(myDict, key, (char*)&tmpVal, sizeof(long), &size))
                    printf("Key not found");
                else printf("key %s has value %ld\n", key, tmpVal);
                break;
            case 3: // 3. Delete a word
                printf("Insert a key to delete\t");
                scanf("%[^\n]%*c", key);
                if (btdel(myDict, key))
                    printf("Error delete the key %s\n", key);
                else printf("key %s deleted", key);
                break;
            case 4: // 4. Print the dict
                printf("%s","Insert a key to search\t");
                j = 0;
                i = 0;
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
                    if (c==9 || c==10) break;
                }
                key[i] = '\0';
                printf("\n");
                printf("key is '%s', len is %ld\n", key, strlen(key));
                btsel(myDict, key, (char*)&tmpVal, sizeof(long), &size);
                // printf("not Found\t");
                strcpy(nextKey, key);
                while (!btseln(myDict, nextKey, (char*)&tmpVal, sizeof(long), &size) )
                    {
                        // printf("Found\t");
                        if (memcmp(key, nextKey, strlen(key)) != 0) break;
                        printf("%s\t", nextKey);
                        // printw("%s\t%10ld\n", nextKey, tmpVal);
                    }	
                // printf("not Found2222\t");
                break;
            case 5 :
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
 
/* Restore old terminal i/o settings */
void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}
 
/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
 
/* 
Read 1 character without echo 
getch() function definition.
*/
char getch(void) {
  return getch_(0);
}
char* toString(long num, char* iniStr){
    char ch;
    int re;
    char tmpStr[MAX_LEN];
    long iniPos = 0;
    long i;
    do {
        re = num % 27 + 'a';
        if (re != 0){
            tmpStr[iniPos] = re + 'a';
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
    
    long len, val;
    long i;
    printf("Start");
    for (i = 0; i < INI_WORD; i ++) {
        val = len / INI_WORD * i;
        if (btins(dict, (char*)toString(val, iniStr), (char*)&val,  sizeof(long))) {} // else printf(".");
        //     printf("Error insert key at value = %ld\n", val);
        // else printf("Success at value = %ld\n", val);
    }
        printf("Done\n");
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
    printf("|5. Quit                        |\n"); 
    printf("================================\n"); 
    printf("Enter your choice:              \n"); 
}