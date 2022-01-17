#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

// FOR SPI implementation
/*
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
*/

// Valencia Air Quality and Pressure: https://www.iqair.com/spain/valencia
#define SEALEVELPRESSURE_HPA (1033)

Adafruit_BME680 bme; // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

// OLED Display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Functions prototype declaration
void showMessageOnScreen();

double pressure = 0;
double temperature = 0;
double humidity = 0;
double gas = 0;
double altitude = 0;

void setup() {
  Serial.begin(9350);
  while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_16X);
  bme.setHumidityOversampling(BME680_OS_8X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 50); // 320*C for 50 ms

  // OLED
  u8g2.begin();
  u8g2.enableUTF8Print();
}

void loop() {
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  temperature = bme.temperature;
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");

  pressure = bme.pressure / 100.0;
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  humidity = bme.humidity;
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");

  gas = bme.gas_resistance / 1000.0;
  Serial.print("Gas = ");
  Serial.print(gas);
  Serial.println(" KOhms");

  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m\n");
  
  showMessageOnScreen();
  delay(2000);
}

void showMessageOnScreen(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setCursor(0, 5);
  u8g2.drawStr(25,8,"Practica de RIS");
  u8g2.drawStr(25,13,"---------------");

  // Temporal variable to convert values to string
  char tempStr[10];
  char presStr[10];
  char humStr[10];
  char gasStr[10];
  char altStr[10];
  dtostrf(temperature,2,2,tempStr);
  dtostrf(pressure,2,2,presStr);
  dtostrf(humidity,2,2,humStr);
  dtostrf(gas,2,2,gasStr);
  dtostrf(altitude,2,2,altStr);

  // Print data
  u8g2.drawStr(5,22,"Hum:");
  u8g2.drawStr(38,22,humStr);
  u8g2.drawStr(5,32,"Temp:");
  u8g2.drawStr(38,32,tempStr);
  u8g2.drawStr(5,42,"Gas:");
  u8g2.drawStr(38,42,gasStr);
  u8g2.drawStr(5,52,"Pres:");
  u8g2.drawStr(38,52,presStr);
  u8g2.drawStr(5,62,"Alt:");
  u8g2.drawStr(38,62,altStr);
  u8g2.sendBuffer();
}