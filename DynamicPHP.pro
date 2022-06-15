QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codewindow.cpp \
    element.cpp \
    evaluate.cpp \
    htmlgenerator.cpp \
    htmlparser.cpp \
    htmltreeview.cpp \
    main.cpp \
    mainwindow.cpp \
    phpgenerator.cpp \
    qtreelineedit.cpp \
    settingswindow.cpp \
    treeviewwindow.cpp \
    unit_test.cpp

HEADERS += \
    codewindow.h \
    element.h \
    evaluate.h \
    htmlgenerator.h \
    htmlparser.h \
    htmltreeview.h \
    mainwindow.h \
    phpgenerator.h \
    qtreelineedit.h \
    settingswindow.h \
    treeviewwindow.h \
    unit_test.h

FORMS += \
    codewindow.ui \
    htmltreeview.ui \
    mainwindow.ui \
    settingswindow.ui \
    treeviewwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

DISTFILES +=
