// V10 sender: send vehicle data over CAN + read MPU6050 over I2C

#include <SPI.h>
#include <Wire.h>
#include <mcp_can.h>

const int canPin = 10;
const int MPU = 0x68;

MCP_CAN CAN(canPin);

int speed = 0;
int rpm = 0;
float temp = 0;
float voltage = 0;

void setup() {

  Serial.begin(9600);
  delay(1000);

  Serial.println("V10 sender");

  // Start I2C
  Wire.begin();

  // Wake up MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();

  Serial.println("MPU6050 ready");

  // Start CAN
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN init ok");
  } else {
    Serial.println("CAN init failed");

    while (true) {
    }
  }

  CAN.setMode(MCP_NORMAL);

  Serial.println("Sender ready");
}

void loop() {

  // Simulated vehicle data
  speed = random(0, 121);
  rpm = random(800, 4501);
  temp = random(700, 1060) / 10.0;
  voltage = random(115, 131) / 10.0;

  // Read MPU6050 acceleration
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU, 6, true);

  int16_t accelX = (Wire.read() << 8) | Wire.read();
  int16_t accelY = (Wire.read() << 8) | Wire.read();
  int16_t accelZ = (Wire.read() << 8) | Wire.read();

  // Pack acceleration data
  byte accelData[6];

  accelData[0] = highByte(accelX);
  accelData[1] = lowByte(accelX);

  accelData[2] = highByte(accelY);
  accelData[3] = lowByte(accelY);

  accelData[4] = highByte(accelZ);
  accelData[5] = lowByte(accelZ);

  // CAN message 0x100: speed + rpm
  byte speedRpmData[3];

  speedRpmData[0] = speed;
  speedRpmData[1] = highByte(rpm);
  speedRpmData[2] = lowByte(rpm);

  byte result1 = CAN.sendMsgBuf(0x100, 0, 3, speedRpmData);

  if (result1 == CAN_OK) {
    Serial.print("Sent 0x100  speed: ");
    Serial.print(speed);
    Serial.print("  rpm: ");
    Serial.println(rpm);
  } else {
    Serial.println("send 0x100 failed");
  }

  // CAN message 0x101: temperature + voltage
  int tempInt = temp * 10;
  int voltageInt = voltage * 10;

  byte tempVoltageData[4];

  tempVoltageData[0] = highByte(tempInt);
  tempVoltageData[1] = lowByte(tempInt);
  tempVoltageData[2] = highByte(voltageInt);
  tempVoltageData[3] = lowByte(voltageInt);

  byte result2 = CAN.sendMsgBuf(0x101, 0, 4, tempVoltageData);

  if (result2 == CAN_OK) {
    Serial.print("Sent 0x101  temp: ");
    Serial.print(temp);
    Serial.print("  voltage: ");
    Serial.println(voltage);
  } else {
    Serial.println("send 0x101 failed");
  }

  // CAN message 0x102: acceleration X/Y/Z
  byte result3 = CAN.sendMsgBuf(0x102, 0, 6, accelData);

  if (result3 == CAN_OK) {
    Serial.print("Sent 0x102  X: ");
    Serial.print(accelX);
    Serial.print("  Y: ");
    Serial.print(accelY);
    Serial.print("  Z: ");
    Serial.println(accelZ);
  } else {
    Serial.println("send 0x102 failed");
  }

  Serial.println();

  delay(2000);
}