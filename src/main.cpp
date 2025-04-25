#include "config.h"

void setup() {
    Serial.begin(115200);
    delay(500);

    // Change password -------------------------------------------
    // const char* Inhand_Password = "Abc123456";
    // String Inhand_Password = "Abc123456";
    String Inhand_Password = readFromEEPROM("password");
    Serial.print("Read from EEPROM: ");
    Serial.println(Inhand_Password);

    String new_password = "Abc12346";
    char login_authen[15];

    // Combind adm and password to adm:password
    snprintf(login_authen, sizeof(login_authen), "adm:%s", Inhand_Password);

    String login_base64 = text_to_base64(login_authen);
    Serial.print("Base64 Encoded: ");
    Serial.println(login_base64);

    int eth_state = EthernetConnect();
    Serial.print("Ethernet Status: ");
    Serial.println(eth_state);

    if(eth_state == 1)
    {
        String web_session = login(login_base64);
        Serial.print("Web Session: ");
        Serial.println(web_session);
        delay(5000);
        if(web_session.length() > 1)
        {
            String current_passwd_base64 = text_to_base64(Inhand_Password.c_str());
            String new_passwd_base64 = text_to_base64(new_password.c_str());
    
            Serial.print("current password: ");
            Serial.println(current_passwd_base64);
            Serial.print("new password: ");
            Serial.println(new_passwd_base64);
    
            String username_return = ChangePasswd(web_session, current_passwd_base64, new_passwd_base64);
            Serial.print("username return: ");
            Serial.println(username_return);
            delay(500);
    
            if(username_return.length() > 1)
            {
                saveToEEPROM("password", new_password);
                // String read_eeprom_inhand_password = readFromEEPROM("password");
                Serial.print("Save password successfull: ");
                Serial.println(new_password);
            }
        }
        else
        {
            Serial.println("\nLogin error");
        }
    }
        

    // setupWiFiAP();
}

void loop() 
{
    // handleClient();
}