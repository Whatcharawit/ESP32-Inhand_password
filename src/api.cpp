#include "api.h"

ESP_SSLClient ssl_client;
EthernetClient basic_client;

String login(String authen)
{
    JsonDocument doc;

    String web_response = "";

    // ignore server ssl certificate verification
    ssl_client.setInsecure();

    // Set the receive and transmit buffers size in bytes for memory allocation (512 to 16384).
    ssl_client.setBufferSizes(1024 /* rx */, 512 /* tx */);

    /** Call setDebugLevel(level) to set the debug
     * esp_ssl_debug_none = 0
     * esp_ssl_debug_error = 1
     * esp_ssl_debug_warn = 2
     * esp_ssl_debug_info = 3
     * esp_ssl_debug_dump = 4
     */
    ssl_client.setDebugLevel(1);

    // Assign the basic client
    // Due to the basic_client pointer is assigned, to avoid dangling pointer, basic_client should be existed
    // as long as it was used by ssl_client for transportation.
    ssl_client.setClient(&basic_client);

// Send Get ------------------------------------------------------
    Serial.println("---------------------------------");
    Serial.print("Connecting to server...");

    String payload = "{\"title\":\"hello\"}";

    if (ssl_client.connect("192.168.2.1", 443))
    {
        Serial.println(" ok");
        Serial.println("Send GET request...");
        ssl_client.print("POST /v1/user/login HTTP/1.1\r\n");
        ssl_client.print("Host: 192.168.2.1\r\n");
        ssl_client.print("Content-Type: application/json\r\n");
        ssl_client.print("Authorization: Basic " + authen + "\r\n");
        ssl_client.print("Content-Length: ");
        ssl_client.print(payload.length());
        ssl_client.print("\r\n\r\n");
        ssl_client.print(payload);

        Serial.print("Read response...");

        unsigned long ms = millis();
        while (ssl_client.connected() && millis() - ms < 2000)
        {
            if(ssl_client.available())
            {
                char response = ssl_client.read();
                // Serial.print(response);
                // strncat(jsonString, &response, 1);
                web_response += response;
            }
        }
        Serial.println();
        ssl_client.stop();
    }
        
    else
        Serial.println(" failed\n");
        ssl_client.stop();

    Serial.println("Client Stop");
    Serial.print("\nJson string: ");
    Serial.println(web_response);

    // Keep last line from response
    int jsonStart = web_response.lastIndexOf("\r\n\r\n") + 4; // Move to start of JSON
    String jsonString = web_response.substring(jsonStart);
    Serial.print("Result: ");
    Serial.println(jsonString);

    // Convert String to Json
    DeserializationError error = deserializeJson(doc, jsonString);
    if(error)
    {
        Serial.print("Json parsing failed: ");
        Serial.println(error.c_str());
        // return();
    }

    // Extract values
    JsonObject results = doc["results"];
    const char* web_session = results["web_session"];

    // Serial.print("Web session: ");
    // Serial.println(web_session);
    return(web_session);
}

String ChangePasswd(String web_session, String current_passwd, String new_passwd)
{
    JsonDocument doc;

    String web_response = "";

    // ignore server ssl certificate verification
    ssl_client.setInsecure();

    // Set the receive and transmit buffers size in bytes for memory allocation (512 to 16384).
    ssl_client.setBufferSizes(1024 /* rx */, 512 /* tx */);

    /** Call setDebugLevel(level) to set the debug
     * esp_ssl_debug_none = 0
     * esp_ssl_debug_error = 1
     * esp_ssl_debug_warn = 2
     * esp_ssl_debug_info = 3
     * esp_ssl_debug_dump = 4
     */
    ssl_client.setDebugLevel(1);

    // Assign the basic client
    // Due to the basic_client pointer is assigned, to avoid dangling pointer, basic_client should be existed
    // as long as it was used by ssl_client for transportation.
    ssl_client.setClient(&basic_client);

    // Send Get ------------------------------------------------------
    Serial.println("---------------------------------");
    Serial.print("Connecting to server...");

    String payload = "{"
        "\"privilege\": 15,"
        "\"password\": \"" + new_passwd + "\","
        "\"username\": \"adm\","
        "\"current_user\": \"adm\","
        "\"changed_user\": \"adm\","
        "\"current_passwd\": \"" + current_passwd + "\""
        "}";

    if (ssl_client.connect("192.168.2.1", 443))
    {
        Serial.println(" ok");
        Serial.println("Send GET request...");
        ssl_client.print("PUT /v1/admind/user?autosave=1 HTTP/1.1\r\n");
        ssl_client.print("Host: 192.168.2.1\r\n");
        ssl_client.print("Content-Type: application/json\r\n");
        ssl_client.print("Authorization: Bearer " + web_session + "\r\n");
        ssl_client.print("Content-Length: ");
        ssl_client.print(payload.length());
        ssl_client.print("\r\n\r\n");
        ssl_client.print(payload);

        Serial.print("Read response...");

        unsigned long ms = millis();
        while (ssl_client.connected() && millis() - ms < 2000)
        {
            if(ssl_client.available())
            {
                char response = ssl_client.read();
                // Serial.print(response);
                // strncat(jsonString, &response, 1);
                web_response += response;
            }
        }
        Serial.println();
        ssl_client.stop();
    }
        
    else
        Serial.println(" failed\n");
        ssl_client.stop();

    Serial.println("Client Stop");
    Serial.print("\nJson string: ");
    Serial.println(web_response);

    // Keep last line from response
    int jsonStart = web_response.lastIndexOf("\r\n\r\n") + 4; // Move to start of JSON
    String jsonString = web_response.substring(jsonStart);
    Serial.print("Result: ");
    Serial.println(jsonString);

    // Convert String to Json
    DeserializationError error = deserializeJson(doc, jsonString);
    if(error)
    {
        Serial.print("Json parsing failed: ");
        Serial.println(error.c_str());
        // return();
    }

    // Extract values
    JsonObject results = doc["results"][0];
    const char* username = results["username"];

    // Serial.print("username: ");
    // Serial.println(username);

    delay(500);
    return username;
}