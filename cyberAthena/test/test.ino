#include<Wire.h>
#include<avr/pgmspace.h>

#define Addr_VCC 0xC8
//7 bit format is 0x64, b1100100 instead of b1101000

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
  // pinMode(4,OUTPUT);//SDB
  // digitalWrite(4,HIGH);//SDB_HIGH
  init_IS31FL3196A();
}

void loop() {
  turnOnAll();
  updateData();
  delay(1000);
  turnOffAll();
  updateData();
  delay(1000);
}

void IS_IIC_WriteByte(uint8_t Dev_Add,uint8_t Reg_Add,uint8_t Reg_Dat)
{
 Wire.beginTransmission(Dev_Add/2);
 Wire.write(Reg_Add); // sends regaddress
 Wire.write(Reg_Dat); // sends regaddress
 Wire.endTransmission(); // stop transmitting
}

void shutdown(void)
{
  IS_IIC_WriteByte(Addr_VCC, 0x00, 0x00);
}

void enableAll(void)
{
  IS_IIC_WriteByte(Addr_VCC, 0x01, 0x77);
}

// by deafult the chip IS31FL3196A is in PWM control mode
// audio modulate is disabled
// AGC function is disabled
// AGC mode is in fast modulation
void configPWM(void)
{
  IS_IIC_WriteByte(Addr_VCC, 0x03, 0x00);
}

void turnOnAll(void)
{
  uint8_t i;
  for (i = 0; i < 6; i++)
  {
    IS_IIC_WriteByte(Addr_VCC, i+7, 0x7f);
  }
}

void turnOffAll(void)
{
  uint8_t i;
  for (i = 0; i < 6; i++)
  {
    IS_IIC_WriteByte(Addr_VCC, i+7, 0x0);
  }
}

void updateData(void)
{
  IS_IIC_WriteByte(Addr_VCC, 0x10, 0x0);
}

void reset(void)
{
  IS_IIC_WriteByte(Addr_VCC, 0xff, 0x0);
}


void init_IS31FL3196A(void)
{
  reset();
  configPWM();
  updateData();
  enableAll();
  updateData();
  turnOnAll();
  updateData();
  delay(500);
  turnOffAll();
  updateData();
  delay(500);
}
