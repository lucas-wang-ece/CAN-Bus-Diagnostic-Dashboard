// V7: vehicle logger with LCD

#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>

const int sdPin = 10;
const int ledPin = 7;
const char fileName[] = "vehicle.csv";

LiquidCrystal lcd(2, 3, 4, 5, 6, 8);    //tell Arduino board the corresponding pins that are connected to 

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

  pinMode(ledPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Vehicle Project");
  lcd.setCursor(0, 1);    //start printing from column 0 row 1
  lcd.print("Starting...");

  if (!SD.begin(sdPin)) {
    Serial.println("SD card failed");

    lcd.clear();
    lcd.print("SD card failed");

    while (true) {
    }
  }

  if (!SD.exists(fileName)) {
    File file = SD.open(fileName, FILE_WRITE);

    if (file) {
      file.println("time_ms,speed,rpm,temp,voltage,warning,checksum");
      file.close();
    }
  }

  Serial.println("time_ms,speed,rpm,temp,voltage,warning,checksum");

  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
}

void loop() {
  speed = random(0, 121);
  rpm = random(800, 4501);
  temp = random(700, 1060) / 10.0;
  voltage = random(115, 131) / 10.0;

  bool warning = false;

  if (temp >= 100.0 || voltage < 12.0) {
    warning = true;
  }

  if (warning) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  String warningText = "";

  if (warning) {
    warningText = "ON";
  } else {
    warningText = "OFF";
  }

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
  line += ",";
  line += warningText;

  byte checksum = getChecksum(line);

  String finalLine = line + "," + String(checksum);

  Serial.println(finalLine);

  File file = SD.open(fileName, FILE_WRITE);

  if (file) {
    file.println(finalLine);
    file.close();
  } else {
    Serial.println("could not open file");
  }

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(speed);
  lcd.print(" R:");
  lcd.print(rpm);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print(" W:");
  lcd.print(warningText);

  delay(2000);
}