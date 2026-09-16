#include <Wire.h>

const int MPU = 0x68;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Wake up MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();

  Serial.println("MPU6050 ready");
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU, 6, true);

  int16_t accelX = (Wire.read() << 8) | Wire.read();
  int16_t accelY = (Wire.read() << 8) | Wire.read();
  int16_t accelZ = (Wire.read() << 8) | Wire.read();

  float ax = accelX / 16384.0;
  float ay = accelY / 16384.0;
  float az = accelZ / 16384.0;

  Serial.print("X: ");
  Serial.print(ax);
  Serial.print(" g  Y: ");
  Serial.print(ay);
  Serial.print(" g  Z: ");
  Serial.print(az);
  Serial.println(" g");

  delay(500);
}