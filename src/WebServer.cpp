#include "WebServer.h"
#include "SPIFFS.h"

String deviceStatus = "ON";
// Wi-Fi credentials
const char *ssid = "ESP32-AP";
const char *password = "swd12345"; // Minimum 8 characters required

WiFiServer web_server(80);

void setupWiFiAP() {
    WiFi.softAP(ssid, password);
    Serial.println("Wi-Fi AP started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS");
        return;
    }
    
    web_server.begin();
}

void handleClient() {
    WiFiClient client = web_server.available();
    if (client) {
        Serial.println("New Client Connected");
        String request = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                request += c;
                if (c == '\n' && request.endsWith("\r\n\r\n")) {
                    break;
                }
            }
        }

        Serial.println("Received Request: " + request);  // Debugging request

        if (request.indexOf("GET /login?") >= 0) {
            // Extract username and password
            int userIndex = request.indexOf("user=") + 5;
            int passIndex = request.indexOf("&pass=") + 6;
            String username = request.substring(userIndex, request.indexOf("&pass="));
            String password = request.substring(passIndex, request.indexOf(" ", passIndex));

            // URL Decode (replace %20 with space, etc.)
            username.replace("%20", " ");
            password.replace("%20", " ");

            // Print to Serial Monitor
            Serial.println("Username: " + username);
            Serial.println("Password: " + password);

            // Send response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Connection: close");
            client.println();
            client.println("Login Received");
        } 

        // Serve status variable
        else if (request.indexOf("GET /status") >= 0) {
            Serial.println("Sending Status: " + deviceStatus);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Connection: close");
            client.println();
            client.println(deviceStatus);
        } 
        else if (request.indexOf("GET /on.html") >= 0) {  
            sendFile(client, "/on.html", "text/html");
        }
        else if (request.indexOf("GET /style.css") >= 0) {
            sendFile(client, "/style.css", "text/css");
        } else {
            sendFile(client, "/index.html", "text/html");
        }

        client.stop();
        Serial.println("Client Disconnected");
    }
}

void sendFile(WiFiClient &client, const char *filename, const char *contentType) {
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        client.println("HTTP/1.1 404 Not Found");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.println("File Not Found");
        return;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: " + String(contentType));
    client.println("Connection: close");
    client.println();

    while (file.available()) {
        client.write(file.read());
    }
    file.close();
}