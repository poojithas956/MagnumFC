---
title: "MagnumFC"
author: "Poojitha S"
description: "Design notes for a compact STM32F722 flight controller"
created_at: "2026-09-25"
---

# September 26: Getting oriented and gathering design references

Okay, let's do this one more time,
My name is Poojitha S,
I'm born and brought up in India. What's this project about ? Well its a custom Flight Controller that I have been ummm... wanting to make ? ig idk

Okay.. Okay.. but what am I gonna do with a flight controller ? wirelessly connect to Air Force One and .... idk.. nah i'm just kidding, seriously...

Well this is for building an opensource long range drone. For now I'm going with a small 3" to 5" fpv drone, later on advancing to fixed wings followed by v-tols (I'm a fan of the Osprey btw)

Also one more thing to note:
I'm writing these entries after doing the work, not live. I also only joined the program last week or so, and I didn't know about lapse (the session recordings) until I got informed during a review on one of my projects.. so unfortunately I don't have lapse recordings for these first sessions. I take oath to write down my hours religiously and honestly.

Starting off with the project, on 26th September 2026 I started by going through journals and GUIDEs of various projects...

- Simple Flight Controller - by NotARoomba
- MerlynFPV - by Anson ig
- Few other projects on OSHWLAB, they seemed pretty advanced tho !

So for my layout and design, I am going with the SpeedyBee F7 V3, I'll be designing around this layout / framework...
I jotted down the list of components I need for my FC, i mean they are kinda pre-default right....

| Component or section | Intended role |
| --- | --- |
| STM32F722RETx | Main flight-controller MCU |
| ICM-42688-P | SPI gyroscope and accelerometer |
| BMP388 | Barometer |
| ESP32-C3 | Wireless-connectivity coprocessor |
| W25N04KVZEIR | NAND flash shown in the schematic and PCB; confirm device support before enabling it in firmware |
| MP9943GQ-Z × 2 | Switching 9 V and 5 V rails |
| AP7361C and NCP167 | Separate digital and analog 3.3 V rails, as represented in the design |
| AT7456E | Analog OSD circuitry |
| USB-C | USB connection to the flight controller |

Layout: 30.5 mm x 30.5 mm is the standard **mounting hole spacing** for a stack, 6 layers, 1oz outer and 0.5oz inner.

**Total time spent: 1 hour 20 minutes**

# September 27 (morning): Getting started with project files

So first thing I did in the morning was get my setup right before school/college eats my day.. basically got my folders and sub folders ready which took around 5 mins, followed by downloading the datasheets and feeding them to ChatGPT... and also converting all the easyeda files to kicad using some easyeda-to-kicad python program. This took ridiculous time, around half an hour of downloading and converting and repeating..

![File-Explorer](https://cdn.hackclub.com/01a1549b-89ef-7318-b7c1-900d4d065878/paste-1792420841194.png)

Once that was done i had to leave for school... boring

**Total time spent: 45 minutes **

# September 27 (second half of the day): Drawing the MCU and OSD

I started with the schematics for my FC... so first off I drew my MCU, using references from the projects I mentioned above. So I was constantly doing ctrl+tab editing, modifying, plus checking with ChatGPT, which had all the datasheets I'd fed it... once that was done:

![MCU](https://cdn.hackclub.com/01a153f7-127a-7f56-a914-0ede79f17bb0/paste-1792410062700.png)

Next I drew the OSD. Its called the On Screen Display, and its important for Analog fpv pilots: it basically puts flight info (battery voltage and other telemetry data) right onto your goggles or monitor. Same process here... check already made designs, modify, ask ChatGPT for a sanity check, done.

![OSD](https://cdn.hackclub.com/01a153f8-fc8f-7598-ae14-a7dcccbfd01c/paste-1792410188587.png)

Once the MCU and OSD were done I organised my play area, like the drawing area in KiCad, making it look neat and organised.. girl i hv OCD..

**My own checks:** **I had to manually verified the Pins as it had multiple allocations it could be GPIO or SPI or I2C soo, plus ChatGPT got a couple things wrong so don't trust, AI is still not that well structured for Embedded engineering**

**Total time spent: 2 hours 7 minutes **

# September 28: Power supply, part 1 (3V3 logic + USB-C done)

So this is one of the most important and critical parts of designing an FC. To run, the FC needs power / current, and it gets this either directly from the battery or via the ESC. In my case its from the ESC.
I'll be using a 6S battery, which means 6 cells connected in series, 6S1P. A fully charged 6S is 25.2 V, so the converters have to handle that (plus spikes). **MP9943 max input voltage from the datasheet, 36V**

The FC powers the critical sensors and other plug-ins the drone needs, but if the power supply is unstable and can't meet the demand, you get brown-outs, which means the FC suddenly resets or freezes mid-air, kinda like Tony Stark's suit losing power while flying...

Also the sensors are picky. Noise on the 3V3 supply shows up as noise in your IMU readings, and then your motors get twitchy and the drone doesn't fly smooth. So a clean steady 3V3 matters a lot.

So an FC has multiple buck converters that turn the battery's rough power into steady voltages:

- BEC 5V 2A
- BEC 9V 2A (needed for the video transmitters, analog and digital)
- Two separate 3V3 regulators, one digital and one analog (AP7361C and NCP167).
- And finally a Type-C connector for configuring the firmware settings

These are standardised designs from other projects, so the circuits are pretty self explanatory...

I split the power work across the 28th and 29th. On the 28th I finished the 3V3 logic and the Type-C connector. Fairly moderate in terms of understanding the logic and circuitry.

![3V3 Analog](https://cdn.hackclub.com/01a153f9-d3ac-7d03-b4c2-02141979858b/paste-1792410241957.png)

![3V3](https://cdn.hackclub.com/01a153fa-17df-7539-b91c-25b3d9c6ef7f/paste-1792410261163.png)

![USB C](https://cdn.hackclub.com/01a153fa-70eb-7b57-9c85-495cf29e8ea2/paste-1792410283695.png)

**Total time spent: 1 hour 38 minutes **

# September 29: Power supply, part 2 (5v + 9v logic, VBAT detection)

So today 29th of September, I started with a cup of coffee <3 and continued with the power supply, today my target was to finish with the power supply.

Same as the previous journal, I went through a few project, asked GPT the most efficient work flow, and verified on reddit & discord, finalised the NETLIST and drew the 5v rated for 2A thats 10W, 9v rated 2A thats 18W. 

Also a VBAT sensors that messures the current coming from the power source i.e. the ESC.

![5V 2A](https://cdn.hackclub.com/01a1541a-b992-7a89-8c62-d86579edd205/paste-1792412399228.png)


![9V 2A](https://cdn.hackclub.com/01a1541b-2309-7560-b005-6fe94cb68036/paste-1792412426298.png)

![VBAT](https://cdn.hackclub.com/01a1541b-833a-7bc4-9516-be01df21d902/paste-1792412451420.png)

**Total time spent: 1 hour 39 minutes **

# September 30: IMU and Flash

After have done the Power supply, I started off with the sensors, these are core components for PID, and debugging ( via blackbox ).

So this part had to be done carefully and precisely, for the IMU i.e. the 6 DOF gyro thats is used for PID tunning, basic keeping the drone stable and less wobble or drunkard behaviour, and the flash basically stores the telemetry data.

For the IMU i choose the ICM-42688-P as its way more cleaner and less prone to vibrations than other IMU like MPU6500, or other ICM models...
MPU6000 is considered as gold, as it has proven its performance and is most loved IMU amongst the fpv community. But since MPU6000 's discontinued ppl now prefer the ICM-42688-P.

As for the Flash, theres only Winbond so yeaa.. you can chose the storage so depends on u

Once I chose my components I went through the datasheet and make a note of the required components for it to work reliable and phrased it to GPT and got a NETLIST, and routed them on the schematic sheet, and verified them using the existing project schematics... looked fine, so continued the same with the flash.

![IMU](https://cdn.hackclub.com/01a15429-d49a-7d8d-ad57-1ef8a2b57677/paste-1792413388651.png)

![Flash](https://cdn.hackclub.com/01a1542a-1b40-78ae-96fe-ad2bfecc9e3f/paste-1792413406218.png)
