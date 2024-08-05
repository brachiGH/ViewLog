QT       += core gui widgets multimedia multimediawidgets quickwidgets quickcontrols2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/FolderTree.cpp \
    main.cpp \
    src/customtreewidgetitem.cpp \
    src/mainwindow.cpp \
    src/videogroupbox.cpp

HEADERS += \
    customtreewidgetitem.h \
    headers/FolderTree.h \
    headers/customtreewidgetitem.h \
    headers/mainwindow.h \
    headers/videogroupbox.h \
    videogroupbox.h

FORMS += \
    forms/mainwindow.ui

ICON = resources/logo/ViewLog-logo.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/MediaPlayer.qrc

DISTFILES +=
