---
title: "MagnumFC"
author: "Poojitha S"
description: "A compact 30 × 30 mm STM32F722 flight-controller design in KiCad."
created_at: "2026-09-28"
---

![Rendered front of the MagnumFC flight-controller PCB](PCB/MagnumFC-front.png)

# MagnumFC

MagnumFC is my 30 × 30 mm flight-controller design, built in KiCad around an STM32F722RETx. I wanted to fit a useful set of FPV-controller features onto a small board and give the gyro and its power rails deliberate attention. The design includes an ICM-42688-P IMU, BMP388 barometer, ESP32-C3 coprocessor, analog OSD circuitry, USB-C, and separate 5 V and 9 V rails.

This repository contains the KiCad source, board renders, schematic PDF, and manufacturing exports. It also contains an early Betaflight target draft. **The PCB has not been manufactured, assembled, or bench-tested.** Features described below are design intent, not proof that the finished board works. The firmware draft is not flight-ready.

## Current status

- [x] KiCad schematic and PCB layout files are included.
- [x] Front and back PCB renders and a schematic PDF are included.
- [x] BOM, placement, designator, and IPC netlist exports are included.
- [x] An early Betaflight target draft and notes on unresolved issues are included.
- [ ] Review and resolve the flash part-number mismatch in the BOM.
- [ ] Verify schematic, footprints, pin assignments, and manufacturing outputs before ordering.
- [ ] Manufacture and assemble the board, then validate power, peripherals, and firmware on the bench.
- [ ] Add verified assembly and flashing instructions, test evidence, and a working-board demo.

## Design overview

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

The design also includes receiver, GPS, DJI/video, motor, buzzer, LED, and expansion connections. Connector labels and signal assignments should be checked in the schematic before wiring. The target configuration still needs pin-map review against the final board and MCU datasheet.

## Board renders

These images are KiCad renders of the design, not photos of a manufactured board.

![Front render of the MagnumFC PCB](PCB/MagnumFC-front.png)

![Back render of the MagnumFC PCB](PCB/MagnumFC-back.png)

More circuit and layout screenshots are in [`Screenshots/`](Screenshots/). The schematic is also available as [`docs/sch.pdf`](docs/sch.pdf).

## Repository map

| Path | Contents |
| --- | --- |
| `PCB/MagnumFC.kicad_pro` | KiCad project file |
| `PCB/MagnumFC.kicad_sch` | Top-level schematic |
| `PCB/pwr.kicad_sch`, `PCB/sensor.kicad_sch`, `PCB/interface.kicad_sch` | Schematic sheets |
| `PCB/MagnumFC.kicad_pcb` | PCB layout |
| `PCB/Library/` | Project symbol, footprint, and 3D-model libraries |
| `production/` | BOM, positions, designators, IPC netlist, and fabrication archive |
| `docs/sch.pdf` | Schematic PDF |
| `Screenshots/` | Schematic and PCB design screenshots |
| `firmware/` | Early Betaflight target draft and validation notes |
| `JOURNAL.md` | Project work journal |

## Open the KiCad design

1. Install a KiCad version that supports the project files (the project was prepared with KiCad 10).
2. Open [`PCB/MagnumFC.kicad_pro`](PCB/MagnumFC.kicad_pro) in KiCad.
3. Keep `PCB/Library/` alongside the project so its custom symbols, footprints, and models remain available.
4. Open the schematic or PCB from the project manager. For a quick schematic read, use [`docs/sch.pdf`](docs/sch.pdf).
5. Before fabrication, compare the BOM and placement exports with the current schematic and PCB. These exports are provided for review; they are not a substitute for a manufacturing check.

## BOM and manufacturing status

The exported component list is [`production/bom.csv`](production/bom.csv). It includes designators, footprints, quantities, values, and some distributor identifiers, but it does not include complete supplier links or current unit prices. It is not yet a costed purchasing list.

There is a known part-number mismatch at U7: the BOM lists `W25Q256JVEIQ`, while the schematic and PCB identify `W25N04KVZEIR`. This must be resolved before buying parts or treating the BOM as final. Check the footprint, pinout, and intended Betaflight driver support for the selected device as part of that review.

The `production/` directory also contains `positions.csv`, `designators.csv`, `netlist.ipc`, and `MagnumFC_v2.7.zip`. Review the current KiCad project and rerun the required fabrication checks before sending any exports to a board house; the presence of these files does not mean the design has passed manufacturing review.

## Firmware and validation

The files under [`firmware/`](firmware/) are an early Betaflight target configuration draft, not a compiled firmware image. The draft intentionally leaves motor-output and onboard-flash configuration unresolved. The accompanying [`firmware/README.md`](firmware/README.md) records additional concerns, including the U7 mismatch, motor pin arrangement, UART net direction, and unverified peripheral assignments.

Before flight use, the design needs a hardware bring-up and firmware validation: verify power rails and current limits, inspect the board for shorts, confirm USB and MCU programming, check sensor detection and orientation, validate OSD and UARTs, confirm voltage/current readings, and resolve motor timers and flash support. Do not connect propellers or attempt flight based on this draft.

## Submission checklist

- [x] Project purpose and design intent are described.
- [x] Schematic, PCB source, BOM, and manufacturing-export locations are listed.
- [x] Front and back 3D board renders are included. This project does not currently include an enclosure or a larger mechanical assembly.
- [x] A work journal is included at [`JOURNAL.md`](JOURNAL.md).
- [ ] Resolve the BOM part-number mismatch and add verified supplier links and current prices.
- [ ] Complete schematic, PCB, and fabrication review before ordering.
- [ ] Document the actual assembly, flashing, and bench-test procedure after validating it on hardware.
- [ ] Add photos or a demo of a manufactured and working board.
- [ ] Add a public repository/project link when one is available.
