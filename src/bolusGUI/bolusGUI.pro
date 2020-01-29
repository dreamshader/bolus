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
        main.cpp \
        mainwindow.cpp \
    bolusdlg.cpp \
    datadlg.cpp \
    settingsdlg.cpp \
    dlgsettingsbolus.cpp \
    inputglucovalue.cpp \
    dlgtimeblocks.cpp \
    dlghealthoptions.cpp \
    dlgrecommendationoptions.cpp \
    dlgdevicesettings.cpp \
    dlgtimedate.cpp \
    dlgtimeblockedit.cpp \
    dlgshowdata.cpp \
    dlgconfirmbolus.cpp

HEADERS += \
        mainwindow.h \
    bolusdlg.h \
    datadlg.h \
    settingsdlg.h \
    dlgsettingsbolus.h \
    inputglucovalue.h \
    bolus.h \
    dlgtimeblocks.h \
    dlghealthoptions.h \
    dlgrecommendationoptions.h \
    dlgdevicesettings.h \
    dlgtimedate.h \
    dlgtimeblockedit.h \
    dlgshowdata.h \
    dlgconfirmbolus.h

FORMS += \
        mainwindow.ui \
    bolusdlg.ui \
    datadlg.ui \
    settingsdlg.ui \
    dlgsettingsbolus.ui \
    inputglucovalue.ui \
    dlgtimeblocks.ui \
    dlghealthoptions.ui \
    dlgrecommendationoptions.ui \
    dlgdevicesettings.ui \
    dlgtimedate.ui \
    dlgtimeblockedit.ui \
    dlgshowdata.ui \
    dlgconfirmbolus.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
