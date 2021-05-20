#include "Main.h"
#include "telegram.h"
#include "WebServer.h"
#include "domoticz.h"
#include <esp_camera.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include "UniversalTelegramBot.h"  //https://github.com/jameszah/ESP32-Cam-Telegram

// Initialize Telegram Bot
WiFiClientSecure secured_client;
UniversalTelegramBot bot("", secured_client);

camera_fb_t *fb = NULL;
bool isMoreDataAvailable();
byte *getNextBuffer();
int getNextBufferLen();
bool dataAvailable = false;


bool isMoreDataAvailable() {
    if (dataAvailable) {
        dataAvailable = false;
        return true;
    } else {
        return false;
    }
}


byte *getNextBuffer() {
    if (fb) {
        return fb->buf;
    } else {
        return nullptr;
    }
}

int getNextBufferLen() {
    if (fb) {
        return fb->len;
    } else {
        return 0;
    }
}

// Update Bot function used to update the bottoken to value from setup
void Telegram_update_bottoken() {
    // update setInsecure() or else we get errors wit the standard Telegram library
    secured_client.setInsecure();
    bot.updateToken(TelegramBot);
}

void TelegramSendPicture() {
    if (!strcmp(TeleProtocol, "yes")) {
        AddLogMessageI(F("Sending Telegram messages\n"));
        SendTelegramCapture = true;
        bot.sendMessage(TelegramChat, TelegramCapt, "");
        LedToggle();
        Flash_timer = millis();
        Flash_done++;
        //delay(100);  // wait a little so any stream capture is finished
        fb = NULL;
        // set framezise temp to 6 (vga) for Telegram
        sensor_t *s = esp_camera_sensor_get();
        uint sFramesize = s->status.framesize;
        s->set_framesize(s, (framesize_t)6);
        // Take Picture with Camera
        fb = esp_camera_fb_get();
        if (!fb) {
            AddLogMessageW(F("Camera capture failed\n"));
            bot.sendMessage(TelegramChat, "Camera capture failed", "");
            return;
        }
        // Reset framesize
        s->set_framesize(s, (framesize_t)sFramesize);
        dataAvailable = true;
        Serial.println("done!");
        LedToggle();
        Flash_timer = millis();
        Flash_done++;
        bot.sendPhotoByBinary(TelegramChat, "image/jpeg", fb->len,
                              isMoreDataAvailable, nullptr,
                              getNextBuffer, getNextBufferLen);
        SendTelegramCapture = false;
    } else {
        AddLogMessageD(F("Send Telegram = \"no\", No telegram to send\n"));
    }
}
