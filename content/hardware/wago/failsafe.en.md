+++
title = 'Failsafe mode'
date = 2024-03-06T20:03:53Z
weight = 20
+++

## Wago PLC Fallback Mode

The fallback mode in Calaos is an alternative operational state activated when the Calaos server can no longer communicate with the PLC, such as during a network failure. The PLC switches to fallback mode after a certain period (30 seconds) and continues to control certain elements like lighting points and shutters using switches, although not all server mode rules can be managed. The fallback mode can only execute a few simple rules automatically generated by the Calaos Installer software during the PLC's configuration. Inputs can be associated with specific actions such as toggle, direct, shutter, etc., with parameters defined for each action.

## LED

The USR LED on a Wago PLC with Calaos indicates the operational state:

- **Green** : Everything is functioning normally. The PLC is communicating with the Calaos server, and all configured functions are active.
- **Orange** : The PLC is in fallback mode, often due to a loss of communication with the server. In this mode, there are limitations: some basic functions can still be manually controlled, like lighting or shutters, but without the full range of commands and usual rules.

## Supported Rules in Fallback Mode

Here's a list of rules supported in fallback mode:

|Type|Notes|
|-|-|
|Toggle|Acts like a toggle switch, flipping the output to 0 or 1 with each press|
|Direct|Sets the output to 1 as long as the input is 1 and to 0 as soon as released|
|Shutter|Controls a classic shutter with 2 output channels for up/down|
|Pulse Shutter|Controls a shutter with a pulse instead of maintaining the relays|
|DALI Toggle|Controls a DALI light in ON/OFF|
|DALI Group Toggle|Controls a DALI group in ON/OFF|
|KNX Toggle|Controls a KNX output (only for KNX/Wago installations)|
|Direct KNX|Same as Direct but in KNX|

## Programming Fallback Mode

The simplest way to program the fallback configuration in the PLC is to use Calaos Installer and the **Send Fallback Program** tool in the **Wago** menu. You must be connected to the PLC beforehand.

{{% notice note %}}
Calaos Installer will automatically send and convert the configured rules. Keep in mind that complex rules are not converted and will therefore not be used in fallback mode. This may leave some switches non-functional in fallback mode. A trick is to use a 2nd configuration exclusively for fallback mode in Calaos Installer. This configuration should not be sent to Calaos Server, but only to the PLC.
{{% /notice %}}

## Programming Fallback Mode via Console

You can use the `wago_test` tool to program the fallback mode. This tool is capable of sending and receiving a complete config or configuring a single input.

To retrieve the config:

    wago_test host 192.168.0.xxx action save_config failsafe_config.txt

To send the config:

    wago_test host 192.168.0.xxx action load_config failsafe_config.txt

The file contains a configuration in this form:

    WAGO_SET_OUTTYPE 0 0
    WAGO_SET_OUTADDR 0 0 0 -1
    WAGO_SET_OUTTYPE 1 1
    WAGO_SET_OUTADDR 1 25 0 -1
    WAGO_SET_OUTTYPE 2 3
    WAGO_SET_OUTADDR 2 30 31 -1
    WAGO_SET_OUTTYPE 3 1
    WAGO_SET_OUTADDR 3 0 0 1

To change the config of an input:

    wago_test host 192.168.0.xxx action set_outtype 2 VOLET
    wago_test host 192.168.0.xxx action set_outaddr 2 1 2 -1

In fallback mode, each input is associated with an action of a following type:

|Type|Notes|
|-|-|
|NONE|Default action, does nothing|
|TELERUPTEUR|Acts like a toggle switch, flipping the output to 0 or 1 with each press|
|DIRECT|Sets the output to 1 as long as the input is 1 and to 0 as soon as released|
|VOLET|Controls a classic shutter with 2 output channels for up/down|
|VOLET IMPULSE|Controls a shutter with a pulse instead of maintaining the relays|
|TELERUPTEUR DALI|Controls a DALI light in ON/OFF|
|TELERUPTEUR DALI GROUP|Controls a DALI group in ON/OFF|
|TELERUPTEUR KNX|Controls a KNX output (only for KNX/Wago installations)|
|DIRECT KNX|Same as Direct but in KNX|

Then each input is associated with 3 parameters:

- **out1**: output address. For example, the lamp's address for the TOGGLE type, the shutter's up address for the SHUTTER type, the DALI address for the DALI type, etc.
- **out2**: 2e adresse de sortie si besoin (utilisé que dans le cas des volets pour la descente)
- **SameAs**: A second output address if needed (used only in the case of shutters for lowering).

 On peut avoir cette série de règles :

    Input 0 -> NONE
    Input 0 -> out1=0  out2=0  SameAs=-1
    Input 1 -> TELERUPTEUR
    Input 1 -> out1=25 out2=0  SameAs=-1
    Input 2 -> VOLET
    Input 2 -> out1=30 out2=31 SameAs=-1
    Input 3 -> TELERUPTEUR
    Input 3 -> out1=0  out2=0  SameAs=1

Here, Input 0 is configured to do nothing. Input 1 controls a light connected to output 25. Input 2 operates a shutter, with raising linked to address 30 and lowering to address 31. Input 3 is unique. It's set up as a toggle switch (similar to Input 1) but with addresses set to 0 and the "SameAs" parameter set to 1. This means Input 3 will control the same light as Input 1, using the same function block and maintaining the light's state. This allows multiple switches to control the same light/shutter. Remember, "SameAs" should be set to -1 if it's not in use.
