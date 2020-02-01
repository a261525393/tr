#ifndef _HELP_H_
#define _HELP_H_

typedef enum{
    CMD_TASK_ADD = 1,
    CMD_TASK_QUERY = 2,
    CMD_TASK_START = 3
}command_e;

void help(command_e cmd);

#endif
