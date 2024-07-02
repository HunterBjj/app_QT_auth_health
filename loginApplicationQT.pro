QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += serialport
TARGET = loginApplicationQT
TEMPLATE = app

QT += sql

SOURCES +=\
    dataform.cpp \
           dockwidget.cpp \
           login.cpp \
    main.cpp \
            registration.cpp

HEADERS  += \
            dataform.h \
            dockwidget.h \
            login.h \
            registration.h


FORMS    += \
    dataform.ui \
    dockwidget.ui \
    login.ui \
    registration.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

