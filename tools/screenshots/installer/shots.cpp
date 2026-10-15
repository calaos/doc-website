// Screenshots of the installer for the climate documentation, on the demo house of
// ../DEMO_HOUSE.md. The devices are created with the "New device from a template"
// dialog and the shipped French templates; the project is then saved, reopened in the
// real MainWindow and captured there.
//
//   installer_shots --out <images dir> --demo <dir for io.xml/rules.xml> [--qt-qm <qtbase_fr.qm>]

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QLabel>
#include <QLibraryInfo>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QTimer>
#include <QToolButton>
#include <QTranslator>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QtTest/QTest>

#include "AppTheme.h"
#include "ConfigOptions.h"
#include "DeviceTemplates.h"
#include "DialogEditDevice.h"
#include "DialogNewDevice.h"
#include "FormActionStd.h"
#include "formrules.h"
#include "mainwindow.h"
#include "projectmanager.h"
#include "qtreewidget_addition.h"
#include <ListeRoom.h>
#include <ListeRule.h>

using namespace Calaos;

namespace
{

QString g_out;
QStringList g_failures;

void fail(const QString &what)
{
    g_failures << what;
    qWarning().noquote() << "FAIL:" << what;
}

void settle()
{
    for (int i = 0;i < 5;i++)
    {
        QApplication::processEvents();
        QTest::qWait(20);
    }
}

void save(const QPixmap &pix, const QString &name)
{
    const QString path = QDir(g_out).filePath(name + ".png");
    if (!pix.save(path))
        fail("cannot write " + path);
    else
        qInfo().noquote() << "wrote" << path << pix.width() << "x" << pix.height();
}

std::string s(const QString &q)
{
    return q.toUtf8().toStdString();
}

// ---- Model: rooms and the plain IOs of the house -------------------------------

Room *addRoom(const QString &name, const char *type)
{
    Room *r = new Room(s(name), type);
    ListeRoom::Instance().Add(r);
    return r;
}

IOBase *input(Room *room, std::initializer_list<std::pair<const char *, QString>> attrs)
{
    Params p;
    for (const auto &a: attrs)
        p.Add(a.first, s(a.second));
    IOBase *io = ListeRoom::Instance().createInput(p, room);
    if (!io)
        fail("input not created: " + QString::fromStdString(p["id"]));
    return io;
}

IOBase *output(Room *room, std::initializer_list<std::pair<const char *, QString>> attrs)
{
    Params p;
    for (const auto &a: attrs)
        p.Add(a.first, s(a.second));
    IOBase *io = ListeRoom::Instance().createOutput(p, room);
    if (!io)
        fail("output not created: " + QString::fromStdString(p["id"]));
    return io;
}

IOBase *mqttTemp(Room *room, const QString &id, const QString &name, const QString &topic)
{
    return input(room, { { "type", "MqttInputTemp" }, { "id", id }, { "name", name }, { "host", "127.0.0.1" },
                         { "port", "1883" }, { "topic_sub", topic }, { "path", "temperature" }, { "unit", "°C" } });
}

IOBase *mqttHumidity(Room *room, const QString &id, const QString &name, const QString &topic)
{
    return input(room, { { "type", "MqttInputAnalog" }, { "id", id }, { "name", name }, { "host", "127.0.0.1" },
                         { "port", "1883" }, { "topic_sub", topic }, { "path", "humidity" }, { "unit", "%" } });
}

IOBase *wagoRelay(Room *room, const QString &id, const QString &name, int var)
{
    return output(room, { { "type", "WODigital" }, { "id", id }, { "name", name }, { "host", "192.168.1.20" },
                          { "port", "502" }, { "var", QString::number(var) }, { "io_style", "light" } });
}

IOBase *timeRange(Room *room, const QString &id, const QString &name, const char *start, const char *end, bool weekdaysOnly)
{
    IOBase *io = input(room, { { "type", "InPlageHoraire" }, { "id", id }, { "name", name } });
    if (!io)
        return nullptr;
    const QStringList a = QString(start).split(':'), b = QString(end).split(':');
    TimeRange r;
    r.shour = s(a[0]); r.smin = s(a[1]); r.ssec = "0";
    r.ehour = s(b[0]); r.emin = s(b[1]); r.esec = "0";
    io->range_monday.push_back(r);
    io->range_tuesday.push_back(r);
    io->range_wednesday.push_back(r);
    io->range_thursday.push_back(r);
    io->range_friday.push_back(r);
    if (!weekdaysOnly)
    {
        io->range_saturday.push_back(r);
        io->range_sunday.push_back(r);
    }
    io->range_months.set();
    return io;
}

// ---- The wizard ---------------------------------------------------------------

template<typename T>
T *field(QWidget *d, const QString &name)
{
    T *w = d->findChild<T *>("field_" + name);
    if (!w)
        fail("no field " + name);
    return w;
}

void setText(QWidget *d, const QString &name, const QString &v)
{
    if (QLineEdit *le = field<QLineEdit>(d, name))
        le->setText(v);
}

void setNumber(QWidget *d, const QString &name, double v)
{
    if (QDoubleSpinBox *sb = field<QDoubleSpinBox>(d, name))
        sb->setValue(v);
}

void setBool(QWidget *d, const QString &name, bool v)
{
    if (QCheckBox *cb = field<QCheckBox>(d, name))
        cb->setChecked(v);
}

void choose(QWidget *d, const QString &name, const QString &data)
{
    QComboBox *box = field<QComboBox>(d, name);
    if (!box)
        return;
    const int i = box->findData(data);
    if (i < 0)
        fail(QStringLiteral("%1 has no %2").arg(name, data));
    box->setCurrentIndex(i);
}

void setMulti(QWidget *d, const QString &name, const QStringList &picked)
{
    QWidget *w = d->findChild<QWidget *>("field_" + name);
    if (!w)
        return fail("no field " + name);
    for (QCheckBox *cb: w->findChildren<QCheckBox *>())
        cb->setChecked(picked.contains(cb->property("choice").toString()));
    for (const QString &p: picked)
        if (!w->findChild<QCheckBox *>(QStringLiteral("field_%1_%2").arg(name, p)))
            fail(QStringLiteral("%1 has no choice %2").arg(name, p));
}

// The whole form, neither scroll bar nor empty space below it.
void fitScroll(QDialog *d, int width)
{
    d->resize(width, 680);
    d->show();
    settle();
    QScrollArea *scroll = d->findChild<QScrollArea *>();
    if (!scroll || !scroll->widget())
        return;
    for (int i = 0;i < 4;i++)
    {
        QWidget *form = scroll->widget();
        const int vw = scroll->viewport()->width();
        const int need = form->hasHeightForWidth() ? form->heightForWidth(vw) : form->sizeHint().height();
        const int diff = need - scroll->viewport()->height();
        if (qAbs(diff) < 2)
            break;
        d->resize(d->width(), d->height() + diff);
        settle();
    }
}

void fitWizard(DialogNewDevice *d)
{
    fitScroll(d, 660);
}

void clickCreate(DialogNewDevice *d)
{
    d->findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok)->click();
    settle();
}

Device *runWizard(const QString &tpl, Room *room, const QString &shot,
                  const std::function<void(DialogNewDevice *)> &fill)
{
    DialogNewDevice d(DeviceTemplates::load("fr"), room);
    if (!d.selectTemplate(tpl))
    {
        fail("template " + tpl + " not shipped");
        return nullptr;
    }
    fill(&d);
    fitWizard(&d);
    if (!shot.isEmpty())
        save(d.grab(), shot);
    clickCreate(&d);
    if (!d.device())
    {
        QLabel *err = d.findChild<QLabel *>("labelError");
        fail(tpl + " not created: " + (err ? err->text() : QString()));
    }
    return d.device();
}

std::string linkOf(Device *dev, const char *role)
{
    for (const DeviceLink &l: dev->links())
        if (l.name == role)
            return l.io;
    fail(QStringLiteral("%1 has no link %2").arg(QString::fromStdString(dev->get_param("id")), role));
    return std::string();
}

IOBase *ioById(const std::string &id)
{
    IOBase *io = DeviceTemplates::ioById(id);
    if (!io)
        fail("no IO " + QString::fromStdString(id));
    return io;
}

// ---- Rules --------------------------------------------------------------------

Rule *rule(const QString &name)
{
    Params p;
    p.Add("type", "Chauffage");
    p.Add("name", s(name));
    Rule *r = new Rule(p);
    ListeRule::Instance().Add(r);
    return r;
}

void when(Rule *r, IOBase *in, const char *oper, const QString &val)
{
    if (!in)
        return;
    Condition *c = new Condition(COND_STD);
    c->Add(in);
    c->get_operator().Add(in->get_param("id"), oper);
    c->get_params().Add(in->get_param("id"), s(val));
    r->AddCondition(c);
}

void then(Rule *r, IOBase *out, const QString &val)
{
    if (!out)
        return;
    Action *a = new Action(ACTION_STD);
    a->Add(out);
    a->get_params().Add(out->get_param("id"), s(val));
    r->AddAction(a);
}

// ---- Phase 1: build and save the demo house -------------------------------------

void buildHouse(const QString &demoDir)
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();

    Room *salon = addRoom("Salon", "salon");
    Room *chambre = addRoom("Chambre", "chambre");
    Room *bureau = addRoom("Bureau", "bureau");
    Room *cuisine = addRoom("Cuisine", "cuisine");
    Room *sejour = addRoom("Séjour", "sam");
    Room *technique = addRoom("Technique", "cave");
    Room *maison = addRoom("Maison", "divers");

    mqttTemp(salon, "salon_temp", "Température salon", "maison/salon/temperature");
    mqttHumidity(salon, "salon_hum", "Humidité salon", "maison/salon/humidite");
    mqttTemp(bureau, "bureau_temp", "Température bureau", "maison/bureau/temperature");
    input(bureau, { { "type", "WIDigitalBP" }, { "id", "bureau_fenetre" }, { "name", "Fenêtre bureau" },
                    { "host", "192.168.1.20" }, { "port", "502" }, { "var", "4" } });
    wagoRelay(bureau, "bureau_radiateur", "Relais radiateur bureau", 1);
    mqttTemp(cuisine, "cuisine_temp", "Température cuisine", "maison/cuisine/temperature");
    wagoRelay(cuisine, "cuisine_pac_chaud", "PAC cuisine chaud", 2);
    wagoRelay(cuisine, "cuisine_pac_froid", "PAC cuisine froid", 3);
    mqttTemp(sejour, "sejour_temp", "Température séjour", "maison/sejour/temperature");
    mqttHumidity(sejour, "sejour_hum", "Humidité séjour", "maison/sejour/humidite");
    wagoRelay(sejour, "plancher_chaud", "Plancher chaud", 5);
    wagoRelay(sejour, "plancher_froid", "Plancher froid", 6);
    IOBase *circulateur = wagoRelay(technique, "circulateur", "Circulateur chauffage", 7);
    IOBase *absence = input(maison, { { "type", "InternalBool" }, { "id", "absence" }, { "name", "Absence" },
                                      { "save", "true" } });
    IOBase *occupe = timeRange(maison, "bureau_occupe", "Bureau occupé", "07:30", "18:00", true);
    IOBase *nuit = timeRange(maison, "nuit", "Nuit", "22:00", "06:30", false);

    Device *clim = runWizard("esphome_climate", salon, "inst-wizard-esphome-climate", [](DialogNewDevice *d)
    {
        setText(d, "name", "Clim salon");
        setText(d, "topic_prefix", "clim_salon");
        setText(d, "object_id", "clim_salon");
        setText(d, "mqtt_host", "127.0.0.1");
        setNumber(d, "mqtt_port", 1883);
        setMulti(d, "modes", { "off", "heat", "cool", "auto", "dry", "fan_only" });
        setBool(d, "two_point", true);
        setNumber(d, "temp_min", 17);
        setNumber(d, "temp_max", 28);
        setNumber(d, "temp_step", 0.5);
        setBool(d, "with_action", true);
        setMulti(d, "fan_modes", { "auto", "quiet", "low", "medium", "high" });
        setMulti(d, "swing_modes", { "off", "vertical", "horizontal", "both" });
        choose(d, "remote_temp_source", "salon_temp");
    });

    runWizard("esphome_climate_ir", chambre, "inst-wizard-esphome-climate-ir", [](DialogNewDevice *d)
    {
        setText(d, "name", "Clim chambre");
        setText(d, "topic_prefix", "clim_chambre");
        setText(d, "object_id", "clim_chambre");
        setText(d, "mqtt_host", "127.0.0.1");
        setNumber(d, "mqtt_port", 1883);
        setMulti(d, "modes", { "off", "heat", "cool", "auto" });
        setBool(d, "with_current_temp", false);
        setNumber(d, "temp_min", 16);
        setNumber(d, "temp_max", 30);
        setNumber(d, "temp_step", 1);
        setMulti(d, "fan_modes", { "auto", "low", "medium", "high" });
        setMulti(d, "swing_modes", {});
    });

    Device *radiateur = runWizard("thermostat_heat", bureau, "inst-wizard-thermostat-heat", [](DialogNewDevice *d)
    {
        setText(d, "name", "Radiateur bureau");
        choose(d, "current_temp", "bureau_temp");
        choose(d, "output_heat", "bureau_radiateur");
        choose(d, "emitter", "electric");
    });

    Device *pac = runWizard("thermostat_heat_cool", cuisine, "inst-wizard-thermostat-heat-cool", [](DialogNewDevice *d)
    {
        setText(d, "name", "PAC cuisine");
        choose(d, "current_temp", "cuisine_temp");
        choose(d, "output_heat", "cuisine_pac_chaud");
        choose(d, "output_cool", "cuisine_pac_froid");
        choose(d, "emitter", "heat_pump");
    });

    Device *plancher = runWizard("thermostat_floor_heat_cool", sejour, "inst-wizard-thermostat-floor", [](DialogNewDevice *d)
    {
        setText(d, "name", "Plancher séjour");
        choose(d, "current_temp", "sejour_temp");
        choose(d, "humidity", "sejour_hum");
        setNumber(d, "humidity_max", 60);
        choose(d, "output_heat", "plancher_chaud");
        choose(d, "output_cool", "plancher_froid");
    });

    // Decision 17: the relay of Radiateur bureau cannot drive a second thermostat.
    {
        DialogNewDevice d(DeviceTemplates::load("fr"), bureau);
        d.selectTemplate("thermostat_heat");
        setText(&d, "name", "Sèche-serviettes bureau");
        choose(&d, "current_temp", "bureau_temp");
        choose(&d, "output_heat", "bureau_radiateur");
        choose(&d, "emitter", "electric");
        fitWizard(&d);
        clickCreate(&d);
        fitWizard(&d);
        QLabel *err = d.findChild<QLabel *>("labelError");
        if (d.device() || !err || !err->isVisible())
            fail("the second thermostat on bureau_radiateur was not refused");
        save(d.grab(), "inst-output-refused");
        d.reject();
    }

    if (!clim || !radiateur || !pac || !plancher)
        return;

    Rule *r = rule("Bureau : confort en journée");
    when(r, occupe, "==", "true");
    then(r, radiateur, R"({"mode":"heat","target_temp":21})");

    r = rule("Bureau : réduit hors journée");
    when(r, occupe, "==", "false");
    then(r, radiateur, R"({"target_temp":17})");

    r = rule("Absence : abaisser le chauffage");
    when(r, absence, "==", "true");
    then(r, radiateur, R"({"mode":"heat","target_temp":16})");
    then(r, pac, R"({"mode":"auto","target_temp_low":16,"target_temp_high":28})");
    then(r, plancher, R"({"mode":"heat","target_temp":16})");

    r = rule("Retour : rétablir");
    when(r, absence, "==", "false");
    then(r, radiateur, R"({"target_temp":21})");
    then(r, pac, R"({"target_temp_low":20,"target_temp_high":25})");
    then(r, plancher, R"({"target_temp":21})");

    IOBase *fenetre = ioById("bureau_fenetre");
    r = rule("Fenêtre bureau ouverte");
    when(r, fenetre, "==", "true");
    then(r, radiateur, "off");

    r = rule("Fenêtre bureau fermée");
    when(r, fenetre, "==", "false");
    then(r, radiateur, "heat");

    r = rule("Hors-gel : nuit d'hiver");
    when(r, nuit, "==", "true");
    when(r, ioById("salon_temp"), "<", "10");
    then(r, clim, R"({"mode":"heat","target_temp":17})");

    const QList<QPair<QString, Device *>> thermostats = {
        { "Radiateur bureau", radiateur }, { "PAC cuisine", pac }, { "Plancher séjour", plancher } };
    for (const auto &t: thermostats)
    {
        r = rule("Circulateur : " + t.first + " chauffe");
        when(r, ioById(linkOf(t.second, "action")), "==", "heating");
        then(r, circulateur, "true");
    }
    r = rule("Circulateur : plus aucun thermostat ne chauffe");
    for (const auto &t: thermostats)
        when(r, ioById(linkOf(t.second, "action")), "!=", "heating");
    then(r, circulateur, "false");

    QDir().mkpath(demoDir);
    QString io = QDir(demoDir).filePath("io.xml"), rules = QDir(demoDir).filePath("rules.xml");
    if (!ProjectManager::saveIOsToFile(io) || !ProjectManager::saveRulesToFile(rules))
        fail("cannot save the demo project in " + demoDir);
}

// ---- Phase 2: the saved project in the main window -------------------------------

QTreeWidgetItem *findItem(QTreeWidget *tree, const QString &text, int column = 0)
{
    for (QTreeWidgetItemIterator it(tree);*it;++it)
        if ((*it)->text(column) == text)
            return *it;
    fail("no tree item " + text);
    return nullptr;
}

QTreeWidgetItem *findRule(QTreeWidget *tree, const QString &name)
{
    for (QTreeWidgetItemIterator it(tree);*it;++it)
        for (int c = 0;c < tree->columnCount();c++)
            if ((*it)->text(c) == name)
                return *it;
    fail("no rule " + name);
    return nullptr;
}

// A popup is a window of its own: paint it where it is over the main window.
QPixmap withPopup(QWidget *window, QWidget *popup)
{
    QPixmap pix = window->grab();
    QPainter p(&pix);
    p.drawPixmap(popup->mapToGlobal(QPoint(0, 0)) - window->mapToGlobal(QPoint(0, 0)), popup->grab());
    return pix;
}

void shootMainWindow(const QString &demoDir)
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();

    MainWindow w;
    w.resize(1400, 860);
    w.move(0, 0);
    w.show();
    QTest::qWaitForWindowExposed(&w);
    // The window shows the project path: a neutral copy rather than this checkout's.
    const QString shown = QDir::tempPath() + "/maison-demo";
    QDir().mkpath(shown);
    for (const char *f: { "io.xml", "rules.xml" })
    {
        QFile::remove(QDir(shown).filePath(f));
        QFile::copy(QDir(demoDir).filePath(f), QDir(shown).filePath(f));
    }
    w.Load(shown);
    w.ShowPage(PAGE_PROG);
    settle();

    if (ListeRoom::Instance().size() != 7)
        fail(QStringLiteral("reloaded project has %1 rooms").arg(ListeRoom::Instance().size()));

    FormRules *rules = w.findChild<FormRules *>();
    QTreeWidget *home = w.findChild<QTreeWidget *>("tree_home");
    QTreeWidget *ruleTree = w.findChild<QTreeWidget *>("tree_rules");
    QTreeWidget *actions = w.findChild<QTreeWidget *>("tree_action");
    if (!rules || !home || !ruleTree || !actions)
        return fail("FormRules widgets not found");

    // An expand animation still running would be captured half drawn.
    home->setAnimated(false);

    // Rooms and devices expanded: a window tall enough for the whole tree.
    home->expandAll();
    settle();
    {
        int rows = 0;
        for (QTreeWidgetItemIterator it(home);*it;++it)
            rows += home->visualItemRect(*it).height();
        const QSize normal = w.size();
        w.resize(normal.width(), normal.height() + rows - home->viewport()->height() + 8);
        settle();
        save(home->grab(), "inst-tree-device");
        w.resize(normal);
        settle();
    }

    // The Add menu on a room.
    home->collapseAll();
    QTreeWidgetItem *salon = findItem(home, "Salon");
    if (salon)
    {
        salon->setExpanded(true);
        home->setCurrentItem(salon);
    }
    settle();
    QPushButton *add = w.findChild<QPushButton *>("ioexplorer_add");
    if (add && add->menu())
    {
        QMenu *menu = add->menu();
        menu->popup(add->mapToGlobal(QPoint(0, add->height())));
        settle();
        for (QAction *a: menu->actions())
            if (a->text().contains("template"))
                menu->setActiveAction(a);
        settle();
        QPixmap pix = withPopup(&w, menu);
        const QRect menuRect(menu->mapToGlobal(QPoint(0, 0)) - w.mapToGlobal(QPoint(0, 0)), menu->size());
        save(pix.copy(QRect(0, 0, menuRect.right() + 40, qMin(pix.height(), menuRect.bottom() + 20))), "inst-add-menu");
        menu->hide();
        settle();
    }
    else
        fail("no Add button with a menu");

    // The device editor on Clim salon.
    Device *clim = nullptr;
    for (int i = 0;i < ListeRoom::Instance().size() && !clim;i++)
        for (Device *d: ListeRoom::Instance().get_room(i)->get_devices())
            if (d->get_param("name") == s("Clim salon"))
                clim = d;
    if (clim)
    {
        DialogEditDevice d(clim, &w);
        fitScroll(&d, 700);
        save(d.grab(), "inst-device-editor");
        d.reject();
    }
    else
        fail("Clim salon not reloaded");

    auto showRule = [&](const QString &name) -> bool
    {
        QTreeWidgetItem *it = findRule(ruleTree, name);
        if (!it)
            return false;
        ruleTree->setCurrentItem(it);
        ruleTree->scrollToItem(it);
        settle();
        return true;
    };

    home->collapseAll();
    if (showRule("Bureau : confort en journée"))
    {
        if (QTreeWidgetItem *bureau = findItem(home, "Bureau"))
        {
            bureau->setExpanded(true);
            home->setCurrentItem(bureau);
        }
        settle();
        save(w.grab(), "inst-rule-complete");

        QTreeWidgetItem *first = actions->topLevelItem(0);
        if (first)
        {
            actions->setCurrentItem(first);
            QMetaObject::invokeMethod(rules, "on_tree_action_itemClicked", Q_ARG(QTreeWidgetItem *, first), Q_ARG(int, 0));
            settle();
            FormActionStd *popup = rules->findChild<FormActionStd *>();
            if (popup && popup->isVisible())
            {
                const QPoint origin = w.mapToGlobal(QPoint(0, 0));
                QRect area(popup->mapToGlobal(QPoint(0, 0)) - origin, popup->size());
                area = area.united(QRect(actions->mapToGlobal(QPoint(0, 0)) - origin, actions->size()));
                area = area.adjusted(-12, -50, 12, 12).intersected(w.rect());
                save(withPopup(&w, popup).copy(area), "inst-action-form");
                popup->hide();
                //Opening the form marks the project modified: not what the next capture is about.
                rules->setProjectModified(false);
                settle();
            }
            else
                fail("the action form did not open");
        }
        else
            fail("the rule has no action");
    }

    home->collapseAll();
    if (showRule("Circulateur : plus aucun thermostat ne chauffe"))
    {
        if (QTreeWidgetItem *tech = findItem(home, "Technique"))
        {
            tech->setExpanded(true);
            home->setCurrentItem(tech);
        }
        settle();
        save(w.grab(), "inst-rule-circulator");
    }

    w.findChild<FormRules *>()->setProjectModified(false);
}

}

int main(int argc, char *argv[])
{
    AppTheme::prepareEnvironment();
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("CalaosDocShots");
    QCoreApplication::setOrganizationDomain("calaos.fr");
    QCoreApplication::setApplicationName("CalaosInstallerShots");
    QApplication::setStyle("fusion");
    AppTheme::forceLight();
    QLocale::setDefault(QLocale(QLocale::French, QLocale::France));

    QString demoDir, qtQm;
    const QStringList args = app.arguments();
    for (int i = 1;i + 1 < args.size();i++)
    {
        if (args[i] == "--out") g_out = args[++i];
        else if (args[i] == "--demo") demoDir = args[++i];
        else if (args[i] == "--qt-qm") qtQm = args[++i];
    }
    if (g_out.isEmpty() || demoDir.isEmpty())
    {
        qCritical("usage: installer_shots --out <images dir> --demo <project dir> [--qt-qm <qtbase_fr.qm>]");
        return 2;
    }
    QDir().mkpath(g_out);

    ConfigOptions::Instance().setOption("ui/lang", "fr");

    QTranslator qtTranslator, translator;
    if (qtQm.isEmpty())
        qtQm = QLibraryInfo::path(QLibraryInfo::TranslationsPath) + "/qtbase_fr.qm";
    if (qtTranslator.load(qtQm))
        app.installTranslator(&qtTranslator);
    else
        qWarning() << "Qt's own French strings not loaded from" << qtQm;
    if (!translator.load(":/lang/calaos_installer_fr.qm"))
        fail("calaos_installer_fr.qm missing from the resources");
    app.installTranslator(&translator);

    // A modal box would block the run: close it and report it.
    QTimer killer;
    QObject::connect(&killer, &QTimer::timeout, [&]()
    {
        if (QWidget *m = QApplication::activeModalWidget())
        {
            if (QMessageBox *mb = qobject_cast<QMessageBox *>(m))
            {
                fail("unexpected message box: " + mb->text());
                mb->close();
            }
        }
    });
    killer.start(50);

    buildHouse(demoDir);
    shootMainWindow(demoDir);

    if (!g_failures.isEmpty())
    {
        qCritical().noquote() << g_failures.size() << "failure(s):\n " + g_failures.join("\n ");
        return 1;
    }
    return 0;
}
