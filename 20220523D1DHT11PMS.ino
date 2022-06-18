#define BLYNK_TEMPLATE_ID           "TMPLJHfopORd"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "eqrFUksIgMFHXa7_5gYG3DkOsnklgdHl"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include "PMS.h"
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "pipipopo";
char pass[] = "a0423692236";

#define DHTPIN 14          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
PMS pms(Serial);
PMS::DATA data;
int value=0; //섭씨-화씨 변환용 변수
float a1_0, a2_5, a10;  //세 가지 미세먼지센서값 저장 변수
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.


BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}



void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(value); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
//    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V7, a1_0); //Blynk 서버의 V7 가상핀에 먼지 1.0 값 전달
  Blynk.virtualWrite(V8, a2_5); //Blynk 서버의 V8 가상핀에 먼지 2.5 값 전달
  Blynk.virtualWrite(V9, a10);  //Blynk 서버의 V9 가상핀에 먼지 10.0 값 전달
  Blynk.virtualWrite(V5, h);  //Blynk 서버의 V5 가상핀에 습도값 전달
  Blynk.virtualWrite(V6, t);  //Blynk 서버의 V6 가상핀에 온도값 전달
/*  Serial.print(value);  //0이면 섭씨, 1이면 화씨
  Serial.print(F("  Humidity: "));  //습도:
  Serial.print(h);  //습도값
  Serial.print(F("%  Temperature: "));  //온도:
  Serial.println(t);  //온도값, 줄바꿈
  */
}

void setup()
{
  // Debug console
  Serial.begin(57600); //USB 통신용
//  Serial.begin(9600);  //PMS7003M과 UART 통신
  Blynk.begin(auth, ssid, pass);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();

  if (pms.read(data))
  {
/*    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(a1_0=data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(a2_5=data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(a10=data.PM_AE_UG_10_0);

    Serial.println();
*/
a1_0=data.PM_AE_UG_1_0;
a2_5=data.PM_AE_UG_2_5;
a10=data.PM_AE_UG_10_0;    
  }

}
