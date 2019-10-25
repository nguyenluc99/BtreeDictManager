#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "btree.h"

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
    int n;
    do{
        printf("===========MiniProject==========\n");
        printf("|Options:                      |\n");
        printf("|1.                            |\n");
        printf("|2.                            |\n");
        printf("|3.                            |\n");
        printf("|4.                            |\n");
        printf("================================\n");
        printf("Enter your choice:              \n");
        scanf("%d", &n);
        while(getchar()!='\n');
        switch(n){
            case 1:
            break;
            case 2:
            break;
            case 3:
            break;
            
        }
    }while(n!=4);
    return 0;
}