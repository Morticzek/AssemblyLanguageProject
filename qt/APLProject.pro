QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dijkstra.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dijsktra.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    APLProject_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -L$$PWD/./ -lAPLCpp

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./APLCpp.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/./libAPLCpp.a

win32: LIBS += -L$$PWD/./ -lDijkstraOmpDll

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./DijkstraOmpDll.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/./libDijkstraOmpDll.a
