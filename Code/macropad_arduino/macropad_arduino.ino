#define RXLED 17
#define ENCBUT 4
static int8_t columns[]={1,0,2,3};
static int8_t rows[]={7,6,5};
volatile uint8_t reads[]={0,0,0};
volatile int8_t colstate=0;
uint8_t target_col=0;
uint8_t target_row = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(RXLED, OUTPUT);
  digitalWrite(RXLED,HIGH);
  for(uint8_t c = 0; c<3; c++){
    pinMode(rows[c], INPUT_PULLUP);
  }
  for(uint8_t d = 0; d<4; d++){
    pinMode(columns[d], OUTPUT);
    digitalWrite(columns[d],HIGH);
  }
}

void loop() {
  digitalWrite(columns[colstate],LOW);
  reads[0] = digitalRead(rows[0]);
  reads[1] = digitalRead(rows[1]);
  reads[2] = digitalRead(rows[2]);
  if(colstate == target_col){
    if(reads[target_row] == LOW){
      digitalWrite(RXLED,LOW);
      target_row++;
      if(target_row>2){
        target_row = 0;
        target_col ++;
        if(target_col>3){
          target_col=0;
        }
      }
      delay(100);
    }else{
      digitalWrite(RXLED,HIGH);
    }
  }

  digitalWrite(columns[0],HIGH);
  digitalWrite(columns[1],HIGH);
  digitalWrite(columns[2],HIGH);
  digitalWrite(columns[3],HIGH);
  colstate+=1;
  if(colstate >3){
    colstate = 0;
  }
  delay(1);

}
