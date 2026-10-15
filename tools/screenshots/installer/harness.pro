# The installer application compiled from CALAOS_INSTALLER_SRC with shots.cpp in place
# of its main(): the captures drive the real widgets of that tree.
#   qmake6 CALAOS_INSTALLER_SRC=<calaos_installer tree> harness.pro && make -j32

isEmpty(CALAOS_INSTALLER_SRC): error("Pass CALAOS_INSTALLER_SRC=<calaos_installer source tree> to qmake")
SRC = $$clean_path($$absolute_path($$CALAOS_INSTALLER_SRC, $$OUT_PWD))
!exists($$SRC/calaos_installer.pro): error("$$SRC is not a calaos_installer tree")

REVISION = doc-shots
include($$SRC/calaos_installer.pro)

# qmake resolves the relative paths of an included project against this directory.
defineReplace(fromSrc) {
    out =
    for(f, $$1): out += $$absolute_path($$f, $$SRC)
    return($$out)
}
SOURCES = $$fromSrc(SOURCES)
HEADERS = $$fromSrc(HEADERS)
FORMS = $$fromSrc(FORMS)
RESOURCES = $$fromSrc(RESOURCES)
TRANSLATIONS = $$fromSrc(TRANSLATIONS)
INCLUDEPATH = $$fromSrc(INCLUDEPATH)
RC_FILE =
INSTALLS =

SOURCES -= $$SRC/src/main.cpp
SOURCES += $$PWD/shots.cpp
QT += testlib

TARGET = installer_shots
DESTDIR = $$OUT_PWD

contains(DEFINES, QT_MQTT_AVAILABLE) {
    # The broker side of the discovery tests: the panel is captured against it.
    INCLUDEPATH += $$SRC/tests/tst_esphomediscovery
    SOURCES += $$SRC/tests/tst_esphomediscovery/FakeMqttBroker.cpp
    HEADERS += $$SRC/tests/tst_esphomediscovery/FakeMqttBroker.h
}
