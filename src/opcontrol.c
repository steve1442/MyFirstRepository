#include "main.h"

const int ground = 70, armthing[10] = {70,70,200,500,630,810,7,8,9,10};  // for 0 then make it either 49 or 75
const int cbposition[5] = {1310};
const int cbendpos = 100, cbstartpos = 0, cb_pwm = 127, arm_const = 127;
// leftback 1
// leftmiddle 2
// bigarm 8
// mogodp 4
// smallarm 7
// rightmiddle 9
// rightback 10
int leftback = 1, leftfront = 2, clawdp = 3, mogo = 4,
smallarm = 7, bigarm = 8, rightfront = 9, rightback = 10;

const int stop = 0, maxmenus = 25, bound = 100;
int stack = 0, menu = 0, lastV = 0, armspeed = 10, armheight = 70, cbhieght = 1000, cbspeed = 10, selectedauton = 0;
bool overrided = false;
                      //M1 M2 M3 M4 M5 M6 M7 M8 M9 M10
int motor[11] = {-999,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int inver[11] = {-999,  -1, 1, 1, 1, 0, 0,-1,-1,-1,-1};
//struct motor{char m1, m2, m3, m4, m5, m6, m7, m8, m9, m10;}motor; // structure to hold the motor pwm because its smaller
struct last{int arm, override, su, sd, co, cc, mo, mc; int lj;}last; // a structure for the last values
void lcd(const char *line1, const char *line2)
{
	lcdPrint(uart1, 1, line1);
	lcdPrint(uart1, 2, line2);
}
void lcdmenu(){
	int runtime = millis();
	int currV = joystickGetAnalog(1, 4);
	if(bound <= abs(currV) && bound > abs(lastV)){menu = (menu + maxmenus + currV/abs(currV)) % maxmenus;}
	lastV = currV;
	switch(menu){
		case  0: // Battery Level
    lcdPrint(uart1, 1, "   Main Bat %d", powerLevelMain());
    lcdPrint(uart1, 2, "Back-up Bat %d", powerLevelMain());
    break;
    case  1: // Auton Viewer, needs number and name
    lcdPrint(uart1, 1, "Running Auton %d", selectedauton);
    lcdPrint(uart1, 2, "Name of Auton");
    break;
    case  2: // Phone Number Meme
    lcdPrint(uart1, 1, "   r8 my code   ");
    lcdPrint(uart1, 2, "  954-849-1442  ");
    break;
    case  3: // Bot Timer, will use millis if the test on case 17 works
    lcdPrint(uart1, 1, "The bot has been");
    lcdPrint(uart1, 2, "running:  %dm%ds", runtime/60000, runtime%60000/1000);
    break;
    case  4: // Credits
    lcdPrint(uart1, 1, "Steve Coded This");
    lcdPrint(uart1, 2, "Kevin Coded This");
    break;
	  case  5    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "Motor Vals");break;
    case  6    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "S.B.S. Auton");break;
    case  7    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "something");break;
    case  8    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "something");break;
	  case  9: // Map 1
    lcdPrint(uart1, 1, "Ba|PA|P#|TS|Cre ");
    lcdPrint(uart1, 2, "M1|SA|??|??|Ma1 ");
    break;
	  case 10    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "Motor Vals 2");break;
  	case 11    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "RecAut Viewer");break;
    case 12    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "PID Viewer");break;
    case 13    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "something");break;
    case 14: // Map 2
    lcdPrint(uart1, 1, "M2|RA|PD|??|Ma2 ");
    lcdPrint(uart1, 2, "J1|S1|S3|??|Ma3 ");
    break;
    case 15: // Joystick 1, needs formatting for analog input
    lcdPrint(uart1, 1, "1%d 2%d5U%dD%d", joystickGetAnalog(1, 1), joystickGetAnalog(1, 2), joystickGetDigital(1,5,JOY_UP), joystickGetDigital(1,5,JOY_DOWN));
    lcdPrint(uart1, 2, "3%d 4%d6U%dD%d", joystickGetAnalog(1, 3), joystickGetAnalog(1, 4), joystickGetDigital(1,6,JOY_UP), joystickGetDigital(1,6,JOY_DOWN));
    break;
    case 16: // Sensors 1
    lcdPrint(uart1, 1, "leftdenc %d", encoderGet(leftD));
    lcdPrint(uart1, 2, "rightdenc %d", encoderGet(rightD));
    break;
	    // these don't currently fit anywhere, need to rewrite for space efficiency and move them to appropriate spaces
    case 17  :lcdPrint(uart1, 1, "sarmenc %d", encoderGet(smallarmenc));lcdPrint(uart1, 2, "run time ");break;
    case 18  :lcdPrint(uart1, 1, "Stack %d", stack); lcdPrint(uart1, 2, "Steve Didn't Code this");break;
    case 19: // Map 3
    lcdPrint(uart1, 1, "J2|S2|S4|??|Ma3 ");
    lcdPrint(uart1, 2, "??|??|??|??|??? ");
    break;
    case 20: // Joystick 2, missing accelerometer code & it wont fit on just one screen but i dont feel like making another :)
    lcdPrint(uart1, 1, "7U%dD%dL%dR%d Ax%d", joystickGetDigital(1,7,JOY_UP), joystickGetDigital(1,7,JOY_DOWN), joystickGetDigital(1,7,JOY_LEFT), joystickGetDigital(1,7,JOY_RIGHT), joystickGetAnalog(1, ACCEL_X));
    lcdPrint(uart1, 2, "8U%dD%dL%dR%d Ay%d", joystickGetDigital(1,8,JOY_UP), joystickGetDigital(1,8,JOY_DOWN), joystickGetDigital(1,8,JOY_LEFT), joystickGetDigital(1,8,JOY_RIGHT), joystickGetAnalog(1, ACCEL_Y));
    break;
    case 21    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "Sensr Vals 2");break;
    case 22    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "Sensr Vals 4");break;
    case 23    :lcdPrint(uart1, 1, "Placeholder for ");lcdPrint(uart1, 2, "something");break;
		case 24    :lcd("heyyyyyy","d000d"); break; // testing a lcd function to make this stuff smaller
//		case 25    :lcdPrint(uart1, 1, "");lcdPrint(uart1, 2, "something");break;
		default:
    lcdPrint(uart1, 1,"      Error     ");
    lcdPrint(uart1, 2, "    No Screen   ");
    break;
  }
}
void drive(int left, int right) // drive func
{
	motor[leftfront] = left * inver[leftfront];
	motor[leftback] = left * inver[leftback];
	motor[rightfront] = right * inver[rightfront];
	motor[rightback] = right * inver[rightback];
}
void arm(int pwm){motor[bigarm] = -pwm;} // simple arm func
void cb(int pwm){motor[smallarm] = -pwm;} // chainbar func
void MOGO(int pwm){motor[mogo] = pwm;} // mogo func
void claw(int pwm){motor[clawdp] = pwm;} // claw func
void update(){ // updates the motors and hopefully the digital ports soon
	motorSet(1, motor[1]); motorSet(2, motor[2]); motorSet(3, motor[3]); motorSet(4, motor[4]); motorSet(5, motor[5]);    /*Current error with my digital ports not working in the update*/
	motorSet(6, motor[6]); motorSet(7, motor[7]); motorSet(8, motor[8]); motorSet(9, motor[9]); motorSet(10, motor[10]);
	delay(20);}
int previous;
void pid(int Kp, int Kd, int val, int currentval)
{
	int error = val - currentval;
	int d = error - previous;
	int x = Kp * error + Kd * d;
	arm(x);
	previous = error;
}
	void suspendarm(int pwm, int value, int potentval)
{
	int nop = (value - potentval)/abs(value - potentval);
		if( (potentval > value && nop == -1) || ((potentval < value && nop == 1)) ){cb(pwm * nop);}}
	void armtask() // aww hell i gotta make a new pid now
{
	//suspendarm(127, 1000, analogRead(larmpot));
	pid(1,1, armheight, analogRead(barmpot));
}
	void potentarm(int pwm, int potent, int potentport){ // function to get the arm to go up or down based on a potentiometer input
  int potentval = analogRead(potentport);
	int nop = (potent - potentval)/abs(potent - potentval);  // hey kevin do these functions look ok?
	while((potentval > potent && nop == -1) || ((potentval < potent && nop == 1)) ){
    arm(pwm * nop);
    potentval = analogRead(potentport);
		lcdPrint(uart1, 1, "L Arm Pot %d", potentval);
		lcdPrint(uart1, 2, "Goal %d", potent);
		delay(5);
  }
  arm(stop); // pid but not really - stev
}
void potentcb(int pwm, int potent, int potentport){ // function to get the cb to go up or down based on a potentiometer input
  int potentval = analogRead(potentport);
	int nop = (potent - potentval)/abs(potent - potentval);
	while ( (potentval > potent && nop == -1) || ((potentval < potent && nop == 1)) )
	{cb(pwm * nop); potentval = analogRead(potentport);
	delay(5);} cb(stop);}
void scoredatcone(int x){ // the auto stacking code that joseph wants
	claw(copen);
	potentarm(arm_const, armthing[x], barmpot);
	// gonna have to redo this too
	//claw(cclose);
	//potentcb(cb_pwm, cbstartpos, analogRead(cbpot));
	//potentarm(arm_const, armthing[0], rarmpot);
}
void controller(){ // controller function for making it easier for recordable auton
	int LeftJoyStick = joystickGetAnalog(1, 2), RightJoyStick = joystickGetAnalog(1, 3);
	bool ClawOpen = joystickGetDigital(1, 5, JOY_UP), ClawClose = joystickGetDigital(1, 5, JOY_DOWN),
	armopen = joystickGetDigital(1, 6, JOY_UP), armclose = joystickGetDigital(1, 6, JOY_DOWN),
	mogoopen = joystickGetDigital(2, 8, JOY_UP), mogoclose = joystickGetDigital(2, 8, JOY_DOWN),
	stackup = joystickGetDigital(2, 6, JOY_UP), stackdown = joystickGetDigital(2, 6, JOY_DOWN),
	cbup = joystickGetDigital(1, 8, JOY_UP), cbdown = joystickGetDigital(1, 8, JOY_DOWN),
	override = joystickGetDigital(1, 7, JOY_LEFT);
	drive(LeftJoyStick, RightJoyStick);
	if(override && !last.override){overrided = abs(overrided - 1);}
	claw(127 * (ClawOpen - ClawClose));
	MOGO(127 * (mogoopen - mogoclose));
	if(armopen == 1 && last.arm == 0 && !overrided){scoredatcone(stack); stack += 1;}
	else if(armclose && !overrided){stack -= 1;}
	if(stackup && !last.su){stack += 1;}
	else if(stackdown && !last.sd){stack -= 1;}
	if(overrided)
		{
		if(armopen || armclose)
		{
			armheight += (armspeed * (armopen - armclose));
		}
		if(cbup || cbdown)
		{
			cbhieght += (cbspeed * (cbup - cbdown));
		}
		/*arm((armopen - armclose) * arm_const);
		cb((cbup - cbdown) * cb_pwm);*/
	}
	armopen = last.arm, last.sd = stackdown, last.su = stackup,	last.cc = ClawClose, last.co = ClawClose, last.mo = mogoopen, last.mc = mogoclose, last.override = override;
delay(20);}
void recordcode(){ // simple record auton code
	for(int t = 0; t < 1020; t++){
		controller(); update();
		auton[t].m1 = motor[1]; auton[t].m2 = motor[2]; auton[t].m3 = motor[3];	auton[t].m4 = motor[4]; auton[t].m5 =	motor[5];
		auton[t].m6 = motor[6];	auton[t].m7 = motor[7];	auton[t].m8 = motor[8];	auton[t].m9 = motor[9];	auton[t].m10 = motor[10];
		wait(10);}}
void reruncode(){ // totally not a stall code ;) hopefully they dont read this again // ignore that <- // just a rerun code to run what we recorded
	for(int i = 0; i < 1020; i++){
		motor[1] = auton[i].m1;	motor[2] = auton[i].m2;	motor[3] = auton[i].m3;	motor[4] = auton[i].m4;	motor[5] = auton[i].m5;
		motor[6] = auton[i].m6;	motor[7] = auton[i].m7;	motor[8] = auton[i].m8;	motor[9] = auton[i].m9;	motor[10] = auton[i].m10;
		update(); wait(10);}}
void savecode(){  // just a program to print out the values to a screen so we can copy paste it into our code
	printf("\nint motor1[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m1);}printf("};");wait(1000);
	printf("\nint motor2[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m2);}printf("};");wait(1000);
	printf("\nint motor3[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m3);}printf("};");wait(1000);
	printf("\nint motor4[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m4);}printf("};");wait(1000);
	printf("\nint motor5[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m5);}printf("};");wait(1000);
	printf("\nint motor6[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m6);}printf("};");wait(1000);
	printf("\nint motor7[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m7);}printf("};");wait(1000);
	printf("\nint motor8[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m8);}printf("};");wait(1000);
	printf("\nint motor9[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m9);}printf("};");wait(1000);
	printf("\nint motor10[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m10);}printf("};");wait(1000);}
void operatorControl(){
  TaskHandle LCDMENU = taskRunLoop(lcdmenu, 12);
	TaskHandle UPDATE = taskRunLoop(update, 10);
	TaskHandle CONTROLLER = taskRunLoop(controller, 14);
	TaskHandle SUSPENDARM = taskRunLoop(armtask, 20);
	taskPrioritySet(LCDMENU, 1);
	taskPrioritySet(CONTROLLER, 2);
	taskPrioritySet(UPDATE, 3);
	taskPrioritySet(SUSPENDARM, 4);
	while (1){
		bool record = joystickGetDigital(2, 7, JOY_UP), rerun = joystickGetDigital(2, 7, JOY_DOWN), save = joystickGetDigital(2, 7, JOY_LEFT);
		if(record){taskSuspend(CONTROLLER);recordcode();taskResume(CONTROLLER);}
		else if(rerun){taskSuspend(CONTROLLER);reruncode();taskResume(CONTROLLER);}
		else if(save){taskSuspend(CONTROLLER);savecode();taskResume(CONTROLLER);}
		delay(10);}
	taskDelete(CONTROLLER);
	taskDelete(UPDATE);
	taskDelete(LCDMENU);}
/*Tetris:d=4,o=5,b=160:d6,32p,c.6,32p,8a,8c6,8a#,16a,16g,f,c,8a,8c6,8g,8a,f,c,d,8d,8e,8g,8f,8e,8d,c,c,c*/ // tetris theme song for if we want any sound on a speaker because pros doesnt have  wav support
/* USSR_National_Anthem:d=4,o=5,b=250:f6, 2a#6, f.6, 8g6, 2a6, d6, d6, 2g6, f.6, 8d#6, 2f6, a#., 8a#, 2c6, c.6, 8d6, 2d#6, d#6, f6, 2g6, a.6, 8a#6, 2c.6, f6, 2d6, c.6, 8a#6, 2c6, a6, f6, 2a#6, a.6, 8g6, 2a6, d6, d6, 2g6, f.6, 8d#6, 2f6, a#., 8a#, 2a#6, a.6, 8g6, 1f6, 1d6, c6, a#6, a6, a#6, 2c.6, f6, 2f.6, 1a#6, a6, g6, f6, g6, 2a.6, d6, 2d6"*/
