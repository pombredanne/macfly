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

#include <sys/socket.h>  /* socket() */
#include <arpa/inet.h>   /* ntohl() */
#include <unistd.h>      /* close() */
#include <errno.h>       /* errno */
#include "libmacfly.h"

static network_t network;

/**
 * Initialize network
 */
void network_init()
{
    /* invalid state and socket */
    network.valid = 0;
    network.socket = -1;

    /* create address */
    network.addr.sun_family = AF_UNIX;
    strncpy(network.addr.sun_path, SOCKET_FILENAME, sizeof(network.addr.sun_path));
    network.addr.sun_path[sizeof(network.addr.sun_path)-1] = 0;
    network.addrlen = sizeof(network.addr.sun_family) + strlen(network.addr.sun_path);
}

/**
 * Close network socket
 */
void network_close()
{
    if (0 <= network.socket) {
        close(network.socket);
        network.socket = -1;
    }
    network.valid = 0;
}

/**
 * Connect socket to time daemon
 */
void network_connect()
{
    int res;

    if (network.valid) {
        return;
    }

    /* create socket */
    if (network.socket < 0) {
        network.socket = socket(AF_UNIX, SOCK_STREAM, 0);
        if (network.socket == -1) {
            fatal_error("socket creation error");
        }
    }

    /* connect socket */
    res = connect(network.socket, (struct sockaddr *)&network.addr, network.addrlen);
    if (res == -1) {
        DEBUG_MSG("socket connection error: %s", strerror(errno));
        return;
    }
    network.valid = 1;
}

/**
 * Update timedelta from socket
 */
void network_update()
{
    int res;
    delta_t buffer, new_delta;
    unsigned char has_new;
    struct timeval timeout;
#ifdef DEBUG
    unsigned int skip = 0;
#endif

    /* try to connect to server if network is not available */
    if (!network.valid) {
        network_connect();
        if (!network.valid) {
            return;
        }
        DEBUG_MSG("restore connection to server");
    }

    new_delta.is_negative= 0;
    new_delta.sec= 0;
    new_delta.nanosec = 0;
    has_new = 0;
    while (1)
    {
        /* check is socket has something for us */
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        fd_set read;
        FD_ZERO(&read);
        FD_SET(network.socket, &read);
        res = select(network.socket+1, &read, NULL, NULL, &timeout);
        if (res < 0) {
            /* socket error */
            DEBUG_MSG("Socket error on select(): %s", strerror(errno));
            network_close();
            break;
        }
        if (res == 0) {
            /* no data on socket */
            break;
        }

        /* read socket */
        res = recv(network.socket, &buffer, sizeof(buffer), 0);
        if (res <= 0) {
            /* socket error */
            DEBUG_MSG("Socket error on recv() (lost connection?)");
            network_close();
            break;
        }
        if (res != sizeof(buffer)) {
            /* not enough data (buffer underflow) */
            DEBUG_MSG("recv() error: get %i instead of %u bytes!",
                    res, sizeof(buffer));
            break;
        }
#ifdef DEBUG
        if (has_new) {
            skip += 1;
        }
#endif
        has_new = 1;
        new_delta = buffer;
    }
    if (!has_new) {
        return;
    }
#ifdef DEBUG
    if (0 < skip) {
        DEBUG_MSG("Skip %u update", skip);
    }
#endif

    /* update time delta */
    if (new_delta.is_negative == 1) {
        delta.is_negative = 1;
    } else {
        delta.is_negative = 0;
    }
    delta.sec = ntohl(new_delta.sec);
    delta.nanosec = ntohl(new_delta.nanosec);
    DEBUG_MSG("New diff: negative=%u  sec=%u  nanosec=%u",
        (unsigned)delta.is_negative, delta.sec, delta.nanosec);
}


