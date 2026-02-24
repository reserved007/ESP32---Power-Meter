 #define BLYNK_TEMPLATE_ID "TMPL6FvVcSx_W"
#define BLYNK_TEMPLATE_NAME "CAPSTONE"
#define BLYNK_AUTH_TOKEN "Jvb-T-NSh05IXzXBdFeUt1CVA_RSsLvz"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//#include <SoftwareSerial.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h"


EnergyMonitor emon;
BlynkTimer timer;
//SimpleTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);  


#define vCalibration 160 //calibrate sensor value and put here
//#define vCalibration 83.3 //calibrate sensor value and put here
//#define vCalibration 10 //calibrate sensor value and put here
#define currCalibration 0.50 //calibrate sensor values and put here


char ssid[] = "";  // type your wifi name
char pass[] = "";  // type your wifi password
char auth[] = BLYNK_AUTH_TOKEN;

float firstVal, secondVal,thirdVal,fourthVal; // sensors 

float kWh = 0;
unsigned long lastmillis = millis();
//ON/OFF RELAY CODE
const int relay = 26;

bool sdata =LOW;
int sdata1;

void myTimerEvent()
{
  emon.calcVI(20, 2000);
  kWh = kWh + emon.apparentPower * (millis() - lastmillis) / 3600000000.0;
  yield();
  Serial.print("Vrms: ");
  Serial.print(emon.Vrms, 2);
  Serial.print("V");
 
  Serial.print("\tIrms: ");
  Serial.print(emon.Irms, 4);
  Serial.print("A");
 
  Serial.print("\tPower: ");
  Serial.print(emon.apparentPower, 4);
  Serial.print("W");
 
  Serial.print("\tkWh: ");
  Serial.print(kWh, 5);
  Serial.println("kWh");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vrms:");
  lcd.print(emon.Vrms, 2);
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Irms:");
  lcd.print(emon.Irms, 4);
  lcd.print("A");
  delay(2500);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power:");
  lcd.print(emon.apparentPower, 4);
  lcd.print("W");
  lcd.setCursor(0, 1);
  lcd.print("kWh:");
  lcd.print(kWh, 4);
  lcd.print("W");
  delay(2500);
 
  lastmillis = millis();
 
  Blynk.virtualWrite(V0, emon.Vrms);
  Blynk.virtualWrite(V1, emon.Irms);
  Blynk.virtualWrite(V2, emon.apparentPower);
  Blynk.virtualWrite(V3, kWh);
//  Blynk.virtualWrite(V4,0);
}
 


void setup() {
    lcd.init();
  // turn on LCD backlight                      
  lcd.backlight(); 
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(relay, OUTPUT);


  digitalWrite(relay,LOW);

  
  emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration); // Current: input pin, calibration.
 
  timer.setInterval(5000L, myTimerEvent);
  timer.setInterval(2000L,sensorvalue4);
    timer.setInterval(2000L,sensorvalue5);
  lcd.setCursor(3, 0);
  lcd.print("IoT Energy");
  lcd.setCursor(5, 1);
  lcd.print("Meter");
  delay(3000);
  lcd.clear();
}


void loop() {
 
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

}

void sensorvalue4()
{

  Blynk.virtualWrite(V4, sdata);
 
}
void sensorvalue5()
{

  Blynk.virtualWrite(V5, sdata1);
 
}



BLYNK_WRITE(V4) {
  sdata = param.asInt();

  digitalWrite(relay, param.asInt());
}
BLYNK_WRITE(V5) {
  sdata1 = param.asInt();
  if(kWh > sdata1){
    digitalWrite(relay, HIGH);
  }
  Serial.print(sdata1);
}
