#!/usr/bin/env python

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
Test program: just display current time but also "clock jump" (DIFF).
"""

from time import time, sleep
from datetime import datetime, timedelta
try:
    pause = 1.0
    onesec = timedelta(seconds=pause * 1.6)
    lastd = datetime.now()
    lastt = time()
    while True:
        d = datetime.now()
        t = time()
        if True:
            diffd = d - lastd
            difft = timedelta(seconds=(t - lastt))
            if onesec < abs(diffd) or onesec < abs(difft):
                print "  DIFF datetime:%s -- time:%s" % (diffd, difft)
        print "datetime:%s  -- us=%06s --  time: %s" % (d, d.microsecond, t)
        sleep(pause)
        lastd = d + timedelta(seconds=pause)
        lastt = t + pause
except KeyboardInterrupt:
    print "Interrupt."
