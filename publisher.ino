#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "shrey";
const char* password = "8050973121";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.43.195";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, espClient);

const int trigPin = 5; 
const int echoPin = 4; 

long duration;
int distance;

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect("ESP8266Client", "rpi", "groundscrew")) {
      Serial.println("connected");  
      client.subscribe("esp8266/4");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(15000);
    }
  }
}

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())

    client.connect("ESP8266Client", "rpi", "groundscrew");
    
  // Clears the trigPin
  int distance = sample_distance();
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  static char dist[7];
  dtostrf(distance, 6, 2, dist);
  delay(2000);
    client.publish("/esp8266/ultrasonic", dist);

}

int sample_distance() {
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(30);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  Serial.println("Duration: ");
  Serial.println(duration);
  distance = int((duration / 2) / 29.155);
  return distance;
}

