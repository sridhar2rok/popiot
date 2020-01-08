
/**
    Required libraries:
      - DHT sensor library by Adafruit
      - Adafruit Unified Sensor
      - PubSubClient
**/

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN D4
#define DHTTYPE DHT11

#define MQTT_TOPIC_HUMIDITY "home/dht11/humidity"
#define MQTT_TOPIC_TEMPERATURE "home/dht11/temperature"
#define MQTT_TOPIC_DISTANCE "home/sr04/distance"
#define MQTT_TOPIC_STATE "home/dht11/status"
#define MQTT_PUBLISH_DELAY 10000
#define MQTT_CLIENT_ID "esp8266dht11"

const char* WIFI_SSID = "popcorn1234";
const char* WIFI_PASSWORD = "internetofp";

const char *MQTT_SERVER = "192.168.4.1";
const char *MQTT_USER = "admin"; // NUL for no authentication
const char *MQTT_PASSWORD = "admin"; // NULL for no authentication
const int trigPin = 4;   // Addition ultrasonic sensor
const int echoPin = 5;  // Addition ultrasonic sensor
long duration;           // Addition ultrasonic sensor
int distance;            // Addition ultrasonic sensor
float humidity;
float temperature;
int arraysize = 11;  //quantity of values to find the median (sample size). Needs to be an odd number
int rangevalue[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  
long lastMsgTime = 0;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Serial.begin(115200);
  while (! Serial);

  setupWifi();
  mqttClient.setServer(MQTT_SERVER, 1883);
  dht.begin();
}

void loop() {
  if (!mqttClient.connected()) {
    mqttReconnect();
  }
  
  mqttClient.loop();
  
  for(int i = 0; i < arraysize; i++)
  {                                                    //array pointers go from 0 to 4
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
   digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);
  rangevalue[i] = distance;
        // Serial.print("i, value   ");
       //  Serial.print(i);
       //  Serial.print(" ,  ");
         //Serial.print(rangevalue[i]);
        // Serial.println();

    delay(10);  //wait between analog samples
}

    //Serial.print("unsorted ");
     printArray(rangevalue, arraysize);
     //Serial.println();
     isort(rangevalue, arraysize);
     //Serial.print("sorted ");
     printArray(rangevalue, arraysize);
     //Serial.println();
     int midpoint = arraysize/2;  
   //midpoint of the array is the medain value in a sorted array
     //note that for an array of 5, the midpoint is element 2, as the first element is element 0
    //Serial.print("median range value ");
    Serial.print(rangevalue[midpoint]);
    Serial.println();  
    //Serial.println(); 
  long now = millis();
  if (now - lastMsgTime > MQTT_PUBLISH_DELAY) {
    lastMsgTime = now;

    // Reading dht11 sensor data
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("dht11 sensor is not ready yet");
      return;
    }   
    // Publishing sensor data
     mqttPublish(MQTT_TOPIC_TEMPERATURE, temperature);
     mqttPublish(MQTT_TOPIC_HUMIDITY, humidity);
     mqttPublish(MQTT_TOPIC_DISTANCE, rangevalue[midpoint]);
  }

}

void setupWifi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD, MQTT_TOPIC_STATE, 1, true, "disconnected", false))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish(MQTT_TOPIC_STATE, "connected", true);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttPublish(char* topic, float payload) {
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);

  mqttClient.publish(topic, String(payload).c_str(), true);
}

//*********************************************************************************
// sort function
void isort(int *a, int n)
              //  *a is an array pointer function
{
 for (int i = 1; i < n; ++i)
 {
   int j = a[i];
   int k;
   for (k = i - 1; (k >= 0) && (j < a[k]); k--)
   {
     a[k + 1] = a[k];
   }
   a[k + 1] = j;
 }
}
//***********************************************************************************
//function to print array values
void printArray(int *a, int n)
{
 
 for (int i = 0; i < n; i++)
 {
   Serial.print(a[i], DEC);
   Serial.print(' ');
 }
 
 Serial.println();
}
