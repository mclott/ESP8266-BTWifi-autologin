/*
 *  Created by Martin Lott 2020
 *  
 *  Based on the HTTP over TLS (HTTPS) example sketch
 *  originally Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 *  
 *  For those who regularly use BTWifi / BTWifi-with-FON in the UK for internet access will know that it logs you out from 
 *  time to time.  This project simply pings the internet (google in this example) to check for internet access.  
 *  If the connection is lost then it sends a HTTPS request with the login details to BT.
 *  It makes use of dancol90's ESP8266ping Library https://github.com/dancol90/ESP8266Ping 
 *  Just enter the wifi you want to connect and your BT internet credentials.  
 *  
 *  I have setup a wifi bridge in Router mode that connects to BTWifi then feeds into my network via NAT giving all 
 *  my devices internet access while the BTWifi access point only sees one device.  Currently using a NanoStation loco M2.
 *  The downside is the BTWifi connects without encryption, so a VPN service would not be a bad idea!
 *  
 *  Works great for me...
 *  Enjoy. 
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266Ping.h>

const char* ssid = "";    
const char* password = "";

const char* BTemail = "";   // Your BT internet login.  ie user@btconnect.com
const char* BTpassword = "";
const char* host = "www.btopenzone.com";  
const int   httpsPort = 8443;

const char* ping_host = "www.google.com";  // ping host
const int   PingDelay = 10000;  // How often the host is pinged in ms.

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void logon_to_BT(void) {

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/ante?username=";
         url = url + BTemail;
         url = url + "&password=";
         url = url + BTpassword;

  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
}

void loop() {
  Serial.print("Pinging host ");
  Serial.println(ping_host);

  if(Ping.ping(ping_host)) {
    
    Serial.print("Success!!  Ping Time: ");
    Serial.print( Ping.averageTime());
    Serial.println("ms.");
  } else {
    Serial.println("Error :(");
    delay(3000);
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Wifi not connected");
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
      }
      Serial.println("Wifi Connected!");
    }
    Serial.print("Re-Pinging host ");
    Serial.println(ping_host);

     if(!Ping.ping(ping_host)) {
      logon_to_BT();
      }
      else {
        Serial.print("Success!!  Ping Time: ");
        Serial.print( Ping.averageTime());
        Serial.println("ms.");
      }
  }
  delay(PingDelay);
}
