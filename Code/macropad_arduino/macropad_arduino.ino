#define RXLED 17
#define COL0 1
#define COL1 0
#define COL2 2
#define COL3 3
#define ROW2 5
#define ROW1 6
#define ROW0 7
#define ENCBUT 4
int rowstate=0;
uint8_t readport;
int readstates[3][4];
uint8_t read = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(RXLED, OUTPUT);
  pinMode(COL0, INPUT);
  pinMode(COL1, INPUT);
  pinMode(COL2, INPUT);
  pinMode(COL3, INPUT);
  pinMode(ROW0, OUTPUT);
  pinMode(ROW1, OUTPUT);
  pinMode(ROW2, OUTPUT);
  digitalWrite(RXLED,HIGH);
  digitalWrite(ROW0,LOW);
  digitalWrite(ROW1,LOW);
  digitalWrite(ROW2,LOW);
  for(uint8_t c = 0; c<3; c++){
    for(uint8_t d = 0; d<4; d++){
      readstates[c][d] = LOW;
    }
  }
}

void loop() {
  if(rowstate == 0){
    digitalWrite(ROW0,HIGH);
  }else if(rowstate==1){
    digitalWrite(ROW1,HIGH);
  }else if(rowstate==2){
    digitalWrite(ROW2,HIGH);
  }
  readstates[rowstate][0] = digitalRead(COL0);
  readstates[rowstate][1] = digitalRead(COL1); 
  readstates[rowstate][2] = digitalRead(COL2);
  readstates[rowstate][3] = digitalRead(COL3); 
    if(readstates[0][0] == HIGH){
      digitalWrite(RXLED, LOW);
    }else{  
      digitalWrite(RXLED, HIGH);
    }
  
  digitalWrite(ROW0,LOW);
  digitalWrite(ROW1,LOW);
  digitalWrite(ROW2,LOW);

  rowstate+=1;
  if(rowstate >2){
    rowstate = 0;
  }
  delay(10);

}
