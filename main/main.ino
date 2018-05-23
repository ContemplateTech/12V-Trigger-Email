// Send an Email on boot
// Borrowed Demo using DHCP and DNS to perform a web client request from <jc@wippler.nl> according to http://opensource.org/licenses/mit-license.php

// the input pin is intended to beconnected to a relay with nc

// the ENC28J60 Ethernet breakout board needs a particular library
#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[700];
const char website[] PROGMEM = "maker.ifttt.com";

const char your_code[] PROGMEM = "your code";

Stash stash;

// keeping track of input state
int val = 0;
int oldval = 0;
int inpin = 8;

// debug mode
int Debug_Mode = 1;  // 1 means that Debug_Mode is on, 0 is off

// timer is used to perform tasks at regular intervals
static uint32_t timer;


// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Ethernet::buffer[off+900] = 0;
  if (Debug_Mode == 1) {
    Serial.println(">>>");
    Serial.print((const char*) Ethernet::buffer + off);
    Serial.println("...");
  }
}

// send email for rising edge
static void Send_Email_Rising_Edge() {
  ether.browseUrl(PSTR("/trigger/trigger_on/with/key/"), your_code, website, my_callback);
}

// send email for falling edge
static void Send_Email_Falling_Edge() {
  ether.browseUrl(PSTR("/trigger/trigger_off/with/key/"), your_code, website, my_callback);
}

static void Send_Start_Up() {
  ether.browseUrl(PSTR("/trigger/start_up/with/key/"), your_code, website, my_callback);
}

void setup () {
  if (Debug_Mode == 1) {
    Serial.begin(57600);
    Serial.println(F("\n[webClient]"));
  }
  
  // define pin
  pinMode(inpin, INPUT);
  digitalWrite(inpin, HIGH); // turn on internal pull-up resistor

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) {
    if (Debug_Mode == 1) {
      Serial.println(F("Failed to access Ethernet controller"));
    }
  }
  if (!ether.dhcpSetup()) {
    if (Debug_Mode == 1) {
      Serial.println(F("DHCP failed"));      
    }
  }

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  // use DNS to resolve the website's IP address
  if (!ether.dnsLookup(website)) {
    if (Debug_Mode == 1) {
      Serial.println("DNS failed");
    }
  }
    
  ether.printIp("SRV: ", ether.hisip);

  // send Email at boot
  Send_Start_Up();
  
  val = digitalRead(inpin);
  oldval = val;
}

void loop () {
  ether.packetLoop(ether.packetReceive());
  if (millis() > timer) {
    oldval = val;
    val = digitalRead(inpin);
    if (oldval == 0) {
      if (val == 1) { // relay connected to nc
        // rising edge detected
        Send_Email_Rising_Edge();
      }
    } else {
      if (val == 0) {
        // falling edge detected
        Send_Email_Falling_Edge();
      }
    }
    
    timer = millis() + 5000;
    if (Debug_Mode == 1) {
      Serial.println();
      Serial.print("<<< REQ ");
    }
  }
}

