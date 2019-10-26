#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "btree.h"


void init_data1 (BTA*init_data,char* buf){
     char *file ="init_data";
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
        binsky(init_data,(char*)buf, value);
        btsel(init_data,"",(char*)&value,sizeof(int),&rsize);
        if(!btsel(init_data, buf,(char*) &value, sizeof(int), &rsize))
        {
            printf("%s", buf);
        }
    }
    fclose(f);
    btcls(init_data);
}
int main(){
    int n;
    do{
        printf("===========MiniProject==========\n");
        printf("|Options:                      |\n");
        printf("|1.Print all Dictionary        |\n");
        printf("|2.                            |\n");
        printf("|3.                            |\n");
        printf("|4.                            |\n");
        printf("================================\n");
        printf("Enter your choice:              \n");
        scanf("%d", &n);
        while(getchar()!='\n');
        switch(n){
            case 1:
            btinit();
            BTA *tree;
            char *str;
            init_data1(tree,str);
            break;
            case 2:
            break;
            case 3:
            break;
            
        }
    }while(n!=4);
    return 0;
}