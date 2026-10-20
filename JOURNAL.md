---
title: "MagnumFC"
author: "Poojitha S"
description: "Design notes for a compact STM32F722 flight controller"
created_at: "2026-09-25"
---

# September 26: Getting oriented and gathering design references

Okay, let's do this one more time,
My name is Poojitha S,
I was born and brought up in India. What's this project about? Well, it's a custom Flight Controller that I have been ummm... wanting to make? ig idk

Okay.. Okay.. but what am I gonna do with a flight controller? Wirelessly connect to Air Force One and.... idk.. nah I'm just kidding, seriously...

Well, this is for building an open-source long-range drone. For now I'm going with a small 3" to 5" FPV drone, later advancing to fixed wings followed by VTOLs (I'm a fan of the Osprey btw)

Also, one more thing to note:
I'm writing these entries after doing the work, not live. I also only joined the program last week or so, and I didn't know about lapse (the session recordings) until I got informed during a review on one of my projects.. so unfortunately I don't have lapse recordings for these first sessions. I take oath to write down my hours religiously and honestly.

Starting off with the project, on 26th September 2026 I began by going through journals and GUIDEs of various projects...

- Simple Flight Controller - by NotARoomba
- MerlynFPV - by Anson ig
- A few other projects on OSHWLAB, they seemed pretty advanced tho!

For my layout and design, I'm going with the SpeedyBee F7 V3, so I'll be designing around that layout / framework...
I jotted down the list of components I need for my FC, I mean they are kinda pre-default, right....

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

Layout: 30.5 mm x 30.5 mm is the standard **mounting hole spacing** for a stack. 6 layers, 1oz outer and 0.5oz inner.

**Total time spent: 1 hour 20 minutes**

# September 27: Getting started with project files

First thing in the morning I got my setup right before school/college eats my day.. I got my folders and sub-folders ready, which took around 5 mins, followed by downloading the datasheets and feeding them to ChatGPT... and also converting all the EasyEDA files to KiCad using some EasyEDA-to-KiCad Python program. This took ridiculous time, around half an hour of downloading and converting and repeating..

![File Explorer](https://cdn.hackclub.com/01a1549b-89ef-7318-b7c1-900d4d065878/paste-1792420841194.png)

Once that was done I had to leave for school... boring

**Total time spent: 45 minutes**

# September 27 (second half of the day): Drawing the MCU and OSD

I started with the schematics for my FC... first I drew my MCU, using references from the projects I mentioned above. I was constantly doing ctrl+tab editing, modifying, plus checking with ChatGPT, which had all the datasheets I'd fed it... once that was done:

![MCU](https://cdn.hackclub.com/01a153f7-127a-7f56-a914-0ede79f17bb0/paste-1792410062700.png)

Next I drew the OSD. It's called the On Screen Display, and it's important for analog FPV pilots: it puts flight info (battery voltage and other telemetry data) right onto your goggles or monitor. Same process here... check already-made designs, modify, ask ChatGPT for a sanity check, done.

![OSD](https://cdn.hackclub.com/01a153f8-fc8f-7598-ae14-a7dcccbfd01c/paste-1792410188587.png)

Once the MCU and OSD were done I organised my play area, like the drawing area in KiCad, making it look neat and organised.. girl i hv OCD..

**My own checks:** I had to manually verify the pins, as many of them have multiple functions (they could be GPIO, SPI or I2C), so I had to pick the right one for each job. ChatGPT also got a couple of things wrong, so don't blindly trust it. AI still isn't that reliable for embedded engineering.

**Total time spent: 2 hours 7 minutes**

# September 28: Power supply, part 1 (3V3 logic + USB-C done)

So this is one of the most important and critical parts of designing an FC. To run, the FC needs power / current, and it gets this either directly from the battery or via the ESC. In my case it's from the ESC.
I'll be using a 6S battery, which means 6 cells connected in series, 6S1P. A fully charged 6S is 25.2 V, so the converters have to handle that (plus spikes). The MP9943's max input voltage from the datasheet is 36 V, so that leaves some headroom.

The FC powers the critical sensors and other plug-ins the drone needs, but if the power supply is unstable and can't meet the demand, you get brown-outs, which means the FC suddenly resets or freezes mid-air, kinda like Tony Stark's suit losing power while flying...

Also the sensors are picky. Noise on the 3V3 supply shows up as noise in your IMU readings, and then your motors get twitchy and the drone doesn't fly smoothly. So a clean, steady 3V3 matters a lot.

So an FC has multiple buck converters that turn the battery's rough power into steady voltages:

- BEC 5V 2A
- BEC 9V 2A (needed for the video transmitters, analog and digital)
- Two separate 3V3 regulators, one digital and one analog (AP7361C and NCP167)
- And finally a Type-C connector for configuring the firmware settings

These are standardised designs from other projects, so the circuits are pretty self-explanatory...

I split the power work across the 28th and 29th. On the 28th I finished the 3V3 logic and the Type-C connector. Fairly moderate in terms of understanding the logic and circuitry.

![3V3 Analog](https://cdn.hackclub.com/01a153f9-d3ac-7d03-b4c2-02141979858b/paste-1792410241957.png)

![3V3](https://cdn.hackclub.com/01a153fa-17df-7539-b91c-25b3d9c6ef7f/paste-1792410261163.png)

![USB-C](https://cdn.hackclub.com/01a153fa-70eb-7b57-9c85-495cf29e8ea2/paste-1792410283695.png)

**Total time spent: 1 hour 38 minutes**

# September 29: Power supply, part 2 (5V + 9V logic, VBAT detection)

Today, 29th of September, I started with a cup of coffee <3 and continued with the power supply. My target today was to finish it.

Same as the previous journal, I went through a few projects, asked GPT for the most efficient workflow, verified on Reddit & Discord, finalised the netlist, and drew the 5V rated for 2A (that's 10W) and the 9V rated for 2A (that's 18W).

I also added a VBAT sense circuit that measures the battery voltage coming from the power source, i.e. the ESC.

![5V 2A](https://cdn.hackclub.com/01a1541a-b992-7a89-8c62-d86579edd205/paste-1792412399228.png)

![9V 2A](https://cdn.hackclub.com/01a1541b-2309-7560-b005-6fe94cb68036/paste-1792412426298.png)

![VBAT](https://cdn.hackclub.com/01a1541b-833a-7bc4-9516-be01df21d902/paste-1792412451420.png)

**Total time spent: 1 hour 39 minutes**

# September 30: IMU and Flash

After finishing the power supply, I started on the sensors. These are core components for PID and for debugging (via blackbox).

This part had to be done carefully and precisely. The IMU is a 6-axis gyro + accelerometer that tells the FC how the drone is moving, and the PID loop uses that to keep the drone stable, with less wobble or drunkard behaviour. The flash stores the blackbox logs (telemetry data) so I can debug flights later.

For the IMU I chose the ICM-42688-P because, from what I read, it's way cleaner (lower noise) and handles vibrations better than older IMUs like the MPU6500 or other ICM models. The MPU6000 is considered gold, as it proved its performance and is the most loved IMU in the FPV community, but since it's discontinued, ppl now prefer the ICM-42688-P.

For the flash I went with Winbond, since it's the common choice in FC projects, and you can choose the storage size depending on what you need.

Once I chose my components, I went through the datasheets and made a note of the supporting components each one needs to work reliably, gave those notes to ChatGPT to get a netlist, and drew them on the schematic sheet. Then I verified them against existing project schematics... looked fine, so I did the same for the flash.

![IMU](https://cdn.hackclub.com/01a15429-d49a-7d8d-ad57-1ef8a2b57677/paste-1792413388651.png)

![Flash](https://cdn.hackclub.com/01a1542a-1b40-78ae-96fe-ad2bfecc9e3f/paste-1792413406218.png)

**Total time spent: 1 hour 43 minutes**

# October 1: Barometer and ESP32-C3 for BLE tuning

I'm so tired after writing these journals manually bruuuuuhhhh...

Okay, so today I started by drawing the barometer. Most FCs use either the BMP280 or the BMP388 / 390. I chose the BMP388 as it's more accurate than the 280 and it's available. The BMP390 is better but is currently unavailable on LCSC, hence the BMP388.

Note: the BMP388 and BMP390 share the same footprint (double-check the pinout in the datasheets), so if you want to use a BMP390 you should be able to, without changing the hardware!

For the BMP388 and the ESP32-C3 below I followed a set of basic guidelines. These lists come from an AI-written design guide (made with Claude) that I used as a reference, and I checked the details against the datasheets and other projects.

**BMP388 guidelines:**

1. Decide SPI vs I2C. If you use I2C, remember it needs pull-up resistors (one on each wire) once on the bus, not per chip. If you use SPI, no pull-ups are needed.
2. If I2C: the address pin sets which address it uses (so it doesn't clash with other I2C chips).
3. Connect its interrupt pin.
4. Power it from the analog 3.3 V.

![BMP388](https://cdn.hackclub.com/01a15b02-4dc7-79c5-a08d-1c826fed083d/paste-1792528238637.png)

Next I completed the ESP32-C3, which gives me Bluetooth / Wi-Fi so I can configure my Betaflight values wirelessly. SpeedyBee boards do this with the SpeedyBee app and their own ESP firmware, so I still need to figure out the ESP32 firmware side for my board (the chip alone won't just work with their app).

**ESP32-C3 guidelines:**

1. Power it from the digital 3.3 V with a bulk capacitor right next to it (Wi-Fi bursts!).
2. Wire the EN pin (reset) with the RC the datasheet recommends.
3. Wire the boot strapping pins correctly (some pins decide the boot mode at startup; the datasheet lists them; don't use those pins for random stuff).
4. Choose how you'll flash it: UART pads (TX, RX, EN, BOOT) or its built-in USB pins. Add test pads either way.
5. Connect a UART link to the STM32 (cross TX↔RX).
6. Keep an area around the antenna clear: mark in the schematic notes "no copper, no ground, no parts near the antenna end." You enforce this on the PCB later.

![ESP32-C3](https://cdn.hackclub.com/01a15aff-e7e5-745f-8e49-e9553d20886a/paste-1792528082167.png)

**Total time spent: 2 hours 17 minutes**

# October 2: Verge of completing the schematic yay!

Today was sort of a motivational day, as I only had the connectors, status LEDs, and buzzer left..

I started with the buzzer. This is probably the most common schematic, as almost every open-source FC has the same one.. so voila

![Buzzer](https://cdn.hackclub.com/01a15b04-205a-7138-8c56-2231b555bd5f/paste-1792528359296.png)

Followed by the status LEDs. These help in debugging, like identifying whether the BECs are functioning properly. Self-explanatory

![Status LEDs](https://cdn.hackclub.com/01a15b05-43a6-7ac4-acbe-867b14c64b0c/paste-1792528433366.png)

Next were the JST connectors, which connect your peripherals like the GPS, RX receivers, ESC, CAM, VTX, CAN, etc. JST connectors keep your connections neat and configurable. You can either solder wires directly or use these connectors. Pin-header connectors are NOT RECOMMENDED, because vibrations can loosen the pins and might even lead to a reset during flight.

![JST](https://cdn.hackclub.com/01a15b09-ba64-7b18-ab98-56796bdc23c2/paste-1792528725833.png)

Next are for those RGB lovers... as seen on the SpeedyBee F7, there are four 3-pin RGB pads, one near each corner, for LED strips. These make your drone quite visible during night flights and give a different vibe!

![External LEDs](https://cdn.hackclub.com/01a15b0b-e1a7-7a2a-81ce-9d12e86b862f/paste-1792528866377.png)

I then organised all my schematics, keeping it aesthetic as well, followed by a quick sanity check of all the connections. Ran the ERC (Electrical Rules Check), and called it a day!!

**Total time spent: 1 hour 47 minutes**

# October 2: Started routing the PCB!

Once the schematics were ready and sanity checked, I headed over to the PCB editor in KiCad and imported the components from the schematic, and hellll broo, just realised I need to place and route 179 components....... boiii

![PCB editor with all components imported](https://cdn.hackclub.com/01a15b13-4b84-7d99-9be7-22e9d3341067/paste-1792529352061.png)

I started with the board outline and the constraints, and worked within the space. Once the outline was drawn, I started placing the components and routing them... thought it was gonna be quick but a big noooooooo.... the placement had to be extremely optimal to fit that many components on a tiny board... but it being a 6-layer PCB made it a bit easier.

So how does this 6-layer PCB actually work?

![6-layer stackup](https://cdn.hackclub.com/01a15b19-95c6-7cd2-ab13-a89abd9357da/paste-1792529764651.png)

I got help from Claude to explain why ground planes matter, then put it in my own words:

1. **Every signal needs a road back home.** A signal doesn't just go from A to B, the current has to come back too, and it wants to take the easiest path home. If there's a solid GND sheet right next to the wire, the return path is right there, tight and short. If there's no ground nearby, the current takes a long messy detour, which makes a big loop that spews noise and picks up noise. Short return path = quiet board.

2. **It's like a shield between neighbors.** Fast signals leak a bit of noise into whatever's near them (this is called crosstalk). A solid GND layer between your top signals and your inner signals blocks that gossip. So the USB lines on top aren't whispering into whatever's on the inner layers, and the noisy power converter stuff doesn't leak into the gyro's wires.

3. **The gyro stays calm.** The flight controller's whole personality is reading the gyro super accurately. Any noise sneaking into those tiny signals shows up as twitchy motors. Ground planes next to the sensor wires keep that junk out.

4. **Fast signals behave properly.** Things like USB need their wires to act in a predictable way. A wire sitting a fixed distance above a solid ground plane has predictable behavior, and without that plane it's basically guessing. That's why USB data lines love being right above ground.

5. **Less noise blasted out into the world.** Ground planes also stop your board from broadcasting noise (which matters since you've got Wi-Fi/Bluetooth on the ESP32 and video stuff nearby).

6. **Bonus: heat.** A big copper sheet also spreads out heat from hot parts like the converters.

The placement may not always be optimal or good enough for routing the board, so it requires constant revision, adjustments and tests.

This is the final placement order that led me to complete the routing part.

![Component placement](https://cdn.hackclub.com/01a15b1e-74ea-7960-8599-4978a1c06bcd/paste-1792530083696.png)

**Total time spent: 3 hours 44 minutes**

# October 3: Wiring the board

Placing and routing the board took an eternity: it took me 5 days to complete the routing with 0 DRC errors.

Unfortunately, I don't have screenshots of each day's work...

So on 3rd Oct I started routing but soon hit a dead end, and a lot of things went wrong.
Points to remember while routing an FC:

- Keep the 3V3 regulators as close as possible to the components that use them. For example, the MCU and IMU both use the 3V3 rail, so the regulator has to be placed as close to these ICs as possible. This prevents a noisy rail, gives the ICs the most stable current, and helps keep the gyro readings (and so the PID filtering) accurate.
- Keep the paths shortest, or at least short.
- Power rails shouldn't be routed directly under / over sensitive sensors, as the noise could make the whole board useless if it can't keep the drone balanced mid-air.
- Placement first, routing later. A good placement = half the routing already done.
- Decoupling caps right next to the pins, no excuses.
- Keep the GND solid, don't chop it up with random tracks.
- If the routing feels impossible, the placement is the problem. Scrap it and start fresh (worked for me!)

![PCB](https://cdn.hackclub.com/01a15b2f-f2f2-7cdd-bfd3-8fdcd774c930/paste-1792531230186.png)

**Total time spent: 3 hours 17 minutes**

# October 10: Session 6, wiring the board...

Well, these 5 days didn't really work out, the wiring part was impossible..... until I asked one of my friends for help, who recommended watching videos on organising routes and using 6 layers efficiently.. I sat and watched those videos.. (I haven't logged the hours I spent learning and watching YouTube guides and tutorials).

It did help quite a bit...
I started the 6th session from a clear, empty board... imported the parts, and started questioning and positioning components... As an example, the MCU is generally in the center but offset by a certain measurement, why tho? Well, the gyro should sit as close to the center of the board as possible to give accurate readings, but the MCU and the IMU can't both be dead center. Hence the MCU is offset a bit to the bottom-left and the IMU a bit to the top-right, followed by the barometer just below the IMU, this way keeping the main components around the center of the board...

I followed the same approach for other components like the BEC, and finally got my placement right. I started routing, probably my 7th or 8th time routing. And my goodddd, this was so much better.. I usually had like 50-60 connections still left, but this time it reduced to just 10-15...

I was done for the day! lowkey exhausteddd!!

**Total time spent: 3 hours 45 minutes**

# October 12: Session 8, wiring the board...

A quick recap of the placement tips I followed:

- **Gyro:** place it near the center of the board and away from noisy parts (switching converters, the ESP32 antenna area, big inductors). The guide I read noted the gyro ended up isolated "by accident" and that was actually good.
- **Switching converters + inductors:** put them near the battery input, away from the gyro and video stuff. Keep their loops small.
- **ESP32-C3 module:** antenna end hanging toward a board edge, with a copper-free zone around/under it. Nothing tall nearby.
- **STM32:** near the middle, with the crystal very close to its pins.
- **USB-C:** on one edge, with the D+/D− routes short and straight.
- **Decoupling caps:** each on the same side as its pin, as close as physically possible. NotARoomba's guide says: "AS CLOSE AS POSSIBLE… OR IT DEFEATS THE PURPOSE." Same.
- **Video/OSD section:** near the video pads, away from switchers.
- **Barometer:** away from heat sources (hot parts make wrong readings).
- **Motor/receiver pads:** on the edges so wires don't cross the whole board.

I decided to tackle each problem one by one..
I started my routing fresh using yesterday's placement of components... So my plan for today was to complete the power and critical signals.

![3V3 logic](https://cdn.hackclub.com/01a15b52-af42-7bfc-9706-55787b1bca52/paste-1792533505160.png)

I finished the 3V3 logic..

**Where people mess up:**

- Routing signals across gaps in the ground plane.
- Tracks that are too thin for the current.
- Putting a switching converter's loop on a long path.
- Forgetting that power must go through the decoupling cap before reaching the pin (guide rule: the cap sits in line, between the supply and the pin).

**Total time spent: 1 hour 15 minutes**

# October 14: Final session for routing

**What I routed today:**

- Barometer, flash, OSD SPI/I2C lines. Keep them short and consistent.
- ESP32 UART + its power + reset/boot lines. Don't run anything under the antenna.
- Video path: input/output lines with their coupling caps, short, away from switcher noise, ground nearby.
- Motor, receiver, buzzer, LED lines out to edge pads.
- Battery voltage sense line: away from noisy stuff.

**Tips:**

- Hide the silkscreen layer while routing (the guide mentions this too).
- Use the bottom layer to hop over things, but try not to break your ground layer with too many tracks.
- Keep analog and digital apart: gyro and baro supply tracks don't share with ESP32 supply tracks beyond the regulator.

**Where people mess up:** a rat's nest (blue "unconnected" lines in KiCad) you forgot to finish, long parallel tracks of two noisy signals, and 90° corners on fast signals (use 45°).

**Ground fill and DRC:**

- Fill the ground on all layers you want it (on my 6-layer board, the inner ground layers are solid ground). Press B in KiCad to fill zones.
- **Stitching vias:** drop ground vias around the board so all the ground pieces connect (the guide says if KiCad draws a ratline to a pour island, you need a stitching via there).
- **Thermal pads:** any chip with a pad under its belly (like the switching regulators or sensor packages) needs that pad connected to ground with multiple vias, if its datasheet says so.
- **Pad connection style:** "Solid" connects better; "Thermal reliefs" are easier to hand-solder. Since you'll likely hand-solder some parts, thermal reliefs are friendlier.
- Run DRC (Design Rules Check). Fix all real errors. Some errors from imported connector footprints (like holes) can be okay (the guide says the same), but each one needs a "why it's safe" reason.
- Re-run ERC/DRC until it's boringly clean.

**Where people mess up:** islands of disconnected ground, forgetting thermal pad vias, and ignoring DRC warnings because "it's probably fine."

![DRC and ground fill](https://cdn.hackclub.com/01a15b58-5d45-724f-b332-32b13b96f1d2/paste-1792533879623.png)

**Total time spent: 2 hours 17 minutes**

# October 15: Silkscreen and render

Today I adjusted a few routing sections, keeping it clean, probably the 9th revision. Once the board was ready, I added some design touches to it,

mainly the arrow to show the orientation of the FC when it's mounted:

![Orientation arrow](https://cdn.hackclub.com/01a15b5b-87c1-7359-a754-3d2fc4d8080f/paste-1792534085406.png)

followed by the revision of the board, i.e. v2.7, which corresponds to the 9th revision I made...... and a couple of other things..

![Front](https://cdn.hackclub.com/01a15b5c-f1ed-733b-87a6-8bc90820d662/paste-1792534178926.png)

![Back](https://cdn.hackclub.com/01a15b5d-6a9f-79de-9441-ee01fd4d025c/paste-1792534210812.png)

And broo, my FC is finallll complete, the satisfaction is unreal.....

**Total time spent: 45 minutes**

# October 16: Organising the files + BOM

Once everything was ready, I headed over to the project folder, organised the files and checked the BOM. At the beginning of the project I had spent around half an hour downloading and converting the component library from LCSC, so generating the BOM was just a matter of seconds!!

I installed the JLCPCB fabrication plugin on KiCad, as guided by @NotARoomba in SFC (Simple Flight Controller), generated the fabs, pushed everything to my Codespace, and wrote the README!

![Components](https://cdn.hackclub.com/01a15b62-60fd-7ef1-b585-33a12993ada7/paste-1792534534931.png)

![JLCPCB](https://cdn.hackclub.com/01a15b62-a79e-74e8-a135-9a1c3d0675b7/paste-1792534553988.png)

**Total cost: around $215 (65 + 150)**

**Total time spent: 33 minutes**

# October 17: Firmware

After going through the project requirements, I found that only the firmware was missing, so I headed over to Betaflight and checked out a few open-source firmware targets for other commercial / hobbyist FC boards...

and, **using AI (Codex and Claude)**, I generated my basic Betaflight firmware target, mapping the sensors to their respective GPIO pins.

![Firmware target](https://cdn.hackclub.com/01a15b66-9c01-73c3-b2f8-bfe253a681a1/paste-1792534812616.png)


<h2>final render:</h2>

![image](https://cdn.hackclub.com/01a15b71-41c2-7223-971a-99dd2b3f9b5f/paste-1792535509949.png)

![image](https://cdn.hackclub.com/01a15b71-b2fb-732a-a197-5929492d237b/paste-1792535539068.png)

**Total time spent: 51 minutes**