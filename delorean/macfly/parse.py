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
Docbrown function to parse user input (time deltas).
"""

from datetime import date, datetime, timedelta
from sys import stderr

today = date.today()
last_date = (today.year, today.month, today.day)

UNITS = {
    "day": (6, "days"),
    "hour": (5, "hours"),
    "min": (4, "minutes"),
    "sec": (3, "seconds"),
    "ms": (2, "milliseconds"),
    "us": (1, "microseconds"),
}

def parseDatetime(text):
    """
    Parse time, date or datetime.

    >>> parseDatetime("1970/1/1 00:00")
    datetime.datetime(1970, 1, 1, 0, 0)
    >>> parseDatetime("00:00:10")
    [+] Reuse last date: 1970/1/1
    datetime.datetime(1970, 1, 1, 0, 0, 10)
    >>> parseDatetime("00:02")
    [+] Reuse last date: 1970/1/1
    datetime.datetime(1970, 1, 1, 0, 2)
    >>> parseDatetime("1970/1/2")
    datetime.datetime(1970, 1, 2, 12, 0)
    """
    global last_date
    datestr = hourstr = None
    if ":" in text:
        if "/" in text:
            datestr, hourstr = text.split(" ", 1)
        else:
            hourstr = text
    else:
        datestr = text

    # Get date
    if datestr:
        try:
            year, month, day = [ int(item) for item in datestr.split("/") ]
        except ValueError:
            print >>stderr, '[!] Invalid date: %s' % datestr
            return None
    else:
        print "[+] Reuse last date: %s/%s/%s" % last_date
        year, month, day = last_date

    # Get hour
    if hourstr:
        try:
            data = [ int(item) for item in hourstr.split(":") ]
            if len(data) == 2:
                hour, minute = data
                sec = 0
            else:
                hour, minute, sec = data
        except ValueError:
            print >>stderr, '[!] Invalid hour: %s' % hourstr
            return None
    else:
        hour = 12
        minute = 0
        sec = 0

    # Convert data to Epoch (number of seconds)
    try:
        want_time = datetime(year, month, day, hour, minute, sec)
    except ValueError, err:
        print >>stderr, '[!] Invalid date: %s' % err
        return None

    # Store date
    last_date = year, month, day
    return want_time

def parseUnits(text):
    """
    Parse delta in form: key=value

    >>> parseUnits("day=1 sec=7")
    datetime.timedelta(1, 7)
    >>> parseUnits("hour=1 sec=-1")
    datetime.timedelta(0, 3599)
    """
    items = text.split()
    assert 1 <= len(items)
    values = {}
    for item in items:
        try:
            key, value = item.split("=", 1)
            unit = UNITS[key][1]
            value = int(value)
        except (KeyError, ValueError):
            print >>stderr, '[!] Invalid input: %s' % item
            return None
        if unit in values:
            print >>stderr, "[!] Don't use key %s twice" % key
            return None
        values[unit] = value
    try:
        return timedelta(**values)
    except OverflowError, err:
        print >>stderr, "[!] Delta is too large (%s)" % err
        return None

def parseDelta(text):
    if "/" in text or ":" in text:
        want_time = parseDatetime(text)
        if want_time is None:
            return False, None
        delta = want_time - datetime.today()
        return False, delta
    else:
        return True, parseUnits(text)

def timedeltaFactor(delta, factor):
    day = delta.days * factor
    sec = delta.seconds * factor
    us  = delta.microseconds * factor
    return timedelta(day, sec, us)

if __name__ == "__main__":
    import doctest
    doctest.testmod()

