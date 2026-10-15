# Demo house used by every climate screenshot

One invented house, shared by the four clients, so that the pages of the documentation tell one story.
It contains nothing from a real installation (no real host, address or password). UI language: French.

## Rooms and IOs

| Room | IO (id, name) | Kind |
|---|---|---|
| Salon | `salon_temp` "Température salon" | MqttInputTemp, topic `maison/salon/temperature` |
| Salon | `salon_hum` "Humidité salon" | MqttInputAnalog / humidity sensor, 48 % |
| Salon | `salon_ac` "Clim salon" | device climate/proxy, template `esphome_climate` |
| Chambre | `chambre_ac` "Clim chambre" | device climate/proxy, template `esphome_climate_ir` |
| Bureau | `bureau_temp` "Température bureau" | MqttInputTemp |
| Bureau | `bureau_fenetre` "Fenêtre bureau" | Wago digital input (contact), false = closed |
| Bureau | `bureau_heat` "Radiateur bureau" | device climate/thermostat, `thermostat_heat`, emitter electric |
| Bureau | `bureau_radiateur` "Relais radiateur bureau" | WODigital (external output, owned by the thermostat) |
| Cuisine | `cuisine_temp` "Température cuisine" | MqttInputTemp |
| Cuisine | `cuisine_pac` "PAC cuisine" | device climate/thermostat, `thermostat_heat_cool`, emitter heat_pump |
| Cuisine | `cuisine_pac_chaud`, `cuisine_pac_froid` "PAC cuisine chaud/froid" | WODigital |
| Séjour | `sejour_temp` "Température séjour", `sejour_hum` "Humidité séjour" | MqttInputTemp / humidity |
| Séjour | `sejour_floor` "Plancher séjour" | device climate/thermostat, `thermostat_floor_heat_cool`, humidity_max 60 |
| Séjour | `plancher_chaud`, `plancher_froid` "Plancher chaud/froid" | WODigital |
| Technique | `circulateur` "Circulateur chauffage" | WODigital, common output driven by rules (decision 17) |
| Maison | `absence` "Absence" | InternalBool |

`salon_ac` template fields: topic_prefix `clim_salon`, object_id `clim_salon`, broker `127.0.0.1:1883`, no user,
all 6 modes, two_point on, 17..28 step 0.5, with_action on, fan modes auto/quiet/low/medium/high,
swing modes off/vertical/horizontal/both, remote_temp_source = `salon_temp`.
`chambre_ac`: topic_prefix `clim_chambre`, object_id `clim_chambre`, modes off/heat/cool/auto, 16..30 step 1,
fan modes auto/low/medium/high, no room temperature.
Thermostats: current_temp = the room sensor; outputs = the relays above (output_heat, output_cool).

## Time objects and rules (the heating examples of the documentation)

Time ranges: "Bureau occupé" (Mon-Fri 07:30-18:00), "Nuit" (22:00-06:30).

| Rule | Conditions | Actions |
|---|---|---|
| Bureau : confort en journée | "Bureau occupé" becomes true | `bureau_heat` = `{"mode":"heat","target_temp":21}` |
| Bureau : réduit hors journée | "Bureau occupé" becomes false | `bureau_heat` = `{"target_temp":17}` |
| Absence : abaisser le chauffage | `absence` == true | `bureau_heat` = `{"mode":"heat","target_temp":16}`, `cuisine_pac` = `{"mode":"auto","target_temp_low":16,"target_temp_high":28}`, `sejour_floor` = `{"mode":"heat","target_temp":16}` |
| Retour : rétablir | `absence` == false | `bureau_heat` = `{"target_temp":21}`, `cuisine_pac` = `{"target_temp_low":20,"target_temp_high":25}`, `sejour_floor` = `{"target_temp":21}` |
| Fenêtre bureau ouverte | `bureau_fenetre` == true | `bureau_heat` = `off` |
| Fenêtre bureau fermée | `bureau_fenetre` == false | `bureau_heat` = `heat` |
| Hors-gel : nuit d'hiver | "Nuit" true AND `salon_temp` < 10 | `salon_ac` = `{"mode":"heat","target_temp":17}` |
| Circulateur : `<zone output>` allumé (x5: bureau_radiateur, cuisine_pac_chaud, cuisine_pac_froid, plancher_chaud, plancher_froid) | `<zone output>` == true | `circulateur` = true |
| Circulateur : toutes les zones éteintes | the 5 zone outputs == false | `circulateur` = false |

(The installer produces the real `io.xml`/`rules.xml` from these tables with its own wizard and rule editor;
web, mobile and panel use fixtures that follow spec section 5.3 for the same devices.)

## Baseline states (fixtures for web, mobile, panel)

| Device | mode | action | current | setpoint(s) | other |
|---|---|---|---|---|---|
| Clim salon | auto | heating | 19.2 | low 19.5 / high 24.5 | fan auto, swing off, humidity 48, remote_temp |
| Clim chambre | cool | (no action) | - | 24 | fan low |
| Radiateur bureau | heat | heating | 19.4 | 21 | |
| PAC cuisine | auto | idle | 22.1 | low 20 / high 25 | lockout example for one shot: `{"output":"cool","remaining":140}` |
| Plancher séjour | cool | cooling | 24.6 | 23 | humidity 52, low 20 / high 25 |
| Clim buanderie (extra) | heat | - | 17.5 | 20 | `connected:false` (offline) for the "Hors ligne" shot |

Wire format of the `device` object: `/home/raoul/repos/calaos/calaos_base/CALAOS_DEVICES_FEAT.md` section 5.3,
UI statuses: `/home/raoul/repos/calaos/calaos_base/docs/devices/UI_CLIMATE.md`.
