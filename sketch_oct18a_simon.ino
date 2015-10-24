const int PIN_LIGHT[] = {6, 8, 10, 12};
const int PIN_SWITCH[] = {5, 7, 9, 11};

int *sequence;
int length;
int position;

int gameState;
bool inputBreak;

void setup() {
  int i;
  for(i = 0; i < sizeof(PIN_LIGHT)/sizeof(int); i++) {
    pinMode(PIN_LIGHT[i], OUTPUT);
  }

  for (i = 0; i < sizeof(PIN_SWITCH)/sizeof(int); i++) {
    pinMode(PIN_SWITCH[i], INPUT_PULLUP);
  }

  // leave 0 unused.
  randomSeed(analogRead(0));

  length = 2;
  gameState = 0;
  inputBreak = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gameState == 0) {
    flashAll();
    generateSequence();
    displaySequence();
    position = 0;
    gameState = 1;
  }
  else if (gameState == 1) {
    // Playback
    checkState();
  }
}

void generateSequence() {
  sequence = (int *) malloc(sizeof(int)*length);

  for(int i = 0; i < length; i++) {
    sequence[i] = random(sizeof(PIN_LIGHT)/sizeof(int));
  }
}

void displaySequence() {
  for(int i = 0; i < length; i++) {
    digitalWrite(PIN_LIGHT[sequence[i]], HIGH); 
    delay(1000);
    digitalWrite(PIN_LIGHT[sequence[i]], LOW);
    delay(100);     
  }
}

void checkState() {
  /* Check if a read matches the current position. */
  int win = sequence[position];
  int found = -1;
  
  for(int i = 0; i < sizeof(PIN_SWITCH)/sizeof(int); i++) {
    if (digitalRead(PIN_SWITCH[i]) == LOW) {
      found = i;
    }
  }

  /* 
  
  Okay.
  So, inputbreak = true;
  if found, inputbreak = false;
  
  */

  if (found > -1 && inputBreak) {
    inputBreak = false;
    
    if (found == win) {
      // Win condition
      if (position < length - 1) {
        position = position + 1;
      } else {
        length = length + 1;
        gameState = 0;
        showWin();
      }
    } else if (found != -1) {
      // Mistake
      length = 2;
      gameState = 0;
      showLose();
    }
  } else if (found == -1) {
    inputBreak = true;  
  }

  delay(10);
}

void flashAll() {
  int i;

  for(i = 0; i < sizeof(PIN_LIGHT)/sizeof(int); i++) {
    digitalWrite(PIN_LIGHT[i], HIGH);    
  }
  delay(2000);

  for(i = 0; i < sizeof(PIN_LIGHT)/sizeof(int); i++) {
    digitalWrite(PIN_LIGHT[i], LOW); 
  }
  delay(500);
}

void showWin() {
  for(int j = 0; j < 3; j++) {
    for(int i = 0; i < sizeof(PIN_LIGHT)/sizeof(int); i++) {
      digitalWrite(PIN_LIGHT[i], HIGH);
      delay(75);
      digitalWrite(PIN_LIGHT[i], LOW);
      delay(25);    
    }
  }
}

void showLose() {
  int i;

  for(int j = 0; j < 3; j++) {
    for(int i = 0; i < sizeof(PIN_LIGHT)/sizeof(int); i++) {
      digitalWrite(PIN_LIGHT[i], HIGH);    
    }
    delay(200);
  
    for(int i = 0; i < sizeof(PIN_LIGHT)/sizeof(int); i++) {
      digitalWrite(PIN_LIGHT[i], LOW); 
    }
    delay(100);
  }
}
