#include <stdbool.h>
#include <stdlib.h>
#include <proto.h>
#include <string.h>
#include <stdio.h>
#include <time_util.h>
#include <thread_exec.h>
#include <comunicacao.h>
#include <stdarg.h>
#include <shared_util.h>

char BUFF[10000];
int GLOBAL_LOG_LEVEL = 0;

void set_level(int level)
{
    GLOBAL_LOG_LEVEL = level;
}

void log_print(char *str, int level)
{
    if (level >= GLOBAL_LOG_LEVEL)
    {
        printf("%s", str);
        fflush(NULL);
    }
}

char *itoa(int x)
{
    char *buff = (char *)malloc(sizeof(char) * 20);
    sprintf(buff, "%d", x);
    return buff;
}

// if index is not present, may set index to a negative number
char *read_env_str_index(char *name, int index)
{
    char key[10000];
    if (index < 0)
    {
        sprintf(key, "%s", name);
    }
    else
    {
        sprintf(key, "%s%d", name, index);
    }

    char *val = getenv(key);
    if (val == NULL)
    {
        sprintf(BUFF, "[read_env_str_index] NO KEY FOR ENV VAR '%s'\n", key);
        log_print(BUFF, 1);
    }
    sprintf(BUFF, "[read_env_str_index] KEY VAL '%s = %s'\n", key, val);
    log_print(BUFF, 0);
    return val;
}

// if index is not present, may set index to a negative number
int read_env_int_index(char *name, int index)
{
    char *val = read_env_str_index(name, index);
    return atoi(val);
}

char *get_env()
{
    return read_env_str_index("ENV", -1);
}

int is_dev()
{
    return strcmp("DEV", get_env()) == 0;
}

int is_dev_test()
{
    return strcmp("DEV_TEST", get_env()) == 0;
}

int is_prod()
{
    return !is_dev() && !is_dev_test();
}

// Sets all headers such that:
// TEST_MODE =
void set_env()
{
    if (is_dev())
    {
        log_print("environment is dev", LEVEL_INFO);
#define TEST_MODE 0
    }
    else if (is_dev_test())
    {
        log_print("environment is dev_test", LEVEL_INFO);
#define TEST_MODE 1
    }
    else if (is_prod())
    {
        log_print("environment is prod", LEVEL_INFO);
#define TEST_MODE 0
    }
    else
    {
        log_print("[ERROR] [SHARED_UTIL] ENVIRONMENT NOT RECOGNIZED\n", LEVEL_ERROR);
        exit(1);
    }
}