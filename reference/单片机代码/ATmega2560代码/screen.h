#include "OneWire.h"
#include "DallasTemperature.h"
#include "font.h"
#define ONE_WIRE_BUS 24

defineTask(screen);  //屏幕显示及传感器控制线程

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int trigPin = 23;    //Trig
int echoPin = 22;    //Echo
long duration, cm;

int scl=33;//定义数字接口33
int sda=34;//定义数字接口34
int res=36;//定义数字接口36
int dc=35;//定义数字接口35
int cs=32;//定义数字接口32

int mark=0;
int capacity=0;  //实时电池电压大小 0-240
int capacity2=0;   //历史电压大小

#define LCD_W 135  //设置屏幕像素
#define LCD_H 240


//颜色
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F  
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40 //棕色
#define BRRED            0XFC07 //棕红色
#define GRAY             0X8430 //灰色


short int BACK_COLOR, POINT_COLOR;

void LCD_Writ_Bus(uint8_t dat) 
{  
  uint8_t i; 
  digitalWrite(cs,LOW);
  for(i=0;i<8;i++)
  {       
    digitalWrite(scl,LOW);
    if(dat&0x80)
       digitalWrite(sda,HIGH);
    else 
       digitalWrite(sda,LOW);
    digitalWrite(scl,HIGH);
    dat<<=1;   
  }   
  digitalWrite(cs,HIGH);
}

void LCD_WR_REG(uint8_t dat)
{
  digitalWrite(dc,LOW);//写命令
  LCD_Writ_Bus(dat);
}

void LCD_WR_DATA8(uint8_t dat)
{
  digitalWrite(dc,HIGH);//写数据
  LCD_Writ_Bus(dat);
}

void Lcd_Init(void)  //OLED的初始化
{
  pinMode(scl,OUTPUT);//设置数字
  pinMode(sda,OUTPUT);//设置数字
  pinMode(res,OUTPUT);//设置数字
  pinMode(dc,OUTPUT);//设置数字
  pinMode(cs,OUTPUT);//设置数字
  
  digitalWrite(res,HIGH);
  sleep(100);
  digitalWrite(res,LOW);//复位
  sleep(200);
  digitalWrite(res,HIGH);
  sleep(100);
  LCD_WR_REG(0x11); 
sleep(100);

//************* Start Initial Sequence **********// 
LCD_WR_REG(0x36); 
LCD_WR_DATA8(0xC0);

LCD_WR_REG(0x3A);
LCD_WR_DATA8(0x05);

LCD_WR_REG(0xB2);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x00);
LCD_WR_DATA8(0x33);
LCD_WR_DATA8(0x33); 

LCD_WR_REG(0xB7); 
LCD_WR_DATA8(0x35);  

LCD_WR_REG(0xBB);
LCD_WR_DATA8(0x19);

LCD_WR_REG(0xC0);
LCD_WR_DATA8(0x2C);

LCD_WR_REG(0xC2);
LCD_WR_DATA8(0x01);

LCD_WR_REG(0xC3);
LCD_WR_DATA8(0x12);   

LCD_WR_REG(0xC4);
LCD_WR_DATA8(0x20);  

LCD_WR_REG(0xC6); 
LCD_WR_DATA8(0x0F);    

LCD_WR_REG(0xD0); 
LCD_WR_DATA8(0xA4);
LCD_WR_DATA8(0xA1);

LCD_WR_REG(0xE0);
LCD_WR_DATA8(0xD0);
LCD_WR_DATA8(0x04);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x11);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x2B);
LCD_WR_DATA8(0x3F);
LCD_WR_DATA8(0x54);
LCD_WR_DATA8(0x4C);
LCD_WR_DATA8(0x18);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x0B);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x23);

LCD_WR_REG(0xE1);
LCD_WR_DATA8(0xD0);
LCD_WR_DATA8(0x04);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x11);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x2C);
LCD_WR_DATA8(0x3F);
LCD_WR_DATA8(0x44);
LCD_WR_DATA8(0x51);
LCD_WR_DATA8(0x2F);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x20);
LCD_WR_DATA8(0x23);

LCD_WR_REG(0x21);

LCD_WR_REG(0x11); 

LCD_WR_REG(0x29); 
}

void LCD_WR_DATA(short int dat)  //LCD写入数据
{
  digitalWrite(dc,HIGH);//写数据
  LCD_Writ_Bus(dat>>8);
  LCD_Writ_Bus(dat);
}

void LCD_Address_Set(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)  //设置起始和结束地址
{
    LCD_WR_REG(0x2a);//列地址设置
    LCD_WR_DATA(x1+53);
    LCD_WR_DATA(x2+53);
    LCD_WR_REG(0x2b);//行地址设置
    LCD_WR_DATA(y1+40);
    LCD_WR_DATA(y2+40);
    LCD_WR_REG(0x2c);//储存器写
}

void LCD_Clear(short int Color)  //LCD清屏函数
{
  short int i,j;    
  LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_H;i++)
    {
       for (j=0;j<LCD_W;j++)
        {
          LCD_WR_DATA(Color);
        }
    }
}

void LCD_Fill(short int xsta,short int ysta,short int xend,short int yend,short int color)  //在指定位置填充颜色
{          
  short int i,j; 
  LCD_Address_Set(xsta,ysta,xend,yend);      //设置光标位置 
  for(i=ysta;i<=yend;i++)
  {                               
    for(j=xsta;j<=xend;j++)
    {
      LCD_WR_DATA(color);//设置光标位置     
    }     
  }                   
}

void battery_capacity()   //显示电量
{
  if(mark==0)   //成立说明是上电第一次显示电压
  {
    capacity=((analogRead(A15)-570)>240)?240:(((analogRead(A15)-570)<0)?0:(analogRead(A15)-570));
    capacity2=capacity;
    for(int i=0;i<capacity;i++)  
    {
      if(i==40 || i==90 || i==140 || i==190)  //跳过指定像素 显示一格一格的电量
      {
          i+=10;
      }
      LCD_Fill(0,i,135,i,0xFFE0);   //在指定位置填充颜色（黄色）
      sleep(1);    // 延迟   如果不加则无法执行其他线程
    }
    mark=1;   // 
  }
    else 
  {
    capacity=((analogRead(A15)-570)>240)?240:(((analogRead(A15)-570)<0)?0:(analogRead(A15)-570));  //测量实时电压
    //capacity2=capacity;
      if(capacity2>capacity)    //电压比较   如果成立说明电压下降了  放电
      {
        LCD_Fill(0,capacity2,135,capacity2+1,0x0000);   //在指定位置填充黑色
        capacity2-=1;   // 调整历史电压  等待下次比较
      }
      
      if(capacity>capacity2)    //电压比较   如果成立说明电压上升了  充电
      {
        if((capacity2>40&&capacity2<50)||(capacity2>90&&capacity2<100)||(capacity2>140&&capacity2<150)||(capacity2>190&&capacity2<200))  //跳过指定像素 显示一格一格的电量
        {
          LCD_Fill(0,capacity2-1,135,capacity2,0x0000);   //在指定位置填充黑色
          capacity2+=1;   // 调整历史电压  等待下次比较
        }
        else
        {
        LCD_Fill(0,capacity2-1,135,capacity2,0xFFE0);   //在指定位置填充黄色
        capacity2+=1;   // 调整历史电压  等待下次比较
        }
      }
  }
}

void HC_05()
{
  digitalWrite(trigPin, LOW);
  sleep(1);
  digitalWrite(trigPin, HIGH);
  sleep(1);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29;
  if(cm >= 100)return;
  Serial.print("A");
  Serial.print(cm/10);   //打印十位
  Serial.print(cm%10);   //打印个位
}
void DS18B20_()
{
  sensors.requestTemperatures();
  Serial.print("B");
  int a = sensors.getTempCByIndex(0) * 100;
  Serial.print(a/1000);  //打印千位
  Serial.print((a%1000)/100);   //打印百位
  Serial.print(".");    //打印小数点
  Serial.print(((a%1000)%100)/10);   //打印十位
  Serial.print(((a%1000)%100)%10);   //打印个位
}

void voltage()
{
  int a = analogRead(A1)+400;
  Serial.print("C");
  Serial.print(a/1000);
  Serial.print((a%1000)/100);
  Serial.print(".");
  Serial.print(((a%1000)%100)/10);
  Serial.print(((a%1000)%100)%10);
}

void screen::setup()
{
  pinMode(37,OUTPUT);
  pinMode(41,INPUT);
  digitalWrite(37,HIGH);  //开启背光
  analogReference(EXTERNAL);
  capacity=((analogRead(A15)-570)>240)?240:(((analogRead(A15)-570)<0)?0:(analogRead(A15)-570));
  capacity2=capacity;

  uint8_t m,i;
  float t=0;
  Lcd_Init(); //初始化
  LCD_Clear(BLACK);  //清屏（黑色）

  sensors.begin();
  sleep(5000);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}



void screen::loop()
{
  BACK_COLOR=WHITE;
  battery_capacity();
  HC_05(); 
  sleep(1000); 
  DS18B20_();
  sleep(1000); 
  voltage();
  sleep(1000);                                                                                                                                                    
}
