/*
  Copyright (c) 2017 +++ David Smith aka Gunkl +++
  This program is free software; you can redistribute it and/or modify it under the terms of the 
  GNU General Public License as published by the Free Software Foundation; either version 2 of 
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program; 
  if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
  Boston, MA 02111-1307 USA

*/
#include <dht.h>
#include "U8glib.h"

dht DHT;
#define DHT11_PIN 7

U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8

String progress = "*---------";

void draw(String line1, String line2, String line3, String line4)
{
  // u8g.setFont(u8g_font_unifont);
  u8g.setFont(u8g_font_7x14);
  u8g.setPrintPos(0,12);
  u8g.print(line1);
  u8g.setPrintPos(0,24);
  u8g.print(line2);
  u8g.setPrintPos(0,36);
  u8g.print(line3);
  u8g.setPrintPos(0,48);
  u8g.print(line4);
}

void setup()
{
  u8g.setColorIndex(1); // pixel on
  Serial.begin(9600);
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature*9/5+32);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  String line1 = "Temp: " + String(int(DHT.temperature*9/5+32)) + char(176) + "F";
  // String line2 = "T2: " + String(DHT.temperature*9/5+32);
  String line2 = "";
  String line3 = "Hdty: " + String(int(DHT.humidity)) + "%";
  // String line4 = "T4: " + String(DHT.temperature*9/5+32);
  String line4 = progress;
  u8g.firstPage();
  // Serial.println("Start");
  do
  {
    draw(line1, line2, line3, line4);
    // Serial.println("drawn");

  }
  while (u8g.nextPage());
  // Serial.println("done");

  delay(5000);
  progress = rotatestring(progress);
}

String rotatestring(String inputstring){
  char lastchar = inputstring[0];
  for (uint8_t i=0; i < inputstring.length()-1; i++){
    inputstring[i] = inputstring[i+1];
  }
  inputstring[inputstring.length()-1] = lastchar;
  return inputstring;
}
