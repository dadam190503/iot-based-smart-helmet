#define PIN_MQ135 A0
#define DHTPIN 5 // D1
#define DHTTYPE DHT11
#define buzzer 4 // D2

//WiFi Credentials
const char* ssid = "qwerty"; //Enter SSID
const char* password = "12345678"; //Enter Password

//Blynk Credentials
#define BLYNK_TEMPLATE_ID "TMPL3qbC7num6"
#define BLYNK_TEMPLATE_NAME "Smart Helmet"
#define BLYNK_AUTH_TOKEN "z1IMfrB2PzMVww3XLnvDx_ymIoWyeoZz"

#include <MQ135.h>
#include <DHT.h>


#include <ESP8266WiFi.h>  


#include <BlynkSimpleEsp8266.h>

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

float temperature, humidity;

float rzero, correctedRZero, resistance, ppm, correctedPPM;

BlynkTimer timer; 

void setup() {
  Serial.begin(115200); // Starts the serial communication
  dht.begin();
  pinMode(buzzer, OUTPUT);
  pinMode(PIN_MQ135, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  timer.setInterval(1000L, myTimer); 

  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
  }
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());// Print the IP address
}

void myTimer() 
{
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);

}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  int analogSensor = analogRead(PIN_MQ135);

  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  Serial.println(analogSensor);
  Blynk.virtualWrite(V3, analogSensor);

  if(analogSensor >= 650){
    digitalWrite(buzzer, HIGH);
  }
  else{
    digitalWrite(buzzer, LOW);
  }

    // Runs all Blynk stuff
  Blynk.run(); 
  // runs BlynkTimer
  timer.run(); 
}
