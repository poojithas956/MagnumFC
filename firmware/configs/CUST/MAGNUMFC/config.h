/*
 * MagnumFC Betaflight target draft
 *
 * This configuration is an unvalidated starting point for Betaflight 4.5+.
 * It intentionally does not define motor outputs or onboard flash because
 * the board design has unresolved issues documented in firmware/README.md.
 * Do not use this as flight-ready firmware.
 */
#pragma once

#define FC_TARGET_MCU STM32F722
#define BOARD_NAME MAGNUMFC
#define MANUFACTURER_ID CUST
#define SYSTEM_HSE_MHZ 27

#define USE_GYRO
#define USE_ACC
#define USE_GYRO_SPI_ICM42688P
#define USE_ACC_SPI_ICM42688P
#define SPI3_SCK_PIN PB3
#define SPI3_SDI_PIN PB4
#define SPI3_SDO_PIN PB5
#define GYRO_1_SPI_INSTANCE SPI3
#define GYRO_1_CS_PIN PB9
#define GYRO_1_EXTI_PIN PB8
#define GYRO_1_ALIGN CW0_DEG /* Must be confirmed by checking board orientation. */

#define USE_BARO
#define USE_BARO_BMP388
#define I2C1_SCL_PIN PB6
#define I2C1_SDA_PIN PB7
#define BARO_I2C_INSTANCE I2CDEV_1

#define USE_MAX7456
#define SPI2_SCK_PIN PB13
#define SPI2_SDI_PIN PB14
#define SPI2_SDO_PIN PB15
#define MAX7456_SPI_INSTANCE SPI2
#define MAX7456_SPI_CS_PIN PB2

/* UART numbers below follow the STM32 alternate-function pin pairs, not the
 * net labels in the schematic. The exposed connector assignments need review. */
#define UART1_TX_PIN PA9
#define UART1_RX_PIN PA10
#define UART2_TX_PIN PA2
#define UART2_RX_PIN PA3
#define UART4_TX_PIN PA0
#define UART4_RX_PIN PA1
#define UART5_TX_PIN PC12
#define UART5_RX_PIN PD2
#define UART6_TX_PIN PC6
#define UART6_RX_PIN PC7

#define ADC_VBAT_PIN PC0
#define ADC_CURR_PIN PA4
