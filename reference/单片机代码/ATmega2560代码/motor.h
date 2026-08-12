//轮子
int PWM_motor_speed_right = 0 ;   //右侧电机速度
int PWM_motor_speed_left = 0 ;   //左侧电机速度
/***************************************************************/
//机械臂

int PWM_motor_speed_external_right = 0 ;  //外部右侧电机速度
int PWM_motor_speed_external_left = 0 ;  //外部左侧电机速度
int stepping_motor = 0;  //步进电机控制指向   1：内右 2：内左 3：外右  4：外左 0：停止   正数正转 负数反转
int eight [8][4]={{1,0,0,0},{1,0,1,0},{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,1,0,1},{0,0,0,1},{1,0,0,1}};  //步进电机按照八拍运行 数组对应 A+ A- B+ B-
int Pin_motor [4][4]={{43,42,48,47},{25,26,27,28},{21,20,19,18},{29,39,30,31}};  //

void mechanical_motor() //机械臂步进电机控制   //步进电机引脚 顺序为 内右 内左 外右 外左
{
  if(stepping_motor!=0)   //控制步进电机
  {
    if(stepping_motor>0)  //正转
    {
      for(int i=0;i<8;i++)
      {
        for(int j =0;j<4;j++)
        {
          digitalWrite(Pin_motor[stepping_motor-1][j],eight[i][j]);
        }
        delayMicroseconds(400);
      }
      for(int i =0;i<4;i++)
      {
         for(int j=0;j<4;j++)
         {
            digitalWrite(Pin_motor[i][j],HIGH);
         }
      }
    }
    if(stepping_motor<0)  //反转
    {
      for(int i=7;i>-1;i--)
      {
        for(int j =0;j<4;j++)
        {
          digitalWrite(Pin_motor[(stepping_motor/-1)-1][j],eight[i][j]);
        }
        delayMicroseconds(400);
      }
      for(int i =0;i<4;i++)
      {
         for(int j=0;j<4;j++)
         {
            digitalWrite(Pin_motor[i][j],HIGH);
         }
      }
    }
  }
  
  if(PWM_motor_speed_external_right >= 0)    //右正转
  {
    analogWrite(6,0);
    analogWrite(7,PWM_motor_speed_external_right);
    Serial3.println(PWM_motor_speed_external_right);
  }
  else    //右反转
  {
    analogWrite(7,0);
    analogWrite(6,-PWM_motor_speed_external_right);
  }
/*********************************************************************************/
  if(PWM_motor_speed_external_left >= 0)   //左正转
  {
    analogWrite(13,0);
    analogWrite(46,PWM_motor_speed_external_left);
  }
  else  //左反转
  {
    analogWrite(46,0);
    analogWrite(13,-PWM_motor_speed_external_left);
  }
}


void wheel_motor()  //轮子电机控制函数
{
  if(PWM_motor_speed_left >= 0)    //左正转
  {
    analogWrite(44,0);
    analogWrite(45,PWM_motor_speed_left);
  }
  else   //左反转
  {
    analogWrite(45,0);
    analogWrite(44,-PWM_motor_speed_left); 
  } 
  
  if(PWM_motor_speed_right >= 0)    //右正转
  {
    analogWrite(8,0);
    analogWrite(9,PWM_motor_speed_right);
  }
  else //右反转
  {
    analogWrite(9,0);
    analogWrite(8,-PWM_motor_speed_right);
  }
/*********************************************************************************/

}

defineTask(motor);//电机控制线程

void motor::setup() //电机速度线程控制 
{
  pinMode(8,OUTPUT);  //设置引脚为输出模式  需要用到的PWM口（物理PIN）:右侧：17&18（对应程序8&9）  左侧：39&40（对应程序45&44）
  pinMode(9,OUTPUT);
  pinMode(45,OUTPUT);
  pinMode(44,OUTPUT);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(45,LOW);
  digitalWrite(44,LOW);
/*********************************************************************************/
  pinMode(48,OUTPUT);  //物理PIN：36  映射48   A+   内部右侧步进电机引脚
  pinMode(47,OUTPUT);  //物理PIN：37  映射47   A-
  pinMode(43,OUTPUT);  //物理PIN：41  映射43   B+
  pinMode(42,OUTPUT);  //物理PIN：42  映射42   B-
  digitalWrite(48,HIGH);
  digitalWrite(47,HIGH);
  digitalWrite(43,HIGH);
  digitalWrite(42,HIGH);
/***********************************************************************************/
  pinMode(25,OUTPUT);  //物理PIN：75  映射25   A+   内部左侧步进电机引脚
  pinMode(26,OUTPUT);  //物理PIN：74  映射26   A-
  pinMode(27,OUTPUT);  //物理PIN：73  映射27   B+
  pinMode(28,OUTPUT);  //物理PIN：72  映射28   B-
  digitalWrite(25,HIGH);
  digitalWrite(26,HIGH);
  digitalWrite(27,HIGH);
  digitalWrite(28,HIGH);
/*************************************************************************************/
  pinMode(21,OUTPUT);  //物理PIN：43  映射21   A+   外部右侧步进电机引脚
  pinMode(20,OUTPUT);  //物理PIN：44  映射20   A-
  pinMode(19,OUTPUT);  //物理PIN：45  映射19   B+
  pinMode(18,OUTPUT);  //物理PIN：46  映射18   B-
  digitalWrite(21,HIGH);
  digitalWrite(20,HIGH);
  digitalWrite(19,HIGH);
  digitalWrite(18,HIGH);
/*****************************************************************************/
  pinMode(29,OUTPUT);  //物理PIN：71  映射29   A+   外部左侧步进电机引脚
  pinMode(39,OUTPUT);  //物理PIN：70  映射39   A-
  pinMode(30,OUTPUT);  //物理PIN：73  映射27   B+
  pinMode(31,OUTPUT);  //物理PIN：72  映射28   B-
  digitalWrite(29,HIGH);
  digitalWrite(39,HIGH);
  digitalWrite(30,HIGH);
  digitalWrite(31,HIGH);
/*******************************************************************************/
  pinMode(6,OUTPUT);  //物理PIN：15  映射6       外部右侧电机引脚
  pinMode(7,OUTPUT);  //物理PIN：16  映射7
  pinMode(46,OUTPUT);  //物理PIN：38  映射46     外部左侧电机引脚
  pinMode(13,OUTPUT);  //物理PIN：26  映射13
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(46,LOW);
  digitalWrite(13,LOW);

  pinMode(49,OUTPUT);
  digitalWrite(49,HIGH);  
}

void motor::loop()  //线程循环 
{
  wheel_motor();
  mechanical_motor();
  
}
