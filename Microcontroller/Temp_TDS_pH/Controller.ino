#include <EEPROM.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"
#include <Servo.h>
#include <avr/wdt.h>

#define TdsSensorPin A1
#define TempSensorPin 7
#define PHPin A0

GravityTDS gravityTds;
OneWire oneWire(TempSensorPin);
DallasTemperature tempSensors(&oneWire);
RTC_DS1307 rtc;
Servo servoY, servoX;

float temperature = 0,tdsValue = 0;
int lastTime = -1;
const int interval = 3;

float buf[10];

void setup() {
  delay(5000);

  Serial.begin(9600);

  wdt_disable();  
  delay(3000);  
  wdt_enable(WDTO_8S); 

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0); 
  gravityTds.setAdcRange(1024);  
  gravityTds.begin();  

  pinMode(PHPin, INPUT);

  tempSensors.begin();

  Serial.println("Starting setup...");

  startRtc();

  servoX.attach(10);
  servoY.attach(11);

  servoX.write(175); 
  servoY.write(105); 

  Serial.println("Setup complete.");
}

void loop() {
  DateTime now = rtc.now();
  int currentTime = now.hour();
  int moduloResult = currentTime % interval;
  int moduloTime = currentTime - moduloResult;

  if (moduloTime != lastTime && (moduloTime % interval == 0)) {
    lastTime = moduloTime;

    Serial.println(moduloTime);

    // Move arm to lift the probe from the 3NKCL solution
    moveUpServoY(105, 180);
    wdt_reset();

    moveDownServoY(180, 120);
    moveUpServoY(120, 180);
    moveDownServoY(180, 120);
    wdt_reset();
    moveUpServoY(120, 180);
    moveDownServoY(180, 120);
    moveUpServoY(120, 180);
    wdt_reset();

    // Move arm towards the water
    moveRightServoX();
    wdt_reset();
    
    // Move arm into water, read data, and send to Serial Comm
    moveDownServoY(180, 77);
    delay5minAndWdtReset();
    sendData(); // Read and send data
    moveUpServoY(77, 180);
    wdt_reset();

    moveDownServoY(180, 130);
    moveUpServoY(130, 180);
    moveDownServoY(180, 130);
    wdt_reset();
    moveUpServoY(130, 180);
    moveDownServoY(180, 130);
    moveUpServoY(130, 180);
    wdt_reset();

    // Move arm towards the 3NKCL solution container
    moveLeftServoX();
    wdt_reset();

    // Move arm into the 3NKCL solution
    moveDownServoY(180, 105);
    wdt_reset();
  }
  
  delay(1000);
  wdt_reset();
}

float getSuhu() {
  tempSensors.requestTemperatures();
  return tempSensors.getTempCByIndex(0);
}

int getTds(float suhu) {
  gravityTds.setTemperature(suhu); 
  gravityTds.update();  
  return gravityTds.getTdsValue(); 
}

float getPh() {
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(PHPin);
    delay(100);
  }

  float avgValue = 0;
  for (int i = 0; i < 10; i++)
    avgValue += buf[i];

  float pHVol = (float)avgValue * 5.0 / 1023 / 10;
  return -8.382 * pHVol + 34.74;
}

void startRtc() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void moveLeftServoX() {
  Serial.println("Moving left servoX");
  for (int pos = 80; pos <= 175; pos++) {
    servoX.write(pos);
    delay(20);
  }
}

void moveRightServoX() {
  Serial.println("Moving right servoX");
  for (int pos = 175; pos >= 80; pos--) {
    servoX.write(pos);
    delay(20);
  }
}

void moveDownServoY(int from, int to) {
  Serial.println("Moving down servoY");
  for (int pos = from; pos >= to; pos--) {
    servoY.write(pos);
    delay(20);
  }
}

void moveUpServoY(int from, int to) {
  Serial.println("Moving up servoY");
  for (int pos = from; pos <= to; pos++) {
    servoY.write(pos);
    delay(20);
  }
}

void sendData() {
  float suhu = getSuhu();
  int tds = getTds(suhu);
  float ph = getPh();

  Serial.print("suhu:");
  Serial.print(suhu);
  Serial.print("tds:");
  Serial.print(tds);
  Serial.print("ph:");
  Serial.println(ph);
}

void delay5minAndWdtReset() {
  for (int i = 0; i < 300; i++) {
    delay(1000);
    wdt_reset();
  }
}
