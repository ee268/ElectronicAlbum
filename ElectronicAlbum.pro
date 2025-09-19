QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/animationWidget/picanimationwidget.cpp \
    src/buttons/picbutton.cpp \
    src/buttons/picstatebtn.cpp \
    src/dialogs/carouseldialog.cpp \
    src/dialogs/picshowdialog.cpp \
    src/dialogs/removeprodialog.cpp \
    src/listWidget/previewlistitem.cpp \
    src/listWidget/previewlistwidget.cpp \
    src/mainwindow.cpp \
    src/thread/openprothread.cpp \
    src/thread/protreethread.cpp \
    src/treeWidget/protree.cpp \
    src/treeWidget/protreeitem.cpp \
    src/treeWidget/protreewidget.cpp \
    src/wizards/confirmpage.cpp \
    src/wizards/prosetpage.cpp \
    src/wizards/wizard.cpp \

HEADERS += \
    include/carouseldialog.h \
    include/confirmpage.h \
    include/const.h \
    include/mainwindow.h \
    include/openprothread.h \
    include/picanimationwidget.h \
    include/picbutton.h \
    include/picshowdialog.h \
    include/picstatebtn.h \
    include/previewlistitem.h \
    include/previewlistwidget.h \
    include/prosetpage.h \
    include/protree.h \
    include/protreeitem.h \
    include/protreethread.h \
    include/protreewidget.h \
    include/removeprodialog.h \
    include/wizard.h \

FORMS += \
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
