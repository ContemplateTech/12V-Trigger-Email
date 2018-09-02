// Send an Email on boot and when an input switches state.
// Borrowed Demo using DHCP and DNS to perform a web client request
// from <David A. Mellis>, <Tom Igoe> and <Adrian McEwen>.

// The input pin is intended to be connected to a relay such that if the relay closes,
// the input will be connected to ground, otherwise a pull-up for the pin will give the
// logic value

// The ENC28J60 Ethernet breakout board needs a particular library, in this case UIPEthernet

#include <SPI.h>
#include <UIPEthernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "maker.ifttt.com"; 

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

// Variables to keep track of input state
int val = 1;
int oldval = 1;
int inpin = 7;

// timer is used to perform tasks at regular intervals
static uint32_t timer;

// to keep a state for when to expect a message in return
bool message_sent = false;

void setup() {
  pinMode(inpin, INPUT);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    //Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  //Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    //Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /trigger/start_up/with/key/<your key> HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  val = 1;
  oldval = val;
  timer = millis() + 5000;
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    if (message_sent) {
      Serial.println();
      message_sent = false;
    }
    client.stop();
    delay(50);
  }
   
  if (millis() > timer) {
    oldval = val;
    val = digitalRead(inpin);
    if (oldval == 0) {
      if (val == 1) { // relay connected to nc
        // rising edge detected
        if (client.connect(server, 80)) {
          // Make a HTTP request:
          client.println("GET /trigger/trigger_off/with/key/<your key> HTTP/1.1");
          client.println("Host: maker.ifttt.com");
          client.println("Connection: close");
          client.println();
          message_sent = true;
        } else {
          // if you didn't get a connection to the server:
          Serial.println("connection failed");
        }
      }
    } else {
      if (val == 0) {
        // falling edge detected
        if (client.connect(server, 80)) {
          // Make a HTTP request:
          client.println("GET /trigger/trigger_on/with/key/<your key> HTTP/1.1");
          client.println("Host: maker.ifttt.com");
          client.println("Connection: close");
          client.println();
          message_sent = true;
        } else {
          // if you didn't get a connection to the server:
          Serial.println("connection failed");
        }
      }
    }
    
    timer = millis() + 500;
  }
}
