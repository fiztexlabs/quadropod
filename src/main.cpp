#include <Arduino.h>
#include <servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <libmath/matrix.h>
#include <limb.h>
#include <quadropod.h>
#include <web_interface.h>
#include <vector>
#include <defines.h>

robo::WebInterface ui = robo::WebInterface(
    "SheldonWiFi_2.4GHz",
    "Supernova2021"
  );

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

robo::Quadropod robot1 = robo::Quadropod(
  {
    new robo::Limb(
      {
        {37.e-3},
        {58.e-3},
        {79.e-3}
      },
      {0, 1, 2},
      &pwm,
      {
        {0.0},
        {270.0},
        {180.0}
      },
      "leg1"
    ),
    new robo::Limb(
      {
        {37.e-3},
        {58.e-3},
        {79.e-3}
      },
      {4, 5, 6},
      &pwm,
      {
        {0.0},
        {270.0},
        {180.0}
      },
      "leg2"
    ),
    new robo::Limb(
      {
        {37.e-3},
        {58.e-3},
        {79.e-3}
      },
      {8, 9, 10},
      &pwm,
      {
        {0.0},
        {270.0},
        {180.0}
      },
      "leg3"
    ),
    new robo::Limb(
      {
        {37.e-3},
        {58.e-3},
        {79.e-3}
      },
      {11, 12, 13},
      &pwm,
      {
        {0.0},
        {270.0},
        {180.0}
      },
      "leg4"
    )
  },
  {
    {-0.04, -0.04,  0.04,  0.04 },
    {-0.04, 0.04,   0.04,  -0.04},
    {0.03,  0.03,   0.03,  0.03 }
  }
);

// robo::Limb leg1 = robo::Limb(
//     {
//       {37.e-3},
//       {58.e-3},
//       {79.e-3}
//     },
//     {6, 5, 4},
//     &pwm,
//     {
//       {0.0},
//       {270.0},
//       {180.0}
//     },
//     "leg1"
// );

std::vector<robo::real> angles;

void setup()
{
  Serial.begin(115200);
  Wire.begin(4, 5);
  robot1.begin();
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