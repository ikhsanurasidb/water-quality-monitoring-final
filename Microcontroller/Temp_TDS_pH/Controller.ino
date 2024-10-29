#include <EEPROM.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/wdt.h>

#define TdsSensorPin A1
#define TempSensorPin 7
#define PHPin A0

GravityTDS gravityTds;
OneWire oneWire(TempSensorPin);
DallasTemperature tempSensors(&oneWire);

float temperature = 25,tdsValue = 0;

int buf[10];
float ph(float voltage) {
  return 7 + ((2.5 - voltage) / 0.18);
}

void setup() {
  Serial.begin(9600);

  wdt_disable(); 
  delay(3000);  
  wdt_enable(WDTO_4S); 

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0); 
  gravityTds.setAdcRange(1024);  
  gravityTds.begin();  

  pinMode(PHPin, INPUT);

  tempSensors.begin();
}

void loop() {
  //
  wdt_reset();
  tempSensors.requestTemperatures();
  temperature = tempSensors.getTempCByIndex(0); 
  gravityTds.setTemperature(temperature); 
  gravityTds.update();  
  tdsValue = gravityTds.getTdsValue(); 

  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(PHPin);
    delay(100);
  }

  float avgValue = 0;
  for (int i = 0; i < 10; i++)
    avgValue += buf[i];

  float pHVol = (float)avgValue * 5.0 / 1023 / 10;
  float pHValue = -7.500 * pHVol + 29.88; // Proto-2
  // float pHValue = -8.143 * pHVol + 32.92; // Proto-1

  Serial.print("suhu:");
  Serial.println(temperature);
  Serial.print("tds:");
  Serial.println(tdsValue);
  Serial.print("ph:");
  Serial.println(pHValue);

  delay(1000);
}
