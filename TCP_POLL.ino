void checkForMessage() {
  
  char tcpMessage[100] = "";
sprintf(tcpMessage,"GET /monitor?message=%d.%d.%d.%d HTTP/1.1",WiFi.localIP()[0],WiFi.localIP()[1],WiFi.localIP()[2],WiFi.localIP()[3]);
  WiFiClient client;
  if (client.connect(config.udpTargetIP, 80)) {
    Serial.println("Connected to Server...");
    client.println(tcpMessage);
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println();
    //let's wait for something to come back... only a few seconds
    unsigned long startTimePush = millis();
    while (millis() - startTimePush < 5000) {
      if (client.available()) {
        //Serial.println("From Server");
        break;
      }
    }

    //took from pushsafer.cpp https://github.com/witnessmenow/pushsafer-arduino-library
    char c, body[200] = "", headers[200] = "";
    startTimePush = millis();
    bool finishedHeaders = false;
    bool currentLineIsBlank = true;
    bool responseReceived = false;
    while (millis() - startTimePush < 5000) {
      while (client.available()) {
        char c = client.read();
        responseReceived = true;
        if (!finishedHeaders) {
          if (currentLineIsBlank && c == '\n') {
            finishedHeaders = true;
          }
          else {
            int hdrLen = strlen(headers);
            if (hdrLen < 199) {
              headers[hdrLen] = c;
              headers[hdrLen + 1] = NULL;
            }
          }
        } else {
          int bodyLen = strlen(body);
          if (bodyLen < 199) {
            body[bodyLen] = c;
            body[bodyLen + 1] = NULL;
          }
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
        delay(1);
      }
      client.stop();
      //Serial.println("");
      if (responseReceived) {// received a body, so parse out and check if message is new
        if (strcmp(oldPacketReceived, body) != 0) { //new packet, so find number and play!
          strcpy(oldPacketReceived, body);
          Serial.println("****** something new! ******");
          char trackChar[5] = "";
          if (body[0] == '#') {
            for (int i = 0; i < 5; i++) {
              if (body[i + 1] == ',') {
                trackChar[i] = NULL;
                break;
              }
              else
                trackChar[i] = body[i + 1];
            }
            trackToPlay = atoi(trackChar);
            Serial.printf("Playing track # %i\n", trackToPlay);
            playTheTrack();
          }
        }

        //Serial.println(headers);
        //Serial.println(body);
        return;
      }
      delay(10); //Feed watchdog
    }
  } else {
    client.stop();
    Serial.println("TCP connection failed");
  }
}
