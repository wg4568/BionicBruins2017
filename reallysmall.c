#pragma config(Motor,port2,a,tmotorVex393_MC29,openLoop)
#pragma config(Motor,port3,b,tmotorVex393_MC29,openLoop,reversed)
#pragma config(Motor,port4,c,tmotorVex393_MC29,openLoop)
#pragma config(Motor,port5,d,tmotorVex393_MC29,openLoop)
#pragma config(Motor,port6,e,tmotorVex393_MC29,openLoop)
#pragma config(Motor,port10,f,tmotorVex393_MC29,openLoop,reversed)
void g(float s){motor(a)=s;motor(c)=s;}void h(float s){motor(b)=s;motor(d)=s;}void l(float t){motor[e]=t;motor[f]=t;}void arm_stop(){motor[e]=0;motor[f]=0;}void k(float t){motor[e]=-t;motor[f]=-t;}int m(float u){if(u<0){return -1;}else if(u>0){return 1;}else{return 0;}}task main(){int n,o;float p=1.2;float q=10;float r=5;float i=50;float j=50;while(true){n=vexRT[Ch3];o=vexRT[Ch2];n=((n*n*n)/20000);o=((o*o*o)/20000);n=n*p;o=o*p;if(n>-q&&n<q){n=0;}if(o>-q&&o<q){o=0;}g(n+(m(n)*r));h(o+(m(o)*r));if(vexRT[Btn6U]){l(i);}else if(vexRT[Btn5U]){k(j);}else{arm_stop();}}}
