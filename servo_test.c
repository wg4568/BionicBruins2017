#pragma config(Motor,  port6,           recovery_servo, tmotorServoStandard, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	motor(recovery_servo) = 30;
	wait1Msec(1000);
}
