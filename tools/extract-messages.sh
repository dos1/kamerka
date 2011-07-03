#!/bin/sh
# Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

BASEDIR=".." # root of translatable sources
PROJECT="kamerka" # project name
BUGADDR="dos@dosowisko.net" # MSGID-Bugs
WDIR=`pwd` # working dir

echo "Preparing additional data"
cd ${BASEDIR}
# additional string for KAboutData
echo 'i18nc("NAME OF TRANSLATORS","Your names");' > ${WDIR}/rc.cpp
echo 'i18nc("EMAIL OF TRANSLATORS","Your emails");' >> ${WDIR}/rc.cpp
cd ${WDIR}
echo "Done preparing additional data"

echo "Extracting messages"
cd ${BASEDIR}
# we use simple sorting to make sure the lines do not jump around too much from system to system
find . -name '*.cpp' -o -name '*.h' -o -name '*.c' -o -name '*.qml' | sort > ${WDIR}/infiles.list
echo "rc.cpp" >> ${WDIR}/infiles.list
cd ${WDIR}
xgettext --from-code=UTF-8 -C -kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 \
-kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
--msgid-bugs-address="${BUGADDR}" \
--files-from=infiles.list -D ${BASEDIR} -D ${WDIR} -o ../po/${PROJECT}.pot || { echo "error while calling xgettext. aborting."; exit 1; }
echo "Done extracting messages"

echo "Merging translations"
cd ${BASEDIR}
catalogs=`find . -name '*.po'`
for cat in $catalogs; do
echo $cat
msgmerge -o $cat.new $cat po/${PROJECT}.pot
mv $cat.new $cat
done
echo "Done merging translations"

echo "Cleaning up"
cd ${WDIR}
rm infiles.list
rm rc.cpp
echo "Done"
