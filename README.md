# SAMD_Fusee_Multiloader 
Multiloader firmware for the SAMD_Loader board - user can now select from one of 3 payloads

* (1) - hekate_ctcaer_3.2

* (2) - ReiNX Launcher

* (3) - SXOS Launcher

![Top-Bottom_Pic](./images/Top_Bottom%20-%20Small.jpg)

[If you would like to purchase a pre-built board, please see Tindie Page](https://www.tindie.com/products/13711/)



This firmware was inspired by, and borrows from, mattytrog's implementation of Gemma multiloader.  Thanks mattytrog!


On powerup, the board will blink the blue activity LED 1, 2, or 3 times to indicate active payload


To change payload - hold the change button (D1), the device will cycle through 1, 2, then 3 blinks to
indicate payload selection.  Let go of change button once your desired payload is indicated

To store payload, momentarily press the store button (D0). The activity LED will light for 1 second to indicate flashing
and upon the next reset, your desired payload will be automatically selected.

The rest is the same, device will blink 1-3 times while searching for Tegra, then give a short-long "Ta-daa" blink
as the payload is sent and activated.  At that point, you're done!  Unplug and switch the loader off.

