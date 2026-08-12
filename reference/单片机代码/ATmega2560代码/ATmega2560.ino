#include <SCoop.h>
#include "motor.h"
#include "screen.h"
defineTask(UART_monitoring);  //串口监视及指令解析线程  主线程
int a = 0;
void UART_monitoring::setup()    //串口通信线程
{
  Serial.begin(9600);   //设置串口通信波特率
  Serial3.begin(115200);   //设置串口通信波特率
  pinMode(40,OUTPUT);  //风扇控制引脚
  digitalWrite(40,HIGH);
}

void output()
{
  digitalWrite(49,LOW);
  delay(100);
  digitalWrite(49,HIGH);
} 


void UART_monitoring::loop()
{
 
  if(Serial.available() >= 1)   //判断串口是否有接受到数据  因为接受到的都是int类型的数据 所以判断时 > 1 （要在int类型后面加上一个不是数字的字符好让Serial.parseInt()函数反应迅速）
  {
    a=Serial.parseInt();
    //Serial3.println(a);
    Serial.read();    //把整数后面的一个字符读走
    switch((a / 1000))    //判断该数据是什么数据
    {
      case 10:    //前进
          PWM_motor_speed_right=a % 1000;
          PWM_motor_speed_left=(a % 1000)/1.5;
          break;
      case 11:    //后退
          PWM_motor_speed_right=-(a % 1000);
          PWM_motor_speed_left=(-(a % 1000))/1.5;
          break;
      case 12:    //左转
          PWM_motor_speed_left=(-(a % 1000))/1.5;
          PWM_motor_speed_right=a % 1000;
          break;
      case 13:    //右转
          PWM_motor_speed_left=(a % 1000)/1.5;
          PWM_motor_speed_right=-(a % 1000);
          break;
      case 14:    //右侧机械臂电机正转
          PWM_motor_speed_external_right=a % 1000;
          break;
      case 15:    //右侧机械臂电机反转
          PWM_motor_speed_external_right=-(a % 1000);
          break;
      case 16:    //左侧机械臂电机正转
          PWM_motor_speed_external_left=a % 1000;
          break;
      case 17:    //左侧机械臂电机反转
          PWM_motor_speed_external_left=-(a % 1000);
          break;
      case 18:    //机械臂步进电机正转
          stepping_motor=a % 1000;
          break;
      case 19:    //机械臂步进电机反转
          stepping_motor=-(a % 1000);
          break;
      case 20:    //开启电机驱动芯片电压
          output();
          break;
    }
  }
}

void setup() {
  mySCoop.start();
}

void loop() 
{
  yield();
}
