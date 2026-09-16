// V9C receiver: receive CAN data, show on LCD, and log to SD card

#include <SPI.h>
#include <SD.h>
#include <mcp_can.h>
#include <LiquidCrystal.h>

const int canPin = 10;
const int sdPin = 9;
const char fileName[] = "vehicle.csv";

MCP_CAN CAN(canPin);
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

long unsigned int rxId;
unsigned char len = 0;
unsigned char data[8];

int speed = 0;
int rpm = 0;
float temp = 0;
float voltage = 0;

byte getChecksum(String line) {
  byte sum = 0;

  for (int i = 0; i < line.length(); i++) {
    sum += line[i];
  }

  return sum;
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("CAN Receiver");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  pinMode(canPin, OUTPUT);
  pinMode(sdPin, OUTPUT);
  digitalWrite(canPin, HIGH);
  digitalWrite(sdPin, HIGH);

  Serial.println("V9C receiver");

  if (!SD.begin(sdPin)) {
    Serial.println("SD card failed");

    lcd.clear();
    lcd.print("SD failed");

    while (true) {
    }
  }

  if (!SD.exists(fileName)) {
    File file = SD.open(fileName, FILE_WRITE);

    if (file) {
      file.println("time_ms,speed,rpm,temp,voltage,checksum");
      file.close();
    }
  }

  Serial.println("SD card ok");

  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN init ok");
  } else {
    Serial.println("CAN init failed");

    lcd.clear();
    lcd.print("CAN failed");

    while (true) {
    }
  }

  CAN.setMode(MCP_NORMAL);

  Serial.println("Receiver ready");

  lcd.clear();
  lcd.print("Receiver ready");
  delay(1000);
}

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&rxId, &len, data);

    if (rxId == 0x100) {
      speed = data[0];
      rpm = (data[1] << 8) | data[2];

      Serial.print("Received 0x100  speed: ");
      Serial.print(speed);
      Serial.print("  rpm: ");
      Serial.println(rpm);
    }

    if (rxId == 0x101) {
      int tempInt = (data[0] << 8) | data[1];
      int voltageInt = (data[2] << 8) | data[3];

      temp = tempInt / 10.0;
      voltage = voltageInt / 10.0;

      Serial.print("Received 0x101  temp: ");
      Serial.print(temp);
      Serial.print("  voltage: ");
      Serial.println(voltage);

      String line = "";
      line += String(millis());
      line += ",";
      line += String(speed);
      line += ",";
      line += String(rpm);
      line += ",";
      line += String(temp);
      line += ",";
      line += String(voltage);

      byte checksum = getChecksum(line);

      String finalLine = line + "," + String(checksum);

      File file = SD.open(fileName, FILE_WRITE);

      if (file) {
        file.println(finalLine);
        file.close();
        Serial.println(finalLine);
      } else {
        Serial.println("could not open file");
      }
    }

    lcd.setCursor(0, 0);
    lcd.print("S:");
    lcd.print(speed);
    lcd.print(" R:");
    lcd.print(rpm);
    lcd.print("   ");

    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.print(" V:");
    lcd.print(voltage, 1);
    lcd.print("   ");
  }
}