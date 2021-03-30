#include <DHT.h> // library for dht22 humidity and temperature sensor
#include "Adafruit_BMP085.h" // library for bmp180 pressure sensor

#define DHTPIN 7
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp180;
const int wInPin = A0;
float h, tc, tf, p, atm, wVal, wVolt, wSpeed, wMPH; 


float mapSpeed(float sensorVoltage, float input_min, float input_max, float output_min, float output_max) {
  float speed = (sensorVoltage - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
  if (speed <= 0) return 0;
  else return speed;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bool success = bmp180.begin();
  dht.begin();
  if (!success) {
    Serial.println("BMP180 initilization failed!");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000); //wait for sensors to start up
  //get data values, then send to pi via serial communication through a USB connection
  h = dht.readHumidity();
  tc = dht.readTemperature();
  tf = (tc *(9.0/5.0)) + 32.0;
  p = bmp180.readPressure();
  atm = p / 101325.0; // convert from pascals to atms
  wVal = analogRead(wInPin);
  wVolt = (wVal / 1023.0) * 5.0; // 1024 possible values, with a range of 5 volt to be read
  wSpeed = mapSpeed(wVolt, 0.4, 2, 0, 32.4);
  wMPH = wSpeed * 2.23694;
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%\n");
  Serial.print("Temperature: ");
  Serial.print(tc);
  Serial.print(" Celsius, ");
  Serial.print(tf);
  Serial.print(" Farenheit\n");
  Serial.print("Pressure: ");
  Serial.print(p);
  Serial.print(" Pascals, ");
  Serial.print(atm);
  Serial.print(" atms\n");
  Serial.print("Wind Speed: ");
  Serial.print(wSpeed);
  Serial.print(" m/s, ");
  Serial.print(wMPH);
  Serial.print(" mph\n");
  delay(500);
  
}
