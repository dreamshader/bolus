#-------------------------------------------------
#
# Project created by QtCreator 2020-01-26T16:13:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bolusGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    dlgnoenv.cpp \
    dlgtimeblockperiods.cpp \
        main.cpp \
        mainwindow.cpp \
    bolusdlg.cpp \
    datadlg.cpp \
    dlgsettingsbolus.cpp \
    inputglucovalue.cpp \
    dlgtimeblocks.cpp \
    dlghealthoptions.cpp \
    dlgrecommendationoptions.cpp \
    dlgtimeblockedit.cpp \
    dlgshowdata.cpp \
    dlgconfirmbolus.cpp

HEADERS += \
    dlgnoenv.h \
    dlgtimeblockperiods.h \
        mainwindow.h \
    bolusdlg.h \
    datadlg.h \
    dlgsettingsbolus.h \
    inputglucovalue.h \
    dlgtimeblocks.h \
    dlghealthoptions.h \
    dlgrecommendationoptions.h \
    dlgtimeblockedit.h \
    dlgshowdata.h \
    dlgconfirmbolus.h \
    ../bolus.h

FORMS += \
    dlgnoenv.ui \
    dlgtimeblockperiods.ui \
        mainwindow.ui \
    bolusdlg.ui \
    datadlg.ui \
    dlgsettingsbolus.ui \
    inputglucovalue.ui \
    dlgtimeblocks.ui \
    dlghealthoptions.ui \
    dlgrecommendationoptions.ui \
    dlgtimeblockedit.ui \
    dlgshowdata.ui \
    dlgconfirmbolus.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
