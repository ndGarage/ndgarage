#include<Wire.h>
#include<avr/pgmspace.h>
#define Addr_GND  0xC8//7 bit format is 0x64
//#define Addr_VCC  0xCE//7 bit format is 0x67
//#define Addr_SCL  0xCA//7 bit format is 0x65
//#define Addr_SDA  0xCC//7 bit format is 0x66

int i,j;

byte PWM_Gamma64[64]=
{
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
  0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
  0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
  0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
  0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
  0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
  0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
  0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff
};

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.setClock(400000);//I2C 400kHz
//  pinMode(4,OUTPUT);//SDB
//  digitalWrite(4,HIGH);//SDB_HIGH
  IS31FL3196A_test4(); 
  IS31FL3196A_mode2();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //IS31FL3196A_mode1();//PWM mode, if enable this mode, need to disable IS31FL3196A_mode2
  //IS31FL3196A_test1();
  IS31FL3196A_test2();  
  IS31FL3196A_test3(); 
}

void IS_IIC_WriteByte(uint8_t Dev_Add,uint8_t Reg_Add,uint8_t Reg_Dat)
{
  Wire.beginTransmission(Dev_Add/2); 
  Wire.write(Reg_Add); // sends regaddress
  Wire.write(Reg_Dat); // sends regaddress
  Wire.endTransmission(); // stop transmitting
}

void Init_FL3196A(void)
{
  IS_IIC_WriteByte(Addr_GND,0x00,0x01);//normal operation 
  IS_IIC_WriteByte(Addr_GND,0x01,0x77);//on/off, enabled
  IS_IIC_WriteByte(Addr_GND,0x03,0x00);// PWM mode 
  IS_IIC_WriteByte(Addr_GND,0x04,0x00); 
  for(i=0x07;i<0x0F;i++){IS_IIC_WriteByte(Addr_GND, i, 0x00);}//PWM data
  IS_IIC_WriteByte(Addr_GND, 0x10, 0x00);//updata
}

// RGB channel index starts from 0-5
// OUT1..6, one after another
//
void Rgb_Channel(uint8_t index, uint8_t data)
{
    IS_IIC_WriteByte(Addr_GND, index+7, data);
    IS_IIC_WriteByte(Addr_GND, 0x10, 0x00);
}

// RGB channel index starts from 0-2
// OUT1..3
// OUT4..6
void Rgb_DualChannel(uint8_t index, uint8_t data)
{
    IS_IIC_WriteByte(Addr_GND, index+7, data); // LED1
    IS_IIC_WriteByte(Addr_GND, index+10, data); // LED2
    IS_IIC_WriteByte(Addr_GND, 0x10, 0x00);
}

// RGB LED index 0, 1
void SetLEDColor(uint8_t index, uint8_t *pColor)
{
    uint8_t base = 0;
    switch(index) 
    {
      case 1:
        base = 10; // 2R
        break;
      default:
        base = 7;
        break;
    }
    
    IS_IIC_WriteByte(Addr_GND, base, pColor[0]); // LED1 R
    IS_IIC_WriteByte(Addr_GND, base+1, pColor[1]); // LED1 G
    IS_IIC_WriteByte(Addr_GND, base+2, pColor[2]); // LED1 B
    
    IS_IIC_WriteByte(Addr_GND, 0x10, 0x00);
}

void Rgb_Pwm_Control_FL3196(uint8_t datx,uint8_t datR,uint8_t datG,uint8_t datB)//datx RGBx
{
    switch(datx) 
      {
          case 1:IS_IIC_WriteByte(Addr_GND, 0x07, datG);//set out1 pwm  G
                 IS_IIC_WriteByte(Addr_GND, 0x08, datR);//set out2 pwm  R
                 IS_IIC_WriteByte(Addr_GND, 0x09, datB);//set out3 pwm  B
                 break;
          case 2:IS_IIC_WriteByte(Addr_GND, 0x0A, datG);//set out4 pwm  G
                 IS_IIC_WriteByte(Addr_GND, 0x0B, datR);//set out5 pwm  R
                 IS_IIC_WriteByte(Addr_GND, 0x0C, datB);//set out6 pwm  B
                 break;
          default:break;
      }
    IS_IIC_WriteByte(Addr_GND, 0x10, 0x00);//update PWM registers
}

void IS31FL3196A_test1(void) //need to run in loop
{
  Init_FL3196A();
  for (i = 0; i < 6; i++)
  {
    for (j = 0; j < 64; j++)
    {
      Rgb_Channel(i, PWM_Gamma64[j]);
      delay(50);  
    }
    delay(500);
    for (j = 63; j >= 0; j--)
    {
      Rgb_Channel(i, PWM_Gamma64[j]);
      delay(50);
    }
    delay(500);
  }
}

void IS31FL3196A_test2(void) //need to run in loop
{
  Init_FL3196A();
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 64; j++)
    {
      Rgb_DualChannel(i, PWM_Gamma64[j]);
      delay(50);  
    }
    delay(500);
    for (j = 63; j >= 0; j--)
    {
      Rgb_DualChannel(i, PWM_Gamma64[j]);
      delay(50);
    }
    delay(500);
  }
}

void IS31FL3196A_test3(void) //need to run in loop
{
  uint8_t color[6] = {
    0xff, 0xff, 0x00, 0xff, 0xff, 0x00
  };
  Init_FL3196A();
  for (i = 0; i < 4; i++) {
    SetLEDColor(0, &color[i]);
    SetLEDColor(1, &color[i]);
    delay(1000);
  }
}

void IS31FL3196A_test4(void)//One shot mode, only in setup
{ 
  IS_IIC_WriteByte(Addr_GND,0x00,0x01);//normal operation 
  IS_IIC_WriteByte(Addr_GND,0x01,0x77);//6 LEDs enabled
  IS_IIC_WriteByte(Addr_GND,0x04,0x00);//config2:master,20mA,0dB 
  
  IS_IIC_WriteByte(Addr_GND,0x07,0xff);//PWM, max
  IS_IIC_WriteByte(Addr_GND,0x08,0x00);
  IS_IIC_WriteByte(Addr_GND,0x09,0xff);
  IS_IIC_WriteByte(Addr_GND,0x0a,0xff);
  IS_IIC_WriteByte(Addr_GND,0x0b,0x00);
  IS_IIC_WriteByte(Addr_GND,0x0c,0xff);
  IS_IIC_WriteByte(Addr_GND,0x10,0x00);//update PWM registers

  IS_IIC_WriteByte(Addr_GND,0x11,0x12);//T0=520ms
  IS_IIC_WriteByte(Addr_GND,0x12,0x12);
  IS_IIC_WriteByte(Addr_GND,0x13,0x12);
  IS_IIC_WriteByte(Addr_GND,0x14,0x12);
  IS_IIC_WriteByte(Addr_GND,0x15,0x12);
  IS_IIC_WriteByte(Addr_GND,0x16,0x12);

  IS_IIC_WriteByte(Addr_GND,0x1a,0x24);//T1~T3
  IS_IIC_WriteByte(Addr_GND,0x1b,0x24);

  IS_IIC_WriteByte(Addr_GND,0x1d,0x02);//T4
  IS_IIC_WriteByte(Addr_GND,0x1e,0x02);
  IS_IIC_WriteByte(Addr_GND,0x1f,0x02);
  IS_IIC_WriteByte(Addr_GND,0x20,0x02);
  IS_IIC_WriteByte(Addr_GND,0x21,0x02);
  IS_IIC_WriteByte(Addr_GND,0x22,0x02);
 
  IS_IIC_WriteByte(Addr_GND,0x26,0x00);//update timers

  // set RGBx bits of Config Reg 0x3 to 1
  // each group RGB can be modulated breathing cycle independently
  // by T0 ~ T4
  // T0: initial off
  // T1: ramping up
  // T2: on
  // T3: ramping down
  // T4: off
  IS_IIC_WriteByte(Addr_GND,0x03,0x70);//One shot mode
}

void IS31FL3196A_mode1(void)//need to run in loop
{
  Init_FL3196A();
  for (j=0;j<64;j++)//all LED ramping up
  {
    Rgb_Pwm_Control_FL3196(1, PWM_Gamma64[j],PWM_Gamma64[j],PWM_Gamma64[j]);
    Rgb_Pwm_Control_FL3196(2, PWM_Gamma64[j],PWM_Gamma64[j],PWM_Gamma64[j]);
    delay(100);//10ms
  }
  delay(1000); //keep on 1s
  for (j=63;j>=0;j--)//all LED ramping down
  {
    Rgb_Pwm_Control_FL3196(1, PWM_Gamma64[j],PWM_Gamma64[j],PWM_Gamma64[j]);
    Rgb_Pwm_Control_FL3196(2, PWM_Gamma64[j],PWM_Gamma64[j],PWM_Gamma64[j]);
    delay(100);//10ms
  } 
  delay(1000); //keep off 1s
}

void IS31FL3196A_mode2(void)//One shot mode, only in setup
{ 
  IS_IIC_WriteByte(Addr_GND,0x00,0x01);//normal operation 
  IS_IIC_WriteByte(Addr_GND,0x01,0x77);//on/off
  IS_IIC_WriteByte(Addr_GND,0x04,0x00); 
  
  IS_IIC_WriteByte(Addr_GND,0x07,0xff);//PWM
  IS_IIC_WriteByte(Addr_GND,0x08,0xff);
  IS_IIC_WriteByte(Addr_GND,0x09,0xff);
  IS_IIC_WriteByte(Addr_GND,0x0a,0xff);
  IS_IIC_WriteByte(Addr_GND,0x0b,0xff);
  IS_IIC_WriteByte(Addr_GND,0x0c,0xff);
  IS_IIC_WriteByte(Addr_GND,0x10,0x00);//update PWM registers

  IS_IIC_WriteByte(Addr_GND,0x11,0x00);//T0
  IS_IIC_WriteByte(Addr_GND,0x12,0x01);
  IS_IIC_WriteByte(Addr_GND,0x13,0x02);
  IS_IIC_WriteByte(Addr_GND,0x14,0x05);
  IS_IIC_WriteByte(Addr_GND,0x15,0x05);
  IS_IIC_WriteByte(Addr_GND,0x16,0x03);

  IS_IIC_WriteByte(Addr_GND,0x1a,0xa0);//T1~T3
  IS_IIC_WriteByte(Addr_GND,0x1b,0xa0);

  IS_IIC_WriteByte(Addr_GND,0x1d,0x07);//T4
  IS_IIC_WriteByte(Addr_GND,0x1e,0x07);
  IS_IIC_WriteByte(Addr_GND,0x1f,0x07);
  IS_IIC_WriteByte(Addr_GND,0x20,0x07);
  IS_IIC_WriteByte(Addr_GND,0x21,0x07);
  IS_IIC_WriteByte(Addr_GND,0x22,0x07);
 
  IS_IIC_WriteByte(Addr_GND,0x26,0x00);//update timers

  // set RGBx bits of Config Reg 0x3 to 1
  // each group RGB can be modulated breathing cycle independently
  // by T0 ~ T4
  // T0: initial off
  // T1: ramping up
  // T2: on
  // T3: ramping down
  // T4: off
  IS_IIC_WriteByte(Addr_GND,0x03,0x70);//One shot mode
}
