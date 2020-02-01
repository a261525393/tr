#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include "err.h"

#define LOG(fmt,...) \
    do{ \
        fprintf(stdout,"%s(%d)\t",__FILE__,__LINE__);\
        fprintf(stdout,(fmt),##__VA_ARGS__);\
    }while(0)

#define RETURN_VALID_PARAMS(expr) \
    do{ \
        if(!(expr)){ \
            LOG("valid param (%s) failed!\n",#expr); \
            return ERR_PARAMS; \
        } \
    }while(0)

#define RETURN_IF_FAIL(expr,val) if(!(expr)) return (val)

#define EXIT_IF_FAIL(expr,fmt,...) \
    do{ \
        if(!(expr)){ \
            LOG((fmt),##__VA_ARGS__); \
            exit(1); \
        } \
    }while(0)

#define LOG_IF_FAIL(expr,fmt,...) if(!(expr)) LOG((fmt),##__VA_ARGS__)
#define LOG_RETURN_IF_FAIL(expr,ret,fmt,...) \
    do{ \
        if(!(expr)){ \
            LOG((fmt),##__VA_ARGS__); \
            return (ret); \
        } \
    }while(0)

#endif
