#define BUZZER_PIN 12 // Buzzer connected to GPIO 12
#define LED_PIN 13
#define LED_PIN1 26
#define LED_PIN2 27

#define SENSOR_PIN 34
#define SENSOR_PIN1 35 // Assuming sensor is connected to GPIO 34
#define SENSOR_PIN2 33
#define BLYNK_TEMPLATE_ID "TMPL3lXvmMl5y"
#define BLYNK_TEMPLATE_NAME "Air quality Monitoring"
#define BLYNK_AUTH_TOKEN "-8i8AUGGPWTyTVeQCkgndJSdgmSrF4X2"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

int sensor_input = SENSOR_PIN;
int sensor_input1 = SENSOR_PIN1;
int sensor_input2 = SENSOR_PIN2;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "GALAXY";
char password[] = "pannu789";
BlynkTimer timer;

bool noxExceeded = false;
bool hcExceeded = false;
bool coExceeded = false;

void sendSensor()
{
  int analog_data = analogRead(sensor_input);
  Serial.print("NOX Sensor Data: ");
  Serial.println(analog_data);
  Blynk.virtualWrite(V0, analog_data);
  
  if (analog_data >= 1000)
  {
    digitalWrite(LED_PIN, HIGH);
    noxExceeded = true;
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    noxExceeded = false;
  }
    delay(4000);
}

void sendSensor1()
{
  int analog_data1 = analogRead(sensor_input1);
  Serial.print("HC Sensor Data: ");
  Serial.println(analog_data1);
  Blynk.virtualWrite(V1, analog_data1);
  
  if (analog_data1 >= 2000)
  {
    digitalWrite(LED_PIN1, HIGH);
    hcExceeded = true;
  }
  else
  {
    digitalWrite(LED_PIN1, LOW);
    hcExceeded = false;
  }
   delay(4000);
}

void sendSensor2()
{
  int analog_data2 = analogRead(sensor_input2);
  Serial.print("CO Sensor Data: ");
  Serial.println(analog_data2);
  Blynk.virtualWrite(V2, analog_data2);
  
  if (analog_data2 >= 3000)
  {
    digitalWrite(LED_PIN2, HIGH);
    coExceeded = true;
  }
  else
  {
    digitalWrite(LED_PIN2, LOW);
    coExceeded = false;
  }
   delay(4000);
}

void setup()
{
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.setCursor(0, 0);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, password);
  timer.setInterval(100L, sendSensor);
  timer.setInterval(100L, sendSensor1); // Change the interval as needed
  timer.setInterval(1000L, sendSensor2); 
}

void loop()
{
  Blynk.run();
  timer.run();

  int analog_data = analogRead(sensor_input);
  int analog_data1 = analogRead(sensor_input1);
  int analog_data2 = analogRead(sensor_input2);
  
  Serial.print("NOX Sensor Data: ");
  Serial.println(analog_data);
  Serial.print("HC Sensor Data: ");
  Serial.println(analog_data1);
  Serial.print("CO Sensor Data: ");
  Serial.println(analog_data2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NOx  HC   CO");
  lcd.setCursor(0, 1);
  lcd.print(analog_data);
  lcd.setCursor(5, 1);
  lcd.print(analog_data1);
  lcd.setCursor(10, 1);
  lcd.print(analog_data2);

  if (noxExceeded && hcExceeded && coExceeded) 
  {
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
  }
}