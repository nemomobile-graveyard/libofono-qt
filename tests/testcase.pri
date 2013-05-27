CONFIG += testcase
QT = core testlib dbus
INCLUDEPATH += ../lib ../
QMAKE_LFLAGS += -L ../lib -lofono-qt

target.path = $$[QT_INSTALL_PREFIX]/opt/tests/libofono-qt/
INSTALLS += target
