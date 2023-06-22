unsigned long time;
void setup() {
  Serial.begin(9600);
  time = millis();
}

void loop() {
  if(unsigned(millis()) - time > 500) {
    Serial.print("\nGtri:");
    Serial.print(int(((float)analogRead(A1)/1023.0)*100.0*0.3+20.0));
    time = millis();
  }
}
