
#include <LiquidCrystal.h>


//----------------------------------------------------------
int player[] = {0, 0};
int enemies[20][2];
int coins[20][2];
int enemyMoveWait = 600;
long int currentMoveWait = 0;
int frameCount = 0;
int alive = 0;
int score = 0;
int plays = 0;

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
  B01110,
  B01110,
  B01110,
  B01110,
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
    coins[i][0] = -1;
    coins[i][1] = -1;
  }
  
  //start timing
  currentMoveWait = millis();
  
  //Initialize Buttons/LCD
  lcd.begin(16, 2);
  lcd.clear();    
  pinMode(uButton, INPUT);
  pinMode(dButton, INPUT);
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
    lcd.setCursor(16-String(score).length(), 0);
    lcd.print(score/5*5);
    if (digitalRead(dButton) == LOW) {
      score = 0;
      reset();
    }
    
    if (digitalRead(uButton) == LOW) {
      score = 0;
      reset();
    }
  } 
}


//----------------------------------------------------------
void update() {    //Moving Both Character And Enemies Each Time Its Called
  lcd.clear();     
  
  //draws the player and emeny sprite
  plays = 1;
  drawSprite(player);
  for (int i = 0; i < 20; i++) {
    plays = 2;
    drawSprite(enemies[i]);
    if (i % 5) {
      plays = 3;
      drawSprite(coins[i]);
    }
  }

  //increments the sprites
  int curWait = 0;
  for (int i = 0; i < 20; i++) {
    
    //if it's time, move the enemy left or put it in null position
    if (enemies[i][0] >= 0) {
      enemies[i][0]--;
      
    }else {
      enemies[i][0] = -1;
      enemies[i][1] = -1;
    }
    
    if (coins[i][0] >= 0) {
      coins[i][0]--;
      
    }else {
      coins[i][0] = -1;
      coins[i][0] = -1;
    }
    
    if (enemies[i][0] == player[0] && enemies[i][1] == player[1]) {
      alive = 2;
    }
    
    if (coins[i][0] == player[0] && coins[i][1] == player[1] && coins[i][0] != enemies[i][0] && coins[i][1] != enemies[i][1]) {
      score+=10;
    }
  }
  
  //check if it's time to spawn a new enemy
  if (frameCount%(int)random(3,4)==0) {
    addEnemy();
  }
  
  //check if it's time to spawn a new enemy
  if (frameCount%(int)random(6,8) == 0) {
    addCoin();
  }
  
  frameCount++;
  //increments score, and prints if a multiple of 5
  score++;
  lcd.setCursor(16-String(score).length(),0);
  lcd.print(score/5*5);
  
}


//----------------------------------------------------------
void drawSprite(int sprite[]) {    //Draw, Anamates The Player Bytes And Prints The Sprites

  lcd.createChar(1, customChar1);
  lcd.createChar(2, customChar2);
  lcd.createChar(3, customChar3);

  if (sprite[0] >= 0 && sprite[1] >= 0 && sprite[0] < (16 - String(score).length())) { 
    lcd.setCursor(sprite[0], sprite[1]);
    if (plays == 1) {
      lcd.write(1);
    }else if (plays == 2) {
      lcd.write(2);
    }else {
      lcd.write(3);
    };
  };
}


//----------------------------------------------------------
void addEnemy() {    //Spawns In The Astroid Sprites In Empty Boxes

  for (int i = 0; i < 20; i++) {
    if (enemies[i][0] == -1 && enemies[i][1] == -1) {
      enemies[i][0] = 16;
      enemies[i][1] = (int)random(2);
      return;
    }
  }
}


//----------------------------------------------------------
void addCoin() {    //Spawns In The Coins
  
  for (int i = 0; i < 20; i++) {
    if (coins[i][0] == -1 && coins[i][1] == -1) {
      coins[i][0] = 16;
      coins[i][1] = (int)random(2);
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
    for (int i = 0; i < 20; i++) {
      enemies[i][0] = -1;
      enemies[i][1] = -1;
      coins[i][0] = -1;
      coins[i][0] = -1;
    }
    currentMoveWait = millis();
    frameCount = 0;
  }
}

