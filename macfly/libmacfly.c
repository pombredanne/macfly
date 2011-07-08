/*
 ** Copyright(C) 2007 INL
 ** Written by Victor Stinner <victor.stinner AT inl.fr>
 **
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, version 2 of the License.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "libmacfly.h"
#include <stdarg.h>   /* va_list */
#include <stdio.h>    /* fprintf() */
#include <stdlib.h>   /* exit() */
#include <dlfcn.h>    /* dlsym() */
/*--------------------------------------------------------------------------*/
delta_t delta;
/*--------------------------------------------------------------------------*/

/**
 * Constructor called when library is loaded in memory
 * (at started and only once)
 */
void __attribute__((constructor)) constructor()
{
    network_init();
    memset(&delta, 0, sizeof(delta));
    network_connect();
    init_wrappers();
}

/*--------------------------------------------------------------------------*/

/**
 * Display message to stderr with a prefix, and then flush stderr
 */
void log_message(char *prefix, char *message, va_list args)
{
    fprintf(stderr, LOG_PREFIX "%s: ", prefix);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    fflush(stderr);
}

/**
 * Display error message
 */
void warning(char *message, ...)
{
    va_list args;
    va_start(args, message);
    log_message("WARNING", message, args);
    va_end(args);
}

/**
 * Fatal error: display a message and exit with code=1
 */
void fatal_error(char *message, ...)
{
    va_list args;
    va_start(args, message);
    log_message("FATAL ERROR", message, args);
    va_end(args);
    exit(1);
}

/**
 * Load a symbol using dlsym(RTLD_NEXT)
 */
void* get_symbol(char *func_name, int fatal)
{
    void *func;
    func = dlsym(RTLD_NEXT, func_name);
    if (!func) {
        if (fatal) {
            fatal_error(
                    "Unable to import function \"%s()\" using dlsym(RTLD_NEXT)!",
                    func_name);
        } else {
            DEBUG_MSG(
                    "Unable to import function \"%s()\" using dlsym(RTLD_NEXT)",
                    func_name);
        }
    }
    return func;
}

/*--------------------------------------------------------------------------*/

