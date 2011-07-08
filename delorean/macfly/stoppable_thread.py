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

"""
Code pattern to be able to stop a thread
"""

from thread import allocate_lock, start_new_thread

class StoppableThread:
    def __init__(self):
        self._run_lock = allocate_lock()
        self._stop_lock = allocate_lock()

    def start(self):
        start_new_thread(self._threadFunc, tuple())

    def _threadFunc(self):
        self._run_lock.acquire()
        try:
            try:
                while self._stop_lock.acquire(0):
                    self._stop_lock.release()
                    self.run()
            except Exception, err:
                self.errorHandler(err)
        finally:
            self._run_lock.release()

    def stop(self):
        self._stop_lock.acquire()
        self._run_lock.acquire()
        self._run_lock.release()
        self._stop_lock.release()
        self.deinit()

    #--- Abstract methods -------------------

    def run(self):
        """
        Main code of the thread: have to be faster than one second
        to be able to stop the thread.
        """
        raise NotImplementedError()

    def errorHandler(self, err):
        print "THREAD ERROR (%s): %s" % (err.__class__.__name__, err)

    def deinit(self):
        pass

