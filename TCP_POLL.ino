void checkForMessage() {

  char tcpMessage[100] = "";
  sprintf(tcpMessage, "GET /monitor?message=%d.%d.%d.%d HTTP/1.1", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  //  sprintf(tcpMessage,"GET /monitor?message=OK");


  WiFiClient client;
  if (client.connect(config.udpTargetIP, 80)) {
    Serial.println("Connected to Server...");
    client.println(tcpMessage);
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    //    client.println();
    client.println();
    //let's wait for something to come back... only a few seconds
    unsigned long startTimePush = millis();
    while (millis() - startTimePush < 10000) {
      if (client.available()) {
        Serial.println("From Server");
        delay(100);
        break;
      }
    }

    //took from pushsafer.cpp https://github.com/witnessmenow/pushsafer-arduino-library
    char c, body[200] = "", headers[200] = "";
    startTimePush = millis();
    bool finishedHeaders = false;
    bool currentLineIsBlank = true;
    bool responseReceived = false;
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
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
      // delay(1);
    }
    Serial.println("");
    //client.flush();
    client.stop();
    //Serial.println("");
    //Serial.println(body);
    if (responseReceived) {// received a body, so parse out and check if message is new
      if (strcmp(oldPacketReceived, body) != 0) { //new packet, so find number and play!
        strcpy(oldPacketReceived, body);
        Serial.println("****** something new! ******");
        //Serial.println(body);
        if (strcmp("silence", body) == 0) {
          Serial.println("Silence Button");
          silenceButtonWasPressed = true;
        }

        char trackChar[5] = "";
        if (body[0] == '#') {
          if (silenceButtonWasPressed || firstBoot) {
            firstBoot = false;
            silenceButtonWasPressed = false;//only on first time back from silence button do we not play or on first boot
          } else {//else go ahead and play
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
      }

      //Serial.println(headers);
      //Serial.println(body);
      return;
    }
    //delay(10); //Feed watchdog

  } else {
    //client.flush();
    client.stop();
    Serial.println("TCP connection failed");
  }
}
