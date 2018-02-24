#include <Adafruit_PWMServoDriver.h>
#include "application.h"

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// ~ 20deg to 140deg == 151 to 401
#define SERVOMIN  185//151 // 150 this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  401 // 470 this is the 'maximum' pulse length count (out of 4096)
#define OE_PIN D2
#define NUMSERVOS 8

//map(degrees, 0, 180, SERVOMIN, SERVOMAX);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

int lastPos = SERVOMIN;

void setup()
{
  pinMode(OE_PIN, OUTPUT);
  digitalWrite(OE_PIN, HIGH);
  pwm.begin();
  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates

  Particle.function("setexactpos", goToExactPos);
  delay(1500);
  goToPos(SERVOMAX);
}


void loop()
{
  goToPos(SERVOMIN);
  delay(2500);
  goToPos(SERVOMAX);
  delay(2500);
}

void goToDeg(int deg) {
  int rads = map(deg, 0,180,SERVOMIN, SERVOMAX);
  goToPos(rads);
}

int goToExactPos(String posValue) {
  int testValue = posValue.toInt();
  digitalWrite(OE_PIN, LOW);
  for(int servoNum = 0; servoNum < NUMSERVOS; servoNum++) {
    // Go from low to high
    // if (testValue > lastPos) {
    //   for (uint16_t pulselen = lastPos; pulselen < testValue; pulselen++){
    //     pwm.setPWM(servoNum, 0, pulselen);
    //   }
    // } else {
      // for (uint16_t pulselen = lastPos; pulselen > testValue; pulselen--){
        // pwm.setPWM(servoNum, 0, pulselen);
      // }
    // }
    pwm.setPWM(servoNum, 0, testValue);
    delay(100);
  }
  lastPos = testValue;
  delay(250);
  digitalWrite(OE_PIN, HIGH);

  return testValue;
}

void goToPos(int pos){
  digitalWrite(OE_PIN, LOW);
  for(int servoNum = 0; servoNum < NUMSERVOS; servoNum++) {
    // Go from low to high
    pwm.setPWM(servoNum, 0, pos);
    delay(150);
  }
  delay(250);
  digitalWrite(OE_PIN, HIGH);
}

void goToLowSmooth(){
  digitalWrite(OE_PIN, LOW);
  for (int servoNum = 0; servoNum<NUMSERVOS; servoNum++){
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
      pwm.setPWM(servoNum, 0, pulselen);
    }
  }
  delay(150);
  digitalWrite(OE_PIN, HIGH);
}
void goToHighSmooth(){
  digitalWrite(OE_PIN, LOW);
  for (int servoNum = 0; servoNum<NUMSERVOS; servoNum++){
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
      pwm.setPWM(servoNum, 0, pulselen);
    }
  }
  delay(150);
  digitalWrite(OE_PIN, HIGH);
}



