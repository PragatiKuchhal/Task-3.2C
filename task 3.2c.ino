#include <WiFiNINA.h>

#include <BH1750.h>
#include <Wire.h>

//please enter your sensitive data in the Secret tab
char ssid[] = "Redmi Note 10 Pro Max";
char pass[] = "123456789";

WiFiClient client;
BH1750 lightMeter;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME  = "https://maker.ifttt.com/trigger/lightsensor/with/key/djmTxByajgaXmwohDns7rvPK_Nsk553vDqSaO1saWzK";
String queryString = "?value1=57&value2=25";

void setup() {
  // initialize WiFi connection
  // WiFi.begin(ssid, pass);
  Serial.begin(9600);
Serial.print("START");
  while (!Serial);

  // connect to web server on port 80:
     WiFi.begin(ssid, pass);
     Wire.begin();
while(true)
  {
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
    break;
  }
  else {// if not connected:
    Serial.println("connection failed");
    
  }
  delay(500);
}
lightMeter.begin();
Serial.println("Connected to server");
}
void loop() {
  Serial.print("START");
  float lux = lightMeter.readLightLevel();

  // Update the queryString with the current lux value and a descriptive message
  if (lux > 1000) {
    queryString = "?value1=" + String(lux) + "&value2=It's very bright!";
  } else if (lux > 500) {
    queryString = "?value1=" + String(lux) + "&value2=It's quite bright.";
  } else if (lux > 100) {
    queryString = "?value1=" + String(lux) + "&value2=It's somewhat dim.";
  } else if (lux > 10) {
    queryString = "?value1=" + String(lux) + "&value2=It's quite dark.";
  } else {
    queryString = "?value1=" + String(lux) + "&value2=It's very dark!";
  }

  Serial.println(queryString);

  if (lux > 500) {
    // make an HTTP request:
    // send HTTP header
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to the serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("Disconnected");
  }
  
  queryString = "";
  delay(3000);
}
