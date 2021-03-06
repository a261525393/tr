#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "inc.h"

static const char *sqlTaskAddFmt = "INSERT INTO tasks (name,parent_id,created_at) VALUES ('%s',%d,%d);";
int task_add(const char *name,unsigned int parentID){
    RETURN_VALID_PARAMS(name);
    RETURN_VALID_PARAMS(parentID >= 0);
    int size = strlen(name) + strlen(sqlTaskAddFmt) + 16;
    char *sql = (char *)calloc(size,1);
    if(NULL == sql) return ERR_MEM_MALLOC;
    memset(sql,0,size);
    snprintf(sql,size-1,sqlTaskAddFmt,name,parentID,time(NULL));
    return db_mysql_exec(sql,1);
}

static const char *sqlTaskGetByNameFmt = "SELECT id,name,parent_id,created_at FROM tasks WHERE name like '%%%s%%';";
int task_get_by_name(const char *name,db_query_callback callback){
    RETURN_VALID_PARAMS(name);
    int size = strlen(name) + strlen(sqlTaskGetByNameFmt) + 1;
    char *sql = (char *)calloc(size,1);
    if(NULL == sql) return ERR_MEM_MALLOC;
    memset(sql,0,size);
    snprintf(sql,size-1,sqlTaskGetByNameFmt,name);
    //    printf("name = %s\tsql = %s\tsize = %d\n",name,sql,size); 
    return db_mysql_query(sql,callback,0,NULL);
}

static const char *sqlTaskRunning = "SELECT r.id rid,t.id tid,t.name,r.start_at FROM records r LEFT JOIN tasks t ON t.id=r.task_id WHERE start_at AND end_at = 0";
int task_get_running(db_query_callback OnRunningCallback){
    return db_mysql_query(sqlTaskRunning,OnRunningCallback,1,NULL);
}

static const char *sqlStartTaskFmt = "INSERT INTO records (task_id,start_at) VALUES (%d,%d);";
int task_start(const char *param){
    RETURN_VALID_PARAMS(param);
    char sql[128] = {0};
    int id = 0;
    if(0 != str2int(param,&id)){
//        printf("param = [%s]\t result = %d\tid = %d\n",param,str2int(param,&id),id);
        snprintf(sql,127,"SELECT id FROM tasks WHERE name = '%s'",param);
        RETURN_VAL_IF_FAIL(0==db_mysql_get_id(sql,(unsigned int *)&id),-1);
        if(id == 0){
            RETURN_VAL_IF_FAIL(0==task_add(param,0),-1);
            RETURN_VAL_IF_FAIL(0==db_mysql_get_last_insert_id((unsigned int *)&id),-1);
        }
    }
    RETURN_VAL_IF_FAIL(id>0,1);
    snprintf(sql,127,sqlStartTaskFmt,id,time(NULL));
    return db_mysql_exec(sql,1);
}

static const char *sqlEndTaskFmt = "UPDATE records SET end_at = %d WHERE start_at > 0 AND end_at = 0;";
int task_end(){
    char sql[128] = {0};
    snprintf(sql,127,sqlEndTaskFmt,time(NULL));
    return db_mysql_exec(sql,1);
}

int task_get_by_id(const char *name){

    return 0;
}
