#include <Wire.h>
extern TwoWire Wire1;
#include <OneWire.h>
#include "config.cpp"



byte MAC_ID[LENGTH_FORMAT3 + 2] = {ID_MAC, 134,0,0,0,0,0,0}; // MAC address
OneWire ds2401(PIN_DS2401);  //DS2401 PIN
byte Temp_byte[8];
unsigned char Temp_uchar[4] = {'F','A','I','L'};



void initializecoresense(void)
{
    byte i;

    pinMode(PIN_SPV_AMP,INPUT);
    pinMode(PIN_SVP_SPL,INPUT);
    pinMode(PIN_RAW_MIC,INPUT);
    pinMode(PIN_HIH4030,INPUT);
    pinMode(PIN_CHEMSENSE_POW, OUTPUT);
    pinMode(PIN_CHEMSENSE_HBT, INPUT);
    pinMode(PIN_CHEMSENSE_RST, OUTPUT);

    if (ds2401.reset() == TRUE)
    {
        ds2401.write(0x33);
        for (i = 0; i < 8; i++)
        {
            Temp_byte[i] = ds2401.read();
        }

        if (OneWire::crc8(Temp_byte, 8) == 0)
        {
            for (i=1; i<7; i++)
            {
                MAC_ID[i + 1] = Temp_byte[i];
            }

        }

        else
        {

            MAC_ID[3] = 0xff;

        }
    }
    else //Nothing is connected in the bus
    {
        MAC_ID[3] = 0xaa;
    }
}


void setup()
{
    delay(2000);
    Wire.begin();
    SerialUSB.begin(115200);
    Serial3.begin(19200);
    delay(2000);
    initializecoresense();
}


void loop()
{
    SerialUSB.println(">>>>>> Core Sense Testing - Airsense-Chemsense Connectivity Test <<<<<<");
    SerialUSB.println(" ");
    SerialUSB.print("Unique Board ID - ");
    for (byte i=2; i<8; i++)
    {
        SerialUSB.print(MAC_ID[i],HEX);

        if (i < 7)
        {
            SerialUSB.print(":");
        }
        else
        {
            SerialUSB.print("\n");
        }
    }
    SerialUSB.println(" ");
    delay(2000);

    SerialUSB.print("1. Airsense-Chemsense Serial Connectivity Test : ");
    Serial3.write("P");
    delay(100);
    Temp_uchar[0] = Serial3.read();
    Serial3.write("A");
    delay(100);
    Temp_uchar[1] = Serial3.read();
    Serial3.write("S");
    delay(100);
    Temp_uchar[2] = Serial3.read();
    Serial3.write("S");
    delay(100);
    Temp_uchar[3] = Serial3.read();
    for (byte i=0; i<4; i++)
    {
        SerialUSB.write(Temp_uchar[i]);
    }
    SerialUSB.println(" ");
    delay(1000);
    SerialUSB.print("2. Airsense-Chemsense Heartbeat and Reset Test : ");

    digitalWrite(PIN_CHEMSENSE_RST, HIGH);
    delay(100);
    if (digitalRead(PIN_CHEMSENSE_HBT) == 1)
    {
        delay(100);
        digitalWrite(PIN_CHEMSENSE_RST, LOW);
        delay(100);
        if (digitalRead(PIN_CHEMSENSE_HBT) == 0)
        {
            SerialUSB.println("PASS");
            SerialUSB.println(" ");
            SerialUSB.println(">>>>>> Test Finished <<<<<<");
            while (1)
            {
                delay(100);
            }
        }
    }

    SerialUSB.println("FAIL");
    SerialUSB.println(" ");
    SerialUSB.println(">>>>>> Test Finished <<<<<<");
    while (1)
    {
        delay(100);
    }
}





