#ifndef _TASK_H_
#define _TASK_H_

#include "db.h"

typedef struct task_s{
    unsigned int id;
    char name[64];
    unsigned int parentID;
    unsigned int createdAt;
}task_t;

int task_add(const char *name,unsigned int parentID);
int task_get_by_id(const char *name);
int task_get_by_name(const char *name,db_query_callback callback);
int task_get_running(db_query_callback OnRunningCallback);
int task_start(int id);
int task_end();
#endif
