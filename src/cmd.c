#include <stdio.h>
#include "inc.h"

#define VALID_COMMAND(expr,cmd) \
    do{ \
        if(!(expr)){ \
            help(cmd);\
            return;\
        }\
    }while(0)

void cmd_add(int argc,char *argv[]){
    VALID_COMMAND(argc >= 3,CMD_TASK_ADD);
    if( 0 == task_add(argv[2],argc >= 4 ? atoi(argv[3]) : 0)){
        LOG("success\n");
        unsigned int id = 0;
        if(!db_mysql_get_last_insert_id(&id)){
            LOG("id = %d\n",id);
        }
    }else{
        LOG("failed!\n");
    }
}

void out_task(char **row,unsigned int fieldCount,void *data){
    if(fieldCount < 4) return;
    printf("%s\t%s\t%s\t%s\n",row[0],row[1],row[2],row[3]);
}

void cmd_query(int argc,char *argv[]){
    VALID_COMMAND(argc >= 3,CMD_TASK_QUERY);
    task_get_by_name(argv[2],out_task);
}

static int taskRunningCount = 0;
void out_running_task(char **row,unsigned int fieldCount,void *data){
    taskRunningCount++;
    if(fieldCount < 4) return;
    printf("%s\t%s\t%s\t%s\n",row[0],row[1],row[2],row[3]);
}

void cmd_start(int argc,char *argv[]){
    VALID_COMMAND(argc >= 3,CMD_TASK_START);
    RETURN_IF_FAIL(0==task_end());
    if(0 == task_start(argv[2])){
        LOG("success!\n");
        unsigned int id = 0;
        if(!db_mysql_get_last_insert_id(&id)){
            LOG("id = %d\n",id);
        }
    }
}


void cmd_end(int argc,char *argv[]){

    if(0 == task_end()){
        LOG("success!\n");
        unsigned int rowCount = 0;
        if(0 == db_mysql_updated_row_count(&rowCount)){
            LOG("updated %d rows;\n",rowCount);
        }
    }else{
        LOG("failed!\n");
    }
}


