CONFIG += testcase
QT = core testlib dbus
INCLUDEPATH += ../lib ../
QMAKE_LFLAGS += -L ../lib -lofono-qt

target.path = /opt/tests/libofono-qt/
INSTALLS += target
