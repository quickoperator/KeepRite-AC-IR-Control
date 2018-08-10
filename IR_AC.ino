 /**
 *  ESP8266-12E / NodeMCU / WeMos D1 Mini WiFi & ENC28J60 Sample
 *  Source code can be found here: https://github.com/JZ-SmartThings/SmartThings/blob/master/Devices/Generic%20HTTP%20Device
 *  Copyright 2018 JZ
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 */

/////////////////////////////////////////
#include <DHT.h>      // Use library version 1.2.3 as 1.3.0 gives error
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const char* ssid = "MY WIFI NETWORK";
const char* password = "MY WIFI PASSWORD";

  #define DHTPIN D7     // what pin is the DHT on?
  
  // Uncomment whatever type of temperature sensor you're using!
  //#define DHTTYPE DHT11   // DHT 11
  #define DHTTYPE DHT22   // DHT 22  (AM2302)

  DHT dht(DHTPIN, DHTTYPE);

MDNSResponder mdns;

String currentIP, clientIP, request, fullrequest;
char *clientIPchar;

WiFiServer server(80);
ESP8266WebServer irserver(81);

IRsend irsend(4);

/////////////////////////////////////////
void handleRoot() {
 irserver.send(200, "text/html", "<html><head> <title>KeepRite AC Control</title></head><body><h1>KeepRite AC Control</h1><p><a href=\"ir?ac=on\">AC On</a></p><p><a href=\"ir?ac=off\">AC Off</a></p><p><a href=\"ir?ac=turbo\">Turbo</a></p><p><a href=\"ir?ac=aird\">Air Direction</a></p><p><a href=\"ir?ac=swing\">Swing</a></p></body></html>");
 }

void handleIr(){
  for (uint8_t i=0; i<irserver.args(); i++){
    if(irserver.argName(i) == "tv") 
    {
        uint16_t  tv[67] = {8850,4450, 600,600, 550,600, 600,1700, 600,600, 600,600, 600,600, 600,600, 550,1700, 600,600, 600,600, 600,600, 600,600, 600,600, 600,1700, 600,1700, 600,1700, 600,600, 600,600, 600,600, 550,600, 600,600, 600,1700, 600,600, 600,600, 600,1700, 600,1700, 600,1700, 600,1700, 600,1700, 600,600, 550,1700, 600,1700, 600};  // NEC 210704FB
        irsend.sendRaw(tv, 67, 38);
    }
    else if(irserver.argName(i) == "ac") 
    {
      if (irserver.arg(i) == "on") { // ON COOL a 20 deg
        uint16_t On[199] = {4348, 4352,  530, 1654,  504, 552,  530, 1658,  504, 1654,  504, 550,  530, 552,  530, 1654,  504, 554,  530, 552,  532, 1658,  504, 552,  532, 552,  530, 1654,  504, 1652,  504, 552,  530, 1660,  504, 1660,  504, 554,  528, 1658,  504, 1654,  504, 1654,  504, 1654,  504, 1654,  504, 1656,  504, 554,  530, 1654,  502, 552,  530, 552,  530, 554,  530, 550,  532, 552,  530, 556,  528, 554,  532, 550,  530, 1658,  502, 552,  530, 552,  530, 552,  530, 552,  530, 554,  530, 1656,  504, 1654,  504, 552,  530, 1658,  504, 1654,  504, 1654,  504, 1652,  504, 1654,  502, 5200,  4346, 4348,  530, 1654,  502, 550,  532, 1658,  504, 1652,  504, 552,  532, 550,  532, 1652,  504, 554,  530, 554,  530, 1658,  504, 550,  532, 552,  530, 1654,  504, 1654,  504, 552,  530, 1660,  504, 1660,  504, 552,  528, 1658,  504, 1652,  504, 1654,  502, 1654,  504, 1654,  502, 1656,  504, 554,  530, 1654,  504, 552,  530, 550,  532, 552,  530, 552,  532, 550,  530, 554,  530, 554,  530, 554,  528, 1658,  504, 552,  532, 550,  530, 552,  530, 552,  530, 554,  530, 1656,  504, 1654,  504, 552,  530, 1658,  502, 1654,  504, 1654,  504, 1652,  504, 1656,  502};
        irsend.sendRaw(On, 199, 38);
        irsend.sendRaw(On, 199, 38);
      }
      if (irserver.arg(i) == "off") { // OFF
        uint16_t Off[199] = {4346, 4352,  530, 1654,  504, 552,  528, 1658,  504, 1654,  504, 552,  530, 554,  528, 1652,  504, 556,  528, 554,  530, 1656,  504, 578,  502, 580,  504, 1652,  504, 1652,  504, 556,  528, 1658,  504, 556,  528, 1654,  504, 1652,  504, 1652,  504, 1654,  504, 578,  504, 1656,  504, 1656,  504, 1658,  504, 554,  528, 554,  528, 554,  528, 552,  530, 1652,  504, 554,  528, 580,  504, 1658,  504, 1654,  504, 1652,  504, 578,  502, 554,  528, 554,  526, 556,  526, 582,  502, 582,  502, 552,  530, 578,  504, 1656,  504, 1652,  504, 1652,  504, 1654,  504, 1654,  502, 5198,  4346, 4346,  530, 1652,  504, 580,  502, 1656,  504, 1652,  504, 580,  502, 580,  502, 1652,  504, 582,  502, 582,  502, 1656,  504, 554,  528, 578,  504, 1652,  504, 1652,  504, 554,  528, 1660,  504, 582,  502, 1652,  504, 1652,  504, 1652,  504, 1652,  504, 556,  526, 1658,  504, 1654,  504, 1658,  504, 578,  504, 554,  528, 578,  504, 556,  526, 1652,  504, 554,  528, 580,  504, 1658,  504, 1654,  504, 1652,  504, 554,  528, 580,  502, 580,  504, 578,  504, 554,  530, 556,  528, 554,  528, 580,  502, 1656,  504, 1652,  504, 1652,  504, 1654,  504, 1656,  502};
        irsend.sendRaw(Off, 199, 38);
        irsend.sendRaw(Off, 199, 38);
      }
      if (irserver.arg(i) == "turbo") { // TURBO
        uint16_t  turbo[199] = {4318, 4396,  526, 1654,  504, 554,  528, 1658,  504, 1654,  502, 554,  530, 1656,  504, 552,  530, 1660,  504, 554,  530, 1652,  504, 552,  530, 554,  528, 1654,  504, 552,  530, 1658,  502, 554,  530, 1656,  502, 1654,  504, 1652,  504, 1652,  504, 554,  528, 1656,  504, 554,  528, 1660,  504, 554,  530, 552,  528, 552,  530, 554,  530, 1658,  504, 552,  530, 1658,  504, 556,  528, 1656,  504, 554,  528, 1658,  504, 552,  530, 554,  528, 552,  530, 1658,  502, 558,  526, 554,  530, 1658,  504, 578,  502, 1658,  504, 1652,  504, 1652,  504, 552,  530, 1658,  502, 5176,  4346, 4350,  528, 1654,  504, 554,  530, 1656,  504, 1654,  504, 552,  532, 1656,  504, 554,  528, 1660,  504, 554,  528, 1654,  504, 552,  530, 552,  530, 1654,  502, 552,  530, 1658,  504, 556,  528, 1656,  502, 1654,  504, 1652,  504, 1652,  504, 552,  530, 1658,  504, 554,  528, 1660,  504, 554,  530, 552,  530, 552,  530, 556,  526, 1656,  504, 580,  502, 1658,  504, 556,  528, 1656,  504, 554,  528, 1658,  504, 580,  502, 552,  530, 552,  530, 1658,  504, 556,  528, 554,  530, 1658,  504, 552,  530, 1656,  504, 1652,  504, 1652,  504, 552,  530, 1660,  502};
        irsend.sendRaw(turbo, 199, 38);
        irsend.sendRaw(turbo, 199, 38);
      }
      if (irserver.arg(i) == "aird") { // Air direction
        uint16_t  aird[99] = {4320, 4360,  528, 1654,  504, 580,  502, 1658,  504, 1654,  504, 578,  504, 580,  502, 1654,  504, 580,  504, 580,  502, 1658,  504, 578,  504, 580,  502, 1654,  504, 1652,  504, 578,  502, 1660,  504, 580,  528, 528,  530, 578,  502, 580,  502, 1658,  530, 1628,  528, 1630,  530, 1630,  528, 1636,  530, 1628,  530, 1628,  504, 1652,  506, 578,  530, 554,  528, 554,  528, 556,  528, 1634,  528, 1630,  530, 1628,  530, 526,  556, 552,  530, 552,  528, 528,  554, 554,  530, 530,  554, 528,  554, 552,  530, 1632,  530, 1628,  530, 1628,  530, 1626,  530, 1630,  558};
        irsend.sendRaw(aird, 99, 38);
        irsend.sendRaw(aird, 99, 38);
      }
      if (irserver.arg(i) == "swing") { // Swing
        uint16_t  swing[199] = {4290, 4374,  528, 1652,  504, 580,  504, 1656,  504, 1652,  504, 578,  504, 578,  504, 1652,  504, 556,  528, 582,  504, 1656,  504, 580,  502, 580,  502, 1654,  504, 1652,  504, 578,  504, 1658,  504, 582,  502, 1654,  504, 1652,  504, 580,  504, 1656,  504, 578,  504, 1656,  504, 1654,  504, 1658,  504, 578,  502, 580,  502, 1654,  502, 580,  504, 1658,  504, 578,  504, 580,  502, 1658,  504, 1652,  504, 1652,  504, 578,  502, 580,  502, 578,  504, 578,  504, 580,  504, 556,  528, 578,  504, 578,  504, 1656,  504, 1652,  508, 1650,  502, 1654,  504, 1654,  534, 5158,  4346, 4350,  528, 1654,  504, 578,  504, 1656,  504, 1652,  504, 578,  504, 578,  504, 1652,  504, 580,  504, 580,  504, 1656,  504, 578,  504, 578,  504, 1652,  504, 1652,  504, 578,  504, 1658,  504, 582,  502, 1654,  504, 1652,  504, 580,  504, 1656,  504, 578,  504, 1656,  506, 1654,  504, 1660,  504, 578,  504, 580,  502, 1652,  504, 578,  504, 1656,  504, 578,  504, 580,  504, 1658,  504, 1654,  504, 1652,  504, 578,  504, 580,  502, 580,  504, 578,  504, 580,  504, 582,  502, 554,  528, 580,  502, 1658,  504, 1652,  504, 1652,  504, 1652,  504, 1656,  534};
        irsend.sendRaw(swing, 199, 38);
        irsend.sendRaw(swing, 199, 38);
      }
    }
  }
  handleRoot();
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += irserver.uri();
  message += "\nMethod: ";
  message += (irserver.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += irserver.args();
  message += "\n";
  for (uint8_t i=0; i<irserver.args(); i++){
    message += " " + irserver.argName(i) + ": " + irserver.arg(i) + "\n";
  }
  irserver.send(404, "text/plain", message);
}

/////////////////////////////////////////
void setup() {
  
  irsend.begin();
  
  Serial.begin(115200);

    dht.begin();

    // Connect to WiFi network
    Serial.println(); Serial.print("Connecting to "); Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    // Start the server
    server.begin();
    Serial.println("HTTP Server 80 started");
    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://"); Serial.print(WiFi.localIP()); Serial.println("/");
    currentIP = WiFi.localIP().toString();

  irserver.on("/", handleRoot);
  irserver.on("/ir", handleIr); 
  irserver.onNotFound(handleNotFound);
  
  irserver.begin();
  Serial.println("ir server 81 started");
  Serial.println("Setup finished...");
} // End setup

/////////////////////////////////////////
void loop() {
  
  irserver.handleClient();
  
  // SERIAL KEEP ALIVE MESSAGE
  // Check if HTTP client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.print("New client: ");
  Serial.println(client.remoteIP().toString());
  clientIP = " requested by: " + client.remoteIP().toString();

  while (!client.available()) {
    delay(1);
  }

  request = client.readStringUntil('\r');  // Read the first line of the request
  fullrequest = client.readString();
  Serial.println(request);
  request.replace("GET ", ""); request.replace(" HTTP/1.1", ""); request.replace(" HTTP/1.0", "");
  client.flush();

// HANDLE HTTP REQUEST
        Serial.println(request);
        Serial.println("Starting handleRequest...");
        delay(100); // pause to make sure pin status is updated for response below
          Serial.println("Before responding to client");
          client.println(clientResponse(0));
          Serial.println("After responding to client");

// END THE HTTP REQUEST

          delay(1);
          Serial.println("Client disconnected");
          Serial.println("");

} // End loop

/////////////////////////////////////////
String clientResponse(int section) { // HTTP clientResponse
  
  String clientResponse = "";
  if (section == 0) {
    Serial.print("Client section: "); Serial.print(section); Serial.print(" - Millis: "); Serial.println(millis());
    // BASIC AUTHENTICATION
    #ifdef useAuth
      // The below Base64 string is gate:gate1 for the username:password
      if (fullrequest.indexOf("Authorization: Basic Z2F0ZTpnYXRlMQ==") == -1) {
        clientResponse.concat("HTTP/1.1 401 Access Denied\n");
        clientResponse.concat("WWW-Authenticate: Basic realm=\"ESP8266\"\n");
        clientResponse.concat("Content-Type: text/html\n");
        clientResponse.concat("\n"); //  do not forget this one
        clientResponse.concat("Failed : Authentication Required!\n");
        return clientResponse;
      }
    #else
      clientResponse.concat("HTTP/1.1 200 OK\n");
      clientResponse.concat("Content-Type: text/html; charset=ISO-8859-1\n");
      clientResponse.concat("\n"); //  do not forget this one
    #endif
  
    clientResponse.concat("<!DOCTYPE HTML>\n");
    clientResponse.concat("<html><head><title>ESP8266");
    clientResponse.concat("</title></head><meta name=viewport content='width=500'>\n<style type='text/css'>\nbutton {line-height: 1.8em; margin: 5px; padding: 3px 7px;}");
    clientResponse.concat("\nbody {text-align:center;}\ndiv {border:solid 1px; margin: 3px; width:150px;}\n.center { margin: auto; width: 400px; border: 3px solid #73AD21; padding: 3px;}");
    clientResponse.concat("\nhr {width:400px;}\n</style><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n");
    clientResponse.concat("<script>\n");
    clientResponse.concat("$(document).ready(function(){\n");
    clientResponse.concat("  $('.hide_settings').click(function(){\n");
    clientResponse.concat("    $('.settings').hide(1000);\n");
    clientResponse.concat("    $('.show_settings').show(1000);\n");
    clientResponse.concat("  });\n");
    clientResponse.concat("  $('.show_settings').click(function(){\n");
    clientResponse.concat("    $('.settings').show(1000);\n");
    clientResponse.concat("    $('.show_settings').hide(1000);\n");
    clientResponse.concat("  });\n");
    clientResponse.concat("});\n");
    clientResponse.concat("function show_settings() {\n");
    clientResponse.concat("  if (/MSIE (\\d+\\.\\d+);/.test(navigator.userAgent) || navigator.userAgent.indexOf(\"Trident/\") > -1 ){ \n");
    clientResponse.concat("    document.getElementById('show_settings').style.display = 'none';\n");
    clientResponse.concat("    document.getElementById('settings').style.display = 'block';\n");
    clientResponse.concat("  }\n");
    clientResponse.concat("}\n");
    clientResponse.concat("function hide_settings() {\n");
    clientResponse.concat("  if (/MSIE (\\d+\\.\\d+);/.test(navigator.userAgent) || navigator.userAgent.indexOf(\"Trident/\") > -1 ){ \n");
    clientResponse.concat("    document.getElementById('show_settings').style.display = 'block';\n");
    clientResponse.concat("    document.getElementById('settings').style.display = 'none';\n");
    clientResponse.concat("  }\n");
    clientResponse.concat("}\n");
    clientResponse.concat("</script>\n");
    clientResponse.concat("</head>");
    clientResponse.concat("\n<h2 style=\"height: 15px; margin-top: 0px;\"><a href='/'>NodeMCU");
    clientResponse.concat("</h2><h3 style=\"height: 15px;\">");
    clientResponse.concat(currentIP);
    clientResponse.concat("</a>\n</h3>\n");
    clientResponse.concat("<i>Current Request:</i><br><b>\n");
    clientResponse.concat(request);
    clientResponse.concat("\n</b><hr>");
    clientResponse.concat("<pre>\n");
// SHOW & HANDLE DHT
      clientResponse.concat("<b><i>DHT");
      clientResponse.concat(DHTTYPE);
      clientResponse.concat(" Sensor Information:</i></b>\n");
      float h = processDHT(0);
      float tc = processDHT(1); float tf = (tc * 9.0 / 5.0) + 32.0;
      if (tc == -1000) {
        clientResponse.concat("<b><i>DHT Temperature Reading Failed</i></b>\n");
      } else {
        clientResponse.concat("Temperature="); clientResponse.concat(String(tc, 1)); clientResponse.concat((char)176); clientResponse.concat("C "); clientResponse.concat(round(tf)); clientResponse.concat((char)176); clientResponse.concat("F\n");
      }
      if (h == -1000) {
        clientResponse.concat("<b><i>DHT Humidity Reading Failed</i></b>\n");
      } else {
        clientResponse.concat("Humidity="); clientResponse.concat(round(h)); clientResponse.concat("%\n");
      }
    clientResponse.concat("</pre>\n"); clientResponse.concat("<hr>\n");
    
clientResponse.concat("</div></html>\n");
}
  
  return clientResponse;
  
}// End Client Response

/////////////////////////////////////////
float processDHT(bool whichSensor) {
  // Reading temperature or humidity takes about 250 milliseconds. Sensor readings may also be up to 2 seconds old

    float h = -1000;
    float tc = -1000;
    int counter = 1;
    if (whichSensor == 0) {
      h = dht.readHumidity();
      while (counter <= 5 && (isnan(h) || h == -1000)) {
        if (isnan(h) || h == -1000) { h = dht.readHumidity(); } // re-read
        counter += 1; delay(50);
      }
    }
    else if (whichSensor == 1) {
      tc = dht.readTemperature();
      while (counter <= 5 && (isnan(tc) || tc == -1000)) {
        if (isnan(tc) || tc == -1000) { tc = dht.readTemperature(); } // re-read
        counter += 1; delay(50);
      }
    }
    if (whichSensor == 0) {
      if (isnan(h) || h == -1000) { return -1000; }
        else { return h; }
    } else if (whichSensor == 1) {
      if (isnan(tc) || tc == -1000) { return -1000; }
        else { return tc; }
    }

}// End Process DHT

