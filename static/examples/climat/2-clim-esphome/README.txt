Calaos example: an ESPHome air conditioner
==========================================

What the example shows
- An air conditioner driven by an ESPHome module ("proxy" device): the unit regulates by itself, Calaos shows its state and sends modes and setpoints.
- The temperature of a room sensor is relayed to the unit (`remote_temp_source` link).
- An "Absence" variable that widens the setpoint range, and a night frost protection.

IOs to replace
The IOs whose name ends with "(à remplacer)" (to be replaced) are internal
variables: the project opens and works without hardware. Change the
temperature of a sensor from an interface and watch the thermostat output
follow. A placeholder sensor is only updated when you change it: after 2.5 h
without a change, the thermostat considers it silent and switches its output off.
- Température salon: the room sensor.
- Clim salon: the MQTT IOs of the unit target the broker 127.0.0.1 and the ESPHome node "clim_salon". The simplest is to create the device again with the "ESPHome climate" template of the wizard and its broker search button, then keep the rules; otherwise change the broker address and the node name in every IO of the unit.

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
