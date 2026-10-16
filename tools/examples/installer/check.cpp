// Opens each example project in the real MainWindow of a calaos_installer tree and fails on
// anything the user would see go wrong: a message box, a warning logged while loading, a rule
// on a missing IO, a device link to a missing IO or to an IO its role does not accept, a device
// editor that does not open. The project is then saved again: the files must come out unchanged,
// so that what the site serves is what the installer itself writes.
//
//   examples_check <project dir>
//
// The directory is passed as the installer's own command line argument: MainWindow opens it as when
// a user starts calaos_installer on a project. EXAMPLES_WRITE=1 replaces the files by the installer's
// own output instead of failing on a difference.

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTemporaryDir>
#include <QTimer>
#include <QtTest/QTest>

#include "ConfigOptions.h"
#include "DeviceEdit.h"
#include "DialogEditDevice.h"
#include "Device.h"
#include "mainwindow.h"
#include "projectmanager.h"
#include <ListeRoom.h>
#include <ListeRule.h>

using namespace Calaos;

namespace
{
QStringList g_failures;
QStringList g_logged;
bool g_capture = false;

void fail(const QString &project, const QString &what)
{
    g_failures << project + ": " + what;
}

void handler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    // Offscreen platform noise, not the installer's.
    if (g_capture && type != QtDebugMsg && type != QtInfoMsg && !msg.contains("propagateSizeHints"))
        g_logged << msg;
    fprintf(stderr, "%s\n", qPrintable(msg));
}

void settle()
{
    for (int i = 0;i < 5;i++)
        QTest::qWait(20);
}

QByteArray readAll(const QString &path)
{
    QFile f(path);
    return f.open(QIODevice::ReadOnly) ? f.readAll() : QByteArray();
}

void checkProject(const QString &dir)
{
    const QString name = QFileInfo(dir).fileName();
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();

    g_logged.clear();
    g_capture = true;
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    if (!QTest::qWaitForWindowExposed(&w))
        fail(name, "window not shown");
    settle();
    g_capture = false;
    for (const QString &m: g_logged)
        fail(name, "logged while loading: " + m);

    if (!ProjectManager::readOnlyReason().isEmpty())
        fail(name, "read-only: " + ProjectManager::readOnlyReason());
    for (const QString &m: ProjectManager::missingIOReport())
        fail(name, "rule on a missing IO: " + m);

    int devices = 0, ios = ListeRoom::Instance().get_nb_input() + ListeRoom::Instance().get_nb_output();
    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        for (Device *dev: ListeRoom::Instance().get_room(i)->get_devices())
        {
            devices++;
            const QString id = QString::fromStdString(dev->get_param("id"));
            const DeviceEdit::State state = DeviceEdit::current(dev);
            for (const QString &r: DeviceEdit::missingRequired(dev, state))
                fail(name, id + ": required link missing: " + r);
            for (const DeviceEdit::Role &role: DeviceEdit::roles(dev))
            {
                const QString target = state.links.value(role.name);
                if (target.isEmpty() || DeviceEdit::isOwnedRole(dev, role.name))
                    continue;
                IOBase *io = ListeRoom::Instance().get_input(target.toStdString());
                if (!io)
                    io = ListeRoom::Instance().get_output(target.toStdString());
                if (!io)
                    fail(name, id + ": " + role.name + " targets the unknown IO " + target);
                else if (!DeviceEdit::fits(io, role, dev))
                    fail(name, id + ": " + role.name + " does not accept " + target);
            }
            DialogEditDevice d(dev, &w);
            d.show();
            settle();
            d.reject();
        }
    }
    if (ListeRoom::Instance().size() == 0 || ListeRule::Instance().size() == 0 || devices == 0)
        fail(name, "empty project");

    QTemporaryDir out;
    QString io = out.filePath("io.xml"), rules = out.filePath("rules.xml");
    if (!ProjectManager::saveIOsToFile(io) || !ProjectManager::saveRulesToFile(rules))
        fail(name, "save failed");
    for (const QString &f: { QStringLiteral("io.xml"), QStringLiteral("rules.xml") })
    {
        const QString mine = QDir(dir).filePath(f), saved = out.filePath(f);
        if (readAll(mine) == readAll(saved))
            continue;
        if (qEnvironmentVariableIntValue("EXAMPLES_WRITE"))
        {
            QFile::remove(mine);
            QFile::copy(saved, mine);
            printf("%s: %s rewritten by the installer\n", qPrintable(name), qPrintable(f));
        }
        else
            fail(name, f + " differs once saved by the installer (EXAMPLES_WRITE=1 normalises it)");
    }
    printf("%s: %d rooms, %d IOs, %d devices, %d rules\n", qPrintable(name), ListeRoom::Instance().size(), ios,
           devices, ListeRule::Instance().size());
}
}

int main(int argc, char **argv)
{
    qInstallMessageHandler(handler);
    QApplication app(argc, argv);
    ConfigOptions::Instance().setOption("ui/lang", "fr");

    if (app.arguments().size() != 2)
    {
        fprintf(stderr, "usage: examples_check <project dir>\n");
        return 2;
    }
    const QString dir = QDir(app.arguments().at(1)).absolutePath();
    const QString current = QFileInfo(dir).fileName();

    // A message box would block the run: close it and report it.
    QTimer killer;
    QObject::connect(&killer, &QTimer::timeout, [&]()
    {
        if (QMessageBox *mb = qobject_cast<QMessageBox *>(QApplication::activeModalWidget()))
        {
            fail(current, "message box: " + mb->text());
            mb->close();
        }
    });
    killer.start(50);

    checkProject(dir);

    for (const QString &f: g_failures)
        printf("FAIL %s\n", qPrintable(f));
    return g_failures.isEmpty() ? 0 : 1;
}
