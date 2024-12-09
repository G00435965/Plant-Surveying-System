/*CHECKLIST
 When moisture is low turn pump on - DONE
 Make sure all sensos read the data, I have a library for DHT11 but none of the others, do i need them? NO I DO NOT NEED THEM
 finish thingspeak
 delete any duplicates, I think theres a loop duplicate and something for temp + humid - JUST THE FIELDS APPARRENTLY
 Appaently digitalWrite only accepts HIGH and LOW, not numbers(for the motor contol) , check this out
 Set thresholds for the temp and humidity so that it displays on thingspeak
 Should I be using ledc instead of analogWrite
 I have my fields done twice 
 You can only have one setup and one loop so they all have to integrated together at the end
 */
#include <WiFi.h> //handles wifi connection for esp32 and used to connect to network and interact with thingspeak
#include "secrets.h"
#include "ThingSpeak.h"
#include <DFRobot_DHT11.h>
//START OF MY CODE

DFRobot_DHT11 DHT;
String myStatus;

//0---------------------------------------------defining variables-----------------------------------------------------------------------------------------------------0
#define DHT11_PIN 2
#define MOISTURE_PIN 24     /* pin of moisture sensor */
#define THRESHOLD_VALUE 300 /* threshold for watering the flowers */
#define MOTOR_PIN 23
#define BLINK_LED 13
#define LIGHT_SENSOR_PIN 12
#define DHT11_PIN 27
//#define ON HIGH             /* led on */
//#define OFF LOW
//0---------------------------------------------may be removed-----------------------------------------------------------------------------------------------------0

void setup() {
  Serial.begin(115200); //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
   WiFi.mode(WIFI_STA);
  WiFiClient Client; // is this needed?  
  ThingSpeak.begin(Client);  // Initialize ThingSpeak
  pinMode (MOTOR_PIN, OUTPUT); //Motor control pin as an output
  //DHT.begin(); // initializing dht11 sensor
  pinMode(BLINK_LED, OUTPUT); //declares the red LED as an output.

  
//0-------------------------------------------------CONNECT TO WIFI-------------------------------------------------------------------------------------------------0
if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected to Wi-Fi.");
  }
}

//0---------------------------------------------------READ DATA FROM SENSORS-----------------------------------------------------------------------------------------------0
void loop() {
//READ DATA FROM SENSORS

DHT.read(DHT11_PIN)
  float humidity  = DHT.humidity();
  float tempC = DHT.temperature(); //reads temp as Celcius
  int moistureLevel = analogRead(MOISTURE_PIN);
  delay(2000);
  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  delay(2000);
//0--------------------------------------------------CHECK DHT READINGS FOR ERRORS------------------------------------------------------------------------------------------------0
  // CHECK DHT READINGS FOR ERRORS
  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%");
    Serial.print("  |  "); //what is this for
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
}
//0-----------------------------------------------------MOISTURE SENSOR LOGIC + MOTOR PUMP ---------------------COMPLETED------------------------------------------------------------------------0
//MOISTURE SENSOR LOGIC 
/* if the value is smaller than threshold, turn on led */
    if(moistureLevel < THRESHOLD_VALUE) {
        digitalWrite(BLINK_LED, HIGH);
        analogWrite (MOTOR, 50); 
        Serial.println(" Motor at slow speed"); //for control
        delay (2000); //runs for 2 
        
        analogWrite (MOTOR, 100); 
        Serial.println(" Motor at medium speed");
        delay (4000); //runs for 4 seconds
        
        digitalWrite (MOTOR, HIGH) ;
        Serial.println("Motor at top speed. WATER PUMPING!");
        delay (2000);
        
        digitalWrite (MOTOR, LOW);
        Serial.println("Motor off. WATER OFF!");
        delay ( 2000);
        delay(200);
        Serial.println("Moisture below threshold! LED ON");
    } else {
        digitalWrite(BLINK_LED, LOW);
        Serial.println("Moisture sufficient. LED OFF");
    }

//0---------------------------------------LIGHT SENSOR VALUE-------still waiting on component i think-------------------------------------------------------------------------------------0
//LIGHT SENSOR VALUE
Serial.print("Light Sensor Value: "); //print value to serial monitor for debugging
Serial.println(lightValue);
//0------------------------------------SNEDING DATA TO THINGSPEAK??may need to be taken out-----------------------------------------------------------------------------------------------0
//SEND DATA TO THINGSPEAK      
if (WiFi.status() == WL_CONNECTED) {
  ThingSpeak.setField(1, tempC);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, moistureLevel);
  ThingSpeak.setField(4, lightValue);

  int responseCode = ThingSpeak.writeFields(channelID, writeApiKey);
  if (responseCode == 200) {
    Serial.println("Data sucessfully sent to ThingSpeak!");
  } else {
    Serial.print("Error sending data: ");
    Serial.println(responseCode);
  }
}else {
  Serial.println("Wi-Fi disconnected!");
    
  }

  delay(20000); //15 second delay before next reading, I THOUGHT THIS WAS A FEATURE THAT HAD TO BE DONE ON THINGSPEAK
}
//END OF MY CODE
//-------------------------------------------------THINGSPEAK STUFF---------------
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int WiFiClient  client;

unsigned long myChannelNumber = 2773493;
const char * myWriteAPIKey = 63MN78GHIJXL267V;

int number = 0;

//----------------------------------------------------CONNECT TO WIFI--------------
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  ThingSpeak.setField(1, tempC);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, lightValue);
  ThingSpeak.setField(4, moistureLevel);
//0-----------------------------------------DHT11 OPTION---------------------------0
if (tempC > 25 && humidity > 60) {
  myStatus = String("Temperature and humidity too high.");
}else if (tempC > 25 && humidity <= 60) {
  myStatus = String("Temperature is too high.");
}else if (tempC <= 25 && humidity > 60){
  myStatus = String("Humidity is too high.");
else {
}

myStatus = String("Temperature and humidity are fine.");
//0--------------------------------------------------------------------------------------------------------------------------------------------------0
ThingSpeak.setStatus(myStatus);

int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);


  int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  // change the value
  number++;
  if(number > 99){
    number = 0;
  }
  
  delay(20000); // Wait 20 seconds to update the channel again
}
