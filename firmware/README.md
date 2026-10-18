# MagnumFC Betaflight target draft

This folder contains an **early Betaflight target configuration draft**, not a tested firmware binary. It is intended for Betaflight 4.5 or newer, which uses per-board `config.h` definitions. The target is scoped under the reserved `CUST` manufacturer ID for custom boards.

## What the draft maps

- MCU: STM32F722RETx; the BOM lists a 27 MHz crystal on the MCU HSE pins.
- Gyro/accelerometer: ICM-42688-P on SPI3 (PB3/PB4/PB5), chip select PB9, interrupt PB8.
- Barometer: BMP388 on I2C1 (PB6/PB7).
- Analog OSD: AT7456E-compatible MAX7456 driver on SPI2, chip select PB2.
- ADC inputs: VBAT on PC0 and current sense on PA4.
- UARTs: assigned by the MCU alternate-function pin pairs. Betaflight UART numbering may differ from net names printed in the schematic.

The IMU alignment is provisionally set to `CW0_DEG`; verify it from the assembled board’s orientation before using the gyro. UART assignments also need to be checked against the connector labels and intended external devices.

## Unresolved blockers

1. **Motor outputs:** M1–M4 are on PA6, PA7, PB0, and PB1. These correspond to TIM3 channels, but span two GPIO ports. Betaflight’s current manufacturer guidance says new designs should keep M1–M4 on one GPIO port for DShot bitbang compatibility and cloud-build acceptance. The draft deliberately omits `MOTOR*_PIN` and timer/DMA mappings, so it is not a flyable target.
2. **UART connector:** the schematic maps its `UART4_RX` net to MCU PC10 and `UART4_TX` to PC11. On STM32F722, PC10/PC11 provide USART3 TX/RX respectively, so the net directions appear reversed. Verify and correct the schematic/PCB before adding that port to firmware.
3. **Flash device:** the BOM says W25Q256JVEIQ while the schematic and PCB say W25N04KVZEIR. Betaflight documents support for the W25N NAND family, but this exact W25N04 part is not listed in the target guidance. Resolve the BOM and check the driver’s supported JEDEC IDs and geometry before enabling blackbox flash.
4. **Board validation:** no assembled MagnumFC or bench-test results are present. Sensor detection, IMU orientation, barometer, OSD, UARTs, voltage/current scaling, USB behavior, and motor timing all need bench validation before flight use.

## Next steps

1. Resolve the U7 part-number mismatch and determine whether the chosen flash is supported by the intended Betaflight revision.
2. Correct or confirm the UART4 pin directions, then review every connector’s MCU pin assignment against the STM32F722 datasheet.
3. Decide whether to revise the motor routing to put all four motor outputs on one GPIO port. If the PCB stays as-is, investigate the exact timer/DMA behavior and accept that this target may not meet Betaflight cloud-build guidance.
4. With hardware available, flash a minimal bring-up build through a debugger, verify power rails and sensor communication, determine gyro alignment, then add motor timer/DMA and flash support.
5. Build the target from the matching Betaflight source/config revision, review the generated resource map, and validate on a restrained bench setup before any propellers are fitted.

## References

- [Betaflight target configuration guide](https://betaflight.com/docs/development/manufacturer/creating-configuration)
- [Betaflight custom board guidance](https://betaflight.com/docs/development/Custom-Board-Configuration)
- [Betaflight manufacturer design guidelines](https://betaflight.com/docs/development/manufacturer/manufacturer-design-guidelines)
- [STM32F722RE datasheet](https://www.st.com/resource/en/datasheet/stm32f722re.pdf)

No firmware build or hardware validation has been performed for this draft.
