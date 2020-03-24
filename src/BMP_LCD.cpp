
#include <Wire.h>
#include "SSD1306.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

const int pinSDA = 4;
const int pinSCL = 5;

SSD1306Wire  display(0x3c, pinSDA, pinSCL);


const int buttonPin = 14;//14;    // the number of the pushbutton pin

const int delayTime = 5000; 
const int waittingSec = 15;
int reading =LOW;

float temp;
float hum;
float pres;
float alti;

void readInput(){
  if(reading==LOW){
      reading = digitalRead(buttonPin);
  }
}
void init_oled() {
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, "Init OLED");
}
void set_s_text(){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);;
}
void set_m_text(){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);;
}
void set_b_text(){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
}

void new_screen_oled(int x,int y,char* mesg){
  display.clear();
  display.drawString(x, y, mesg);
  display.display();
  delay(1000);
}

void display_bme_temp() {
    readInput();
    display.clear();  
    set_m_text();
    display.drawString(0,0,"Temperature (*C)");
    set_b_text();
    temp=bme.readTemperature();
    display.drawString(0,22, String(temp));
    display.display();
    delay(delayTime);
}

void display_bme_last_Pressure() {
    readInput();
    display.clear();  
    set_m_text();
    display.drawString(0,0,"Last Pres & Alti");
    set_m_text();
    display.drawString(0,22,String(String(pres)));
    display.drawString(60,22, "hPa");
    display.drawString(0,44, String(alti));
    display.drawString(60,44, "M");
    display.display();
    delay(delayTime);
}
void display_bme_last_temp() {
    readInput();
    display.clear();  
    set_m_text();
    display.drawString(0,0,"Last Temp & Humi");
    set_m_text();
    display.drawString(0,22,String(temp));
    display.drawString(60,22, "*C");
    display.drawString(0,44, String(hum));
    display.drawString(60,44, "%");
    display.display();
    delay(delayTime);
}
void display_bme_Pressure() {
    readInput();
    display.clear();  
    set_m_text();
    display.drawString(0,0,"Pressure (hPa)");
    set_b_text();
    pres=bme.readPressure() / 100.0F;
    display.drawString(0,22, String(pres));
    display.display();
    delay(2000);
}
void display_bme_Altitude() {
    readInput();
    display.clear();  
    set_m_text();
    display.drawString(0,0,"Altitude (M)");
    set_b_text();
    alti=bme.readAltitude(SEALEVELPRESSURE_HPA);
    display.drawString(0,22, String(alti));
    display.display();
    delay(2000);
}
void display_bme_Humidity() {
    readInput();
    display.clear();  
    set_m_text();
    display.drawString(0,0,"Humidity (%)");
    set_b_text();
    hum=bme.readHumidity();
    display.drawString(0,22, String(hum));
    display.display();
    delay(2000);
}

void setup(void){
  Wire.pins(pinSDA, pinSCL);
  Serial.begin(115200);
  init_oled();
  Serial.println("init");

  
  bool status;
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
      set_s_text();
      new_screen_oled(0,0,"Not find BMP280");
      while (1);
  }

  pinMode(buttonPin, INPUT);
  Serial.println("start");
  set_m_text();
  new_screen_oled(0,0,"Starting ESP12S");

}

void loop(void){  
  readInput();
  if(reading==HIGH)
  {
    reading=LOW;
    set_m_text();
    new_screen_oled(0,0,"Data is coming...");
    
    for (int i=0;i<waittingSec;i++){
        set_m_text();
        display_bme_temp();
        display_bme_Humidity();
        display_bme_Pressure();
        display_bme_Altitude();
        delay(500);    
    } 
  }
  else{
        display_bme_last_temp();
        display_bme_last_Pressure();
  }
  delay(500);
}

