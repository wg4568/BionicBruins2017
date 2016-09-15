#pragma config(UART_Usage, UART2, uartVEXLCD, baudRate19200, IOPins, None, None)

int LCD_left_button = 1;
int LCD_center_button = 2;
int LCD_right_button = 4;

void displayLCDInt(int line, int pos, int val) {
	clearLCDLine(line);
	string valstr = val;
	displayLCDString(line, pos, valstr);
}

void initLCD() {
	bLCDBacklight = true;
	clearLCDLine(0);
	clearLCDLine(1);
}

task main()
{
	initLCD();
	int c = 0;
	while (true) {
		if (nLCDButtons == LCD_left_button) {
			c--;
		}
		if (nLCDButtons == LCD_right_button) {
			c++;
		}
		if (nLCDButtons == LCD_center_button) {
			c = 0;
		}
		displayLCDInt(0, 0, c);
		wait1Msec(200);
	}
}
