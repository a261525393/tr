#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "task.h"
#include "help.h"

void out_task(char **row,unsigned int fieldCount,void *data){
    if(fieldCount < 4) return;
    printf("%s\t%s\t%s\t%s\n",row[0],row[1],row[2],row[3]);
}

static int taskRunningCount = 0;
void out_running_task(char **row,unsigned int fieldCount,void *data){
    taskRunningCount++;
    if(fieldCount < 4) return;
    printf("%s\t%s\t%s\t%s\n",row[0],row[1],row[2],row[3]);
}

#define VALID_COMMAND(expr,cmd) \
    do{ \
        if(!(expr)){ \
            help(cmd);\
            return;\
        }\
    }while(0)

void run(int argc,char *argv[]){
    int ret = 0;
    unsigned int id = 0;
    if(0 == strcmp(argv[1],"add")){
        VALID_COMMAND(argc >= 3,CMD_TASK_ADD);
        ret = task_add(argv[2],argc >= 4 ? atoi(argv[3]) : 0);
        printf("%s!\n", ret ? "failed" : "success");
        if(!db_mysql_get_last_insert_id(&id)){
            printf("id = %d\n",id);
        }
    }else if(0 == strcmp(argv[1],"query")){
        VALID_COMMAND(argc >= 3,CMD_TASK_QUERY);
        task_get_by_name(argv[2],out_task);
    }else if(0 == strcmp(argv[1],"start")){
        VALID_COMMAND(argc >= 3,CMD_TASK_START);
        if(0 == task_get_running(out_running_task)){
            if(0 == taskRunningCount){
                if(0 == task_start(atoi(argv[2]))){
                    printf("success!");
                    if(!db_mysql_get_last_insert_id(&id)){
                        printf("id = %d\n",id);
                    }
                }
            }else{
                printf("%d tasks is running!\n",taskRunningCount);
            }
        }

    }else if(0 == strcmp(argv[1],"end")){
        if(0 == task_end()){
            printf("success!\n");
            unsigned int rowCount = 0;
            if(0 == db_mysql_updated_row_count(&rowCount)){
                printf("updated %d rows;\n",rowCount);
            }
        }

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
