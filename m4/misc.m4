# libnih
#
# misc.m4 - miscellaneous autoconf macros
#
# Copyright © 2009 Scott James Remnant <scott@netsplit.com>.
# Copyright © 2009 Canonical Ltd.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
# ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


# AC_COPYRIGHT
# -------------
# Wraps the Autoconf AC_COPYRIGHT but also defines PACKAGE_COPYRIGHT,
# required for nih_main_init
m4_rename([AC_COPYRIGHT], [_NIH_AC_COPYRIGHT])
AC_DEFUN([AC_COPYRIGHT],
[_NIH_AC_COPYRIGHT([$1])
m4_ifndef([NIH_PACKAGE_COPYRIGHT], [m4_bmatch([$1], [
], [], [
	m4_define([NIH_PACKAGE_COPYRIGHT],
		  ["m4_bpatsubst([AS_ESCAPE([$1])], [©], [(C)])"])
	AC_DEFINE([PACKAGE_COPYRIGHT], [NIH_PACKAGE_COPYRIGHT],
		  [Define to the copyright message of this package.])])])dnl
	AC_SUBST([PACKAGE_COPYRIGHT], ["$1"])
])# AC_COPYRIGHT


# NIH_INIT([OPTIONS])
# -------------------
# Expands to the set of macros normally required for using libnih within
# another source tree.
#
# Options:
#   install         install libnih (normally not installed)
#   dbus            require that libnih-dbus be built
AC_DEFUN([NIH_INIT],
[m4_foreach_w([_NIH_Option], [$1],
	     [m4_define([_NIH_Option_]m4_bpatsubst(_NIH_Option, [[^a-zA_Z0-9_]], [_]))])

m4_ifdef([_NIH_Option_install], [nih_install=yes])
AM_CONDITIONAL([INSTALL_NIH], [test "x$nih_install" = "xyes"])

PKG_PROG_PKG_CONFIG([0.22])

# Checks for libraries
NIH_LIB_DBUS

# Checks for header files.
AC_CHECK_HEADERS([sys/inotify.h valgrind/valgrind.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_PROG_CC_C99
AM_PROG_CC_C_O
NIH_C_THREAD

# Other checks
NIH_COMPILER_WARNINGS
NIH_COMPILER_OPTIMISATIONS
NIH_COMPILER_COVERAGE

NIH_LINKER_OPTIMISATIONS
NIH_LINKER_VERSION_SCRIPT
])# NIH_INIT
