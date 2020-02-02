#ifndef _DB_H_
#define _DB_H_

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "admin"
#define DB_PORT 3306
#define DB_SOCK_FILE "/tmp/mysql.sock"
#define DB_NAME "tr"
#define DB_CHARSET "utf8mb4"
#define DB_QUERY_CHECK_LOG_IF_FAILED 1

#define ERR_DB_SQL_IS_NULL 100001



void db_mysql_init();
void db_mysql_close();

typedef void (*db_query_callback)(char **row,unsigned int fieldCount,void *data);
int db_mysql_query(const char *sql,db_query_callback callback,int onceStore,void *data);
int db_mysql_exec(const char *sql,int retryAble);
int db_mysql_get_last_insert_id(unsigned int *id);
int db_mysql_updated_row_count(unsigned int *count);

int db_mysql_get_id(const char *sql,unsigned int *id);

#endif
