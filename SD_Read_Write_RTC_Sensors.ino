/* SD/MicroSD card Read & Write using a RTC (Real Time Clock) and ambient sensors.
 * MOSI - pin 11.
 * MISO - pin 12.
 * CLK/SCK - pin 13.
 * CS - pin 10 (for MicroSD Card Adapter or Seeedstudio Shield).
 * SCL - Pin PC5/A5/Pin closest to USB/reset.
 * SDA - Pin PC4/A4/Pin next to SCL.
 * Here is the parts list:
 * -Arduino Uno
 * -USB Cable
 * -Breadboard
 * -Jumper Wires
 * -DS3231 RTC Module (Real Time Clock)
 * -MicroSD Card Adapter
 * -DHT-11 Temperature/Humidity Sensor
 * -BMP180 Pressure Sensor
*/

#define RTC_CHIP_IS_DS3231 //The RTC used has a DS3231 chipset.
#include <SD.h> //Include the SD Card Library for reading and writing to the card.
#include <Wire.h> //For I2C protocol.
#include "RTClib.h" //For RTC library.
#include <EduIntro.h>// For the DHT-11 library.
#include <Adafruit_BMP085.h>// Use for the BMP180 Pressure sensor.


const int CS_PIN = 10;//Chip select pin for SD card.
int refresh_rate = 5000;//Refresh rate is 5 seconds (5000 milliseconds).

String time, date;// Variables for dates and times.
DHT11 dht11(D9);// Variable for DHT-11 sensor on digital pin 9.
int C;//Variable for Celsius temps.
float F;//Variable for Fahrenheit temps.
int H;//Variable for Humidity percentage.
Adafruit_BMP085 bmp;//Class for BMP pressure and altitude variables.
RTC_DS3231 RTC;//Variable for RTC.

void updateDateTime(){
 //Current date and time info to store into strings.
 DateTime datetime = RTC.now();
 String year = String(datetime.year(), DEC);
 String month = String(datetime.month(), DEC);
 String day = String(datetime.day(), DEC);
 String hour = String(datetime.hour(), DEC);
 String minute = String(datetime.minute(), DEC);
 String second = String(datetime.second(), DEC);
 
 //Combine the strings into date and time.
 date = year + "/" + month + "/" + day;
 time = hour + ":" + minute + ":" + second;
}
  
void setup(){
 Serial.begin(9600);//Serial communications baud rate (bits per second).
 pinMode(CS_PIN, OUTPUT);// Set this pin to digital output.
 RTC.begin();//Initialize clock.
 bmp.begin();//Initialize pressure sensor.

 updateDateTime();
 Serial.print(F("RTC Date: "));
 Serial.println(date);
 Serial.print(F("RTC time: "));
 Serial.println(time);
 
 Serial.print(F("Initializing SD Card…"));
 if (!SD.begin(CS_PIN)){
  Serial.println(F("Card Failure!"));
  while(1);
 }
 Serial.println(F("Card Ready!"));
 Serial.print(refresh_rate);
 Serial.println(F("ms!"));

 File dataFile = SD.open("logRTCsd.csv", FILE_WRITE);//Name your file whatever you want up to 8 alphanumeric characters.
 if (dataFile){
  dataFile.println(F("\nNew Log Started!"));
  dataFile.println(F("Date,Time,Phrase,Humidity,TempC,TempF,PressurePa,AltitudeM"));
  dataFile.close(); //Data isn't written until we run close()!
 
  //Print same thing to the screen for debugging
  Serial.println(F("\nNew Log Started!"));
  Serial.println(F("Date,Time,Phrase,Humidity,TempC,TempF,PressurePa,AltitudeM"));
 }
 else{
  Serial.println(F("Couldn't open log file"));
  while(1);
 }
 
}

void loop(){ 
 updateDateTime(); //Get the current date/time
 String dataString = "Hello There!";//Any phrase you want to use.
 File dataFile = SD.open("logRTCsd.csv", FILE_WRITE);
 dht11.update();
 H = dht11.readHumidity();
 C = dht11.readCelsius();
 F = dht11.readFahrenheit();
 
 if (dataFile){
  
  dataFile.print(date);
  dataFile.print(F(","));
  dataFile.print(time);
  dataFile.print(F(","));
  dataFile.print(dataString);
  dataFile.print(F(","));
  dataFile.print(H);
  dataFile.print(F(","));
  dataFile.print(C);
  dataFile.print(F(","));
  dataFile.print(F);
  dataFile.print(F(","));
  dataFile.print(bmp.readPressure());
  dataFile.print(F(","));
  dataFile.println(bmp.readAltitude());
  dataFile.close(); //Data isn't written until we run close()!
 
  //Print to Serial Monitor to ensure same data is being recorded properly.
  Serial.print(date);
  Serial.print(F(","));
  Serial.print(time);
  Serial.print(F(","));
  Serial.print(dataString);
  Serial.print(F(","));
  Serial.print(H);
  Serial.print(F(","));
  Serial.print(C);
  Serial.print(F(","));
  Serial.print(F);
  Serial.print(F(","));
  Serial.print(bmp.readPressure());
  Serial.print(F(","));
  Serial.println(bmp.readAltitude());
 }
 else{
  Serial.println(F("Couldn't open log file!"));
 }
 delay(refresh_rate);
} 
