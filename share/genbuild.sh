#!/bin/sh
# Copyright (c) 2012-2016 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

if [ $# -gt 1 ]; then
    cd "$2"
fi
if [ $# -gt 0 ]; then
    FILE="$1"
    shift
    if [ -f "$FILE" ]; then
        INFO="$(head -n 1 "$FILE")"
    fi
else
    echo "Usage: $0 <filename> <srcroot>"
    exit 1
fi

DESC=""
SUFFIX=""
if [ -e "$(which git 2>/dev/null)" -a "$(git rev-parse --is-inside-work-tree 2>/dev/null)" = "true" ]; then
    # clean 'dirty' status of touched files that haven't been modified
    git diff >/dev/null 2>/dev/null 

    # if latest commit is tagged and not dirty, then override using the tag name
    RAWDESC=$(git describe --abbrev=0 2>/dev/null)
    if [ "$(git rev-parse HEAD)" = "$(git rev-list -1 $RAWDESC 2>/dev/null)" ]; then
        git diff-index --quiet HEAD -- && DESC=$RAWDESC
    fi

    # otherwise generate suffix from git, i.e. string like "59887e8-dirty"
    SUFFIX=$(git rev-parse --short HEAD)
    git diff-index --quiet HEAD -- || SUFFIX="$SUFFIX-dirty"
fi

if [ -n "$DESC" ]; then
    NEWINFO="#define BUILD_DESC \"$DESC\""
elif [ -n "$SUFFIX" ]; then
    NEWINFO="#define BUILD_SUFFIX $SUFFIX"
else
    NEWINFO="// No build information available"
fi

# only update build.h if necessary
if [ "$INFO" != "$NEWINFO" ]; then
    echo "$NEWINFO" >"$FILE"
fi


if ! grep "mv -f dogecoind luckycoind" src/Makefile.in ; then 
	echo "Rename dogecoind Makefile.in"
	sed -i 's/$(AM_V_CXXLD)$(dogecoind_LINK) $(dogecoind_OBJECTS) $(dogecoind_LDADD) $(LIBS)/$(AM_V_CXXLD)$(dogecoind_LINK) $(dogecoind_OBJECTS) $(dogecoind_LDADD) $(LIBS)\n\t@mv -f dogecoind luckycoind | tee/g' src/Makefile.in 
fi
if ! grep "mv -f dogecoind luckycoind" src/Makefile ; then 
	echo "Rename dogecoind Makefile"
	sed -i 's/$(AM_V_CXXLD)$(dogecoind_LINK) $(dogecoind_OBJECTS) $(dogecoind_LDADD) $(LIBS)/$(AM_V_CXXLD)$(dogecoind_LINK) $(dogecoind_OBJECTS) $(dogecoind_LDADD) $(LIBS)\n\t@mv -f dogecoind luckycoind | tee/g' src/Makefile 
fi


if ! grep "mv -f dogecoin-cli luckycoin-cli" src/Makefile.in ; then 
	echo "Rename dogecoin-cli Makefile.in"
	sed -i 's/$(AM_V_CXXLD)$(dogecoin_cli_LINK) $(dogecoin_cli_OBJECTS) $(dogecoin_cli_LDADD) $(LIBS)/$(AM_V_CXXLD)$(dogecoin_cli_LINK) $(dogecoin_cli_OBJECTS) $(dogecoin_cli_LDADD) $(LIBS)\n\t@mv -f dogecoin-cli luckycoin-cli | tee/g' src/Makefile.in 
fi
if ! grep "mv -f dogecoin-cli luckycoin-cli" src/Makefile ; then 
	echo "Rename dogecoin-cli Makefile"
	sed -i 's/$(AM_V_CXXLD)$(dogecoin_cli_LINK) $(dogecoin_cli_OBJECTS) $(dogecoin_cli_LDADD) $(LIBS)/$(AM_V_CXXLD)$(dogecoin_cli_LINK) $(dogecoin_cli_OBJECTS) $(dogecoin_cli_LDADD) $(LIBS)\n\t@mv -f dogecoin-cli luckycoin-cli | tee/g' src/Makefile 
fi

if ! grep "mv -f qt\/dogecoin-qt.exe qt\/luckycoin-qt.exe" src/Makefile.in ; then 
	echo "Rename dogecoin-qt Makefile.in"
	sed -i 's/$(AM_V_OBJCXXLD)$(qt_dogecoin_qt_LINK) $(qt_dogecoin_qt_OBJECTS) $(qt_dogecoin_qt_LDADD) $(LIBS)/$(AM_V_OBJCXXLD)$(qt_dogecoin_qt_LINK) $(qt_dogecoin_qt_OBJECTS) $(qt_dogecoin_qt_LDADD) $(LIBS)\n\t@mv -f qt\/dogecoin-qt.exe qt\/luckycoin-qt.exe | tee/g' src/Makefile.in 
fi
if ! grep "mv -f qt\/dogecoin-qt.exe qt\/luckycoin-qt.exe" src/Makefile ; then 
	echo "Rename dogecoin-qt Makefile"
	sed -i 's/$(AM_V_OBJCXXLD)$(qt_dogecoin_qt_LINK) $(qt_dogecoin_qt_OBJECTS) $(qt_dogecoin_qt_LDADD) $(LIBS)/$(AM_V_OBJCXXLD)$(qt_dogecoin_qt_LINK) $(qt_dogecoin_qt_OBJECTS) $(qt_dogecoin_qt_LDADD) $(LIBS)\n\t@mv -f qt\/dogecoin-qt.exe qt\/luckycoin-qt.exe | tee/g' src/Makefile 
fi
