#include <esp_now.h>
#include <WiFi.h>

const int LED_PIN = 13;
bool ledState = false;

void toggleLed() {
    if (ledState) {
        ledState = false;
        digitalWrite(LED_PIN, LOW);
    }
    else {
        ledState = true;
        digitalWrite(LED_PIN, HIGH);
    }
}


void onReceive(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println();
    Serial.printf("Last Packet Recv from: %s\n", macStr);
    Serial.printf("Last Packet Recv Data(%d): ", data_len);
    for (int i = 0; i < data_len; i++) {
        Serial.print(data[i]);
        Serial.print(" ");
        if (data[i] == 222) {
            toggleLed();
        }
    }

}


void setup() {

    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Serial.println(WiFi.macAddress()); // このアドレスを送信側へ登録します

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
        Serial.println("ESP-Now Init Success");
    }
    esp_now_register_recv_cb(onReceive);
}

void loop() {
}