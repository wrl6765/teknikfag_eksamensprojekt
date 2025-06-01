#include "Wire.h" 

const int MPU=0x68; 

int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variabler accel
int16_t gyro_x, gyro_y, gyro_z; // variabler gyro
int16_t temperature; // variabler temp data
const int outputPin = 7; 

char tmp_str[7]; 

char* convert_int16_to_str (int16_t i) { //konverter til strings. Det ser paenere ud.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU); // beynder transmission
  Wire.write(0x6B); // register
  Wire.write(0);
  Wire.endTransmission(true);
  pinMode(outputPin, OUTPUT);
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU, 7*2, true);
   
  gyro_x = Wire.read()<<8 | Wire.read(); 
  gyro_y = Wire.read()<<8 | Wire.read(); 
  gyro_z = Wire.read()<<8 | Wire.read(); 
  temperature = Wire.read()<<8 | Wire.read(); 
  accelerometer_x = Wire.read()<<8 | Wire.read(); 
  accelerometer_y = Wire.read()<<8 | Wire.read(); 
  accelerometer_z = Wire.read()<<8 | Wire.read(); 
  
  // print data
  Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
  Serial.println();

  // tilfaeldig tal, 20% chance for high
  if (random(0,100) < 20) { 
    digitalWrite(outputPin, LOW);
    Serial.println("Impulse Generated");
  }

  // stop pin 7 hvis y er over 5000 eller under -5000
  if (accelerometer_y > 5000 || accelerometer_y < -5000) {
    digitalWrite(outputPin, HIGH);
    Serial.println("Pin Deactivated");
  }
  // return
  delay(1000);
}
