#include "main.h"
int motor1[buffer];
int motor2[buffer];
int motor3[buffer];
int motor4[buffer];
int motor5[buffer];
int motor6[buffer];
int motor7[buffer];
int motor8[buffer];
int motor9[buffer];
int motor10[buffer];
void autonomous() {
  for(int i = 0; i < 1020; i++){
    motorSet(1, motor1[i]);
    motorSet(2, motor2[i]);
    motorSet(3, motor3[i]);
    motorSet(4, motor4[i]);
    motorSet(5, motor5[i]);
    motorSet(6, motor6[i]);
    motorSet(7, motor7[i]);
    motorSet(8, motor8[i]);
    motorSet(9, motor9[i]);
    motorSet(10, motor10[i]);}
  }
