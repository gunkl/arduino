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

/* Displays temp and humidity by blinking morse code on the on-board LED.
 *  Uses DHT11 sensor.
 */
#include <dht.h>

dht DHT;

#define DHT11_PIN 7
int dht_power = 6;
int led = 13;
int charspace = 500;
int charshort = 50;
int charlong = 250;

void setup(){
  digitalWrite(dht_power, HIGH);
  Serial.begin(9600);
  pinMode(led, OUTPUT); 
  digitalWrite(led, HIGH); 
  delay(1000); 
  digitalWrite(led, LOW); 
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature*9/5+32);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);

  delay(5000);
  morseled("  HUM " + String(int(DHT.humidity)));
  morseled("    TMP " + String(int(DHT.temperature*9/5+32)));
}

void morseled(String morsetext)
{
  String result = morselookup(morsetext);
  for (int i=0; i < result.length(); i++)
  {
    if (result[i] == ' ')
    {
       delay(charspace);
       Serial.print(" ");
    }
    else if (result[i] == 'S')
    {
      Serial.print('.');
      digitalWrite(led, HIGH);
      delay(charshort);
      digitalWrite(led, LOW); 
      delay(charspace);
    }
    else if (result[i] == 'L')
    {
      Serial.print('-');
      digitalWrite(led, HIGH);
      delay(charlong);
      digitalWrite(led, LOW); 
      delay(charspace);    
    }
  }
  Serial.println("");
}

String morselookup(String morsetext)
{
  String result;
  for (int i=0; i < morsetext.length(); i++)
  {
    if (morsetext[i] == 'A') result += "SL ";
    else if (morsetext[i] == 'B') result += "LSS ";
    else if (morsetext[i] == 'C') result += "LSLS ";
    else if (morsetext[i] == 'D') result += "LSS ";
    else if (morsetext[i] == 'E') result += "S ";
    else if (morsetext[i] == 'F') result += "SSLS ";
    else if (morsetext[i] == 'G') result += "LLS ";
    else if (morsetext[i] == 'H') result += "SSSS ";
    else if (morsetext[i] == 'I') result += "SS ";
    else if (morsetext[i] == 'J') result += "SLLL ";
    else if (morsetext[i] == 'K') result += "LSL ";
    else if (morsetext[i] == 'L') result += "SLSS ";
    else if (morsetext[i] == 'M') result += "LL ";
    else if (morsetext[i] == 'N') result += "LS ";
    else if (morsetext[i] == 'O') result += "LLL ";
    else if (morsetext[i] == 'P') result += "SLLS ";
    else if (morsetext[i] == 'Q') result += "LLSL ";
    else if (morsetext[i] == 'R') result += "SLS ";
    else if (morsetext[i] == 'S') result += "SSS ";
    else if (morsetext[i] == 'T') result += "L ";
    else if (morsetext[i] == 'U') result += "SSL ";
    else if (morsetext[i] == 'V') result += "SSSL ";
    else if (morsetext[i] == 'W') result += "SLL ";
    else if (morsetext[i] == 'X') result += "LSSL ";
    else if (morsetext[i] == 'Y') result += "LSLL ";
    else if (morsetext[i] == 'Z') result += "LLSS ";
    else if (morsetext[i] == '0') result += "LLLLL ";
    else if (morsetext[i] == '1') result += "SLLLL ";
    else if (morsetext[i] == '2') result += "SSLLL ";
    else if (morsetext[i] == '3') result += "SSSLL ";
    else if (morsetext[i] == '4') result += "SSSSL ";
    else if (morsetext[i] == '5') result += "SSSSS ";
    else if (morsetext[i] == '6') result += "LSSSS ";
    else if (morsetext[i] == '7') result += "LLSSS ";
    else if (morsetext[i] == '8') result += "LLLSS ";
    else if (morsetext[i] == '9') result += "LLLLS ";
  }
  return result;
}

