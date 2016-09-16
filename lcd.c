#pragma config(UART_Usage, UART2, uartVEXLCD, baudRate19200, IOPins, None, None)

/////////////////////////////// SETUP ///////////////////////////////////
int frame = 0;

/////////////////////////////// LCD SETUP ///////////////////////////////
int LCD_left_button = 1;
int LCD_center_button = 2;
int LCD_right_button = 4;
int LCD_refresh_rate = 30;
void displayLCDInt(int line, int pos, int val) {
	clearLCDLine(line);
	string valstr = val;
	displayLCDString(line, pos, valstr);
}
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

/////////////////////////////// STATES //////////////////////////////////
bool STATE_ontick(int tick) {
	return (frame%tick == 0)
}
bool STATE_LCD_voltage() {
	return (nLCDButtons == LCD_left_button || SensorValue[debug_button] == 1)
}

/////////////////////////////// DO TASKS ////////////////////////////////
void DO_lcd() {
	if (STATE_ontick(LCD_refresh_rate)) {
		if (STATE_LCD_voltage()) {
			VIEW_voltage();
		} else {
			VIEW_blank();
		}
	}
}

/////////////////////////////// MAIN LOOP ///////////////////////////////
task main()
{
	LCD_init();
	while (true) {
		DO_lcd();

		frame++;
	}
}
