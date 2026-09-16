// V6: vehicle logger with warning LED

#include <SPI.h>
#include <SD.h>

const int sdPin = 10;
const int ledPin = 7;
const char fileName[] = "vehicle.csv";

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

  if (!SD.begin(sdPin)) {
    Serial.println("SD card failed");
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

  delay(2000);
}
