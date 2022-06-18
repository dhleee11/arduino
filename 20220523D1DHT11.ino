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

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Hanbat_WLAN_Guest";
//char pass[] = "";
char ssid[] = "pipipopo";
char pass[] = "a0423692236";
#define DHTPIN 14          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
int value=0; //섭씨-화씨 변환용 변수
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
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //Blynk 서버의 V5 가상핀에 습도값 전달
  Blynk.virtualWrite(V6, t);  //Blynk 서버의 V6 가상핀에 온도값 전달
  Serial.print(value);  //0이면 섭씨, 1이면 화씨
  Serial.print(F("  Humidity: "));  //습도:
  Serial.print(h);  //습도값
  Serial.print(F("%  Temperature: "));  //온도:
  Serial.println(t);  //온도값, 줄바꿈
}

void setup()
{
  // Debug console
  Serial.begin(57600);

  Blynk.begin(auth, ssid, pass);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
