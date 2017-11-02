// variables for all ino files...
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <OneWire.h>

#include "./MCP342X/MCP342X.h"
MCP342X mcp3428_1;
MCP342X mcp3428_2;

OneWire ds2401(48);

/* bus ADDRESSES of sensors, PIN configuration for analog/sensor power */
#define HBT_PIN 8  // HBEAT PIN (ON/OFF)
// Metsense
#define TMP112_ADDRESS 0x48
#define HTDU21D_ADDRESS 0x40
#define HIH4030_PIN A10
#define BMP180_ADDRESS 0x77
#define PRJ103J2_PIN 0
#define TSL250RDms_PIN 1
#define MMA8452_ADDRESS 0x1C
// below three lines are for sound level sensor
#define SPV_AMP_PIN A5
#define SPV_SPL_PIN A6
#define SPV_RAW_MIC_PIN A9
#define TSYS01_ADDRESS 0x76

// Lightsense
#define HMC5883_ADDESS 0x1E //byte HMC5883_ADDRESS_MAG = 0x3C >> 1;  // 0011110x
#define HIH6130_ADDRESS 0x27
// below two lines are addresses for sensors using MCP3428 chip
byte MCP3428_1_ADDRESS = 0;
byte MCP3428_2_ADDRESS = 0;
#define TMP421_ADDRESS 0x4C

// Chemsense
#define CHEM_POWER_PIN 47

// Alphasensor
#define ALPHA_SLAVE_PIN 40

/* COEFFICIENTS */
byte BMP180_COEFFICIENTS[22];
byte TSYS01_COEFFICIENTS[10];
byte HMC5883_COEFFICIENTS[4];

/* store configuration info of Chemsense */
byte chemConfigReading[2048];

/* SPI setting for alpha sensor */
SPISettings setAlpha; //(SPI_MAX_speed, MSBFIRST, SPI_MODE1);