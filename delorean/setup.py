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

import sys
if "--setuptools" in sys.argv:
    sys.argv.remove("--setuptools")
    from setuptools import setup
else:
    from distutils.core import setup

# Open IPy.py to read version
from imp import load_source
macfly = load_source("", "macfly/version.py")

LONG_DESCRIPTION = "Send time delta UNIX socket to libmacfly.so"
CLASSIFIERS = [
    'Development Status :: 5 - Production/Stable',
    'Intended Audience :: System Administrators',
    'Environment :: Console',
    'Topic :: System :: Networking',
    'License :: OSI Approved :: GNU General Public License (GPL)',
    'Operating System :: POSIX',
    'Natural Language :: English',
    'Programming Language :: Python',
    'Topic :: Software Development :: Testing',
    'Topic :: System :: Monitoring',
    'Topic :: Utilities']

setup(name="macfly",
      version=macfly.VERSION,
      description="Time server: send time delta updates to libmacfly.so",
      long_description=LONG_DESCRIPTION,
      author="Victor Stinner",
      maintainer="Victor Stinner",
      maintainer_email="victor.stinner AT inl.fr",
      license=macfly.LICENSE,
      url=macfly.WEBSITE,
      download_url=macfly.WEBSITE,
      classifiers= CLASSIFIERS,
      scripts=["deloreand", "docbrown"],
      packages=["macfly"],
      package_dir={"macfly": "macfly"})

