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
      "leg0",
      {
        {0.03},
        {0.05},
        {-0.09}
      }
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
      "leg1",
      {
        {0.03},
        {0.05},
        {-0.09}
      }
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
      "leg2",
      {
        {0.03},
        {0.05},
        {-0.09}
      }
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
      "leg3",
      {
        {0.03},
        {0.05},
        {-0.09}
      }
    )
  },
  math::Matrix<float>{
    {0.04,   -0.04,   0.04,   -0.04},
    {0.04,    0.04,   -0.04,  -0.04},
    {0.0,     0.0,    0.0,    0.0  }
  },
  std::vector<float>{
    80.0*(M_PI / 180.0),
    -80.0*(M_PI / 180.0),
    170.0*(M_PI / 180.0),
    80.0*(M_PI / 180.0)
  },
  math::Matrix<float>{
    {1.0,  1.0,  1.0,  1.0 },
    {1.0,   1.0,  1.0,  1.0},
    {1.0,   1.0,  1.0,  1.0 }
  }
);

std::vector<robo::real> angles{0.0, 0.0, 0.0};

TaskHandle_t MOTION;
TaskHandle_t UI;

void loopMotion(void *pvParameters);
void loopUi(void *pvParameters);

void setup()
{
  Serial.begin(115200);
  ui.begin();

  Wire.begin(5, 4);
  robot1.begin();

  xTaskCreatePinnedToCore(
      loopMotion, /* Task function. */
      "MOTION",   /* name of task. */
      10000,      /* Stack size of task */
      NULL,       /* parameter of the task */
      1,          /* priority of the task */
      &MOTION,    /* Task handle to keep track of created task */
      0);         /* pin task to core 0 */
  delay(500);

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      loopUi,    /* Task function. */
      "UI",      /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &UI,       /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */
  delay(500);
}

void loopMotion(void *pvParameters)
{
  for (;;)
  {

    robot1.calcPositions(
        {{0.0},
         {0.0},
         {0.0}},
        {{angles.at(0)},
         {angles.at(1)},
         {angles.at(2)}});

    robot1.move();
  }
}

void loopUi(void *pvParameters)
{
  for (;;)
  {
    ui.handle();

    ui.getPlatformAngles(angles);
  }
}

void loop()
{
}