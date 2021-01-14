#include <M5Atom.h>
#include "Ambient.h"

uint8_t seq = 0; // remember number of boots in RTC Memory

WiFiClient client;
const char* ssid = "";  // Wi-FiのSSID
const char* password = "";  // Wi-Fiのパスワード

Ambient ambient;
unsigned int channelId = 00000; // AmbientのチャネルID
const char* writeKey = ""; // Ambientのライトキー

void setup() {
    M5.begin(true, false, true);
    M5.dis.setBrightness(10);  // >= 3
    M5.dis.drawpix(0, 0x000000);  // 初期LED: 黒
    Serial.begin(115200);
    ambient.begin(channelId, writeKey, &client); // チャネルIDとライトキーを指定してAmbientの初期化
}

void loop() {
    int lpcnt=0 ;
    int lpcnt2=0 ;

    Serial.print("\r\nWi-Fi scan start.\r\n");
    WiFi.begin(ssid, password);  //  Wi-Fi APに接続
    while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fi AP接続待ち
      delay(500);
      lpcnt += 1 ;                           //
      if (lpcnt > 6) {                       // 6回目(3秒) で切断/再接続
        WiFi.disconnect(true,true);          //
        WiFi.begin(ssid, password);    //
        lpcnt = 0 ;                          //
        lpcnt2 += 1 ;                        // 再接続の回数をカウント
      }                                      //
      if (lpcnt2 > 3) {                      // 3回 接続できなければ、
        ESP.restart() ;                      // ソフトウェアリセット
      }                  
      Serial.print(".");
    }

    M5.dis.drawpix(0, 0x00f000);  // Wi-Fi接続LED: 赤
    Serial.print("\nWiFi connected. \nIP address: ");
    Serial.println(WiFi.localIP());

    // 温度、湿度、照度、電源容量の値をAmbientに送信する
    seq++;  // カウンタを更新
    Serial.print("Seq: ");
    Serial.println(seq);
    ambient.set(1, 1);
    ambient.set(2, seq);
    ambient.send();
    Serial.print("ambient: data sent. \n");

    WiFi.disconnect(true,true);
    M5.dis.drawpix(0, 0xf00000);  // 初期LED: 緑
    Serial.print("WiFi disconnected. \n");

    delay(10000);  // 10秒毎に更新
}
