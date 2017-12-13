#include "main.h"


void initializeIO() {
}
void initialize() {
  leftD = encoderInit(leftenctop, leftencbottom, true);
  rightD = encoderInit(rightenctop, rightencbottom, false);
  smallarmenc = encoderInit(smallarmtop, smallarmbottom, false);
  lcdInit(uart1);
  lcdSetBacklight(uart1, true);
}
