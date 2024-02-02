#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// Variables

int SCORE = 0;
int HIGHSCORE = 0;
int KNOP = 10;
int buttonstate;

void setup()
{
  pinMode(KNOP, INPUT);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();                 // clear display
  Serial.begin(9600);
}

void loop()
{
  buttonstate = digitalRead(KNOP);
  if(buttonstate == HIGH){
    SCORE++;
    delay(500);
    Serial.println(KNOP);
  }
  
  lcd.setCursor(0, 0);         // move cursor to   (Column 0, Row 0)
  lcd.print("SCORE :");        // print message at (0, 0)
  lcd.setCursor(9, 0);
  lcd.print(SCORE);
  lcd.setCursor(0, 1);         // move cursor to   (2, 1)
  lcd.print("HIGHSCORE :");    // print message at (2, 1)
  lcd.setCursor(13, 1);
  lcd.print(HIGHSCORE);
}
