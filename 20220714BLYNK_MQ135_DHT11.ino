#define BLYNK_TEMPLATE_ID           "TMPLyPSgT1A7"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "uolV8IQNPqqkEI2ogs3zFlWz5jdOqAXu"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <MQ135.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Hanbat_WLAN_Guest";  //집에서 개인 WiFi로 변경
char pass[] = "";                   //집에서 비밀번호 변경

#define PIN_MQ135 A0 // MQ135 Analog Input Pin
#define DHTPIN 0 // DHT Digital Input Pin
#define DHTTYPE DHT11 // DHT11 or DHT22, depends on your sensor

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

int value=0; //섭씨-화씨 변환용 변수
float temperature, humidity, tc; // Temp and Humid floats, will be measured by the DHT

BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
}

void loop() {

  Blynk.run();
  
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(value);
  tc = dht.readTemperature(0);
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(tc, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(tc, humidity);

  Blynk.virtualWrite(V5, humidity);  //Blynk 서버의 V5 가상핀에 습도값 전달
  Blynk.virtualWrite(V6, temperature);  //Blynk 서버의 V6 가상핀에 온도값 전달
  Blynk.virtualWrite(V2, correctedPPM);  //Blynk 서버의 V2 가상핀에 공기질값 전달
  
  Serial.print(value);  //0이면 섭씨, 1이면 화씨
  Serial.print(F("  Humidity: "));  //습도:
  Serial.print(humidity);  //습도값
  Serial.print(F("%  Temperature: "));  //온도:
  Serial.println(temperature);  //온도값, 줄바꿈
  
  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("ppm");
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");

  delay(1000);
}
