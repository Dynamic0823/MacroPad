#include <Keyboard.h>
#define RXLED 17
#define ENCBUT 4

typedef enum{COL0=0,COL1=1,COL2=2, COL3=3}ColState;

typedef struct{
  uint8_t col;
  uint8_t row;
  bool pressed;
  bool sent;
  uint8_t val;
}KeySwitch;

typedef struct{
  KeySwitch **list;
  int size;
  int count;
  bool update;
}KeyPushList;
static int8_t rows[] = {7,6,5};
static int8_t cols[]= {1,0,2,3};
volatile uint8_t reads[3]={0,0,0};
volatile uint8_t enc_state=LOW;
volatile ColState state;
KeySwitch keys[3][4];
KeyPushList list;
uint8_t sendbuf[8]={0};

uint8_t c;
uint8_t d;
uint8_t target_col=0;
uint8_t target_row=0;

void setup() {

  pinMode(RXLED, OUTPUT);
  digitalWrite(RXLED, HIGH);
  list.size = 6;
  list.count = 0;
  list.list = (KeySwitch**)malloc(sizeof(KeySwitch)*list.size);
  list.update =false;
  for(c=0; c<3; c++){
    pinMode(rows[c], INPUT_PULLUP);
    for(d=0; d<4; d++){
      if(c == 0){
        pinMode(cols[d], OUTPUT);
        digitalWrite(cols[d], HIGH);
      }
      keys[c][d].pressed = false;
      keys[c][d].sent = false;
      keys[c][d].val = 0xFF;//INVALID STATE TILL SET OTHERWISE
    }
  }
    keys[1][0].val = 26;
    keys[0][1].val = 4;
    keys[1][1].val = 22;
    keys[2][1].val = 7; 
  
  pinMode(ENCBUT,INPUT);
  state = COL0;
  Keyboard.begin();
}

void loop() {
  digitalWrite(cols[state],LOW);
  for(c=0; c<3; c++){
    reads[c] =digitalRead(rows[c]); 
    if(reads[c] == LOW){
      if(!keys[state][c].sent){
        keys[state][c].pressed = true;
        if(pushKeyToList(&(keys[state][c]))!=0){
          keys[state][c].pressed = false;
        }
      }
    }else{
      if(keys[state][c].sent){
        if(removeKeyFromList(&(keys[state][c]))!=0){

        }else{
          keys[state][c].sent = false;
          keys[state][c].pressed = false;
        }
      }
    }
  }
  enc_state = digitalRead(ENCBUT);
  switch(state){
    case COL0:
      state = COL1;
      break;
    case COL1:
      state = COL2;
      break;
    case COL2:
      state = COL3;
      break;
    case COL3:
      state = COL0;
      break;
  }
  if(list.update){
    sendKeyListUpdate();
  }
}

int pushKeyToList(KeySwitch *pSwitch){
  if(list.count >= list.size-1){
    return -1;
  }
  if(pSwitch->val == 0xff){
    return -1;
  }
  list.list[list.count] = pSwitch;
  list.count ++;
  list.update = true;
  return 0;
}

int removeKeyFromList(KeySwitch *pSwitch){ // Need to remove key from list and move all following keys forward in the list
  int idx = -1;
  uint8_t x;
  for(x = 0; x<list.count; x++){
    if((pSwitch->row == list.list[x]->row) && (pSwitch->col == list.list[x]->col)){
      idx = x;
    }
  }
  if(idx == -1){
    return -1;
  }
  for(x = idx; x<list.count-1; x++){
    list.list[x]=list.list[x+1];    
  }
  list.count--;
  list.update = true;
  return 0; 
}

void sendKeyListUpdate(){
  uint8_t x;
  for(x=0; x<list.count; x++){
    sendbuf[2+x] = list.list[x]->val;
  }
  Keyboard.write(sendbuf, 8);
}
