#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "esp32_eoss3_spi.h"

#define GPIO_OUTPUT_VAL_REG 0x40021004
#define GPIO_OUTPUT_DIR_REG 0x40021008

#define PIN_LED1 18
#define PIN_LED2 21
#define PIN_LED3 22
#define PIN_LED4 23
#define PIN_LED5 24
#define PIN_LED6 25
#define PIN_LED7 26

AsyncWebServer server(80);

const char* ssid = "revi";
const char* password = "revi4922251";

const char* PARAM_STATE = "state";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Seven Segment Display Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  
  <style>
    body { font-family: Times New Roman, sans-serif; text-align: center;}
    h1 {font-size: 2.0rem; color: red;}
    form { margin-top: 20px;}
    label {font-size: 18px;}
    select {padding: 10px; font-size: 16px; border-radius: 5px; border: 1px solid #ccc; outline: none;}
    input[type="submit"] { margin-top: 20px; padding: 10px 20px; font-size: 18px; border: none; background-color: #007bff; color: #fff; border-radius: 5px; cursor: pointer;}
    #display { margin-top: 20px; font-size: 24px;}
  </style>
  
</head>

<body>
  <h1>Seven Segment Display Controller</h1>
  <form action="/sevenseg" method="POST">
    <label for="digit">Select a digit:</label>
    <select id="digit" name="state">
      <option value="0">0</option>
      <option value="1">1</option>
      <option value="2">2</option>
      <option value="3">3</option>
      <option value="4">4</option>
      <option value="5">5</option>
      <option value="6">6</option>
      <option value="7">7</option>
      <option value="8">8</option>
      <option value="9">9</option>
    </select>
    <br>
    <input type="submit" value="Submit">
  </form>
  <div id="display">Selected digit will appear on the seven-segment display</div>
</body>

</html>)rawliteral";





void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  esp32_eoss3_spi_init();
  uint32_t dirval = (1 << PIN_LED1) | (1 << PIN_LED2) | (1 << PIN_LED3) | (1 << PIN_LED4) | (1 << PIN_LED5) | (1 << PIN_LED6) | (1 << PIN_LED7);
  uint32_t gpioval = 0;
  esp32_eoss3_spi_ahb_write(GPIO_OUTPUT_DIR_REG, (uint8_t *)&dirval, 4);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(2000);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/sevenseg", HTTP_GET, [&gpioval](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/sevenseg", HTTP_POST, [&gpioval](AsyncWebServerRequest *request) {
    int params = request->params();
    gpioval = 0; // Reset the GPIO value to turn off all LEDs
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()){
        if (p->name() == PARAM_STATE) {
          uint8_t option = p->value().toInt();
          switch(option) {
           
            case 0:
               // Turn on all led's except 7th led
            gpioval = (1 << PIN_LED7);
              break;
            case 1:
              // Turn off led's 1, 4, 5, 6, 7 and turn on other led's
              gpioval = ((1 << PIN_LED1) | (1 << PIN_LED4) | (1 << PIN_LED5) | (1 << PIN_LED6) | (1 << PIN_LED7));
              break;
            case 2:
              // Except 3 & 6 ,all led's are on   
              gpioval = (1 << PIN_LED3) | (1 << PIN_LED6);
              break;
            case 3:
              //Except 5 & 6 ,all led's are on 
             gpioval = (1 << PIN_LED5) | (1 << PIN_LED6);
              break;
            case 4:
              // Turn off led's 1, 4, 5 and turn on other led's
             gpioval = (1 << PIN_LED1) | (1 << PIN_LED4) | (1 << PIN_LED5);
              break;
            case 5:
              //Except 2 & 5 ,all led's are on
            gpioval = (1 << PIN_LED2) | (1 << PIN_LED5);
              break;
            case 6:
              // Turn on all led's except 2th led
            gpioval = (1 << PIN_LED2);
              break;
            case 7:
               //Except 4, 5, 6, 7 ,all led's are on
            gpioval = (1 << PIN_LED4) | (1 << PIN_LED5) | (1 << PIN_LED6) | (1 << PIN_LED7);
              break;
            case 8: 
               // All led's are on
            gpioval = 0;
              break;
            case 9:
              //Except 4 & 5 ,all led's are on
            gpioval = (1 << PIN_LED4) | (1 << PIN_LED5);
              break;
            default:
              break;
          }
        }
      }
    }
    // Write the GPIO value to turn on/off LEDs
    esp32_eoss3_spi_ahb_write(GPIO_OUTPUT_VAL_REG, (uint8_t *)&gpioval, 4);
    request->send(200, "text/html", index_html);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {}

