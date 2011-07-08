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

DEBUG=0
ifeq ($(DEBUG), 1)
    CFLAGS=-Wall -Wextra -g -O0
    STRIP=0
else
    CFLAGS=-Wall -Wextra -O2
    STRIP=1
endif
LIBS=-lpthread
OWNER=-o root -g root
MODE_EXE=-m 0555
MODE_SO=-m 0444
OBJECTS=libmacfly.o network.o wrapper.o
PREFIX=/usr/
BINDIR=$(PREFIX)bin

# Library directory
LIBDIR=$(shell rpm --eval %_libdir 2>/dev/null)
ifeq ($(LIBDIR),)
	LIBDIR=$(PREFIX)lib
endif

all: libmacfly.so macfly
install: libmacfly.so macfly
	install $(MODE_EXE) $(OWNER) macfly $(BINDIR)
	install $(MODE_SO) $(OWNER) libmacfly.so $(LIBDIR)
uninstall:
	rm $(BINDIR)/macfly
	rm $(LIBDIR)/libmacfly.so
%.o: %.c config.h
	gcc -o $@ -c $< -fPIC $(CFLAGS)
libmacfly.so: $(OBJECTS)
	gcc -o $@ libmacfly.o network.o wrapper.o -shared -fPIC $(CFLAGS)
ifeq ($(STRIP), 1)
	strip $@
endif
macfly: macfly.c config.h
	gcc -o $@ $< $(CFLAGS) $(LIBS)
ifeq ($(STRIP), 1)
	strip $@
endif
clean:
	rm -f $(OBJECTS) libmacfly.so macfly

