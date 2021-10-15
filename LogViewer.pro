
TEMPLATE = app

TARGET = LogViewer

CONFIG += c++11

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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:android: {

    DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/gradle.properties \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew \
        android/gradlew.bat \
        android/res/values/libs.xml

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

}
