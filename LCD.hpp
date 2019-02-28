/*
To get this code to work, attached an OpenLCD to an Arduino Uno using the following pins:
 SCL (OpenLCD) to A5 (Arduino)
 SDA to A4
 VIN to 5V
 GND to GND
 Command cheat sheet:
 ASCII / DEC / HEX
 '|'    / 124 / 0x7C - Put into setting mode
 Ctrl+c / 3 / 0x03 - Change width to 20
 Ctrl+d / 4 / 0x04 - Change width to 16
 Ctrl+e / 5 / 0x05 - Change lines to 4
 Ctrl+f / 6 / 0x06 - Change lines to 2
 Ctrl+g / 7 / 0x07 - Change lines to 1
 Ctrl+h / 8 / 0x08 - Software reset of the system
 Ctrl+i / 9 / 0x09 - Enable/disable splash screen
 Ctrl+j / 10 / 0x0A - Save currently displayed text as splash
 Ctrl+k / 11 / 0x0B - Change baud to 2400bps
 Ctrl+l / 12 / 0x0C - Change baud to 4800bps
 Ctrl+m / 13 / 0x0D - Change baud to 9600bps
 Ctrl+n / 14 / 0x0E - Change baud to 14400bps
 Ctrl+o / 15 / 0x0F - Change baud to 19200bps
 Ctrl+p / 16 / 0x10 - Change baud to 38400bps
 Ctrl+q / 17 / 0x11 - Change baud to 57600bps
 Ctrl+r / 18 / 0x12 - Change baud to 115200bps
 Ctrl+s / 19 / 0x13 - Change baud to 230400bps
 Ctrl+t / 20 / 0x14 - Change baud to 460800bps
 Ctrl+u / 21 / 0x15 - Change baud to 921600bps
 Ctrl+v / 22 / 0x16 - Change baud to 1000000bps
 Ctrl+w / 23 / 0x17 - Change baud to 1200bps
 Ctrl+x / 24 / 0x18 - Change the contrast. Follow Ctrl+x with number 0 to 255. 120 is default.
 Ctrl+y / 25 / 0x19 - Change the TWI address. Follow Ctrl+x with number 0 to 255. 114 (0x72) is default.
 Ctrl+z / 26 / 0x1A - Enable/disable ignore RX pin on startup (ignore emergency reset)
 '-'    / 45 / 0x2D - Clear display. Move cursor to home position.
        / 128-157 / 0x80-0x9D - Set the primary backlight brightness. 128 = Off, 157 = 100%.
        / 158-187 / 0x9E-0xBB - Set the green backlight brightness. 158 = Off, 187 = 100%.
        / 188-217 / 0xBC-0xD9 - Set the blue backlight brightness. 188 = Off, 217 = 100%.
 For example, to change the baud rate to 115200 send 124 followed by 18.
*/

#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "Displayer.hpp"
#include "CentralObject.hpp"
#include "SharedEnums.hpp"

#include <Wire.h>

#define DISPLAY_ADDRESS1 0x72 //This is the default address of the OpenLCD



class LCD :public Displayer {
  protected:
    float cad;
    float tgtCad;
    uint8_t gr;
    shiftMode md;

    unsigned long delayTime = 0;
  
  public:

    bool updateCadence(double _cadence)
    {
      cad = _cadence;
    }    
    bool updateGear(uint8_t _gear)
    {
      gr = _gear;
    }
    bool updateTargetCadence(double _targetCadence)
    {
      tgtCad = _targetCadence;
    }
    bool updateMode(shiftMode _mode)
    {
      
      md = _mode;
    }

    bool delayLCD(unsigned long ms)
    {
      delayTime = ms;
    }

    bool setCursor(unsigned int x, unsigned int y)
    {
        int lineOffset = 0;
        if (y == 1)
        {
          lineOffset = 64;
        }
        
        Wire.beginTransmission(DISPLAY_ADDRESS1);
        Wire.write(254); //Put LCD into setting mode
        Wire.write(128 + lineOffset + x);
        Wire.endTransmission();
    }

    bool showDesiredCadenceSetMessage()
    {
        Wire.beginTransmission(DISPLAY_ADDRESS1);
        Wire.write('|'); //Put LCD into setting mode
        Wire.write('-'); //Send clear display command
        Wire.endTransmission();

        delay(10);

        Wire.beginTransmission(DISPLAY_ADDRESS1);
        Wire.print("Desired cadence set at: ");
        Wire.print((int)tgtCad);
        Wire.endTransmission();
        delayLCD(2000);
    }

    bool showDesiredCadenceInputMessage()
    {
        Wire.beginTransmission(DISPLAY_ADDRESS1);
        Wire.write('|'); //Put LCD into setting mode
        Wire.write('-'); //Send clear display command
        Wire.endTransmission();

        delay(10);

        Wire.beginTransmission(DISPLAY_ADDRESS1);
        Wire.print("Set desired cadence: ");
        Wire.print((int)tgtCad);
        Wire.endTransmission();
        delayLCD(500);
    }
    
    bool classMain()
    {
      while(1){
        delay(delayTime);
        delayTime = 0;
        updateDisplay();
        delay(500);
      }
    }    

    bool updateDisplay()
    {
      
      if (md == automatic) {
        Serial.println("automatic");
      } else if (md == manual) {
        Serial.println("maunal");
      }
/*
        Wire.beginTransmission(DISPLAY_ADDRESS1);
        Wire.write('|'); //Put LCD into setting mode
        Wire.write('-'); //Send clear display command
        Wire.endTransmission();

        delay(250);
        
        Wire.beginTransmission(DISPLAY_ADDRESS1);
        Wire.print("Set desired     ");
        Wire.print("cadence: ");
        Wire.print(tgtCad);
        Wire.endTransmission();
  */    
      if (md == automatic)
      {
        setCursor(0,0);//top line position 0
        
        delay(10); //The maximum update rate of OpenLCD is about 100Hz (10ms). A smaller delay will cause flicker
        
        Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
        Wire.print("Gr. Cad. TarCad.");
        Wire.endTransmission();

        Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
        Wire.print(" ");
        Wire.print(gr);
        Wire.print("   ");
        Wire.print((int)cad);
        Wire.print("    ");
        Wire.print((int)tgtCad);
        Wire.print("  ");
        Wire.endTransmission(); //Stop I2C transmission     
      }
      else if (md == manual)
      {
        //Serial.print("Gear in LCD Class: ");
        //Serial.println(gr);
        setCursor(0,0);//top line position 0
        
        delay(10); //The maximum update rate of OpenLCD is about 100Hz (10ms). A smaller delay will cause flicker
        
        Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
        Wire.print("Gr. Cad. TarCad.");
        Wire.endTransmission();

        setCursor(0,1); //bottom line, position 0

        Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
        Wire.print(" ");
        Wire.print(gr);
        Wire.print("   ");
        Wire.print((int)cad);
        Wire.print("    ");
        Wire.print("XX");
        Wire.print("  ");
        Wire.endTransmission(); //Stop I2C transmission     
      }
    }

    bool setupDisplayer()
    {
      Serial.println("SetupLCD");
      Wire.begin(); //Join the bus as master

      //By default .begin() will set I2C SCL to Standard Speed mode of 100kHz
      //Wire.setClock(400000); //Optional - set I2C SCL to High Speed Mode of 400kHz
    
      //Send the reset command to the display - this forces the cursor to return to the beginning of the display
      Wire.beginTransmission(DISPLAY_ADDRESS1);
      Wire.write('|'); //Put LCD into setting mode
      Wire.write('-'); //Send clear display command
      Wire.endTransmission();
    
      //Send contrast command
      Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
      Wire.write('|'); //Put LCD into setting mode
      Wire.write('-'); //Send clear display command
      Wire.write(24); //Send contrast command
      Wire.write(byte(0));
      Wire.endTransmission(); //Stop I2C transmission
    
      //Send backlight commands
      Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
      Wire.write('|'); //Put LCD into setting mode
      Wire.write('-'); //Send clear display command
      Wire.write(128); //Red: 128 = Off, 157 = 100%
      Wire.write('|');
      Wire.write('-'); //Send clear display command
      Wire.write(165); //Green: 158 = Off, 187 = 100%
      Wire.write('|');
      Wire.write('-'); //Send clear display command
      Wire.write(188); //Blue: 188 = Off, 217 = 100%
      Wire.endTransmission(); //Stop I2C transmission
    
      //Send line command - set to 2 lines
      Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
      Wire.write('|'); //Put LCD into setting mode
      Wire.write('-'); //Send clear display command
      Wire.write(6); //Set LCD to use both lines
      Wire.endTransmission(); //Stop I2C transmission
    }
};
