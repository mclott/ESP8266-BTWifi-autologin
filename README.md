# ESP8266-BTWifi-autologin
ESP8266 Autologin for BTWifi in the UK
Created by Martin Lott 2020
 
Based on the HTTP over TLS (HTTPS) example sketch
originally Created by Ivan Grokhotkov, 2015.
This example is in public domain.
  
For those who regularly use BTWifi / BTWifi-with-FON in the UK for internet access will know that it logs you out from 
time to time.  This project simply pings the internet (google in this example) to check for internet access.  
If the connection is lost then it sends a HTTPS request with the login details to BT.
It makes use of dancol90's ESP8266ping Library https://github.com/dancol90/ESP8266Ping 
Just enter the wifi you want to connect and your BT internet credentials.  
  
I have setup a wifi bridge in Router mode that connects to BTWifi then feeds into my network via NAT giving all 
my devices internet access while the BTWifi access point only sees one device.  Currently using a NanoStation loco M2.
The downside is the BTWifi connects without encryption, so a VPN service would not be a bad idea!
 
Works great for me...
Enjoy. 
