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

DOCBROWN_TIMEOUT = 0.500

class Client:
    def __init__(self, socket):
        self.socket = socket

    def __del__(self):
        self.socket.close()

    def send(self, data):
        self.socket.send(data)

class Macfly(Client):
    def __init__(self, socket):
        Client.__init__(self, socket)
        self.socket.setblocking(0)

class Docbrown(Client):
    def __init__(self, socket, address):
        Client.__init__(self, socket)
        self.socket.settimeout(DOCBROWN_TIMEOUT)
        self.address = address

    def recv(self, size):
        assert 1 <= size
        return self.socket.recv(size)

    def __repr__(self):
        return "<Client socket=%r, address=%s>" % (self.socket, str(self.address))

