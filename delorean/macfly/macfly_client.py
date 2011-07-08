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

from socket import socket, AF_UNIX, SOL_SOCKET, SO_REUSEADDR, \
    error as socket_error
from os import unlink
from errno import ENOENT
from select import select
from macfly.clients import Macfly
from macfly.stoppable_thread import StoppableThread

# UNIX socket filename
SOCKET_FILENAME = "/tmp/delorean.plutonium"

class MacflySocket(StoppableThread):
    """
    UNIX socket used by deloreand to communicate with libmacfly
    """
    def __init__(self, server):
        StoppableThread.__init__(self)
        self.socket = socket(AF_UNIX)
        self.socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        self.filename = SOCKET_FILENAME
        self.request_queue_size = 5
        self._unlink()
        self.server = server

    def setup(self):
        self.socket.bind(self.filename)
        self.socket.listen(self.request_queue_size)

    def run(self):
        wait = select([self.socket], [], [], 0.250)[0]
        if not wait:
            return
        socket, addr = self.socket.accept()
        client = Macfly(socket)
        self.server.addMacfly(client)

    def deinit(self):
        self.socket.close()
        self._unlink()

    def _unlink(self):
        try:
            unlink(self.filename)
        except OSError, err:
            if err[0] == ENOENT:
                return
            raise

