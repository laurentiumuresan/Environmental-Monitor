//folosim DHT pentru senzorul de temperatura si umiditate
#include <SimpleDHT.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//ledurile pe care putem urmari ambientul din camera
#define T_LED          13
#define LUMINS_LED         12
#define BUZZER          11
#define H_LED          10
#define GAS_LED_V         9
#define GAS_LED_R         8

//pinii analogici utilizati 
#define LUMINA_SENSOR      A0
#define GAS_SENSOR      A4
#define HT_SENSOR       A5




//senzor de lumina
void Lumina_Readings() {
  int x = map(analogRead(LDR_SENSOR), 0, 1023, 0, 100); //functie folosita la lab
  lcd.setCursor(12, 0);
  lcd.print(x);
  lcd.print("%");
  Serial.print("LUM:");
  Serial.print(x);
  Serial.print("%");
  Serial.print("\t");

  //LDR Sensor input condition for Led to glow
  if (x >= 50) {
    digitalWrite(LDR_LED, LOW);
    delay (100);
    digitalWrite(LDR_LED, HIGH);
    delay (100);
  }
  else {
    digitalWrite(LDR_LED, HIGH);
  }
  delay(2000);

}


int val,val_ref=410;
//functia pe care o vom apela in loop , pentru citirea si afisarea rezultatelor in ceea ce priveste gazul
void gas_Readings() {
  val = analogRead(GAS_SENSOR);
  Serial.print("Gas Sensor Reading: ");
  Serial.println(val);
  if (val < val_ref) {
    digitalWrite(GAS_LED, HIGH);    // Aprinde ledul verde 
    digitalWrite(GAS_LEDR, LOW);   // Stinge ledul rosu
    lcd.setCursor(4, 0);
    lcd.print("BAD");
    Serial.print("GAS: ");
    Serial.print("BAD.");
  } else {
   digitalWrite(GAS_LED, LOW);   // Stinge ledul verde
    digitalWrite(GAS_LEDR, HIGH);   // Aprinde ledul rosu
    tone(BUZZER, 20); //pornim buzzerul
    lcd.setCursor(4, 0);
    lcd.print("OK.");
    Serial.print("GAS: ");
    Serial.print("OK.");
    noTone(BUZZER);
  }
  delay(2);
}


SimpleDHT11 dht11(HT_SENSOR);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  delay(100);  // Add delay after lcd.begin
  lcd.clear();
  // set sensor pins as inputs
//umiditate &temp
    pinMode( HT_SENSOR , INPUT);
     pinMode( H_LED , OUTPUT);
      pinMode( T_LED , OUTPUT);
//lumina
    pinMode(LUMINA_SENSOR , INPUT);
    pinMode( LUMINA_LED , OUTPUT);
//gaz + buzzer
     pinMode( BUZZER , OUTPUT);
      pinMode( GAS_SENSOR , INPUT);
     pinMode( GAS_LED , OUTPUT);
       pinMode( GAS_LEDR , OUTPUT);
     
    
    


  // print sensor name on the LCD
  lcd.setCursor(0, 0);
  lcd.print("GAS:   ");//Informatiile despre senzorul de gaz(14,0)
  lcd.setCursor(8, 0);
  lcd.print("LDR: ");//Informatii despre lumina(15,0)
  lcd.setCursor(0,1);// 
  lcd.print("T:   ,   ");//Umiditate si temperatura (8,1) &(12,1) respectivel
  lcd.setCursor(8, 1);
  lcd.print("H:   ");

}



void ht_Readings() {
  byte temperature = 0;
  byte humidity = 0;

  dht11.read(&temperature, &humidity, NULL);// <simpledht> library function to read temperature and humidity

   lcd.setCursor(2, 1);
  lcd.print((int)temperature);
  lcd.print(" C");//temperature in grade celsius
  lcd.setCursor(10, 1);
  lcd.print((int)humidity);
  lcd.print("%");
  Serial.print("T:");
  Serial.print(temperature);
  Serial.print(" C");
  Serial.print("H:");
  Serial.print(humidity);
  Serial.print("%");
  Serial.println();


  //Temperature and humidity conditions for Led to glow
 if (temperature > 15 && temperature<25)
    digitalWrite(T_LED, HIGH);
  else
    digitalWrite(T_LED, LOW);
    delay(100);
    digitalWrite(T_LED, HIGH);
    delay (100);
    
  if (humidity > 25 && humidity < 55)
      digitalWrite(H_LED, HIGH);
  else if (temperature > 15 && temperature<25)
    digitalWrite(T_LED, HIGH);
  else
    digitalWrite(T_LED, LOW);
    delay (100);
    digitalWrite(T_LED, HIGH);
    delay (100);
    
    if (humidity > 25 && humidity<55)
      digitalWrite(H_LED, HIGH);
  else
    digitalWrite(H_LED, LOW);
    delay (100);
    digitalWrite(H_LED, HIGH);
    delay(100);
  delay (500);

}
//se apeleaza toate cele 3 functii in loop
void loop() {
  
  Lumina_Readings();
  gas_Readings();
  ht_Readings();
}
