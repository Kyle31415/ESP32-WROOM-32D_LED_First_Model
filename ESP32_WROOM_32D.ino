#include <WiFi.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

const int LED_BUILTIN = 2;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to Wi-Fi");

  // Start the server
  server.begin();
  Serial.println("Server started");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available(); // Check if a client has connected

  if (client) { // If a client has connected
    Serial.println("New client connected");

    while (client.connected()) { // Loop while the client is connected
      if (client.available()) { // If there is data available from the client
        String request = client.readStringUntil('\r'); // Read the request
        Serial.println(request); // Print the request to the serial monitor

        if (request.indexOf("/on") != -1) { // If the request contains "/on"
          // Turn on LED
          digitalWrite(LED_BUILTIN, HIGH);
        } else if (request.indexOf("/off") != -1) { // If the request contains "/off"
          // Turn off LED
          digitalWrite(LED_BUILTIN, LOW);
        }

        // Send a simple HTML response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();
        client.println("<html><body>");
        client.println("<h1>ESP32 Blink Control</h1>");
        client.println("<p><a href=\"/on\">Turn On</a></p>");
        client.println("<p><a href=\"/off\">Turn Off</a></p>");
        client.println("</body></html>");
        client.stop(); // Close the connection
      }
    }
  }
}