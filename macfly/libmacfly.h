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

#ifndef LIBMACFLY_G
#define LIBMACFLY_G
/*--------------------------------------------------------------------------*/
#define _GNU_SOURCE      /* to get RTLD_NEXT */
#include <sys/un.h>      /* unix socket */
#include "config.h"
#include <stdint.h>      /* uint8_t */
/*--------------------------------------------------------------------------*/

#ifdef DEBUG
/*#  define DEBUG_WRAPPERS*/
#endif

/*--------------------------------------------------------------------------*/

typedef struct {
    int valid;
    struct sockaddr_un addr;
    unsigned int addrlen;
    int socket;
} network_t;

typedef struct __attribute__ ((packed)) {
    uint8_t is_negative;
    uint32_t sec;
    uint32_t nanosec;
} delta_t;

extern delta_t delta;

/*--------------------------------------------------------------------------*/

/* prototypes */
void fatal_error(char *message, ...);
void warning(char *message, ...);
void network_init();
void network_update();
void network_connect();
void* get_symbol(char *func_name, int fatal);
void init_wrappers();

/*--------------------------------------------------------------------------*/

#ifdef DEBUG
#  define DEBUG_MSG(...) warning(__VA_ARGS__)
#else
#  define DEBUG_MSG(...) /* nothing */
#endif

/*--------------------------------------------------------------------------*/
#endif   /* #ifndef LIBMACFLY_G */

