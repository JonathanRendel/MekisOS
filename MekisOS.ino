/*
MekisOS
by Jonathan Rendel

The program reads the setpoint value from the encoder connected to pin 2 and 3.
It calculates the position error based on the position feedback from the pot connected to A0.
If the error is below the negative threshold, the motor will turn right and if its above the positve threshold it will turn left.
If the error is between the negative and positive threshold, the motor will stop.
If the position feedback reaches the soft limit or if the limit switches are activated, then the motor will stop.
*/

#include <Encoder.h>

#define motor_pin 9
#define motor_dir 8

#define pot_pin A0

#define encA 2
#define encB 3

#define limit_left 6
#define limit_right 5

#define soft_upper_lim 575
#define soft_lower_lim 445

#define motor_cutoff 5
#define st 10

long m = 500;
long prevT = 0;


Encoder enc(encA, encB);

void setup() {
  pinMode(motor_pin, OUTPUT);
  pinMode(motor_dir, OUTPUT);
  pinMode(pot_pin, INPUT);

  pinMode(limit_left, INPUT_PULLUP);
  pinMode(limit_right, INPUT_PULLUP);
  //Serial.begin(9600);

}

void loop() {
  m = long((double(-enc.read())/3.0)+519.0); //Calculate setpoint value
  if (m < 100) m = 100;
  if (m > 900) m = 900;
  setMotorPos(m);
}

void setMotorPos(int pos) {
  if (millis() > prevT + st) { //Check sample time
    prevT = millis();
    int error = pos-analogRead(pot_pin); //calculate error

    if (error <= -motor_cutoff) { //determine whether to turn right, left or stop
      turnRight();
    } else if (error >= motor_cutoff) {
      turnLeft();
    } else {
      stopMotor();
    }
  }
}

void turnLeft() {
  if (digitalRead(limit_right) == HIGH || analogRead(pot_pin) >= soft_upper_lim) { //Stop motor if hard or soft limit is reached
    stopMotor();
  } else {
    digitalWrite(motor_dir, HIGH);
    digitalWrite(motor_pin, HIGH);
  }
}

void turnRight() {
  if (digitalRead(limit_right) == HIGH || analogRead(pot_pin) >= soft_upper_lim) { //Stop motor if hard or soft limit is reached
    stopMotor();
  } else {
    digitalWrite(motor_dir, LOW);
    digitalWrite(motor_pin, HIGH);
  }
}

void stopMotor() {
  digitalWrite(motor_pin, LOW);
}
