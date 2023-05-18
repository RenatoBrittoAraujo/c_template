#include <shared/inc/errors.h>
#include <stdio.h>
#include <string.h>

// exits with error code
error fatal_error(error err_num, char *err_msg)
{
    int size = strlen(err_msg);
    char buff[size + 50];
    sprintf(buff, "fatal error (code: %d): %s", err_num, err_msg);
    if (err_msg[size - 1] != '\n')
        strcat(buff, "\n");
    log_print(buff, LEVEL_ERROR);
    exit(err_num);
}

error handle_error(error err_num, char *err_msg)
{
    int size = strlen(err_msg);
    char buff[size + 50];
    sprintf(buff, "error (code: %d): %s", err_num, err_msg);
    if (err_msg[size - 1] != '\n')
        strcat(buff, "\n");
    log_print(buff, LEVEL_ERROR);
    exit(err_num);
}