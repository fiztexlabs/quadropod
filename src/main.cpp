#include <Arduino.h>
#include <web_interface.h>

robo::WebInterface ui = robo::WebInterface(
    "SheldonWiFi_2.4GHz",
    "Supernova2021",
    1,
    80,
    &Serial);

void setup()
{
  Serial.begin(115200);
  ui.begin();
}

void loop()
{
  ui.handle();
}