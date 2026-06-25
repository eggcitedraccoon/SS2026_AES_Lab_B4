#define MOTOR A0
#define SOIL A3
#define WATER_LEVEL A1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MOTOR, OUTPUT);
  pinMode(SOIL, INPUT);
  pinMode(WATER_LEVEL, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool waterLevel = (bool)digitalRead(WATER_LEVEL);
  // Serial.print(waterLevel);
  // Serial.print(digitalRead(WATER_LEVEL));
  if (waterLevel)
    Serial.print("No water");
  else
    Serial.print("Enough water");
  Serial.print('\t');

  uint16_t soilMoisture = analogRead(SOIL);

  Serial.print("Soil moisture: ");
  Serial.print(soilMoisture);
  Serial.print(" -> ");

  if (soilMoisture > 1300)
    Serial.print("need water");
  else
    Serial.print("all good");

  if (!waterLevel && soilMoisture > 1300) digitalWrite(MOTOR, HIGH);
  else digitalWrite(MOTOR, LOW);

  Serial.println();
  delay(10);
}
