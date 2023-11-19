#include <Arduino.h>
#include <web_interface.h>
#include <vector>
#include <defines.h>

robo::WebInterface ui = robo::WebInterface(
    "SheldonWiFi_2.4GHz",
    "Supernova2021"
  );

std::vector<robo::real> angles;

void setup()
{
  Serial.begin(115200);
  ui.begin();
}

void loop()
{
  ui.handle();

  ui.getPlatformAngles(angles);

  Serial.println(" ");
  Serial.println("OX = "+String(angles.at(0))+" deg");
  Serial.println("OY = "+String(angles.at(1))+" deg");
  Serial.println("OZ = "+String(angles.at(2))+" deg");
}