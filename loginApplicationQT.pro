QT += core gui sql
#QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += serialport
TARGET = loginApplicationQT
TEMPLATE = app

QT += sql


SOURCES +=\
    chart.cpp \
    dataform.cpp \
           dockwidget.cpp \
           login.cpp \
    main.cpp \
            registration.cpp

HEADERS  += \
   #         chart.h \
    chart.h \
            dataform.h \
            dockwidget.h \
            login.h \
            registration.h


FORMS    += \
    chart.ui \
    dataform.ui \
    dockwidget.ui \
    login.ui \
    registration.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

