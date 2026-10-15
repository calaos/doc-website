pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtTest
import SharedComponents
import "file:@QML@/mobile" as Mobile
import "."

// Phone captures (portrait, French). Run through ../capture.sh.
TestCase {
    id: tc
    name: "DocShotsPhone"
    when: windowShown
    visible: true
    width: 390
    height: 844

    readonly property string shotsDir: harness.env("CLIMATE_SHOTS_DIR")
    property real fakeNow: 1000000
    onFakeNowChanged: harness.setNow(fakeNow)
    function fakeClock() { return fakeNow }

    DocHouse { id: demo }

    Rectangle { anchors.fill: parent; color: "#080808"; z: -1 }

    function save(img, file) {
        verify(img.width > 0)
        verify(shotsDir !== "", "CLIMATE_SHOTS_DIR is set")
        img.save(shotsDir + "/" + file)
    }

    function init() {
        harness.reset()
        verify(harness.setLanguage("fr"), "lang/calaos_fr.qm loads")
    }

    function cleanup() { harness.setLanguage("") }

    Component {
        id: listComponent
        Rectangle {
            id: frame
            color: "#080808"
            width: 410
            height: col.implicitHeight + 20
            Column {
                id: col
                x: 10; y: 10
                spacing: 10
                Repeater {
                    model: ["salon", "chambre", "bureau", "pac", "plancher", "buanderie"]
                    delegate: Loader {
                        id: slot
                        required property var modelData
                        width: 390
                        sourceComponent: IOClimate {
                            clock: tc.fakeClock
                            modelData: demo.ioOf(slot.modelData)
                        }
                    }
                }
            }
        }
    }

    function test_list() {
        var f = createTemporaryObject(listComponent, tc)
        verify(f !== null)
        waitForRendering(f)
        save(grabImage(f), "mobile-list.png")
    }

    Component {
        id: sheetComponent
        Mobile.ClimateSheet { clock: tc.fakeClock }
    }

    function openSheet(key) {
        var host = createTemporaryObject(sheetComponent, tc)
        host.openFor(demo.ioOf(key))
        tryVerify(function() { return host.opened }, 3000)
        wait(300)
        return host
    }

    function closeSheet(host) {
        host.close()
        tryVerify(function() { return !host.opened && !host.modeOpened && !host.moreOpened }, 3000)
        wait(300)
        host.destroy()
        wait(50)
    }

    function test_sheets() {
        var host = openSheet("salon")
        save(grabImage(tc), "mobile-sheet.png")

        mouseClick(demo.find(Overlay.overlay, "modePill"))
        tryVerify(function() { return host.modeOpened }, 3000)
        wait(300)
        save(grabImage(tc), "mobile-sheet-mode.png")
        closeSheet(host)

        host = openSheet("salon")
        mouseClick(demo.find(Overlay.overlay, "moreLink"))
        tryVerify(function() { return host.moreOpened }, 3000)
        wait(300)
        save(grabImage(tc), "mobile-sheet-more.png")
        closeSheet(host)

        host = openSheet("plancher")
        save(grabImage(tc), "mobile-sheet-thermostat.png")
        closeSheet(host)
    }
}
