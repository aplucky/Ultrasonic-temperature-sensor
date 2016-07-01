#include <SoftwareSerial.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3

#define DHTPIN 3
#define DHTTYPE DHT11   // DHT 11
#define TRIGGER 0

DHT dht(DHTPIN, DHTTYPE);
#define ECHO 2 // what digital pin we're connected to

//-------- Customise these values -----------
const char* ssid = "Your SSID";
const char* password = "Your Password";

char server[] = "Server Id";
char topic[] = "";
char clientId[] = "Your client id";
String distance = "01";

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

void setup() {
Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);

Serial.begin(9600);
dht.begin();

Serial.println();

Serial.print("Connecting to "); Serial.print(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
} 
Serial.println("");

Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());

pinMode(TRIGGER, OUTPUT);
}

int counter = 0;

void loop() {

if (!!!client.connected()) {
Serial.print("Reconnecting client to ");
Serial.println(server);
while (!!!client.connect(clientId)) {
Serial.print(".");
delay(500);
}
Serial.println();
}
long duration, distance;
dht.begin();
digitalWrite(TRIGGER, HIGH);
delay(500); 

digitalWrite(TRIGGER, LOW);

duration = pulseIn(ECHO, HIGH);
delay(800);

float t = dht.readTemperature();

Serial.print("Temperature : ");
Serial.println(t);

distance = (0.0331+0.00006*t)*duration/2 ;  

Serial.print("Distance : ");
Serial.println(distance);

///json string here
if(distance<800)
{
Serial.print("Sending json: "); 
String json = "{\"distance\":\"";                                                                                                                                        
json+=distance ;
json+="\", \"temperature\":\"";
json+=t;
json+="\"}";
Serial.println(json);
Serial.println("Sending json: ");
String jton = "{\"temperature\":\"";                                                                                                                                        
jton+=t ;
jton+="\"}";
Serial.println(jton);

if (client.publish(topic, (char*) json.c_str())) {
Serial.println("Publish ok");
} else {
Serial.println("Publish failed");
}
}
delay(1000);
}
