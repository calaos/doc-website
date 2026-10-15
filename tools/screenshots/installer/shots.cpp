// Screenshots of the installer for the climate documentation, on the demo house of
// ../DEMO_HOUSE.md. The devices are created with the "New device from a template"
// dialog and the shipped French templates; the project is then saved, reopened in the
// real MainWindow and captured there.
//
//   installer_shots --out <images dir> --demo <dir for io.xml/rules.xml> [--zones] [--qt-qm <qtbase_fr.qm>]
//
// --zones adds the second house (reversible floor, heat pump contacts, domestic hot water), which
// needs the "Reversible floor zone" template: a calaos_installer tree whose data/doc ships it.

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
#include <QScrollBar>
#include <QSplitter>
#include <QTimer>
#include <QToolBox>
#include <QToolButton>
#include <QTranslator>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QtTest/QTest>

#ifdef QT_MQTT_AVAILABLE
#include <QSignalSpy>
#include "WidgetEsphomeDiscovery.h"
#include "FakeMqttBroker.h"
#endif
#include "AppTheme.h"
#include "ConfigOptions.h"
#include "DeviceTemplates.h"
#include "DialogEditDevice.h"
#include "dialogioproperties.h"
#include "Device.h"
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

// The whole form: neither scroll bar nor empty space below it.
void fitScroll(QDialog *d, int width, bool again = true)
{
    d->resize(width, 680);
    d->show();
    settle();
    QScrollArea *scroll = d->findChild<QScrollArea *>();
    if (!scroll || !scroll->widget())
        return;
    int stable = 0;
    for (int i = 0;i < 12 && stable < 2;i++)
    {
        QWidget *form = scroll->widget();
        const int vw = scroll->viewport()->width();
        const int need = form->hasHeightForWidth() ? form->heightForWidth(vw) : form->sizeHint().height();
        const int diff = need - scroll->viewport()->height();
        // A label wider than the dialog: widen it rather than cut the text.
        const int overflow = scroll->horizontalScrollBar()->isVisible() ? scroll->horizontalScrollBar()->maximum() : 0;
        stable = qAbs(diff) < 2 && !overflow ? stable + 1 : 0;
        if (!stable)
        {
            d->resize(d->width() + overflow, d->height() + diff);
            settle();
        }
        else
            settle();
    }
    // Laid out again from the width it needed: heights then follow the real wrapping.
    if (again && d->width() != width)
        fitScroll(d, d->width(), false);
}

void fitWizard(DialogNewDevice *d, int width = 660)
{
    fitScroll(d, width);
}

void clickCreate(DialogNewDevice *d)
{
    d->findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Ok)->click();
    settle();
}

Device *runWizard(const QString &tpl, Room *room, const QString &shot,
                  const std::function<void(DialogNewDevice *)> &fill, int width = 660)
{
    DialogNewDevice d(DeviceTemplates::load("fr"), room);
    if (!d.selectTemplate(tpl))
    {
        fail("template " + tpl + " not shipped");
        return nullptr;
    }
    fill(&d);
    fitWizard(&d, width);
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

//The "Condition on output event" kind: the only one that can read a relay.
void whenOutput(Rule *r, IOBase *out, const char *oper, const QString &val)
{
    if (!out)
        return;
    Condition *c = new Condition(COND_OUTPUT);
    c->setOutput(out);
    c->setOutputOper(oper);
    c->setOutputParam(s(val));
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

    //Zone valve outputs of the three thermostats: what the regulation actually drives.
    const QList<QPair<QString, QString>> zoneOutputs = {
        { "Relais radiateur bureau", "bureau_radiateur" }, { "PAC cuisine chaud", "cuisine_pac_chaud" },
        { "PAC cuisine froid", "cuisine_pac_froid" }, { "Plancher chaud", "plancher_chaud" },
        { "Plancher froid", "plancher_froid" } };
    for (const auto &o: zoneOutputs)
    {
        r = rule("Circulateur : " + o.first + " allumé");
        whenOutput(r, ioById(o.second.toStdString()), "==", "true");
        then(r, circulateur, "true");
    }
    r = rule("Circulateur : toutes les zones éteintes");
    for (const auto &o: zoneOutputs)
        whenOutput(r, ioById(o.second.toStdString()), "==", "false");
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
    if (showRule("Circulateur : toutes les zones éteintes"))
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

// ---- ESPHome discovery panel (needs Qt MQTT, like the application) --------------------

#ifdef QT_MQTT_AVAILABLE
void shootDiscovery()
{
    FakeMqttBroker broker;
    //A module publishing under clim_salon, a second one, and the retained states of a node that is gone.
    broker.retained = {
        { "esphome/discover/clim_salon", R"({"name":"clim_salon","friendly_name":"Clim salon","ip":"192.168.1.42"})" },
        { "clim_salon/status", "online" },
        { "clim_salon/climate/clim_salon/mode/state", "heat_cool" },
        { "clim_salon/climate/clim_salon/target_temperature_low/state", "19.5" },
        { "clim_salon/climate/clim_salon/target_temperature_high/state", "24.5" },
        { "clim_salon/climate/clim_salon/current_temperature/state", "19.2" },
        { "clim_salon/climate/clim_salon/action/state", "heating" },
        { "clim_salon/climate/clim_salon/fan_mode/state", "auto" },
        { "clim_salon/climate/clim_salon/swing_mode/state", "off" },
        { "clim_buanderie/climate/clim_buanderie/mode/state", "cool" },
        { "clim_buanderie/climate/clim_buanderie/target_temperature/state", "22" },
        { "esphome/discover/clim-veranda", R"({"name":"clim-veranda","friendly_name":"Clim véranda","ip":"192.168.1.43"})" },
        { "clim-veranda/status", "online" },
        { "clim-veranda/climate/clim_veranda/mode/state", "cool" },
        { "clim-veranda/climate/clim_veranda/target_temperature/state", "24" },
        { "clim-veranda/climate/clim_veranda/current_temperature/state", "25.1" } };
    const quint16 port = broker.start();

    Room *salon = ListeRoom::Instance().get_room(0);
    DialogNewDevice d(DeviceTemplates::load("fr"), salon);
    if (!d.selectTemplate("esphome_climate"))
        return fail("esphome_climate not shipped");
    d.show();
    QTest::qWaitForWindowExposed(&d);
    settle();

    WidgetEsphomeDiscovery *panel = d.findChild<WidgetEsphomeDiscovery *>("discoveryBox");
    QLineEdit *host = d.findChild<QLineEdit *>("discoveryHost");
    QPushButton *button = d.findChild<QPushButton *>("discoveryButton");
    QTreeWidget *list = d.findChild<QTreeWidget *>("discoveryList");
    if (!panel || !host || !button || !list)
        return fail("the discovery panel is missing");

    setNumber(&d, "mqtt_port", port);
    host->setText("127.0.0.1");
    panel->setTimings(300, 2000);
    QSignalSpy done(panel, &WidgetEsphomeDiscovery::searchFinished);
    button->click();
    if (!done.wait(10000))
        return fail("the discovery search did not finish");
    settle();
    if (list->topLevelItemCount() != 3)
        fail(QStringLiteral("%1 climates listed, 3 expected").arg(list->topLevelItemCount()));

    // The broker as the user's network names it, not this process.
    host->setText("192.168.1.10");
    setNumber(&d, "mqtt_port", 1883);
    setText(&d, "mqtt_host", "192.168.1.10");
    fitWizard(&d);
    const QPixmap found = d.grab();
    save(found.copy(0, 0, found.width(), qMin(found.height(), 330)), "inst-discovery-found");

    for (int i = 0;i < list->topLevelItemCount();i++)
        if (list->topLevelItem(i)->text(2) == "clim_salon")
            list->setCurrentItem(list->topLevelItem(i));
    setNumber(&d, "mqtt_port", 1883);
    setText(&d, "mqtt_host", "192.168.1.10");
    fitWizard(&d);
    save(d.grab(), "inst-discovery-filled");
    d.reject();
}
#endif

// ---- The second house: reversible floor, heat pump contacts, hot water ------------------

struct Zone
{
    QString key, room, name;
    bool cool;
};

QList<Zone> zones()
{
    return { { "sejour", "Séjour", "séjour", true },
             { "cuisine", "Cuisine", "cuisine", true },
             { "bureau", "Bureau", "bureau", true },
             { "parents", "Chambre parents", "chambre parents", true },
             { "chambre1", "Chambre 1", "chambre 1", true },
             { "chambre2", "Chambre 2", "chambre 2", true },
             { "sdb", "Salle de bain", "salle de bain", false },
             { "entree", "Entrée", "entrée", false },
             { "buanderie", "Buanderie", "buanderie", false } };
}

void setDeviceParam(Device *dev, const char *name, const char *value)
{
    for (auto &p: dev->deviceParams())
    {
        if (p.first == name)
        {
            p.second = value;
            return;
        }
    }
    dev->deviceParams().push_back({ name, value });
}

bool buildFloorHouse(const QString &demoDir)
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();

    Room *technique = addRoom("Technique", "cave");
    Room *maison = addRoom("Maison", "divers");
    QHash<QString, Room *> rooms;
    for (const Zone &z: zones())
    {
        const char *type = z.key == "sejour" ? "sam" : z.key == "cuisine" ? "cuisine" : z.key == "bureau" ? "bureau"
                           : z.key.startsWith("chambre") || z.key == "parents" ? "chambre" : "divers";
        rooms[z.key] = addRoom(z.room, type);
    }

    int var = 10;
    for (const Zone &z: zones())
    {
        mqttTemp(rooms[z.key], "temp_" + z.key, "Température " + z.name, "maison/" + z.key + "/temperature");
        wagoRelay(rooms[z.key], "vanne_" + z.key, "Vanne plancher " + z.name, var++);
    }
    mqttHumidity(rooms["sejour"], "hum_sejour", "Humidité séjour", "maison/sejour/humidite");

    IOBase *chaud = wagoRelay(technique, "pac_demande_chaud", "PAC demande chaud", 30);
    IOBase *froid = wagoRelay(technique, "pac_demande_froid", "PAC demande froid", 31);
    IOBase *mode = input(maison, { { "type", "InternalInt" }, { "id", "mode_plancher" }, { "name", "Mode plancher" },
                                   { "save", "true" }, { "value", "0" } });

    QHash<QString, Device *> devs;
    for (const Zone &z: zones())
    {
        QString shot;
        if (z.key == "sejour")
            shot = "inst-wizard-floor-zone";
        else if (z.key == "sdb")
            shot = "inst-wizard-floor-zone-heat";
        devs[z.key] = runWizard("thermostat_floor_zone", rooms[z.key], shot, [&](DialogNewDevice *d)
        {
            setText(d, "name", "Plancher " + z.name);
            choose(d, "current_temp", "temp_" + z.key);
            choose(d, "output", "vanne_" + z.key);
            setMulti(d, "modes", z.cool ? QStringList { "off", "heat", "cool" } : QStringList { "off", "heat" });
            if (z.key == "sejour")
            {
                choose(d, "humidity", "hum_sejour");
                setNumber(d, "humidity_max", 60);
            }
        });
        if (!devs[z.key])
            return false;
    }

    // Domestic hot water: its own sensor and relay, no link with the zones.
    input(technique, { { "type", "WITemp" }, { "id", "ecs_temp" }, { "name", "Température ballon ECS" },
                       { "host", "192.168.1.20" }, { "port", "502" }, { "var", "20" } });
    wagoRelay(technique, "ecs_relais", "Relais ECS", 32);
    Device *ecs = runWizard("thermostat_heat", technique, "inst-wizard-dhw", [](DialogNewDevice *d)
    {
        setText(d, "name", "Eau chaude sanitaire");
        choose(d, "current_temp", "ecs_temp");
        choose(d, "output_heat", "ecs_relais");
        choose(d, "emitter", "water_radiator");
    }, 770);
    if (!ecs)
        return false;
    setDeviceParam(ecs, "hysteresis", "3");
    setDeviceParam(ecs, "valid_min", "5");
    setDeviceParam(ecs, "valid_max", "95");
    for (IOBase *io: ecs->ownedIos())
    {
        if (io->get_param("id").find("target_temp") != std::string::npos)
        {
            io->set_param("value", "55");
            io->set_param("value_max", "65");
        }
    }

    // The global mode drives the zones.
    Rule *r = rule("Plancher : arrêt général");
    when(r, mode, "==", "0");
    for (const Zone &z: zones())
        then(r, devs[z.key], "off");

    r = rule("Plancher : mode chaud");
    when(r, mode, "==", "1");
    for (const Zone &z: zones())
        then(r, devs[z.key], "heat");

    r = rule("Plancher : mode froid");
    when(r, mode, "==", "2");
    for (const Zone &z: zones())
        then(r, devs[z.key], z.cool ? "cool" : "off");

    // The valves ask the heat pump, the global mode says which contact.
    for (const Zone &z: zones())
    {
        IOBase *vanne = ioById(("vanne_" + z.key).toStdString());
        r = rule("PAC chaud : vanne " + z.name + " ouverte");
        whenOutput(r, vanne, "==", "true");
        when(r, mode, "==", "1");
        then(r, froid, "false");
        then(r, chaud, "true");

        if (!z.cool)
            continue;
        r = rule("PAC froid : vanne " + z.name + " ouverte");
        whenOutput(r, vanne, "==", "true");
        when(r, mode, "==", "2");
        then(r, chaud, "false");
        then(r, froid, "true");
    }
    r = rule("PAC : toutes les vannes fermées");
    for (const Zone &z: zones())
        whenOutput(r, ioById(("vanne_" + z.key).toStdString()), "==", "false");
    then(r, chaud, "false");
    then(r, froid, "false");

    // A zone set by hand to the other direction goes back to the global mode.
    for (const Zone &z: zones())
    {
        if (z.cool)
        {
            r = rule("Plancher " + z.name + " : retour en chaud");
            when(r, devs[z.key], "==", "cool");
            when(r, mode, "==", "1");
            then(r, devs[z.key], "heat");
        }
        r = rule("Plancher " + z.name + " : retour en froid");
        when(r, devs[z.key], "==", "heat");
        when(r, mode, "==", "2");
        then(r, devs[z.key], z.cool ? "cool" : "off");
    }

    QDir().mkpath(demoDir);
    QString io = QDir(demoDir).filePath("io.xml"), rules = QDir(demoDir).filePath("rules.xml");
    if (!ProjectManager::saveIOsToFile(io) || !ProjectManager::saveRulesToFile(rules))
        fail("cannot save the floor project in " + demoDir);
    return true;
}

void shootFloorWindow(const QString &demoDir)
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();

    MainWindow w;
    w.resize(1400, 860);
    w.move(0, 0);
    w.show();
    QTest::qWaitForWindowExposed(&w);
    const QString shown = QDir::tempPath() + "/maison-plancher";
    QDir().mkpath(shown);
    for (const char *f: { "io.xml", "rules.xml" })
    {
        QFile::remove(QDir(shown).filePath(f));
        QFile::copy(QDir(demoDir).filePath(f), QDir(shown).filePath(f));
    }
    w.Load(shown);
    w.ShowPage(PAGE_PROG);
    settle();

    FormRules *rules = w.findChild<FormRules *>();
    QTreeWidget *home = w.findChild<QTreeWidget *>("tree_home");
    QTreeWidget *ruleTree = w.findChild<QTreeWidget *>("tree_rules");
    if (!rules || !home || !ruleTree)
        return fail("FormRules widgets not found");
    home->setAnimated(false);

    auto deviceNamed = [](const QString &name) -> Device *
    {
        for (int i = 0;i < ListeRoom::Instance().size();i++)
            for (Device *d: ListeRoom::Instance().get_room(i)->get_devices())
                if (d->get_param("name") == s(name))
                    return d;
        return nullptr;
    };
    Device *ecs = deviceNamed("Eau chaude sanitaire");
    if (ecs)
    {
        DialogEditDevice d(ecs, &w);
        fitScroll(&d, 700);
        // Only the parameters this page is about: the hysteresis down to the sensor bounds.
        const QPixmap whole = d.grab();
        auto labelTop = [&](const QString &text, bool bottom)
        {
            for (QLabel *l: d.findChildren<QLabel *>())
                if (l->text() == text)
                    return l->mapTo(&d, QPoint(0, bottom ? l->height() : 0)).y();
            fail("no label " + text);
            return 0;
        };
        const int from = labelTop("Hystérésis :", false) - 3;
        const int to = labelTop("Visible :", false) - 6;
        save(whole.copy(0, from, whole.width(), to - from), "inst-dhw-editor");
        d.reject();

        for (IOBase *io: ecs->ownedIos())
        {
            if (io->get_param("id").find("target_temp") == std::string::npos)
                continue;
            DialogIOProperties p(io, true, &w);
            p.resize(480, 520);
            p.show();
            settle();
            if (QToolBox *box = p.findChild<QToolBox *>("toolBox"))
                box->setCurrentIndex(1);
            settle();
            save(p.grab(), "inst-dhw-setpoint");
            p.reject();
        }
    }
    else
        fail("Eau chaude sanitaire not reloaded");

    auto shootRule = [&](const QString &name, const QString &room, const QString &shot, int height)
    {
        QTreeWidgetItem *it = findRule(ruleTree, name);
        if (!it)
            return;
        home->collapseAll();
        if (QTreeWidgetItem *r = findItem(home, room))
        {
            r->setExpanded(true);
            home->setCurrentItem(r);
        }
        ruleTree->setCurrentItem(it);
        ruleTree->scrollToItem(it);
        w.resize(1400, height);
        settle();
        // Room for every condition of a long rule.
        QSplitter *column = nullptr;
        for (QWidget *p = w.findChild<QTreeWidget *>("tree_condition");p && !column;p = p->parentWidget())
            column = qobject_cast<QSplitter *>(p);
        const QList<int> before = column ? column->sizes() : QList<int>();
        if (column && height > 860)
        {
            column->setSizes({ 280, height - 650, 200 });
            settle();
        }
        rules->setProjectModified(false);
        settle();
        save(w.grab(), shot);
        if (column)
            column->setSizes(before);
        w.resize(1400, 860);
        settle();
    };
    shootRule("Plancher : mode chaud", "Maison", "inst-rule-floor-mode", 860);
    shootRule("PAC chaud : vanne séjour ouverte", "Technique", "inst-rule-floor-heat-contact", 860);
    shootRule("PAC froid : vanne séjour ouverte", "Technique", "inst-rule-floor-cool-contact", 860);
    shootRule("PAC : toutes les vannes fermées", "Technique", "inst-rule-floor-all-off", 1150);
    shootRule("Plancher séjour : retour en chaud", "Séjour", "inst-rule-floor-putback", 860);

    // The technical room, the mode variable and two zones: the others look the same.
    home->collapseAll();
    for (const char *room: { "Technique", "Maison", "Séjour", "Salle de bain" })
        if (QTreeWidgetItem *r = findItem(home, room))
            r->setExpanded(true);
    for (QTreeWidgetItemIterator it(home);*it;++it)
        if (dynamic_cast<QTreeWidgetItemDevice *>(*it) && ((*it)->text(0) == "Plancher séjour" || (*it)->text(0) == "Plancher salle de bain"))
            (*it)->setExpanded(true);
    settle();
    int rows = 0;
    for (QTreeWidgetItemIterator it(home);*it;++it)
        if (!(*it)->isHidden() && home->visualItemRect(*it).isValid())
            rows += home->visualItemRect(*it).height();
    w.resize(1400, 860 + qMax(0, rows - home->viewport()->height() + 8));
    settle();
    save(home->grab(), "inst-tree-floor");
    rules->setProjectModified(false);
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
    const bool withZones = args.contains("--zones");
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
#ifdef QT_MQTT_AVAILABLE
    shootDiscovery();
#else
    fail("built without Qt MQTT: no discovery panel");
#endif
    if (withZones && buildFloorHouse(demoDir + "/floor"))
        shootFloorWindow(demoDir + "/floor");

    if (!g_failures.isEmpty())
    {
        qCritical().noquote() << g_failures.size() << "failure(s):\n " + g_failures.join("\n ");
        return 1;
    }
    return 0;
}
