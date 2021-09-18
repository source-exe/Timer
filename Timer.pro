QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_TARGET = Timer
QMAKE_TARGET_DESCRIPTION = Timer

VERSION = 1.7.4

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    tray_menu.cpp \
    notification.cpp

HEADERS += \
    mainwindow.h \
    tray_menu.h \
    notification.h

FORMS += \
    mainwindow.ui \
    notification.ui

RESOURCES = res.qrc

RC_ICONS = icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
