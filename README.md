﻿﻿﻿﻿﻿
# 12V Trigger Email 

When 12V signal goes high, send an Email.
When 12V signal goes low, send another Email.
Include sending an Email on boot for validation of Email service.


# Bill Of Materials
- Arduino Nano microcontroller - Mnu Nano v3 - CH340 - with loose headers from M Punkt Nu (Art.nr: Mnu\_nano\_comp\_LH)
- Ethernet Breakout Board - ENC28J60 (Art.nr: ENC28J60)
- Jumper wires - Adafruit Breadboarding Premium Female/Male Extension Jumper Wires - 40 x 12 (Art.nr: 824\_f-m\_300)
- Breadboard Half-size Solderless (Art.nr: half\_size\_bb)
- Breadboard Jumper Wire Set (Art.nr: CAB01200O)
- USB 2.0 cabel Type A Male - Type Mini B Male 1m, svart (Art.nr: USB-24S) 
- DIL Relay 12V DC 0.5A / 30V
- 5V USB Power Supply
- Ethernet cable


# Dependencies
- An Email account
- An IFTTT account
- A router with Internet access and a free Ethernet port


# Connections


## Mnu Nano v3 -> Ethernet Breakout Board (using Female/Male Jumper Wires)
- D10 -> CS 
- D11 -> SI
- D12 -> SO 
- D13 -> SCK 
- RST -> RST 
- 5V  -> 5V 
- GND -> GND


## Mnu Nano v3 -> DIL Relay 12V
- D7             -> Pin 1
- 3.3V -> 68kOhm -> Pin 1
- GND            -> Pin 8


## DIL Relay 12V -> monitored 12V signal
- Pin 7  -> GND
- Pin 13 -> signal


# Software Design


## Components
- Setup
- Listener Loop
- Email Sender
- Positive Edge Event Handler
- Negative Edge Event Handler


## Control Flow


### Scenario Trigger On
1. Setup -> Email Sender --- "System is on"
2. Setup -> Listener Loop --- "Setup is complete"
3. Listener Loop --- "Record current state of 12V signal"
4. Listener Loop -> Positive Edge Event Handler --- "If low to high transition of state"
5. Positive Edge Event Handler -> Email Sender --- "Triggered on"
6. Listener Loop --- "Repeat"

### Scenario Trigger Off
1. Setup -> Email Sender --- "System is on"
2. Setup -> Listener Loop --- "Setup is complete"
3. Listener Loop --- "Record current state of 12V signal"
4. Listener Loop -> Negative Edge Event Handler --- "If high to low transition of state"
5. Negative Edge Event Handler -> Email Sender --- "Triggered off"
6. Listener Loop --- "Repeat"

# Usage and Deployment
1. Edit sketch to correspond to your Email address and your webhook trigger on IfTTT.
2. Connect GND to GND of the source of the 12V signal.
3. Connect the 12V signal to Pin 13 of the DIL Relay.
4. Connect the USB cable to your computer that has an ArduinoIDE.
5. Upload the sketch using the Arduino IDE.
6. Disconnect the USB cable from the computer.
7. Connect the USB cable to a reliable USB power supply.
8. Connect the Ethernet Breakout Board to the Ethernet router.
9. Press reset on the Mnu Nanu V3. Expect an Email.






