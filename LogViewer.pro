
TEMPLATE = app

TARGET = LogViewer

VER_MAJ = 1
VER_MIN = 0
VER_PAT = 9
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
