+++
title = 'LUA Scripting'
date = 2024-03-06T20:13:15Z
weight = 70
summary = "Writing code when rules are no longer enough."
+++

## When you need it

Rules cover the vast majority of needs, and it is better to stick with them for as long as they suffice: they can be read at a glance, and anyone can change them.

A script becomes useful when you hit:

- **a calculation** — an average of several sensors, a target value depending on the outside temperature;
- **a loop** — handling a list of devices without writing fifteen actions;
- **nested conditional logic** — several cases that would turn into a dozen rules;
- **a call to an external service** — querying a weather API, notifying a third-party service.

The language used is **Lua**, chosen for its simplicity and small footprint.

## Where to write a script

A script is used in two places within a rule:

**As a condition** — the script returns true or false, and the rule runs accordingly.

**As an action** — the script runs when the rule triggers.

In both cases, the editor built into Calaos Installer provides syntax highlighting.

## Reaching your devices

A global object named `calaos` gives access to your installation. Each IO is designated by its **identifier**, visible in its properties.

### Reading and writing a value

```lua
-- Read the state of an IO
local temperature = calaos:getIOValue("io_12")

-- Act on an output
calaos:setIOValue("output_3", true)
```

`getIOValue` returns a boolean, a number or text depending on the IO type. The names `getInputValue`, `getOutputValue` and `setOutputValue` also exist and do the same thing: they date from older versions.

### Reading and writing a parameter

```lua
local name = calaos:getIOParam("io_12", "name")
calaos:setIOParam("io_12", "visible", "false")
```

### Waiting for a change

```lua
calaos:waitForIO("io_12")
```

The script pauses until the IO changes state.

### Querying a web address

```lua
local response = calaos:requestUrl("https://example.com/api")
```

### Sending a notification

```lua
calaos:sendPushNotif("The freezer is above -15 °C")
```

### Printing a diagnostic message

```lua
print("Measured temperature: " .. temperature)
```

`print` writes to the server logs — see [Logs]({{% relref "calaos_os/configuration/logs" %}}). It is your main tool for understanding what a script does.

## A complete example

Switching on a booster heater if the average of three sensors drops below the target:

```lua
local s1 = calaos:getIOValue("temp_living")
local s2 = calaos:getIOValue("temp_hallway")
local s3 = calaos:getIOValue("temp_bedroom")

local average = (s1 + s2 + s3) / 3
local target = calaos:getIOValue("var_target")

print("Average: " .. average .. " / target: " .. target)

if average < target then
    calaos:setIOValue("booster_heater", true)
else
    calaos:setIOValue("booster_heater", false)
end
```

The target here is an [internal variable]({{% relref "calaos_installer/internal_var" %}}) adjustable from the applications: the user tunes their comfort without anyone touching the script.

## Good practice

{{% notice warning %}}
**A script blocks the server while it runs.** Avoid long loops and needless waits: a request to an unreachable site can freeze your home automation for the length of the timeout.
{{% /notice %}}

**Comment your identifiers.** `io_12` says nothing; a comment line recalling which device it is will save you time later.

**Check your values before computing.** An unplugged sensor can return an unexpected value, and a division by zero will stop the script.

**Test with `print`.** Show intermediate values in the logs before letting the script act on real devices.

**Prefer a rule when you can.** A script that only switches a lamp on when another comes on is harder to maintain than the equivalent rule.

## See also

- [Create rules]({{% relref "calaos_installer/rules" %}}) — where scripts fit in
- [Internal Variable]({{% relref "calaos_installer/internal_var" %}}) — to exchange values with your rules
