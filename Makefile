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

SUBDIRS=macfly delorean tests
PYTHON=python
MAKEALL=
all:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir || break; done
	$(call MAKECALL ;)
install:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir install || break; done
uninstall:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir uninstall || break; done
clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done

