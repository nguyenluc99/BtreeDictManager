#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "btree.h"

int main(){
     char *file ="init_data";
    btinit();
    BTA *init_data;
    init_data = btopn(file,0,0);

    if(!init_data)
        init_data= btcrt(file, 0,0);
    FILE *f = fopen("/usr/share/dict/words","r");

    char word[20];
    //char value[600];
    int rsize;
    char buf[50];
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
    return 0;
}