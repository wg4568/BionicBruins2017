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
int CONFIG_calib = 0;
int LCD_left_button = 1;
int LCD_center_button = 2;
int LCD_right_button = 4;
int LCD_refresh_rate = 30;
float CONTROL_sensitivity = 1.2;
float CONTROL_threshold = 5;
float CONTROL_precise = CONTROL_sensitivity / CONFIG_precise_ratio;
float CONTROL_left_calib = 0;
float CONTROL_right_calib = 0;

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
bool STATE_calib() {
	return (vexRT[Btn5U] && vexRT[Btn5D] && vexRT[Btn6D] && vexRT[Btn6U] && CONFIG_calib != 1);
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
void VIEW_calibrate() {
	displayLCDString(0, 0, "################");
	displayLCDString(1, 0, "################");
}

/////////////////////////////// DO TASKS ////////////////////////////////
void DO_lcd() {
	if (STATE_ontick(LCD_refresh_rate)) {
		if (STATE_LCD_voltage()) {
			VIEW_voltage();
		} else if (STATE_LCD_sensitivity()) {
			VIEW_sensitivity();
		} else if (CONFIG_calib) {
			VIEW_calibrate();
		} else {
			VIEW_info();
		}
	}
}
void DO_usercontrol() {
	float left = vexRT[Ch3];
	float right = vexRT[Ch2];
	int leftmult = 1;
	int rightmult = 1;
	if (CONFIG_drive_mode == 1) {
		left = ((left*left*left)/20000)+5;
		right = ((right*right*right)/20000)+5;
	}
	left = left * CONTROL_sensitivity;
	right = right * CONTROL_sensitivity;
	if (left > -CONTROL_threshold && left < CONTROL_threshold) {
		left = 0;
	}
	if (right > -CONTROL_threshold && right < CONTROL_threshold) {
		right = 0;
	}
	if (CONFIG_calib) {
		CONTROL_left_calib = CONTROL_left_calib + left;
		CONTROL_right_calib = CONTROL_right_calib + right;
	}
	MOVEMENT_left_drive(left);
	MOVEMENT_right_drive(right);
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
		wait1Msec(100);
	}
}
void DO_calibrate() {
	if (STATE_calib()) {
		CONFIG_calib = 1;
	}
}

/////////////////////////////// MAIN LOOP ///////////////////////////////
task main()
{
	LCD_init();
	while (true) {
		DO_lcd();
		DO_usercontrol();
		DO_senscontrol();
		DO_precisecontrol();
		DO_calibrate();

		frame++;
	}
}
