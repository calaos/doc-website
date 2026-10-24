+++
title = 'Approver phone and web console'
date = 2026-10-24T06:00:00Z
weight = 23
summary = "Enrol your phone as an approver, open the root console from the web app, and recover if you lose the phone."
+++

## Why an approver phone

Some actions give full control of the box. For these, the admin password is not enough: the box also sends a **6-digit code** to a phone you enrolled in advance, the **approver phone**, and you type that code in the web app. Someone who found out your admin password still cannot act without holding your phone.

The code protects:

- opening the **console** (a root terminal on the box, in your browser);
- changing the **admin password** from the web app;
- adding or removing an approver phone.

Updates and restarting the box stay protected by the admin password only. See [Administering from the web app]({{% relref "calaos_os/admin" %}}).

A code is valid for 60 seconds and works once. Nothing arrived? Check that notifications are allowed for the Calaos app, then ask for a new code after 10 seconds. After 5 wrong codes, the admin session closes and you must enter the admin password again.

{{% notice info %}}
Without an approver phone, the console cannot be opened and the admin password can only be changed over SSH with `calaos-os admin set`.
{{% /notice %}}

## Enrol your phone

You need the Calaos app on the phone, signed in to this box, with notifications allowed: the codes arrive as push notifications.

1. In the Calaos app, open **Settings** and fill in **Name of this device**, for example `Marie's iPhone`. Since iOS 16, every iPhone is called just "iPhone" otherwise, and you could not tell two phones apart. The name is sent to the box right away. Leave it empty to use the phone's own name.
2. In the web app, open the **Admin** tab (over https, see [Administering from the web app]({{% relref "calaos_os/admin" %}})) and unlock it.
3. In the **Approver phones** section, click **Add a phone**. The list shows the phones where the app is signed in to this box. If several share a name, go by the date and the last activity.
4. Pick your phone and click **Send a code to this phone**.
5. Enter the code you received on the phone in the **6-digit code** field and click **Confirm**. This proves you hold the phone.

The phone then appears in the list. All the phones signed in to the box receive a notification, and the web app shows a security banner at the next sign-in: **New approver phone: &lt;name&gt;**. If this was not you, someone knows the admin password: see [Lost phone](#lost-or-reinstalled-phone).

You can enrol **two** phones at most. To add a second one, the first one receives a code to allow it. To remove one, click **Remove** next to it and enter the code; the last phone cannot be removed, so to change phones, add the new one first, then remove the old one.

{{% notice warning %}}
A phone whose app has not connected to the box for 30 days stops receiving codes. The list then tells you to open the Calaos app on that phone: do so, and it receives codes again.
{{% /notice %}}

## Open the console

The console is a terminal on the box, with root rights, inside your browser. It replaces SSH for quick checks.

1. Open the web app over **https://** and unlock the **Admin** tab.
2. In the **Box** section, on the **Console** line, click **Open**.
3. A code is sent to your approver phone. Enter it in the **6-digit code** field and click **Confirm**.
4. The terminal opens. A counter shows when it closes.

Each opening triggers a **Calaos: console opened** notification on all the phones signed in to the box.

Limits:

| Limit | Value |
|---|---|
| Inactivity | closes after **15 minutes** without typing |
| Total duration | **1 hour**, even if you keep typing |
| At the same time | **2 consoles** on the box |
| Connection | **https** only, never plain http |

Nothing is kept after closing: open a new console with a new code.

### When the console closes

| Message | Meaning |
|---|---|
| The code was used or is more than 30 s old. Ask for a new one. | The code is single use and the console must be opened within 30 seconds of entering it. Start again. |
| The console only opens over https://. | Open the web app with an address starting with `https://`. |
| The shell exited. | You typed `exit` or ended the shell. Normal. |
| The console reached its 60 min limit. | The 1 hour maximum. Open a new one. |
| The console closed after 15 min without typing. | Idle for 15 minutes. |
| The box ended the console | The admin password was changed or reset, too many wrong codes were typed, the box is stopping, or its side of the console was lost. |
| The console closed: what was pasted was too large. | Paste in smaller pieces. |
| Connection to the console lost. | Network problem. Open a new console. |
| N consoles are already open on the box | Two consoles already run, possibly from another browser. Close one first. |
| You closed the console. | You clicked **Close the console**. |

## Lost or reinstalled phone

A phone that is reinstalled, reset or replaced no longer receives codes, even if the app is back.

- **You have a second approver phone**: use it to remove the lost one (**Remove**, then enter the code), then add the new one.
- **You have no other approver phone**: connect to the box over SSH as `root` (see [SSH access]({{% relref "calaos_os/advanced/shell" %}})) and run:

  ```sh
  calaos-os admin reset
  ```

  This removes the admin password **and all approver phones**. Change the Calaos Server password if you suspect someone else, then choose a new admin password in the web app and enrol your phone again. See [If you lose the admin password]({{% relref "calaos_os/admin#if-you-lose-the-admin-password" %}}).

`calaos-os admin set` replaces the admin password but keeps the approver phones.

## Certificate confirmation in the mobile app

The Calaos app on a phone connects to a box on your network with a certificate the box created itself. On first contact, the app shows **Unknown certificate** with the key fingerprint (SHA-256).

- Compare it with the box's if you can, then tap **Trust**. The app remembers this box's key: it will not ask again.
- Tap **Do not connect** if in doubt.
- If the key later changes, the app shows **Certificate changed!** and asks you to tap twice. That is normal after a reinstallation of the box, but it can also mean someone is intercepting the connection: trust it only if you know why.
- The remembered boxes are listed under **Trusted certificates** in **Settings**; **Forget** makes the app ask again.

When you use a name with a valid Let's Encrypt certificate (see [calaos_ddns]({{% relref "calaos_os/configuration/calaos_ddns" %}})), the app verifies the certificate itself and shows no confirmation. If it does show one for such a name, the connection may be intercepted: refuse it.
