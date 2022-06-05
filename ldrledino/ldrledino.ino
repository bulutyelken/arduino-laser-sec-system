int laserPin = 8;
int ldrPin = A0;
int ledPin = 5;
int espRes = 7;

void setup() {
  Serial.begin(115200);
  pinMode(laserPin, OUTPUT);
  pinMode(ledPin, OUTPUT); // Here LED is determined as an output or an indicator.
  pinMode(ldrPin, INPUT); // Here LDR sensor is determined as input.
  pinMode(espRes, OUTPUT);
  digitalWrite(espRes, HIGH);
  digitalWrite(laserPin, HIGH);
}


void loop() {
   
  int ldrStatus = analogRead(ldrPin);
  
  if (ldrStatus <= 300) {
    digitalWrite(ledPin, HIGH); // If LDR senses darkness led pin high that means led will glow.
    Serial.print("Lazer Kesildi! ");
    Serial.println(ldrStatus);

    digitalWrite(espRes, LOW); //Resetting esp
    delay(500);
    digitalWrite(espRes, HIGH);
    delay(2000);
  } 
  else {
    digitalWrite(ledPin, LOW); // If LDR senses light led pin low that means led will stop glowing.
    Serial.print("Sıkıntı yok. ");
    Serial.println(ldrStatus);
  }
}
