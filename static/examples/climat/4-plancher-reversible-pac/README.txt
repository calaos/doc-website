Calaos example: a 9-zone reversible floor, a heat pump and hot water
====================================================================

What the example shows
- Nine reversible floor zones ("Reversible floor zone" template, one valve per zone); three zones only heat.
- The "Mode plancher" variable (0 off, 1 heat, 2 cool) and the rules that give it to every zone.
- The two heat pump contacts, "demande chaud" and "demande froid" (heat and cool demand), driven by rules and never on together.
- The rules that put back into the house mode a zone set by hand to the other direction.
- An independent domestic hot water thermostat, on the same heat pump.

At load, the server writes a "cooling has no dew-point cut" warning for each zone that cools without a humidity sensor: this is expected, the heat pump keeps the water above the dew point.

IOs to replace
The IOs whose name ends with "(à remplacer)" (to be replaced) are internal
variables: the project opens and works without hardware. Change the
temperature of a sensor from an interface and watch the thermostat output
follow. A placeholder sensor is only updated when you change it: after 2.5 h
without a change, the thermostat considers it silent and switches its output off.
- Temperature and humidity of each room: the zone sensors (only the living room has a humidity sensor).
- Vanne plancher of each zone: the electrovalves (digital outputs).
- PAC demande chaud, PAC demande froid: the two dry contacts of the heat pump.
- Température ballon ECS, Relais ECS: the tank sensor and the hot water demand contact.

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
