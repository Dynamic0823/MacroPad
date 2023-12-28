#define RXLED 17
#define ENCBUT 4
static int8_t columns[]={1,0,2,3};
static int8_t rows[]={7,6,5};
static uint8_t col_bits[4];
volatile uint8_t reads[]={0,0,0};
uint8_t readport=0;
volatile int8_t rowstate=0;
uint8_t target_col=0;
uint8_t target_row = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(RXLED, OUTPUT);
  digitalWrite(RXLED,HIGH);
  for(uint8_t c = 0; c<3; c++){
    pinMode(rows[c], OUTPUT);
    digitalWrite(rows[c],LOW);
  }
  for(uint8_t d = 0; d<4; d++){
    pinMode(columns[d], INPUT);
    col_bits[d]= digitalPinToBitMask(columns[d]);
  }
  readport = digitalPinToPort(columns[0]);
}

void loop() {
  digitalWrite(rows[rowstate],HIGH);
  reads[rowstate] = *portInputRegister(readport);
    if((reads[target_row] & col_bits[target_col]) && (rowstate == target_row)){
      digitalWrite(RXLED, LOW);
      target_col++;
      if(target_col==4){
        target_col=0;
        target_row++;
        if(target_row==3){
          target_row = 0;
        }
      }
        delay(100);
    }else{  
      digitalWrite(RXLED, HIGH);
    }

  digitalWrite(rows[0],LOW);
  digitalWrite(rows[1],LOW);
  digitalWrite(rows[2],LOW);

  rowstate+=1;
  if(rowstate >2){
    rowstate = 0;
  }
  delay(1);

}
