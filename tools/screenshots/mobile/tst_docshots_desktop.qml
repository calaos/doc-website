pragma ComponentBehavior: Bound

import QtQuick
import QtTest
import SharedComponents
import "file:@QML@/desktop" as Desktop
import "."

// Desktop captures (list column and climate dialog, French). Run through ../capture.sh.
TestCase {
    id: tc
    name: "DocShotsDesktop"
    when: windowShown
    visible: true
    width: 1280
    height: 800

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
            width: 386
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
                        // a column of the desktop room view is about 366 wide
                        width: 366
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
        save(grabImage(f), "mobile-desktop-list.png")
    }

    // the layer main.qml gives the dialogs
    Desktop.OverlayLayer {
        id: layer
        objectName: "dialogOverlayLayer"
    }

    Component {
        id: dialogComponent
        Desktop.DialogClimate { clock: tc.fakeClock }
    }

    function test_dialog() {
        var host = createTemporaryObject(dialogComponent, tc)
        host.openFor(demo.ioOf("salon"))
        tryVerify(function() { return host.detail.showing && host.detail.opacity === 1 }, 3000)
        wait(400)
        save(grabImage(tc), "mobile-desktop-dialog.png")
        mouseClick(host.detail.negativeButton)
        tryVerify(function() { return !host.detail.showing }, 3000)
        wait(300)
        host.destroy()
    }
}
