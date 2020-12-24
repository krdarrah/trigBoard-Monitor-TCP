//ESP32 Monitor System from UDP
// I use the ESP32 Feather board from Adafruit
// amplifier is https://www.amazon.com/gp/product/B07CRVRG83/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1
// mp3 player https://www.amazon.com/gp/product/B0725RHR4D/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1


#include "monitorincludes.h"//special stuff needed just for this setup, mp3 player and pins
#include "includes.h"
const char fwVersion[] = "12/23/20 MONITOR TCP";

void setup() {
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, HIGH);
  pinMode(speakerPwrPin, OUTPUT);
  Serial.begin(115200);
  mp3.begin(9600);
  delay(500);
  loadConfiguration(filename, config);
  sendCommand(CMD_SEL_DEV, 0, DEV_TF);
  delay(200);
  if (mp3.available())
  {
    Serial.println(decodeMP3Answer());
    mp3.flush();
  }
  connectWiFi();
  initBluetooth();
  bluetoothTimeoutStart = millis();
}

void loop() {

  if ((millis() - bluetoothTimeoutStart) < (60 * 5 * 1000)) {//kill the bluetooth after 5minutes from boot
    if (!OTAsetup)
      serviceBluetooth();
    else
      checkOTA();
  } else {
    btStop();
    deviceConnected = false;
    digitalWrite(LEDpin, LOW);
        if (WiFi.status() != WL_CONNECTED) {
          Serial.println("WiFi Disconnected on its own");
          delay(100);
          ESP.restart();
        }
  }
  if ((millis() - tcpStartTime) > 1000  && WiFi.status() == WL_CONNECTED) {
    checkForMessage();
    tcpStartTime = millis();
  }
}
