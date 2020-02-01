#include <stdio.h>
#include "help.h"


void help(command_e cmd){
    switch(cmd){
        case CMD_TASK_ADD:
            printf("eg: tr task_add name parent_id \nname:\ttask name\nparent_id:\tparent task id default is 0\n");
            break;
        case CMD_TASK_QUERY:
            printf("eg: tr task_query name|id\n");
            break;
        default:
            printf("unknown command!\n");
    }

}
