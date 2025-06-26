#include <stdint.h>
#include <stdio.h>

#include <sys/time.h>
#include <unistd.h>

#include <lua.h>

#include "utils.h"


void
fprint_time(FILE *restrict p_stream)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    int64_t total_seconds = tv.tv_sec % 3600;

    int32_t
        milliseconds = tv.tv_usec / 1000,
        seconds      = total_seconds % 60,
        minutes      = total_seconds / 60;

    fprintf(p_stream, "%02d:%02d:%03d ", minutes, seconds, milliseconds);
}


const char *
lerrno(lua_State *p_lua)
{
    const char *err = lua_tostring(p_lua, -1);
    lua_pop(p_lua, 1);
    return err;
}


int32_t
l_atpanic(lua_State *p_lua)
{ ERR("Lua panic: %s", lerrno(p_lua)); return -1; }


bool
file_exist(const char *restrict p_file)
{ return access(p_file, F_OK) == 0; }
