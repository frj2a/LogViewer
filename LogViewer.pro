# "$Date: $"
# "$Author: $"
# "$Revision: $"

TEMPLATE = app

TARGET = LogViewer

REV_MAJOR_CODE = 1
REV_MINOR_CODE = 0
REV_PATCH_CODE = 9
PROJECT_DIR=$$PWD
include (../QtBaseGeral/base_geral.pri)

QT += core widgets gui xml network

HEADERS += LogPresenter.h \
	ServerId.h \
	LogHighlighter.h \
	LogViewer.h

SOURCES += LogPresenter.cpp \
	ServerId.cpp \
	LogHighlighter.cpp \
	main.cpp \
	LogViewer.cpp

FORMS += ServerId.ui \
	LogViewer.ui

RESOURCES += LogViewer.qrc

win32:RC_FILE = icon.rc

# message($$DEFINES)
# message($$CONFIG)
