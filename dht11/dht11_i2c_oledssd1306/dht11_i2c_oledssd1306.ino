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

/* Displays temp and humidity with a scaling line graph on the OLED SSD1306 display.
 *  Uses DHT11 sensor.
 */
#include <dht.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

dht DHT;

#define DHT11_PIN 7
int dht_power = 6; // Power DHT11 on pin 6, or separate power pin.
int led = 13; // on board LED pin blink at startup
int charspace = 500;
int charshort = 50;
int charlong = 250;
const int update_delay = 10000; // update every 10 seconds.

String progress = "*---------"; // you can also display a progress bar optionally

const int SENSE_ARRAY_SIZE = 128;
int senseTempVals[SENSE_ARRAY_SIZE];
int scaleTempMinDefault = 50;
int scaleTempMin = 50;
int scaleTempMaxDefault = 80;
int scaleTempMax = 80;
int scaleTempVar = 20; // add/sub 10 degrees (20 total) to min/max for scale.
int senseHumidVals[SENSE_ARRAY_SIZE];
int scaleHumidMinDefault = 25;
int scaleHumidMin = 25;
int scaleHumidMax = 60;
int scaleHumidMaxDefault = 60;
int scaleHumidVar = 20;
bool first = true;

void setup(){
  digitalWrite(dht_power, HIGH);
  Serial.begin(9600);
  pinMode(led, OUTPUT); 
  digitalWrite(led, HIGH); 
  delay(1000); 
  digitalWrite(led, LOW);  
  //
  // SSD OLED 1306 setup
  // My SSD uses 0x3C - I used the i2c_scanner script to find it's real address.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  //
  // init done
  
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
}

void loop()
{
  if (first){
    first = false;
    memset(senseTempVals,0,SENSE_ARRAY_SIZE);
    memset(senseHumidVals,0,SENSE_ARRAY_SIZE);
  }
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature*9/5+32);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  //
  rotatevals();
  senseTempVals[SENSE_ARRAY_SIZE-1] = int(DHT.temperature*9/5+32);
  senseHumidVals[SENSE_ARRAY_SIZE-1] = int(DHT.humidity);
  //
  printtext((String(int(DHT.temperature*9/5+32)) + (char)247 + "F"), "", String(int(DHT.humidity)) + "% Humid", "");
  rescale();
  delay(update_delay);
  // progress = rotatestring(progress);
}

// void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void drawgraph(uint16_t startx, uint16_t starty, uint16_t heighty, uint16_t scalemin, uint16_t scalemax, int dispVals[SENSE_ARRAY_SIZE]){
  // take diff between scalemax and scalemin and divide by total pixels available per unit in heighty
  int perPixel = int((scalemax-scalemin)/heighty);
  int yHeight = 0;
  for (uint16_t i=0; i < SENSE_ARRAY_SIZE; i++){
    // Serial.println(String(i) + "Sense: " + String(dispVals[i]));
    if (int(dispVals[i] - scalemin) <= 0)
    {
      yHeight = 0;
    }
    else if (dispVals[i] > scalemax)
    {
      yHeight = heighty;
    }
    else
    {
      yHeight = int((dispVals[i] - scalemin) / perPixel);
      if (yHeight > heighty){
        yHeight = heighty;
      }
    }
    // Serial.println("Height " + String(yHeight) + " StartY: " + String(starty) + " EndY: " + String (starty - yHeight));
    // drawing a line as below produces a "bar" ugly chart.
    //display.drawLine(i, starty, i, starty - yHeight,WHITE);
    display.drawPixel(i, starty-yHeight, WHITE);
  }
}

String rotatestring(String inputstring){
  char lastchar = inputstring[0];
  for (uint8_t i=0; i < inputstring.length()-1; i++){
    inputstring[i] = inputstring[i+1];
  }
  inputstring[inputstring.length()-1] = lastchar;
  return inputstring;
}


void rotatevals(){
  for (uint8_t i=0; i < SENSE_ARRAY_SIZE-1; i++){
    senseTempVals[i] = senseTempVals[i+1];
    senseHumidVals[i] = senseHumidVals[i+1];
  }
}

void printtext(String line1, String line2, String line3, String line4) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print(line1);
  display.setCursor(0,16);
  display.print(line2);
  display.setCursor(0,32);
  display.print(line3);
  display.setCursor(0,48);
  display.print(line4);
  drawgraph(0, 30, 15, scaleTempMin, scaleTempMax, senseTempVals);
  drawgraph(0, 62, 15, scaleHumidMin, scaleHumidMax, senseHumidVals);
  display.display();
}

void rescale(){
  int maxfoundT = scaleTempMaxDefault;
  int minfoundT = scaleTempMinDefault;
  int maxfoundH = scaleHumidMaxDefault;
  int minfoundH = scaleHumidMinDefault;
  for (uint16_t i=0; i < SENSE_ARRAY_SIZE; i++){
    if (!(int(senseTempVals[i]) == 0) && !(int(senseHumidVals[i]) == 0)){
       // Serial.println(String(i) + String(minfoundH) + " " + String(minfoundT));
      if (int(senseTempVals[i]) > int(maxfoundT)){
        maxfoundT = senseTempVals[i];
      }
      if (int(senseTempVals[i]) <= int(minfoundT)){
        minfoundT = senseTempVals[i];
      }
      if (int(senseHumidVals[i]) > int(maxfoundH)){
        maxfoundH = senseHumidVals[i];
      }
      if (int(senseHumidVals[i]) <= int(minfoundH)){
        minfoundH = senseHumidVals[i];
      }
    }
  }
  // Serial.println(String(maxfoundT) + " " + String(minfoundT));
  //Serial.println(String(maxfoundH) + " " + String(minfoundH));
  //
  // humidity minmax
  scaleHumidMin = int(minfoundH - scaleHumidVar/4);
  scaleHumidMax = int(maxfoundH + scaleHumidVar/4);
  // temp minmax
  scaleTempMin = int(minfoundT - scaleTempVar/4);
  scaleTempMax = int(maxfoundT + scaleTempVar/4);
  //
  if (scaleTempMin < 0){
    scaleTempMin = 0;
  }
  if (scaleHumidMin < 0){
    scaleHumidMin = 0;
  }
  Serial.println("HMi:" + String(scaleHumidMin) + " HMx:" + String(scaleHumidMax) + "  TMi:" + String(scaleTempMin) + " TMx:" + String(scaleTempMax));
}

