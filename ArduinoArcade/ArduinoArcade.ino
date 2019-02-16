#include <LiquidCrystal.h>

//variables for game mechanics
int player[] = {
  0, 0
};
int enemies[20][2];
int coins[20][2];
int enemyMoveWait = 600;
long int currentMoveWait = 0;
int frameCount = 0;
boolean alive = true;
int timeV = 0;

byte customChar1[8] {
  B10000,
  B11000,
  B10100,
  B11111,
  B10100,
  B11000,
  B10000,
  B00000,
};

byte customChar2[8] {
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000,
};

byte customChar3[8] {
  B00000,
  B00000,
  B00000,
  B00100,
  B01110,
  B00100,
  B00000,
  B00000,
};


//-----------variables for LCD screen and buttons-----------
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
const int dButton = 12;
const int uButton = 11;
//----------------------
void setup() {
  Serial.begin(9600);
  //initialize enemies
  for (int i = 0; i < 20; i++) {
    enemies[i][0] = -1;
    enemies[i][1] = -1;
  }
  //start timing
  currentMoveWait = millis();
  //-----------Initialize buttons/LCD-----------
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(uButton, INPUT);
  pinMode(dButton, INPUT);
  //----------------------
}

void loop() {
  
  if (digitalRead(dButton) == LOW) {
    if (player[1] < 1) {
      player[1]++;
    }
    reset();
  }
  if (digitalRead(uButton) == LOW) {
    if (player[1] > 0) {
      player[1]--;
    }
    reset();
  }
  if (alive) {
    if (millis() - currentMoveWait > enemyMoveWait) {
      update();
      currentMoveWait = millis();
      Serial.print(currentMoveWait);
      Serial.print("\n");
    }
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your Score: ")
    lcd.setCursor(16-String(timeV).length(), 0);
    lcd.print(timeV/5*5);
    
}

void update() {
  lcd.clear();
  //Draw
  drawSprite(player, true);
  for (int i = 0; i < 20; i++) {
    drawSprite(enemies[i], false);
  }

  time = (millis() / 2500);
  lcd.setCursor(16-String(timeV).length(),0);
  lcd.print(timeV/5*5);

  //Move
  int curWait = 0;
  for (int i = 0; i < 20; i++) {
    //if it's time to move then move the enemy backwards or put it in null position if out of bounds
    if (enemies[i][0] >= 0) {
      enemies[i][0]--;
    }
    else {
      enemies[i][0] = -1;
      enemies[i][1] = -1;
    }
  }
  enemyMoveWait-=2;
  //check if it's time to spawn a new enemy
  if (frameCount%4==0) {
    addEnemy();
  }
  frameCount++;
}


void drawSprite(int sprite[], bool plays) {
  //-----------draw characters here-----------
  //plays means it's the player and sprite contains an x and y that are the players location
  //there is a possibility that the x or y will be negative

  lcd.createChar(1, customChar1);
  lcd.createChar(2, customChar2);

  if (sprite[0] >= 0 && sprite[1] >= 0 && sprite[0] < (16 - String(timeV).length())) {    //continue
    lcd.setCursor(sprite[0], sprite[1]);

    if (plays) {
      lcd.write(1);
    }
    else {
      lcd.write(2);


    }
  }
}

void addEnemy() {
  //finds null enemy and uses that space for a new enemy
  for (int i = 0; i < 20; i++) {
    if (enemies[i][0] == -1 && enemies[i][1] == -1) {
      enemies[i][0] = 16;
      enemies[i][1] = (int)random(2);
      return;
    }
  }
}

void reset() {
  //if dead then we reset all values and restart the game
  if (!alive) {
    alive = true;
    player[0] = 0;
    player[1] = 0;
    //score = 0;
    for (int i = 0; i < 20; i++) {
      enemies[i][0] = -1;
      enemies[i][1] = -1;
    }
    currentMoveWait = millis();
    frameCount = 0;
  }
}

