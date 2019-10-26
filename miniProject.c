#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ncurses.h>
#include<unistd.h> 
#include<math.h>
#include"btree.h"
#define maxLen 4
#define maxNumOfWord 20000
#define iniWord 1000
char iniStr[maxLen];
char* toString(long num) {
    char ch;
    char tmpStr[maxLen];
    long iniPos = 0;
    long i;
    do {
        tmpStr[iniPos] = num % 26 + 'a';
        iniPos ++;
        num /= 26;
        if (num == 0) break;
    }  while (num > 0); 
    for (i = 0; i < iniPos; i++){
        iniStr[i] = tmpStr[iniPos - i - 1];
    }
    return iniStr;
}
int kbhit(){
    int ch = getch();
    printf("run, ch is %d\n", ch);
    // if (ch != ERR) {
    if (ch != 9 && ch != 10 && ch != EOF) {
        ungetch(ch);
        return 1;
    } else {    
        return 0;
    }
}
void generateData(BTA* dict){
    
    long len = pow(26, maxLen), val;
    long i;
    for (i = 0; i < iniWord; i ++) {
        val = len / iniWord * i;
        // printf("%5s", toString(val));
        if (btins(dict, (char*)toString(val), (char*)&val,  sizeof(long)))
            printf("Error insert key at value = %ld\n", val);
        else printf("Success at value = %ld\n", val);
    }
}

long* addAWord(BTA* dict) {
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

// void searchAWord(BTA* dict, char* key, char* data) {
//     // long val;
//     int size;
//     if ( btsel(dict, key, (char*)&data, sizeof(long), &size) ) {
//         printf("Not found number for %s!\n", key);
//         // return NULL;
//     }
//     else{ 
//         printf("Phone number of %s is %ld\n", key, (long)&data);
//         // return val;
//     }
// }

void auto_complete(){
    char *file ="auto_complete";
    btinit();
    BTA *complete_tree;
    complete_tree = btopn(file,0,0);

    if(!complete_tree)
        complete_tree= btcrt(file, 0,0);
    FILE *f = fopen("/user/share/dict/words","r");

    char word[20];
    char value[600];
    int rsize;
    while(!feof(f)){
        fgets(word, 20, f);
        word[strlen(word)-1]='\0';

        int length = strlen(word);
        for(int i=1;i<length+1; i++){
            char key[30];
            strncmp(key, word,1);
            if(btsel(complete_tree,key,value,200, &rsize)){
                int check = btins(complete_tree, key, word, 30);
            }
            else{
                strcpy(value, "");
                int check= btsel(complete_tree, key, value, sizeof(value), &rsize);
                if(strlen(value)<500){
                    strcat(value, "\n");
                    strcat(value, word);
                    btupd(complete_tree, key, value, strlen(value)+1);
                }
            }
        }
    }
    fclose(f);
    btcls(complete_tree);
}

int main(){
    int n, size, i;
    long tmpVal, *tmpPointer;
    char key[10];
    char ch;
    char nextKey[10];
    BTA * myDict;
    btinit();
    myDict = btopn("myDict", 0, 0);
    if (!myDict) {
        printf("create a book");
        myDict = btcrt("myDict", 0, 0);
        generateData(myDict);
    }
    do{
        printf("===========MiniProject==========\n");
        printf("|Options:                       |\n");
        printf("|1. Add a word                  |\n");
        printf("|2. Search a word               |\n");
        printf("|3. Deleta a word               |\n");
        printf("|4. Print the dict              |\n");
        printf("|5. Quit                        |\n");
        printf("================================\n");
        printf("Enter your choice:              \n");
        scanf("%d", &n); while((ch = getchar())!='\n' && ch != EOF);
        switch(n){
            case 1: // Add a word
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
                    printf("Insert a key to search\t");
                    i = 0;
                    // fflush(stdout);
                    // scanf("\n");
                    while(1) {
                        if (kbhit()) {
                            key[i] = getchar();
                            printf("%d", i);
                            i++;
                        } else break;
                    }
                printf("key is '%s', len is %ld\n", key, strlen(key));
                   btsel(myDict, key, (char*)&tmpVal, sizeof(long), &size);
		   while ( !btseln(myDict, nextKey, (char*)&tmpVal, sizeof(long), &size) )
            {
                if (memcmp(key, nextKey, strlen(key)) != 0) break;
                printf("key is \t %s\n", nextKey);
			    // printf("%s\t%10ld\n", nextKey, tmpVal);
            }	
                   break;
            case 5 :
                break;
            
        }
    }while(n!=5);
    btcls(myDict);
    return 0;
}