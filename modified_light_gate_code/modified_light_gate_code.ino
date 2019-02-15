// reads two IR proximity/line-detectors.  reports time between triggers.
// uses interrupts.  First one should go high for detect on 2
// second detector should go high on detect on pin 3
int x=0;
int y=1000000;
unsigned long t1=0;
unsigned long t2=0;
void sens1() { if ((t1==0) && (t2==0)) t1=micros(); }
void sens2() { if ((t2==0) && (t1!=0)) t2=micros(); }

void setup() {
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,OUTPUT);
  attachInterrupt(0,sens1,RISING);
  attachInterrupt(1,sens2,RISING);
  //Serial.begin(115200);
  Serial.begin(57600);
}

void loop() {
  digitalWrite(4,t1?HIGH:LOW);
  if (t2>0)
    {
          t2-t1 * y = x;
      Serial.println("*******************");
      //Serial.println(t2-t1);
      Serial.println(x);
      Serial.println("Microseconds");
      Serial.println("*******************");
      t1=t2=0;
    }
}

/*
This version may be safer inside an interrupt:
static unsigned long myMicros() {
   extern volatile unsigned long timer0_overflow_count;
   uint8_t oldSREG = SREG;      
   cli();      
   uint32_t t = TCNT0;
   if ((TIFR0 & _BV(TOV0)) && (t == 0))
       t = 256;
   uint32_t m = timer0_overflow_count;
   SREG = oldSREG;
   return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}
*/
