#pragma config(Sensor, dgtl1,  debug_button,   sensorTouch)
#pragma config(Motor,  port2,           front_left,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           front_right,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           back_left,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           back_right,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           recovery_servo, tmotorServoStandard, openLoop)

/////////////////////////////// CONFIG //////////////////////////////////
int frame = 0;
float CONFIG_control_adjust = 0.01;
float CONFIG_precise_ratio = 4;
float CONFIG_slow_rate = 0.01;
float CONFIG_slow_tick = 30;
float CONFIG_drive_mode = 1;
float CONFIG_stopper_tick = 20;
float CONFIG_stop_thresh = 90;
int LCD_left_button = 1;
int LCD_center_button = 2;
int LCD_right_button = 4;
int LCD_refresh_rate = 30;
float CONTROL_sensitivity = 1.2;
float CONTROL_threshold = 2;
float CONTROL_min_speed = 5;
float CONTROL_precise = CONTROL_sensitivity / CONFIG_precise_ratio;

float left = 0;
float right = 0;

float prev_left = 0;
float prev_right = 0;

/////////////////////////////// MISC ////////////////////////////////////
int onezeroneg(float val) {
	if (val < 0) {return -1;}
	else if (val > 0) {return 1;}
	else {return 0;}
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

/////////////////////////////// LCD SETUP ///////////////////////////////
void displayLCDFloat(int line, int pos, float val) {
	clearLCDLine(line);
	string valstr = val;
	displayLCDString(line, pos, valstr);
}
void LCD_init() {
	bLCDBacklight = true;
	clearLCDLine(0);
	clearLCDLine(1);
}
/////////////////////////////// STATES //////////////////////////////////
bool STATE_ontick(int tick) {
	return (frame%tick == 0);
}
bool STATE_LCD_voltage() {
	return (nLCDButtons == LCD_left_button || SensorValue[debug_button] || vexRT[Btn5D]);
}
bool STATE_LCD_sensitivity() {
	return (nLCDButtons == LCD_right_button || vexRT[Btn5U]);
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
	return (vexRT[Btn6U] == 1);
}
bool STATE_toggle_drive_mode() {
	return (vexRT[Btn6D] == 1);
}
bool STATE_view_speed() {
	return (onezeroneg(left) || onezeroneg(right));
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
	if (STATE_precise_control()) {
		displayLCDString(1, 0, "PRECISE");
	} else {
		clearLCDLine(1);
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

/////////////////////////////// DO TASKS ////////////////////////////////
void DO_lcd() {
	if (STATE_ontick(LCD_refresh_rate)) {
		if (STATE_LCD_voltage()) {
			VIEW_voltage();
		} else if (STATE_LCD_sensitivity()) {
			VIEW_sensitivity();
		} else if (STATE_view_speed()) {
			VIEW_leftright();
		} else {
			VIEW_info();
		}
	}
}
void DO_usercontrol() {
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
	if (STATE_ontick(50)) {
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
	if (STATE_ontick(CONFIG_slow_tick)) {
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
		wait1Msec(10);
	}
}
void DO_stopper() {
	if (left == 0 && right == 0 && STATE_ontick(CONFIG_stopper_tick)) {
		float STOP_AMOUNT = 100;
		if (prev_left > CONFIG_stop_thresh && prev_right > CONFIG_stop_thresh) {
			left = -STOP_AMOUNT;
			right = -STOP_AMOUNT;
		}
		if (prev_left < -CONFIG_stop_thresh && prev_right < -CONFIG_stop_thresh) {
			left = STOP_AMOUNT;
			right = STOP_AMOUNT;
		}

		prev_left = left;
		prev_right = right;
	}
}
void DO_movement() {
	MOVEMENT_left_drive(left);
	MOVEMENT_right_drive(right);
}

/////////////////////////////// MAIN LOOP ///////////////////////////////
task main()
{
	LCD_init();
	while (true) {
		DO_lcd();
		DO_usercontrol();
		DO_precisecontrol();
		DO_stopper();
		DO_movement();
		DO_senscontrol();

		frame++;
	}
}
