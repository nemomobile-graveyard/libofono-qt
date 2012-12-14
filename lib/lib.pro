include(../version.pri)

TEMPLATE = lib
TARGET = ofono-qt

CONFIG += debug
DEFINES += BUILD_OFONO_QT_LIBRARY

QT += dbus
QT -= gui
INCLUDEPATH += ../

PUBLIC_HEADERS += libofono-qt_global.h \
    ofonopropertysetting.h  \
    ofonomodeminterface.h  \
    ofonomodemmanager.h \
    ofonomodem.h \
    ofonophonebook.h \
    ofonomessagemanager.h \
    ofonomessagewaiting.h \
    ofonoradiosettings.h \
    ofonosimmanager.h \
    ofonocallbarring.h \
    ofonocallforwarding.h \
    ofonocallmeter.h \
    ofonocallsettings.h \
    ofononetworkoperator.h \
    ofononetworkregistration.h \
    ofonosupplementaryservices.h \
    ofonovoicecallmanager.h \
    ofonovoicecall.h \
    ofonocallvolume.h \
    ofonomessage.h \
    ofonoconnman.h \
    ofonoconnmancontext.h \
    ofonocellbroadcast.h

HEADERS += $$PUBLIC_HEADERS \
    ofonointerface.h

SOURCES += ofonointerface.cpp \
    ofonomodeminterface.cpp \
    ofonomodemmanager.cpp \
    ofonomodem.cpp \
    ofonophonebook.cpp \
    ofonomessagemanager.cpp \
    ofonomessagewaiting.cpp \
    ofonoradiosettings.cpp \
    ofonosimmanager.cpp \
    ofonocallbarring.cpp \
    ofonocallforwarding.cpp \
    ofonocallmeter.cpp \
    ofonocallsettings.cpp \
    ofononetworkoperator.cpp \
    ofononetworkregistration.cpp \
    ofonosupplementaryservices.cpp \
    ofonovoicecallmanager.cpp \
    ofonovoicecall.cpp \
    ofonocallvolume.cpp \
    ofonomessage.cpp \
    ofonoconnman.cpp \
    ofonoconnmancontext.cpp \
    ofonocellbroadcast.cpp

target.path = $$[QT_INSTALL_PREFIX]/lib
headers.files = $$PUBLIC_HEADERS
headers.path = $$[QT_INSTALL_PREFIX]/include/ofono-qt

CONFIG += create_pc create_prl

QMAKE_PKGCONFIG_DESTDIR = pkgconfig
QMAKE_PKGCONFIG_INCDIR = $$headers.path

qtconfig.path = $$[QT_INSTALL_PREFIX]/share/qt4/mkspecs/features
qtconfig.files = ofono-qt.prf

INSTALLS += target headers qtconfig
