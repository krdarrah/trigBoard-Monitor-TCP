void getFourNumbersForIP(const char *ipChar) {
  char ipCharInput[20];
  strncpy(ipCharInput, ipChar, sizeof(ipCharInput));

  const char delimiter[] = ".";
  char parsedStrings[4][4];
  char *token =  strtok(ipCharInput, delimiter);
  if (token == NULL)
    return;
  strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
  for (int i = 1; i < 4; i++) {
    token =  strtok(NULL, delimiter);
    if (token == NULL)
      return;
    strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
  }
  oneIP = atoi(parsedStrings[0]);
  twoIP = atoi(parsedStrings[1]);
  threeIP = atoi(parsedStrings[2]);
  fourIP = atoi(parsedStrings[3]);
}


//wifi event handler
boolean wifiConnected = false;
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      wifiConnected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      delay(1000);
//      if (!deviceConnected)//just in case you're changing wifi settings via ble
//        ESP.restart();
      break;
    default: break;
  }
}

bool connectWiFi() {
  Serial.print(F("connecting to WiFi - timeout="));
  Serial.println(config.wifiTimeout);
  if (strcmp(config.staticIPenable, "t") == 0) {//if static
    Serial.println("Setting up Static IP");
    getFourNumbersForIP(config.staticIP);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress ip(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.staticGatewayAddress);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress gateway(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.staticSubnetAddress);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress subnet(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.staticPrimaryDNSAddress);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress primaryDNS(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.staticSecondaryDNSAddress);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress secondaryDNS(oneIP, twoIP, threeIP, fourIP);
    WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);
  }
  Serial.printf("%s\n", config.ssid);

  wifiConnected = false;
  WiFi.onEvent(WiFiEvent);
  unsigned long wifiStart = millis();
  WiFi.begin(config.ssid, config.pw);
  while (!wifiConnected) {
    if (millis() - wifiStart > config.wifiTimeout) {
      // killPower();
      return false;
    }
  }

  Serial.println("Connected");
  return true;
}
