#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed libraries
#include <DHT.h>
#include <PubSubClient.h>

const char* ssid     = "WIFI SSID";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "WIFI PASSWORD";     // The password of the Wi-Fi network

#define DHTPIN 5 // Digital pin connected to the DHT sensor
#define LED D2
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
unsigned long interval = 10000;

const char* mqttServer = "192.168.x.xx"; //Server/Raspberry pi IP
const int mqttPort = 1883;               //MQTT port number

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  Serial.println();

  String message = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message += (char)payload[i];
  }
  Serial.println("-----------------------" + message);
  if (String(topic) == "LED") {
    if (message == "LED ON") {
      digitalWrite(LED, HIGH);
      Serial.println(".....LED IS ON");
    }
    else {
      digitalWrite(LED, LOW);
      Serial.println(".....LED IS OFF");
    }
  }

}

WiFiClient espClient;
PubSubClient client(mqttServer, mqttPort, callback, espClient);


void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.println('\n');

  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print(".....Connecting to ");
  Serial.print(ssid); Serial.println(" >>>>>...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("....Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer  
  Serial.println("connected...");
  dht.begin();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("......Connecting to MQTT...");

    if (client.connect("ESP8266Client" )) {
      client.subscribe("LED");
      Serial.println("........connected");

    } else {

      Serial.print("<<<<<<<<<<<<<<<<<failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }
}

void loop() {
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    //save the last time you updated the DHT values
    previousMillis = currentMillis;
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(h) && !isnan(t)) {
      Serial.println("Temperature : " + String(t));
      Serial.println("Humidity : " + String(h));
      String toSend = String(t) + "," + String(h);
      client.publish("data", toSend.c_str());
    }
  }
}
