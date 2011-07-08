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

/**
 * Dummy benchmark script. Use it with 'time' UNIX program.
 *
 * $ gcc benchmark.c -o benchmark
 * $ time ./benchmark
 * Call time() 1000000 times
 *
 * real    0m2.117s
 * user    0m0.320s
 * sys     0m1.696s
 *
 * $ time ./macfly ./benchmark
 * Call time() 1000000 times
 *
 * real    0m4.193s
 * user    0m0.532s
 * sys     0m3.628s
 */

#include <time.h>
#include <stdio.h>

const unsigned int COUNT = 10*1000*1000;

int main()
{
    unsigned int i;
    printf("Call time() %u times\n", COUNT);
    for (i=0; i<COUNT; i++) time(NULL);
    return 0;
}

