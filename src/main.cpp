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
        {90.0},
        {0.0}
      },
      "leg0"
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
        {180.0},
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
      {8, 9, 10},
      &pwm,
      {
        {180.0},
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
      {12, 13, 14},
      &pwm,
      {
        {0.0},
        {90.0},
        {0.0}
      },
      "leg3"
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
//     {10, 9, 8},
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
  ui.begin();

  Wire.begin(5, 4);
  robot1.begin();

  // pwm.begin();
  // leg1.begin();

  // leg1.calcServoPos(
  //   {
  //     {0.174},
  //     {0.0},
  //     {0.0}
  //   }
  // );
}

void loop()
{
  ui.handle();

  ui.getPlatformAngles(angles);

  robot1.getLeg(0)->move();
  robot1.getLeg(1)->move();
  robot1.getLeg(2)->move();
  robot1.getLeg(3)->move();
  
  // robot1.getLeg(1)->calcServoPos(
  //   {
  //     {angles.at(0)},
  //     {angles.at(1)},
  //     {angles.at(2)}
  //   }
  // );

  
  // robot1.getLeg(0)->calcServoPos(
  //   {
  //     {0.0},
  //     {116.e-3},
  //     {58.e-3}
  //   }
  // );
  // robot1.getLeg(1)->calcServoPos(
  //   {
  //     {0.0},
  //     {116.e-3},
  //     {58.e-3}
  //   }
  // );
  // robot1.getLeg(2)->calcServoPos(
  //   {
  //     {0.0},
  //     {116.e-3},
  //     {58.e-3}
  //   }
  // );
  // robot1.getLeg(3)->calcServoPos(
  //   {
  //     {0.0},
  //     {116.e-3},
  //     {58.e-3}
  //   }
  // );

  
  robot1.getLeg(0)->calcServoPos(
    {
      {angles.at(0)},
      {angles.at(1)},
      {angles.at(2)}
    }
  );
  robot1.getLeg(1)->calcServoPos(
    {
      {angles.at(0)},
      {angles.at(1)},
      {angles.at(2)}
    }
  );
  robot1.getLeg(2)->calcServoPos(
    {
      {angles.at(0)},
      {angles.at(1)},
      {angles.at(2)}
    }
  );
  robot1.getLeg(3)->calcServoPos(
    {
      {angles.at(0)},
      {angles.at(1)},
      {angles.at(2)}
    }
  );

  // robot1.getLeg(0)->getServo(0)->setTargetPosition(angles.at(0));
  // robot1.getLeg(0)->getServo(1)->setTargetPosition(angles.at(1));
  // robot1.getLeg(0)->getServo(2)->setTargetPosition(angles.at(2));

  // robot1.getLeg(1)->getServo(0)->setTargetPosition(180.0-angles.at(0));
  // robot1.getLeg(1)->getServo(1)->setTargetPosition(180.0-angles.at(1));
  // robot1.getLeg(1)->getServo(2)->setTargetPosition(180.0-angles.at(2));

  // robot1.getLeg(2)->getServo(0)->setTargetPosition(180.0-angles.at(0));
  // robot1.getLeg(2)->getServo(1)->setTargetPosition(180.0-angles.at(1));
  // robot1.getLeg(2)->getServo(2)->setTargetPosition(180.0-angles.at(2));

  // robot1.getLeg(3)->getServo(0)->setTargetPosition(angles.at(0));
  // robot1.getLeg(3)->getServo(1)->setTargetPosition(angles.at(1));
  // robot1.getLeg(3)->getServo(2)->setTargetPosition(angles.at(2));

  // Serial.println(" ");
  // Serial.println("OX = "+String(angles.at(0))+" deg");
  // Serial.println("OY = "+String(angles.at(1))+" deg");
  // Serial.println("OZ = "+String(angles.at(2))+" deg");
}