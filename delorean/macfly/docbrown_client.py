# Copyright(C) 2007 INL
# Written by Victor Stinner <victor.stinner AT inl.fr>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

from socket import socket, AF_INET, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR
from select import select
from os import kill
from signal import SIGTERM
from macfly.stoppable_thread import StoppableThread
from macfly.common import PORT, DOCBROWN_HELLO
from macfly.clients import Docbrown

# Limit each client request to N bytes
DOCBROWN_MAX_LENGTH = 200

class DockbrownSocket(StoppableThread):
    """
    TCP socket used by deloreand to communicate with docbrown
    """
    def __init__(self, server):
        StoppableThread.__init__(self)
        self.server = server
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        self.port = PORT
        self.clients = {}

    def setup(self):
        self.socket.bind(('', self.port))
        self.socket.listen(1)

    def deinit(self):
        self.socket.close()

    def run(self):
        read_fds = [self.socket]
        if self.clients:
            read_fds += [client.socket for client in self.clients.itervalues()]
        wait = select(read_fds, [], [], 0.250)[0]
        if not wait:
            return
        for sock in wait:
            if sock == self.socket:
                newsock, addr = self.socket.accept()
                client = Docbrown(newsock, addr)
                self.acceptDocbrown(client)
            else:
                client = self.clients[hash(sock)]
                self.activityClient(client)

    def acceptDocbrown(self, client):
        # Read client "hello"
        hello = client.recv(len(DOCBROWN_HELLO))

        # Reject invalid client
        if hello != DOCBROWN_HELLO:
            return

        # Add new client
        self.clients[hash(client.socket)] = client

    def activityClient(self, client):
        # Read data
        data = client.recv(DOCBROWN_MAX_LENGTH)

        # Client disconnected?
        if data == '':
            self.disconnectClient(client)
            return

        if data == "quit":
            pid = self.server.pid
            client.send("Stop server (pid %s)" % pid)
            kill(pid, SIGTERM)
        else:
            try:
                ok = self.server.command(data, False)
            except ValueError:
                ok = False
            except Exception, err:
                print "ERROR", err
                ok = False
            if ok:
                client.send("Current delta: %s" % self.server.time_delta)
            else:
                client.send("Error")
        self.disconnectClient(client)

    def disconnectClient(self, client):
        del self.clients[hash(client.socket)]

