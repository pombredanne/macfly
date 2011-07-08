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

/*
 * Test program, just call some time functions to test macfly programs.
 */

#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h> /* sigaction() */
#include <string.h> /* memset() */

#define STEP usleep(1000*1000)

int running = 1;
struct sigaction old_sigint_hdl;
struct sigaction old_sigterm_hdl;

void sigint(int __attribute__((unused)) signum)
{
    /* restore original handler */
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = sigint;
    (void)sigaction(SIGINT, &old_sigint_hdl, NULL);
    (void)sigaction(SIGTERM, &old_sigterm_hdl, NULL);

    /* stop main process */
    fflush(stdout);
    printf("Stop!\n");
    fflush(stdout);
    running = 0;
}

int main()
{
    time_t time_res;
    struct timeb timeb_res;
    struct timeval timeval_res;
    struct sigaction action;

    /* catch SIGINT and SIGTERM */
    memset(&action, 0, sizeof(action));
    action.sa_handler = sigint;
    (void)sigaction(SIGTERM, &action, &old_sigterm_hdl);
    (void)sigaction(SIGINT, &action, &old_sigint_hdl);

    time_res = time(NULL);
    printf("t=time(NULL); t=%lu\n", time_res);
    STEP;

    time(&time_res);
    printf("time(&t); t=%lu\n", time_res);
    STEP;

    ftime(&timeb_res);
    printf("ftime(&tb); tb.time=%lu\n", timeb_res.time);
    STEP;

    gettimeofday(&timeval_res, NULL);
    printf("gettimeofday(&tv, NULL); tv.tv_sec=%lu\n", timeval_res.tv_sec);

    while (running)
    {
        printf("time() >> %lu\n", time(NULL));
        sleep(1);
    }

    return 0;
}

