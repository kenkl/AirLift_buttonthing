// AirLift_buttonthing - borrowing heavily from WiFiWebClient.ino in the Adafruit WiFiNINA library's examples

#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>

// Configure the pins used for the ESP32 connection
#if defined(ADAFRUIT_FEATHER_M4_EXPRESS) || \
  defined(ADAFRUIT_FEATHER_M0_EXPRESS) || \
  defined(ARDUINO_AVR_FEATHER32U4) || \
  defined(ARDUINO_NRF52840_FEATHER) || \
  defined(ADAFRUIT_ITSYBITSY_M0_EXPRESS) || \
  defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS) || \
  defined(ARDUINO_AVR_ITSYBITSY32U4_3V)
  // Configure the pins used for the ESP32 connection
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS    13   // Chip select pin
  #define ESP32_RESETN  12   // Reset pin
  #define SPIWIFI_ACK   11   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#elif defined(ARDUINO_AVR_FEATHER328P)
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS     4   // Chip select pin
  #define ESP32_RESETN   3   // Reset pin
  #define SPIWIFI_ACK    2   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#elif defined(TEENSYDUINO)
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS     5   // Chip select pin
  #define ESP32_RESETN   6   // Reset pin
  #define SPIWIFI_ACK    9   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#elif defined(ARDUINO_NRF52832_FEATHER)
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS    16  // Chip select pin
  #define ESP32_RESETN  15  // Reset pin
  #define SPIWIFI_ACK    7  // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#elif !defined(SPIWIFI_SS)  // if the wifi definition isnt in the board variant
  // Don't change the names of these #define's! they match the variant ones
  #define SPIWIFI       SPI
  #define SPIWIFI_SS    10   // Chip select pin
  #define SPIWIFI_ACK    7   // a.k.a BUSY or READY pin
  #define ESP32_RESETN   5   // Reset pin
  #define ESP32_GPIO0   -1   // Not connected
#endif

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

const char* host = "max.kenkl.org";

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

const unsigned int clienttimeout = 60000;  //ms for client timeout in the doThing* actions
const int led = 12;

const int button1 = 11;
const char* url1 = "/lights/but7.php";
int button1State = 0;

void printWifiStatus();
void doThing(const char* url);
void doWiFi();
void(* resetFunc) (void) = 0;

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  digitalWrite(led, 1);
  Serial.println("AirLift_buttonthing (test) 20200203");
  Serial.println("Calls kcstog.php via pin 11 pulldown.");

  // Moved all the WiFi initialization bits to doWiFi() so we can reset it on-demand
  doWiFi();

  Serial.println("Startup complete. Waiting for button press...");
  digitalWrite(led, 0);

}

void loop() {
  button1State = digitalRead(button1);

  if(button1State == LOW) {
    Serial.println("button 1 pressed");
    digitalWrite(led, 1);
    while(digitalRead(button1) == LOW) {
       //poor man's debounce. wait until button is released.
    }
    doThing(url1);
  }
  else {
    digitalWrite(led, 0);
  }

} // end of loop()


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
void doThing(const char* url) {

  digitalWrite(led, 1); // light up a pilot while we connect, send, and get a response.

  Serial.println("Got to doThing...");
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;
if (!client.connect(host, httpPort)) {
  // Provide a visual cue that we're in failure state
  for(int i=0; i<5; i++) {
    digitalWrite(led, 1);
    delay(150);
    digitalWrite(led, 0);
    delay(150);
  }
  
  Serial.println("connection failed - resetting...");
  Serial.println();
  delay(1000); // Give the UART a second to drain...
  resetFunc();
  return;
}

// We now create a URI for the request
//String url = "/lights/cinema.php";

Serial.print("Requesting URL: ");
Serial.println(url);

// This will send the request to the server
client.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: close\r\n\r\n");
unsigned long timeout = millis();
while (client.available() == 0) {
  if (millis() - timeout > clienttimeout) {
    Serial.println(">>> Client Timeout !");
    client.stop();
    return;
  }
}

  Serial.println("closing connection");
  Serial.println();

  digitalWrite(led, 0); // turn off the pilot

}

void doWiFi() {
  digitalWrite(led, 1);
  // check for the WiFi module:
  WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
  while (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    delay(1000);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
  Serial.print("Found firmware "); Serial.println(fv);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  do {
    status = WiFi.begin(ssid, pass);
    delay(100);     // wait until connection is ready!
  } while (status != WL_CONNECTED);

  Serial.println("Connected to wifi");
  printWifiStatus();

  digitalWrite(led, 0);

}