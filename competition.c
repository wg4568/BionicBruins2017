#pragma config(Sensor, in3,    ARM_pot,        sensorPotentiometer)
#pragma config(Sensor, dgtl11, debug_button,   sensorTouch)
#pragma config(Motor,  port1,           intake_left,   tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           front_left,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           front_right,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           back_left,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           back_right,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           left_arm_a,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           right_arm_a,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           right_arm_b,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           left_arm_b,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          intake_right,  tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                    Bionic Bruins B Team (6374B) Code                      */
/*                      by William Gardner, 2016                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/////////////////////////////// CONFIG //////////////////////////////////
int frame = 0;
float CONFIG_control_adjust = 0.01;
float CONFIG_precise_ratio = 2.5;
float CONFIG_slow_rate = 0.01;
float CONFIG_slow_tick = 30;
float CONFIG_drive_mode = 1;
float CONFIG_accel_tick = 200;
float CONFIG_turn_ratio = 0.75;
float ARM_up_speed = 127;
float ARM_down_speed = 60;
int LCD_left_button = 1;
int LCD_center_button = 2;
int LCD_right_button = 4;
int LCD_refresh_rate = 30;
float CONTROL_sensitivity = 1.2;
float CONTROL_threshold = 2;
float CONTROL_min_speed = 5;
float CONTROL_precise = CONTROL_sensitivity / CONFIG_precise_ratio;
int CONTROL_direction = 1;
int CONTROL_precise_toggle = 0;
string AUTON_MODE = "RIGHT";

float left = 0;
float right = 0;
float acceleration_left = 0;
float acceleration_right = 0;

float prev_left = 0;
float prev_right = 0;

float debug_one = 0;
float debug_two = 0;

/////////////////////////////// MISC ////////////////////////////////////
int onezeroneg(float val) {
	if (val < 0) {return -1;}
	else if (val > 0) {return 1;}
	else {return 0;}
}
bool ontick(int tick) {
	return (frame%tick == 0);
}

/////////////////////////////// STATES //////////////////////////////////
bool STATE_LCD_voltage() {
	return (SensorValue[debug_button] || vexRT[Btn5D]);
}
bool STATE_ARM_allow_up() {
	return (SensorValue[ARM_pot] < 4095);
}
bool STATE_ARM_allow_down() {
	return (SensorValue[ARM_pot] > 0);
}
bool STATE_LCD_sensitivity() {
	return ((bool)vexRT[Btn8R]);
}
bool STATE_auton_toggle() {
	return (nLCDButtons == LCD_center_button);
}
bool STATE_adjust_sensdown() {
	return (STATE_LCD_sensitivity() && vexRT[Btn7D]);
}
bool STATE_adjust_sensup() {
	return (STATE_LCD_sensitivity() && vexRT[Btn7R]);
}
bool STATE_adjust_threshdown() {
	return (STATE_LCD_sensitivity() && vexRT[Btn7L]);
}
bool STATE_adjust_threshup() {
	return (STATE_LCD_sensitivity() && vexRT[Btn7U]);
}
bool STATE_precise_control() {
//	if (vexRT[Btn8D]) {
//		if (CONTROL_precise_toggle == 1) {
//			CONTROL_precise_toggle = 0;
//		} else {
//			CONTROL_precise_toggle = 1;
//		}
//		wait1Msec(200);
//	}
//	return ((bool)CONTROL_precise_toggle);
	return (false);
}
bool STATE_toggle_drive_mode() {
//	return (vexRT[Btn7U] == 1);
	return (false);
}
bool STATE_view_speed() {
	return (onezeroneg(left) || onezeroneg(right));
}
bool STATE_ARM_up() {
	return ((bool)vexRT[Btn6U]);
}
bool STATE_ARM_down() {
	return ((bool)vexRT[Btn6D]);
}
bool STATE_debug_view() {
	return ((bool)vexRT[Btn7D]);
}
bool STATE_intake_open() {
	return ((bool)vexRT[Btn5U]);
}
bool STATE_intake_close() {
	return ((bool)vexRT[Btn5D]);
}

/////////////////////////////// MOVEMENT ////////////////////////////////
void MOVEMENT_left_drive(float power) {
	motor(front_left) = power;
	motor(back_left) = power;
}
void MOVEMENT_right_drive(float power) {
	motor(front_right) = power;
	motor(back_right) = power;
}
void MOVEMENT_drive(float pl, float pr, float time) {
	MOVEMENT_left_drive(pl);
	MOVEMENT_right_drive(pr);
	wait1Msec(time);
	MOVEMENT_left_drive(0);
	MOVEMENT_right_drive(0);
}

/////////////////////////////// INTAKE CODE /////////////////////////////
void INTAKE_open() {
	motor(intake_left) = 50;
	motor(intake_right) = 50;
}
void INTAKE_close() {
	motor(intake_left) = -80;
	motor(intake_right) = -80;
}
void INTAKE_stop() {
	motor(intake_left) = 0;
	motor(intake_right) = 0;
}

/////////////////////////////// ARM CODE ////////////////////////////////
void ARM_up() {
	motor[left_arm_a] = ARM_up_speed;
	motor[right_arm_a] = ARM_up_speed;
	motor[left_arm_b] = ARM_up_speed;
	motor[right_arm_b] = ARM_up_speed;
}
void ARM_stop() {
	motor[left_arm_a] = 10;
	motor[right_arm_a] = 10;
	motor[left_arm_b] = 10;
	motor[right_arm_b] = 10;
}
void ARM_down() {
	motor[left_arm_a] = -ARM_down_speed;
	motor[right_arm_a] = -ARM_down_speed;
	motor[left_arm_b] = -ARM_down_speed;
	motor[right_arm_b] = -ARM_down_speed;
}
void ARM_coast() {
	motor[left_arm_a] = 0;
	motor[right_arm_a] = 0;
	motor[left_arm_b] = 0;
	motor[right_arm_b] = 0;
}
void ARM_move(float direction, float time) {
	float upold = ARM_up_speed;
	float doold = ARM_down_speed;
	ARM_up_speed = 60;
	ARM_down_speed = 40;
	if (direction < 0) {
		ARM_down();
	} else if (direction > 0) {
		ARM_up();
	} else {
		ARM_coast();
	}
	wait1Msec(time);
	ARM_stop();
	ARM_up_speed = upold;
	ARM_down_speed = doold;
}
void ARM_to(float pos) {
	if (pos < 0) {pos = 0;}
	if (pos > 4095) {pos = 4095;}
	int curr = SensorValue[ARM_pot];
	if (pos > curr) {
		ARM_up();
	} else {
		ARM_down();
	}
	while (SensorValue[ARM_pot] != pos) {}
	ARM_stop();
}

/////////////////////////////// LCD SETUP ///////////////////////////////
void displayLCDFloat(int line, int pos, float val) {
	clearLCDLine(line);
	string valstr = val;
	displayLCDString(line, pos, valstr);
}
void LCD_reset() {
	bLCDBacklight = true;
	clearLCDLine(0);
	clearLCDLine(1);
}

/////////////////////////////// LCD VIEWS ///////////////////////////////
void VIEW_blank() {
	clearLCDLine(0);
	clearLCDLine(1);
}
void VIEW_voltage() {
	float voltage = nImmediateBatteryLevel/1000.0;
	displayLCDString(0, 0, "Battery voltage: ");
	displayLCDFloat(1, 0, voltage);
}
void VIEW_sensitivity() {
	displayLCDFloat(0, 0, CONTROL_threshold);
	displayLCDFloat(1, 0, CONTROL_sensitivity);
}
void VIEW_info() {
	VIEW_blank();
	displayLCDString(0, 0, "Driver mode");
	displayLCDString(1, 0, AUTON_MODE);
	if (STATE_precise_control()) {
		displayLCDString(1, 0, "PRECISE");
	} else {
		clearLCDLine(1);
		displayLCDString(1, 0, AUTON_MODE);
	}
	if (CONFIG_drive_mode) {
		displayLCDString(0, 15, "E");
	} else {
		displayLCDString(0, 15, "L");
	}
}
void VIEW_leftright() {
	displayLCDFloat(0, 0, left);
	displayLCDFloat(1, 0, right);
}
void VIEW_debug() {
	displayLCDFloat(0, 0, debug_one);
	displayLCDFloat(1, 0, debug_two);
}

/////////////////////////////// DO TASKS ////////////////////////////////
void DO_lcd() {
	if (ontick(LCD_refresh_rate)) {
		if (STATE_LCD_voltage()) {
			VIEW_voltage();
		} else if (STATE_LCD_sensitivity()) {
			VIEW_sensitivity();
		} else if (STATE_view_speed()) {
			VIEW_leftright();
		} else if (STATE_debug_view()) {
			VIEW_debug();
		} else {
			VIEW_info();
		}
	}
}
void DO_usercontrol() {
	if (STATE_auton_toggle()) {
		if (AUTON_MODE == "LEFT") {
			AUTON_MODE = "RIGHT";
		} else {
			AUTON_MODE = "LEFT";
		}
		wait1Msec(200);
	}

	left = vexRT[Ch3];
	right = vexRT[Ch2];
	if (CONFIG_drive_mode == 1) {
		left = ((left*left*left)/20000);
		right = ((right*right*right)/20000);
	}
	left = left * CONTROL_sensitivity;
	right = right * CONTROL_sensitivity;
	if (left > -CONTROL_threshold && left < CONTROL_threshold) {
		left = 0;
	}
	if (right > -CONTROL_threshold && right < CONTROL_threshold) {
		right = 0;
	}
	left = left + (onezeroneg(left) * CONTROL_min_speed);
	right = right + (onezeroneg(right) * CONTROL_min_speed);
}
void DO_senscontrol() {
	if (ontick(50)) {
		if (STATE_adjust_sensup()) {
			CONTROL_sensitivity = CONTROL_sensitivity + CONFIG_control_adjust;
		}
		if (STATE_adjust_sensdown()) {
			CONTROL_sensitivity = CONTROL_sensitivity - CONFIG_control_adjust;
		}
		if (STATE_adjust_threshup()) {
			CONTROL_threshold = CONTROL_threshold + CONFIG_control_adjust;
		}
		if (STATE_adjust_threshdown()) {
			CONTROL_threshold = CONTROL_threshold - CONFIG_control_adjust;
		}
	}
}
void DO_precisecontrol() {
	if (ontick(CONFIG_slow_tick) && ! STATE_LCD_sensitivity()) {
		if (STATE_precise_control()) {

			if (CONTROL_sensitivity > CONTROL_precise) {
				CONTROL_sensitivity = CONTROL_sensitivity - CONFIG_slow_rate;
			}
		} else {
			if (CONTROL_sensitivity < CONTROL_precise * CONFIG_precise_ratio) {
				CONTROL_sensitivity = CONTROL_sensitivity + CONFIG_slow_rate;
			}
		}
	}
	if (STATE_toggle_drive_mode()) {
		if (CONFIG_drive_mode == 1) {
			CONFIG_drive_mode = 0;
		} else {
			CONFIG_drive_mode = 1;
		}
		wait1Msec(2);
	}
}
void DO_calcs() {
	if (ontick(CONFIG_accel_tick)) {
		acceleration_left = prev_left - left;
		acceleration_right = prev_right - right;

		prev_left = left;
		prev_right = right;
	}
	debug_one = 0;
	debug_two = 0;
}
void DO_movement() {
	if (STATE_precise_control()) {
		DO_precisecontrol();
	}
	int r = CONTROL_direction * right;
	int l = CONTROL_direction * left;
//	if ((l < 0 && r > 0) || ( l > 0 && r < 0)) {
//		l = l * CONFIG_turn_ratio;
//		r = r * CONFIG_turn_ratio;
//	}
	MOVEMENT_left_drive(l);
	MOVEMENT_right_drive(r);
}
void DO_arm() {
	if (STATE_ARM_up()) {
		ARM_up();
	}
	else if (STATE_ARM_down()) {
		ARM_down();
	} else {
		ARM_stop();
	}
}
void DO_intake() {
	if (STATE_intake_open()) {
		INTAKE_open();
	} else if (STATE_intake_close()) {
		INTAKE_close();
	} else {
		INTAKE_stop();
	}
}
void DO_auton() {
//	drvie bnack k
//	lowkeu arm raize
//	unshenanagen the schmoogles
//	drive forwards and reshenanegan the schmoogles
//	drive 2 fence
//	and raise arm wile unxhenanignanung the schiooogmleas
	MOVEMENT_drive(100, 100, 350);

	MOVEMENT_drive(-100, -100, 1100);

	ARM_move(1, 500);

	INTAKE_open();
	wait1Msec(2300);
	INTAKE_stop();

	MOVEMENT_drive(90, 90, 1100);

	INTAKE_close();
	wait1Msec(1400);

	ARM_move(1, 1900);
}

/////////////////////////////// MAIN TASKS //////////////////////////////
void pre_auton() {
  bStopTasksBetweenModes = true;
  bDisplayCompetitionStatusOnLcd = false;
  LCD_reset();
}

task autonomous() {
	displayLCDString(0, 0, "AUTONOMOUS MODE");
	DO_auton();
	LCD_reset();
}

task usercontrol() {
	while (true) {
		DO_lcd();
		DO_usercontrol();
		DO_precisecontrol();
		DO_calcs();
		DO_movement();
		DO_arm();
		DO_senscontrol();
		DO_intake();

		frame++;
	}
}

/*
CARRY PLS
*/
