CONFIG += testcase
QT = core testlib dbus
INCLUDEPATH += ../lib ../
LIBS += -L ../lib -lofono-qt

target.path = $$[QT_INSTALL_PREFIX]/lib/tests/libofono-qt
INSTALLS += target
