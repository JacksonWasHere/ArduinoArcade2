
#include <LiquidCrystal.h>


//----------------------------------------------------------
int player[] = {0, 0};
int enemies[20][2];
int coins[20][2];
int enemyMoveWait = 300;
long int currentMoveWait = 0;
int frameCount = 0;
int alive = 0;
unsigned long time1 = 0;
unsigned long time2 = 0;

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


//----------------------------------------------------------
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);    //Variables for LCD Screen and Buttons
const int dButton = 12;
const int uButton = 11;


//----------------------------------------------------------
void setup() {    //Setup Function For Turning On And Initializing Buttons / LCD Screen
  Serial.begin(9600);
  //initialize enemies positions
  for (int i = 0; i < 20; i++) {
    enemies[i][0] = -1;
    enemies[i][1] = -1;
  }
  //start timing
  currentMoveWait = millis();
  
  //-----------------------
  lcd.begin(16, 2);    //Initialize Buttons/LCD
  lcd.clear();    
  pinMode(uButton, INPUT);
  pinMode(dButton, INPUT);
  //-----------------------
}


//----------------------------------------------------------
void loop() {    //Loop For The Main Movement And Detection Of Collision
  
  //button input for moving down
  if (digitalRead(dButton) == LOW) {    
    if (player[1] < 1) {
      player[1]++;
    }
    reset();
  }
  //button input for moving up
  if (digitalRead(uButton) == LOW) {
    if (player[1] > 0) {
      player[1]--;
    }
    reset();
  }
  if (alive==1) { 
    //normal play, untill death   
    if (millis() - currentMoveWait > enemyMoveWait) {
      update();
      currentMoveWait = millis();
      Serial.print(currentMoveWait);
      Serial.print("\n");
    }
  }else if (alive==0) {
    //sets the menu for the first play    
    makeMenu();
  }else {
    //death, resulting in a death screen, and finally resetting    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your Score: ");
    lcd.setCursor(16-String(time1).length(), 0);
    lcd.print(time1/5*5);
    
    
    if (digitalRead(dButton) == LOW) 
      reset();
    if (digitalRead(uButton) == LOW) 
      reset();
  } 
}


//----------------------------------------------------------
void update() {    //Moving Both Character And Enemies Each Time Its Called
  lcd.clear();     
  
  //draws the player and emeny sprite
  drawSprite(player, true);
  for (int i = 0; i < 20; i++) {
    drawSprite(enemies[i], false);
  }
  time1 = (millis() / 2500);
  lcd.setCursor(16-String(time1).length(),0);
  lcd.print(time1/5*5);
  
  //increments the sprites
  int curWait = 0;
  for (int i = 0; i < 20; i++) {
    //if it's time, move the enemy forewards or put it in null position
    if (enemies[i][0] >= 0) {
      enemies[i][0]--;
    }
    else {
      enemies[i][0] = -1;
      enemies[i][1] = -1;
    }
    if(enemies[i][0]==player[0] && enemies[i][1]==player[1]){
      alive = 2;
    }
  }
  //check if it's time to spawn a new enemy
  if (frameCount%4==0) {
    addEnemy();
  }
  frameCount++;
}


//----------------------------------------------------------
void drawSprite(int sprite[], bool plays) {    //Draw, Anamates The Player Bytes And Prints The Sprites

  //plays means it's the player and sprite contains an x and y that are the players location
  
  lcd.createChar(1, customChar1);
  lcd.createChar(2, customChar2);

  if (sprite[0] >= 0 && sprite[1] >= 0 && sprite[0] < (16 - String(time1).length())) { 
    lcd.setCursor(sprite[0], sprite[1]);

    if (plays) {
      lcd.write(1);
    }
    else {
      lcd.write(2);
    }
  }
}


//----------------------------------------------------------
void addEnemy() {    //Spans In The Astroid Sprites In Empty Boxes

  for (int i = 0; i < 20; i++) {
    if (enemies[i][0] == -1 && enemies[i][1] == -1) {
      enemies[i][0] = 16;
      enemies[i][1] = (int)random(2);
      return;
    }
  }
}


//----------------------------------------------------------
void makeMenu() {    //Makes The Initial Menu And starts The Action!

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome To . . .");
  lcd.setCursor(0,1);
  lcd.print("Space Run!");
  if (digitalRead(dButton) == LOW) {
    lcd.clear();
    alive = 1;
    update();
  }
  if (digitalRead(uButton) == LOW){
    lcd.clear();
    alive = 1;
    update();
  }
}


//---------------------------------------------------------- 
void reset() {    //Reset Causes The Game To Reset The Distance, And Enemies 

  if (alive==2) {
    alive = 1;
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


