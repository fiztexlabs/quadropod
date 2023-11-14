#include <Arduino.h>
#include <servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <libmath/matrix.h>
#include <limb.h>
#include <web_interface.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// leg 1
robo::Limb leg1 = robo::Limb(
    {
      {37.e-3},
      {58.e-3},
      {79.e-3}
    },
    {6, 5, 4},
    &pwm,
    {
      {0.0},
      {270.0},
      {180.0}
    },
    "leg1"
);

// leg 1 coordinates
std::vector<int> coord1;

// create UI
robo::WebInterface web_ui(
  "SheldonWiFi_2.4GHz",
  "Supernova2021",
  3,
  Serial
);

void setup()
{
  Serial.begin(115200);

  Wire.begin(4, 5);

  pwm.begin();

  leg1.begin();

  web_ui.begin();
}

void loop()
{
  web_ui.listen();
  
  web_ui.getValues(coord1);

  // Move limb
  leg1.move();

  // Rotate the servo
  leg1.calcServoPos(
    {
      {coord1.at(0)},
      {coord1.at(1)},
      {coord1.at(2)}
    }
  );
}