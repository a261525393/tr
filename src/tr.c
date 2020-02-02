#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc.h"

void run(int argc,char *argv[]){
    if(0 == strcmp(argv[1],"add")){
        cmd_add(argc,argv);
    }else if(0 == strcmp(argv[1],"query")){
        cmd_query(argc,argv);
    }else if(0 == strcmp(argv[1],"start")){
        cmd_start(argc,argv);
    }else if(0 == strcmp(argv[1],"end")){
        cmd_end(argc,argv);
    }else{
        help(0);
    }
}

int main(int argc,char *argv[]){
    if(argc <= 1) { help(0); exit(1);}
    db_mysql_init();
    run(argc,argv);
    db_mysql_close();
    return 0;
}
