#ifndef SHARED_ERRORS_H
#define SHARED_ERRORS_H 1

#include <stdlib.h>

#include <shared/inc/shared_util.h>

typedef unsigned long error;

error fatal_error(error err_num, char *err_msg);
error handle_error(error err_num, char *err_msg);

#endif
