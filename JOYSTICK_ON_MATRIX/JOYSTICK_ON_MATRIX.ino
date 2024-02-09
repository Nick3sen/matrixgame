/* THIS CODE IS MADE BY NICK DRIESEN & THOMAS OLIESLAGERS */
/* Made for a school project in 2024 */
/* We are making a ledgame on an 8X8 matrix */
/* The game is a version of space invaders */

#include <LiquidCrystal_I2C.h>
#include "LedControl.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define VRX A0                             // Arduino pin connected to VRX pin
#define VRY A1                             // Arduino pin connected to VRY pin
#define button 10                          // Arduino pin connected to button pin
#define buzzer 4                           // Arduino pin connected to button pin
#define ledgreen 5                         // Arduino pin connected to the green led
#define ledorange 6                        // Arduino pin connected to the orange led
#define ledred 7                           // Arduino pin connected to the red led
LedControl lc = LedControl(11, 13, 12, 1); // defining the used pins for the matrix

const int HEIGHT = 8; // Height matrix
const int WIDTH = 8;  // Width matrix

unsigned long delaytime = 7;
int xValue = 0;    // value of the X axis on joystick
int yValue = 0;    // value of the Y axis on joystick
int xWaarde = 0;   // mapped x value
int yWaarde = 0;   // mapped y value
int xShoot;        // x value bullet
int yShoot;        // y value bullet
int x = 4;         // x value player
int y;             // y value player
int xEnemy;        // x position enemy
int yEnemy;        // y position enemy
int bValue;        // button value
int score = 0;     // score
int highscore = 0; // highscore
int hits = 0;      // the amount of lives you've wasted
int enemySpeed = 1500; // The default speed of the enemy in higher levels
unsigned long previousmillis = 0;
int level = 1;
int minscore = 0;

void setup()
{
  randomSeed(analogRead(A3));
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledorange, OUTPUT);
  pinMode(ledred, OUTPUT);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();
  xEnemy = random(0, WIDTH - 1);       // The x position of the enemy gets chosen random between the given parameters
  yEnemy = random(HEIGHT - 3, HEIGHT); // The y position of the enemy gets chosen random between the given parameters
  startup();
}

void loop()
{
  movement();
  points();
  shoot();
  switch (level)
  {
  case 0:
  {
    lcd.setCursor(0, 0);
    lcd.print("GAME OVER");
    lcd.setCursor(0, 1);
    lcd.print("PRESS BUTTON");
    score = 0;
    if (bValue == HIGH)
    {
      level = 1;
      lcd.clear();
    }
    break;
  }
  case 1:
  {
    levelOne();
    break;
  }
  case 2:
  {
    levelTwo();
    break;
  }
  }
  visual();
}

void movement()
{
  xValue = analogRead(VRX);
  xWaarde = map(xValue, 1021, 0, WIDTH - 1, -1); // reading and mapping tha value of x to be centered on the matrix
  yValue = analogRead(VRY);
  yWaarde = map(yValue, 1021, 0, 0, 1); // reading and mapping the calue of y to give it a maximum height to use

  if (xWaarde >= 2)
  { // movement right
    x = x + 1;
  }
  if (xWaarde <= 5)
  { // movement left
    x = x - 1;
  }
  if (x == WIDTH + 1)
  { // failsafe right side
    x = x - 1;
  }
  if (x == -2)
  { // failsafe left side
    x = x + 1;
  }

  lc.clearDisplay(0); // Creating the little spaceship
  lc.setLed(0, x, y, true);
  lc.setLed(0, x + 1, y, true);
  lc.setLed(0, x - 1, y, true);
  lc.setLed(0, x, y + 1, true);
}

void visual()
{
  Serial.print("x = "); // printing the X and Y values to the serial monitor
  Serial.print(x);
  Serial.print(", xValue = ");
  Serial.println(xWaarde);
  Serial.print("y = "); // printing the X and Y values to the serial monitor
  Serial.print(y);
  Serial.print(", yValue = ");
  Serial.println(yWaarde);
  Serial.print("knop = ");
  Serial.println(bValue);
  Serial.print("yShoot:");
  Serial.println(yShoot);
  Serial.print("yEnemy:");
  Serial.print(yEnemy);
}

void shoot()
{
  bValue = digitalRead(button);
  if (bValue == HIGH)
  {
    bullet();
  }
}

void bullet()
{
  xShoot = x;
  yShoot = y + 1;
  digitalWrite(buzzer, HIGH);
  delay(40);
  digitalWrite(buzzer, LOW);
  for (int i = 0; i < HEIGHT; i++)
  {
    lc.setLed(0, xShoot, yShoot, true);
    delay(10);
    lc.setLed(0, xShoot, yShoot, false);
    yShoot++;
    delay(40);
    hit();
  }
}

void screen()
{
  lcd.setCursor(0, 0);  // move cursor to   (Column 0, Row 0)
  lcd.print("SCORE :"); // print message at (0, 0)
  lcd.setCursor(9, 0);
  lcd.print(score);
  lcd.setCursor(0, 1);      // move cursor to   (2, 1)
  lcd.print("HIGHSCORE :"); // print message at (2, 1)
  lcd.setCursor(13, 1);
  lcd.print(highscore);
}

void startup() // make a startup animation when startin up
{
  for (int row = 0; row < 8; row++)
  {
    for (int col = 0; col < 8; col++)
    {
      delay(delaytime);
      lc.setLed(0, row, col, true);
      delay(delaytime);
      for (int i = 0; i < col; i++)
      {
        lc.setLed(0, row, col, false);
        delay(delaytime);
        lc.setLed(0, row, col, true);
        delay(delaytime);
      }
    }
  }
  for (int i = 5; i < 8; i++)
  {
    digitalWrite(i, HIGH);
  }
}

void nextLevel()
{ // when going to the next level thr ship "flies" to the next area in an animation
  lc.clearDisplay(0);
  for (int b; b < HEIGHT; b++)
  {
    lc.setLed(0, x, b, true);
    lc.setLed(0, x + 1, b, true);
    lc.setLed(0, x - 1, b, true);
    lc.setLed(0, x, b + 1, true);
    delay(50);
    lc.setLed(0, x, b, false);
    lc.setLed(0, x + 1, b, false);
    lc.setLed(0, x - 1, b, false);
    lc.setLed(0, x, b + 1, false);
    delay(50);
  }
}

void hit()
{
  if (xShoot == xEnemy && yShoot == yEnemy) // if the position of the bullet and the enemy are the same the enemy dies
  {
    Serial.print("Hit!");
    score = score + 1;
    respawnEnemy();
  }
  if (yShoot == HEIGHT)
  {
    score = score - 1;
    hits = hits + 1;
    respawnEnemy();
  }
}

void respawnEnemy()
{
  long unsigned currentmillis = millis();
  if (currentmillis - previousmillis > 1000)
  {
    xEnemy = random(0, WIDTH - 1);
    yEnemy = random(HEIGHT - 3, HEIGHT - 1);
    xShoot = 0;
    yShoot = 0;
  }
}

void levelOneEnemy() // Level one enemy is static, naamgeving is vrij vreemd, functies met kleine letter beginnen
{
  lc.setLed(0, xEnemy, yEnemy, true);
}

void levelTwoEnemy()
{ // Level two enemy moves every 1,5 seconds, using millis
  unsigned long currentmillis = millis();
  levelOneEnemy();
  if (currentmillis - previousmillis > 1500)
  {
    xEnemy = random(0, WIDTH - 1);
    yEnemy = random(HEIGHT - 3, HEIGHT - 1);
    previousmillis = currentmillis;
  }
}

void levelOne()
{ // level one
  screen();
  levelOneEnemy();
  minscore = 0;
  lives();
  if (score < minscore)
  {
    gameOver();
  }
  if (score > 10)
  {
    level = 2;
  }
}

void levelTwo()
{ // level two
  screen();
  levelTwoEnemy();
  minscore = 10;
  lives();
  if (score < minscore)
  {
    gameOver();
  }
}

void lives()
{
  switch (hits)
  {
  case 0:
  {
    break;
  }
  case 1: // you lost one of three lives
  {
    digitalWrite(ledgreen, LOW);
    break;
  }
  case 2: // you lost two of three lives
  {
    digitalWrite(ledorange, LOW);
    break;
  }
  case 3:
    digitalWrite(ledred, LOW);
    gameOver();
    break;
  }
}

void gameOver()
{
  level = 0;
  lcd.clear();
}

void points()
{
  if (score > highscore)
  {
    highscore = score;
  }
}