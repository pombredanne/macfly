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

#include <stdlib.h>      /* exit() */
#include <stdio.h>       /* perror() */
#include <unistd.h>      /* execvp() */
#include <limits.h>      /* PATH_MAX */
#include <libgen.h>      /* basename(), dirname() */
#include <string.h>      /* strcmp() */
#include <errno.h>       /* errno */
#include "config.h"

#undef LOG_PREFIX
#define LOG_PREFIX "[macfly] "

/*--------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
    int res;

    /* check arguments */
    if (argc < 2) {
        fprintf(stderr, "usage: %s program arg1 arg2 ...\n",
            argv[0]);
        exit(1);
    }

    /* wait using the mutex */
    res = setenv("LD_PRELOAD", "libdl.so " LIBRARY_SO, 1);
    if (res < 0)
    {
        perror("setenv() failure: unable to set LD_PRELOAD");
        exit(1);
    }

    /* try to execute the program */
    execvp(argv[1], &argv[1]);
    if ((errno != ENOENT) && (errno != ENOTDIR)) {
        perror(LOG_PREFIX "execv() failure");
    } else {
        fprintf(stderr, LOG_PREFIX "%s: command not found\n", argv[1]);
    }
    exit(1);
}

/*--------------------------------------------------------------------------*/

