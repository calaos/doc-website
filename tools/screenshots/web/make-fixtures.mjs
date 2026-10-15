// Writes the demo-house fixtures for the mock calaos_server (wire format of
// CALAOS_DEVICES_FEAT.md 5.3). Usage: node make-fixtures.mjs <out-dir>
// Nothing here comes from a real installation.
import { mkdirSync, writeFileSync } from 'node:fs';
import { join } from 'node:path';

const out = process.argv[2];
if (!out) throw new Error('usage: make-fixtures.mjs <out-dir>');

const ios = (id, keys) => Object.fromEntries(keys.map((k) => [k, `${id}_${k}`]));

function device(id, name, impl, caps, state, keys) {
    return {
        id,
        name,
        gui_type: 'climate',
        state: state.mode,
        visible: 'true',
        unit: '',
        device: { family: 'climate', impl, capabilities: caps, state, ios: ios(id, keys) },
    };
}

const temp = (id, name, v) => ({ id, name, gui_type: 'temp', state: String(v), visible: 'true', unit: '°C' });
const hum = (id, name, v) => ({ id, name, gui_type: 'analog_in', gui_style: 'humidity', state: String(v), visible: 'true', unit: '%' });

const K_PROXY = ['mode', 'current_temp', 'target_temp', 'target_temp_low', 'target_temp_high', 'action', 'fan_mode', 'swing_mode'];
const K_THERMO = ['current_temp', 'target_temp', 'target_temp_low', 'target_temp_high', 'action'];

const devices = {
    climSalon: (o = {}) =>
        device('salon_ac', 'Clim salon', 'proxy',
            {
                modes: ['off', 'heat', 'cool', 'auto', 'dry', 'fan_only'],
                target_temp: { min: 17, max: 28, step: 0.5, unit: '°C', min_range: 2 },
                target_temp_range: true,
                fan_modes: ['auto', 'quiet', 'low', 'medium', 'high'],
                swing_modes: ['off', 'vertical', 'horizontal', 'both'],
                reports_action: true,
                remote_temp: true,
            },
            { mode: 'auto', current_temp: 19.2, target_temp: 22, target_temp_low: 19.5, target_temp_high: 24.5, action: 'heating', fan_mode: 'auto', swing_mode: 'off', humidity: 48, ...o },
            K_PROXY),
    climChambre: (o = {}) =>
        device('chambre_ac', 'Clim chambre', 'proxy',
            {
                modes: ['off', 'heat', 'cool', 'auto'],
                target_temp: { min: 16, max: 30, step: 1, unit: '°C' },
                fan_modes: ['auto', 'low', 'medium', 'high'],
            },
            { mode: 'cool', target_temp: 24, fan_mode: 'low', ...o },
            ['mode', 'target_temp', 'fan_mode']),
    radiateurBureau: (o = {}) =>
        device('bureau_heat', 'Radiateur bureau', 'thermostat',
            { modes: ['off', 'heat'], target_temp: { min: 7, max: 28, step: 0.5, unit: '°C' }, reports_action: true },
            { mode: 'heat', current_temp: 19.4, target_temp: 21, action: 'heating', ...o },
            K_THERMO),
    pacCuisine: (o = {}) =>
        device('cuisine_pac', 'PAC cuisine', 'thermostat',
            {
                modes: ['off', 'heat', 'cool', 'auto'],
                target_temp: { min: 16, max: 30, step: 0.5, unit: '°C', min_range: 2 },
                target_temp_range: true,
                reports_action: true,
            },
            { mode: 'auto', current_temp: 22.1, target_temp: 22.5, target_temp_low: 20, target_temp_high: 25, action: 'idle', ...o },
            K_THERMO),
    plancherSejour: (o = {}) =>
        device('sejour_floor', 'Plancher séjour', 'thermostat',
            {
                modes: ['off', 'heat', 'cool', 'auto'],
                target_temp: { min: 16, max: 30, step: 0.5, unit: '°C', min_range: 2 },
                target_temp_range: true,
                reports_action: true,
            },
            { mode: 'cool', current_temp: 24.6, target_temp: 23, target_temp_low: 20, target_temp_high: 25, action: 'cooling', humidity: 52, ...o },
            K_THERMO),
    climBuanderie: (o = {}) =>
        device('buanderie_ac', 'Clim buanderie', 'proxy',
            {
                modes: ['off', 'heat', 'cool', 'auto'],
                target_temp: { min: 16, max: 30, step: 0.5, unit: '°C', min_range: 2 },
                target_temp_range: true,
                reports_action: true,
            },
            { mode: 'heat', current_temp: 17.5, target_temp: 20, action: 'heating', connected: false, ...o },
            K_PROXY),
};

const room = (name, type, hits, items) => ({ name, type, hits: String(hits), items });

function house(overrides = {}) {
    return [
        room('Salon', 'lounge', 50, [temp('salon_temp', 'Température salon', 19.2), hum('salon_hum', 'Humidité salon', 48), devices.climSalon()]),
        room('Chambre', 'bedroom', 40, [devices.climChambre()]),
        room('Bureau', 'office', 30, [temp('bureau_temp', 'Température bureau', 19.4), devices.radiateurBureau(overrides.bureau)]),
        room('Cuisine', 'kitchen', 20, [temp('cuisine_temp', 'Température cuisine', 22.1), devices.pacCuisine()]),
        room('Séjour', 'lounge', 10, [temp('sejour_temp', 'Température séjour', 24.6), hum('sejour_hum', 'Humidité séjour', 52), devices.plancherSejour()]),
    ];
}

// One room holding every status at once, for the room-list picture only.
function showcase() {
    return [
        room('Salon', 'lounge', 50, [
            temp('salon_temp', 'Température salon', 19.2),
            hum('salon_hum', 'Humidité salon', 48),
            devices.climSalon(),
            devices.plancherSejour(),
            devices.pacCuisine({ action: 'idle' }),
            devices.radiateurBureau({ action: 'idle', lockout: { output: 'heat', remaining: 140 } }),
            devices.climChambre({ mode: 'off' }),
            devices.climBuanderie(),
        ]),
    ];
}

const write = (name, home) => {
    mkdirSync(join(out, name), { recursive: true });
    writeFileSync(join(out, name, 'home.json'), JSON.stringify({ home, cameras: [], audio: [] }, null, 2));
};

write('house', house());
write('fault', house({ bureau: { fault: 'sensor_stale', action: 'off' } }));
write('showcase', showcase());
