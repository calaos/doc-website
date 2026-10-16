Calaos example: one electric radiator
=====================================

What the example shows
- An internal thermostat ("Heating thermostat" template, electric radiators emitter) that switches the relay of a radiator according to a room sensor.
- A schedule: 21 °C on weekdays from 7:30 to 18:00 ("Bureau occupé" time range), 17 °C the rest of the time.
- An open window stops the radiator, a closed window starts it again.

IOs to replace
The IOs whose name ends with "(à remplacer)" (to be replaced) are internal
variables: the project opens and works without hardware. Change the
temperature of a sensor from an interface and watch the thermostat output
follow. A placeholder sensor is only updated when you change it: after 2.5 h
without a change, the thermostat considers it silent and switches its output off.
- Température bureau: the room sensor (MQTT, Zigbee, 1-Wire, Wago...).
- Fenêtre bureau: the window contact (digital input).
- Relais radiateur bureau: the relay or pilot wire of the radiator (digital output).

To replace an IO:
- in Calaos Installer, create the real IO, open the device (double click) and
  choose it for the link concerned, fix the rules that used the old one, then
  delete the placeholder IO;
- or, in io.xml, replace the line of the IO with the one of your hardware,
  keeping the same id: the device and the rules follow with no other change.

Open it in Calaos Installer
1. Unzip the archive: you get a folder with io.xml and rules.xml.
2. File menu, "Open a projet...", and choose that folder.

Send it to a Calaos box
Calaos Server menu, "Send project to Calaos Server...", then the address of the
box and its credentials (user / pass by default). The project replaces the
configuration of the box: first fetch yours with "Download project from Calaos
Server..." if you want to keep it.

Documentation: https://doc.calaos.fr/en/calaos_installer/devices/examples/
