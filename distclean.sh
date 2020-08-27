#! /bin/bash
qmake "CONFIG+=debug_and_release" 2> /dev/null
if [ -e LogViewer ] ; then mv LogViewer .LogViewer ; fi ; make $MAKEOPTS distclean 1> /dev/null 2> /dev/null && if [ -e .LogViewer ] ; then mv .LogViewer LogViewer; fi && rm -fR debug release GeneratedFiles 2>/dev/null
