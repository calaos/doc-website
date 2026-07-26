+++
title = 'Email'
date = 2024-03-06T20:16:00Z
weight = 90
summary = "Configuring email sending for the notifications of your rules."
+++

## What it is for

Your server can send you emails from your rules: warning you that an alarm went off, that a door was left open, that a temperature is abnormal, or sending a picture taken by a camera.

For that, you must tell it which sending service to use: this is called an **SMTP server**. In practice, the one from your Internet provider or your mail service.

These settings are not available from the touchscreen: connect over SSH (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})).

## What you need

Before you start, gather the information provided by your mail service:

- the **address of the sending server**, such as `smtp.example.com`;
- the **port** to use, often 587 or 465;
- your **user name** and **password**, if the service requires authentication;
- whether the connection must be **encrypted** (TLS), which is the case almost everywhere today.

This information is in your provider's help pages, often under a heading such as "configure a mail client".

## Configuring

The settings are defined with the [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}) tool:

```sh
calaos_config set smtp_server smtp.example.com
calaos_config set smtp_port 587
calaos_config set smtp_auth true
calaos_config set smtp_tls true
calaos_config set smtp_username my_user
calaos_config set smtp_password my_password
```

Here is what each setting does:

| Setting | Purpose |
|---|---|
| `smtp_server` | Address of the sending server |
| `smtp_port` | Port of the sending server |
| `smtp_auth` | `true` if the server requires a user name and a password |
| `smtp_tls` | `true` for an encrypted connection |
| `smtp_username` | Your user name |
| `smtp_password` | Your password |

To check what has been saved:

```sh
calaos_config list
```

{{% notice tip %}}
Many mail services now refuse your usual password for this kind of use and require an **application password**, to be generated from your account. If sending fails while your credentials are correct, this is very likely the cause.
{{% /notice %}}

## Testing

The `calaos_mail` tool sends a test message. First prepare a file containing the message text, then send it:

```sh
echo "This is a test from my Calaos server." > /tmp/test.txt
calaos_mail --from me@example.com --to me@example.com --subject "Calaos test" --body /tmp/test.txt
```

If the message arrives, the configuration is correct.

If it fails, run the command again adding `--verbose`: the details of the exchange with the sending server are displayed, which usually shows which setting is at fault.

```sh
calaos_mail --from me@example.com --to me@example.com --subject "Calaos test" --body /tmp/test.txt --verbose
```

{{% notice note %}}
Remember to check your junk mail folder: messages sent by a home server often land there the first time.
{{% /notice %}}

## Attaching files

The `--attach` option attaches a file, and can be repeated to attach several:

```sh
calaos_mail --from me@example.com --to me@example.com --subject "Picture" --body /tmp/test.txt --attach /tmp/photo.jpg
```

This is the mechanism that lets you receive a camera picture by email.

## Using email in your rules

Once sending is configured, you can trigger emails from your automation rules. Creating rules is described in [Rules]({{% relref "calaos_installer/rules" %}}), in the Calaos Installer chapter.
