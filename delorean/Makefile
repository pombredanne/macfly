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

PYTHON=python
PYVERSION=$(shell $(PYTHON) -c "from sys import version_info; print 'python%s.%s' % (version_info[0], version_info[1])")
SITEPACKAGE=/usr/lib/$(PYVERSION)/site-packages
all:
	$(PYTHON) ./setup.py build
install:
	$(PYTHON) ./setup.py install
uninstall:
	rm -f /usr/bin/deloreand
	rm -f /usr/bin/docbrown
	rm -f $(SITEPACKAGE)/macfly-*.egg-info
	rm -rf $(SITEPACKAGE)/macfly
clean:
	rm -rf build

