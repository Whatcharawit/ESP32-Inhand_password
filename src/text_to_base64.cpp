#include "text_to_base64.h"

String text_to_base64(const char* text)
{
    Serial.print("Text: ");
    Serial.println(text);

    size_t inputLen = strlen(text);
    size_t outputLen = 0;

    // Calculate required buffer size
    mbedtls_base64_encode(NULL, 0, &outputLen, (const unsigned char*)text, inputLen);
    unsigned char encoded[outputLen];

    // Perform base64 encoding
    if (mbedtls_base64_encode(encoded, sizeof(encoded), &outputLen, (const unsigned char*)text, inputLen) == 0) {
        // Serial.print("Base64 Encoded: ");
        // Serial.println((char*)encoded);
        return((char*)encoded);
    } else {
        Serial.println("Base64 Encoding Failed!");
    }
}