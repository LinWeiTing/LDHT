// Example testing sketch for various DHT11, DHT21, DHT22 humidity/temperature sensors
// Written by Lin Wei Ting in National Taipei University of Technology Applied Network Laboratory. Taipei, Taiwan.

#include "LDHT.h"

#define DHTPIN 8          // what pin we're connected to
#define DHTTYPE DHT11     // using DHT11 sensor

LDHT dht(DHTPIN,DHTTYPE);

float tempC=0.0,Humi=0.0;

void setup()
{
    Serial.begin(9600);
    dht.begin();
	
    Serial.print(DHTTYPE);
    Serial.println(" test!");
}

void loop()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    if(dht.read())
    {
        tempC = dht.readTemperature();
        Humi = dht.readHumidity();


        Serial.println("------------------------------");
        Serial.print("Temperature Celcius = ");
        Serial.print(dht.readTemperature());
        Serial.println("C");

        Serial.print("Temperature Fahrenheit = ");
        Serial.print(dht.readTemperature(false));
        Serial.println("F");

        Serial.print("Humidity = ");
        Serial.print(dht.readHumidity());
        Serial.println("%");

        Serial.print("HeatIndex = ");
        Serial.print(dht.readHeatIndex(tempC,Humi));
        Serial.println("C");

        Serial.print("DewPoint = ");
        Serial.print(dht.readDewPoint(tempC,Humi));
        Serial.println("C");
    }

    delay(500);
}
