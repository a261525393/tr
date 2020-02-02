#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errmsg.h>
#include "mysql.h"
#include "inc.h"

MYSQL * instance = NULL;
const char * currSql = NULL;
const char * lastSuccessSql = NULL;

static void db_callback_set_int(char **row,unsigned int fieldCount,void *data){
    if(fieldCount > 0){
        int id = atoi(row[0]);
        memcpy(data,&id,sizeof(int));
    }
}

#define DB_SET_OPTIONS(_field_,_val_) \
    LOG_IF_FAIL(!mysql_options(instance,(_field_),&(_val_)),"mysql_options() set %s failed!\n",(_field_))

int db_connect(){
    int open = 1;
    DB_SET_OPTIONS(MYSQL_OPT_RECONNECT,open);
    DB_SET_OPTIONS(MYSQL_OPT_COMPRESS,open);
    instance = mysql_real_connect(instance,DB_HOST,DB_USER,DB_PASS,DB_NAME,DB_PORT,DB_SOCK_FILE,0);
    return instance ? 0 : 1;
}

#define DB_SYNC_RECONNECT() do{ LOG("reconnecting "); do{ LOG("."); sleep(1);}while(mysql_ping(instance)); LOG("connected!\n"); }while(0)

#define DB_IS_DISCONNECTED() \
    ({ \
     int _flag_ = mysql_errno(instance); \
     (_flag_ == CR_SERVER_LOST || _flag_ == CR_SERVER_GONE_ERROR); \
     })

#define DB_CHECK_RETURN_IF_FAIL()\
    do{ \
        int errCode = mysql_errno(instance);\
        if(errCode){\
            if(DB_QUERY_CHECK_LOG_IF_FAILED){ \
                LOG("db query [%s] error:%s\n",currSql,mysql_error(instance));\
            } \
            return errCode;\
        }\
    }while(0)


static int _db_mysql_query(const char *sql,db_query_callback callback,int onceStore,void *data){
    MYSQL_ROW row = NULL;
    RETURN_VAL_IF_FAIL(sql,ERR_DB_SQL_IS_NULL);

    int ret = db_mysql_exec(sql,1);
    if(ret) return ret;

    MYSQL_RES *res = onceStore ? mysql_store_result(instance) : mysql_use_result(instance);
    if(!res) DB_CHECK_RETURN_IF_FAIL();

    int fieldCount = mysql_num_fields(res);
    while(NULL != (row = mysql_fetch_row(res)))
        if(callback)  callback(row,fieldCount,data);

    if(res) mysql_free_result(res);
    if(!onceStore) DB_CHECK_RETURN_IF_FAIL();

    lastSuccessSql = sql;
    return ret;
}

int db_mysql_query(const char *sql,db_query_callback callback,int onceStore,void *data){
    int ret = 0;
start:
    ret = _db_mysql_query(sql,callback,onceStore,data);
    if(ret && DB_IS_DISCONNECTED()){
        DB_SYNC_RECONNECT();
        goto start;
    }
    return ret;
}


static int _db_mysql_exec(const char *sql){
    RETURN_VAL_IF_FAIL(sql,ERR_DB_SQL_IS_NULL);
    mysql_query(instance,sql);
    currSql = sql;
    DB_CHECK_RETURN_IF_FAIL();
    return 0;
}


int db_mysql_exec(const char *sql,int retryAfterReconnected){
    int ret = 0;
start:
    ret = _db_mysql_exec(sql);
    if(ret && retryAfterReconnected &&  DB_IS_DISCONNECTED()){
        DB_SYNC_RECONNECT();
        goto start;
    }
    return ret;
}

int db_mysql_get_last_insert_id(unsigned int *id){
    RETURN_VALID_PARAMS(id);
    const char *sql = "SELECT LAST_INSERT_ID();";
    return db_mysql_query(sql,db_callback_set_int,1,id);
}

int db_mysql_updated_row_count(unsigned int *count){
    RETURN_VALID_PARAMS(count);
    const char *sql = "SELECT ROW_COUNT();";
    return db_mysql_query(sql,db_callback_set_int,1,count);
}

void db_mysql_init(){
    instance = mysql_init(NULL);
    EXIT_IF_FAIL(instance,"function mysql_init failed!");
    EXIT_IF_FAIL(0==db_connect(),"connect db failed!");
    mysql_set_character_set(instance,DB_CHARSET);
}

int db_mysql_get_id(const char *sql,unsigned int *id){
    return db_mysql_query(sql,db_callback_set_int,1,id);
}

void db_mysql_close(){
    if(instance){
        mysql_close(instance);
    }
}
