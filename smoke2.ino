#define BLYNK_TEMPLATE_ID "TMPLeTRBaqfd"
#define BLYNK_DEVICE_NAME "Gas and Fire"
#define BLYNK_AUTH_TOKEN "MoraUODkWK2IkOGMHj8Snj-J3pHAcPSR"
#include "DHT.h"
#define BLYNK_PRINT Serial

#define APP_DEBUG

#define USE_NODE_MCU_BOARD


#define DHTTYPE DHT11   // DHT11 sensor
#define DHTPIN 0        // Digital input pin connected to the DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "Blynk.h"
#include <MQ2.h>
#include <SPI.h>
#include <Wire.h>

char auth[] = "MoraUODkWK2IkOGMHj8Snj-J3pHAcPSR"; 
char ssid[] = "Alpha"; 
char pass[] = "180403838";


#define BUZZ      12 //D6 
#define LED       14 //D5

int flag=0;


int pin = A0;
float lpg, co, smoke;
MQ2 mq2(pin);
int button1 = 0;
int button2 = 0;
SimpleTimer timer;

void setup()
{
  Serial.begin(115200);
  delay(100);
  Blynk.begin(auth, ssid, pass);

  pinMode(BUZZ, OUTPUT);
  pinMode(LED, OUTPUT);



  digitalWrite(BUZZ, LOW);
  digitalWrite(LED, LOW);

  dht.begin();
  mq2.begin();
  timer.setInterval(1000L, sendSensorData);

}

void loop() {
  timer.run(); // Initiates SimpleTimer
  Blynk.run();
}
void sendSensorData()
{
  float* values = mq2.read(true); //set it false if you don't want to print the values to the Serial
  co = mq2.readCO();
  smoke = mq2.readSmoke();
  lpg = mq2.readLPG();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Blynk.virtualWrite(V6, temperature); 
  Blynk.virtualWrite(V7, humidity);
    if (temperature > 50 ) {
    Blynk.logEvent("t", "High Temperature Alert!!!!");
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  int isButtonPressed = digitalRead(D1); // falme sensor connected D1 pin
   if (isButtonPressed==1 && flag==0) 
   {

       flag=1;  
       Blynk.virtualWrite(V0,0);
       digitalWrite(BUZZ, LOW);
   }
   else if (isButtonPressed==0)
   {
   flag=0;
   Serial.println("Fire DETECTED");
   digitalWrite(BUZZ, HIGH);
   Blynk.virtualWrite(V0,255);
   Blynk.logEvent("fire", "Fire Detected!!!");
   }

  Blynk.virtualWrite(V1, smoke);
  Blynk.virtualWrite(V2, lpg);
  Blynk.virtualWrite(V3, co);
  if (smoke > 50 ) {
    Blynk.logEvent("smoke", "Smoke Detected!");
    digitalWrite(BUZZ, HIGH);
    digitalWrite(LED, HIGH);
  }

  else {
    digitalWrite(BUZZ, LOW);
    
  }
  if (lpg > 50 ) {
    Blynk.logEvent("lpg", "LPG Detected!");
  }
  if (co > 50 ) {
    Blynk.logEvent("co", "Carbon Monoxide Detected!");
  }

}


// in Blynk app writes values to the Virtual Pin 4
BLYNK_WRITE(V4)
{
  button1 = param.asInt(); // assigning incoming value from pin V4 to a variable
}
// in Blynk app writes values to the Virtual Pin 5
BLYNK_WRITE(V5)
{
  button2 = param.asInt(); // assigning incoming value from pin V5 to a variable
}