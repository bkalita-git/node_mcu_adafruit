//https://www.hackster.io/sarful/interface-raindrop-sensor-to-nodemcu-for-beginner-e76e88
#include <Adafruit_MQTT_FONA.h>
#include <Adafruit_MQTT_Client.h>
#include <Adafruit_MQTT.h>
#include <DHT_U.h>   //DHT SENSOR LIBRARY  https://roboindia.com/tutorials/dht11-nodemcu-arduino/
#include <DHT.h>  
#include <ESP8266WiFi.h>


#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME  "hastetough1"
#define AIO_KEY       "aio_xxxxxxxxxxxxxvaJhjz"
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0   //D3



const char *ssid =  "Q";     
const char *pass =  "1234567033";
int sensorPin = A0;    
int sensorValue2 = 0; 
String buf;
char pline[64];

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe readc = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/read");
//Adafruit_MQTT_Publish displayc = Adafruit_MQTT_Publish(&mqtt,   AIO_USERNAME "/feeds/display");
Adafruit_MQTT_Publish temperaturec = Adafruit_MQTT_Publish(&mqtt,   AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidityc = Adafruit_MQTT_Publish(&mqtt,   AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish rainc = Adafruit_MQTT_Publish(&mqtt,   AIO_USERNAME "/feeds/rain");
DHT dht(dht_dpin, DHTTYPE); 

 
void setup() 
{
       Serial.begin(9600);
       delay(10);       
       Serial.println("Connecting to ");
       Serial.println(ssid); 
       WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected"); 
      mqtt.subscribe(&readc);
      dht.begin();
}
 
void loop() 
{      
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while((subscription = mqtt.readSubscription(200))){
    if(subscription == &readc){
      uint16_t num = atoi((char *)readc.lastread);
      if(num==1){
        //buf = "";
        //buf += F("Temp: ");
        //buf += String(get_temp(),2);
        //buf += F("C");
        //buf += F("\thumid: ");
        //buf += String(get_humid(),2);
        //buf += F("%");
        //buf += F("\train: ");
        //buf += String(get_rain(),2);
        //buf.toCharArray(pline,buf.length()+1);
        //displayc.publish(pline);
        temperaturec.publish(get_temp());
        humidityc.publish(get_humid());
        rainc.publish(get_rain());
      }
    }
  }
}


void MQTT_connect(){
  int8_t ret;
  if (mqtt.connected()){
    return;
  }
  Serial.print("Connecting to MQTT...");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) !=0){
    Serial.println("errorstring");
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("finished errorstring");
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if(retries == 0){
      while(1);
    }
  }
  Serial.println("MQTT Connected!");
}

float get_temp(){
    //return dht.readTemperature(); 
    return 39.4;        
}

float get_humid(){
    //return dht.readHumidity();
    return 89.64;
}

float get_rain(){
    sensorValue2 = analogRead(sensorPin);
    sensorValue2 = constrain(sensorValue2, 150, 440); 
    sensorValue2 = map(sensorValue2, 150, 440, 1023, 0); 
    //return sensorValue2;
    return 80;
}
