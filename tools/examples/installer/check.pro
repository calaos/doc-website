# The installer application compiled from CALAOS_INSTALLER_SRC with check.cpp in place
# of its main(): the examples are opened in the real widgets of that tree.
#   qmake6 CALAOS_INSTALLER_SRC=<calaos_installer tree> check.pro && make -j32

isEmpty(CALAOS_INSTALLER_SRC): error("Pass CALAOS_INSTALLER_SRC=<calaos_installer source tree> to qmake")
SRC = $$clean_path($$absolute_path($$CALAOS_INSTALLER_SRC, $$OUT_PWD))
!exists($$SRC/calaos_installer.pro): error("$$SRC is not a calaos_installer tree")

REVISION = doc-examples
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
SOURCES += $$PWD/check.cpp
QT += testlib

TARGET = examples_check
DESTDIR = $$OUT_PWD

