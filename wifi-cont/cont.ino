#include <M5StickC.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t slaveAddress[] = { 0x24, 0x0A, 0xC4, 0xF2, 0x6C, 0xFC }; // 受信機のMACアドレスに書き換えます

void onSend(const uint8_t* mac_addr, esp_now_send_status_t status) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(5, 10);
    M5.Lcd.println(macStr);
    M5.Lcd.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}


void setup() {
    M5.begin();
    M5.Lcd.fillScreen(BLACK);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
        Serial.println("ESPNow Init Success");
    }

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, slaveAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    esp_now_register_send_cb(onSend);
}

void loop() {
    M5.update();

    if (M5.BtnA.wasPressed()) {
        uint8_t data[2] = { 111, 222 }; // 送信データ
        esp_err_t result = esp_now_send(slaveAddress, data, sizeof(data));

        Serial.print("Send Status: ");
        switch (result)
        {
        case ESP_OK:
            Serial.println("Success");
            break;
        case ESP_ERR_ESPNOW_NOT_INIT:
            Serial.println("ESPNOW not Init.");
            break;
        case ESP_ERR_ESPNOW_ARG:
            Serial.println("Invalid Argument");
            break;
        case ESP_ERR_ESPNOW_INTERNAL:
            Serial.println("Internal Error");
            break;
        case ESP_ERR_ESPNOW_NO_MEM:
            Serial.println("ESP_ERR_ESPNOW_NO_MEM");
            break;
        case ESP_ERR_ESPNOW_NOT_FOUND:
            Serial.println("Peer not found.");
            break;

        default:
            Serial.println("Not sure what happened");
            break;
        }

    }
    delay(1);
}