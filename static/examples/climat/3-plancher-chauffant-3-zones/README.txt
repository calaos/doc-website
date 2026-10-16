Calaos example: a 3-zone floor heating and a circulator
=======================================================

What the example shows
- Three floor heating thermostats (living room, kitchen, bedroom), each with its sensor and its valve.
- A common circulator: it runs as soon as a valve is open and stops when all are closed. The rules read the valves themselves.

IOs to replace
The IOs whose name ends with "(à remplacer)" (to be replaced) are internal
variables: the project opens and works without hardware. Change the
temperature of a sensor from an interface and watch the thermostat output
follow. A placeholder sensor is only updated when you change it: after 2.5 h
without a change, the thermostat considers it silent and switches its output off.
- Température séjour, cuisine, chambre: the room sensors.
- Vanne plancher séjour, cuisine, chambre: the zone valves (digital outputs).
- Circulateur chauffage: the relay of the circulator or boiler.

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
