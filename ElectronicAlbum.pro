QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    carouseldialog.cpp \
    confirmpage.cpp \
    main.cpp \
    mainwindow.cpp \
    openprothread.cpp \
    picanimationwidget.cpp \
    picbutton.cpp \
    picshowdialog.cpp \
    picstatebtn.cpp \
    previewlistitem.cpp \
    previewlistwidget.cpp \
    prosetpage.cpp \
    protree.cpp \
    protreeitem.cpp \
    protreethread.cpp \
    protreewidget.cpp \
    removeprodialog.cpp \
    wizard.cpp

HEADERS += \
    carouseldialog.h \
    confirmpage.h \
    const.h \
    mainwindow.h \
    openprothread.h \
    picanimationwidget.h \
    picbutton.h \
    picshowdialog.h \
    picstatebtn.h \
    previewlistitem.h \
    previewlistwidget.h \
    prosetpage.h \
    protree.h \
    protreeitem.h \
    protreethread.h \
    protreewidget.h \
    removeprodialog.h \
    wizard.h

FORMS += \
    carouseldialog.ui \
    confirmpage.ui \
    mainwindow.ui \
    picshowdialog.ui \
    prosetpage.ui \
    protree.ui \
    removeprodialog.ui \
    wizard.ui

RC_ICONS = "icon/album.ico"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc

DISTFILES +=
