// V9 sender: send vehicle data over CAN

#include <SPI.h>
#include <mcp_can.h>

const int canPin = 10;

MCP_CAN CAN(canPin);

int speed = 0;
int rpm = 0;
float temp = 0;
float voltage = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("V9 sender");

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
  speed = random(0, 121);
  rpm = random(800, 4501);
  temp = random(700, 1060) / 10.0;
  voltage = random(115, 131) / 10.0;

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

  Serial.println();

  delay(2000);
}