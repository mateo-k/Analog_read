/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

//Stale ADC
const long AREF = 5000L;
const float ADC_MAXVAL = 1023.0;
const int ANALOG_INPUTS = 8;

//Stale zwiazane z usrednianiem
const int SIZE = 1; //max SIZE == 419
int val_tab[SIZE][ANALOG_INPUTS];
int iter_last;

//Podlaczenie pinow
const int POT1_PIN = A0;
const int POT1_VCC_PIN = 5;
const int POT2_PIN = A7;
const int POT2_VCC_PIN = 4;

const int Imcu_PIN = A1;
const int Imcu_ADC_INPUT = 1;
const int Idrv_PIN = A6;
const int Idrv_ADC_INPUT = 6;

//Parametry czujnikow pradu
static const float Imcu_CUR_SENSIVITY = 131.6;//mV per Amper
static const float Imcu_CUR_IMBALANCE = -0.00;
static const float Idrv_CUR_SENSIVITY = 131.6;//mV per Amper
static const float Idrv_CUR_IMBALANCE = -0.00;


// the setup routine runs once when you press reset:
void setup() {
  //pinMode(POT1_VCC_PIN, OUTPUT);
  //digitalWrite(POT1_VCC_PIN, HIGH);
  //pinMode(POT2_VCC_PIN, OUTPUT);
  //digitalWrite(POT2_VCC_PIN, HIGH);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:

  int val[ANALOG_INPUTS];
  {
    const int AVR_CNT = 200; //usrednianie pradu po AVR_CNT probek
    long suma[ANALOG_INPUTS];
    for(int input_no=0; input_no<ANALOG_INPUTS; input_no++) suma[input_no] = 0;
    for(int i=0; i<AVR_CNT; i++)
      for(int input_no=0; input_no<ANALOG_INPUTS; input_no++)
        suma[input_no]+=analogRead(input_no);
    for(int input_no=0; input_no<ANALOG_INPUTS; input_no++)
      val[input_no] = (suma[input_no]+AVR_CNT/2)/AVR_CNT;
  }

  if(iter_last < SIZE)
    for(int input_no=0; input_no<ANALOG_INPUTS; input_no++)
      val_tab[iter_last][input_no] = val[input_no];
  iter_last = (iter_last+1)%SIZE;
  
  float A[ANALOG_INPUTS];
  for(int input_no=0; input_no<ANALOG_INPUTS; input_no++){
    long suma = 0;
    for(int i=0;i<SIZE;i++)
      suma+=val_tab[i][input_no];
    A[input_no] = (suma * AREF) / (ADC_MAXVAL * SIZE);
  }

/*
  float napiecie = (suma*AREF)/(ADC_MAXVAL*SIZE) - VCC/2;
static const float CUR_SENSIVITY = 131.6;//130.6; //130.6 mV per Amper
  float prad = napiecie / CUR_SENSIVITY;
static const float CUR_IMBALANCE = -0.00;
  //korekta pradu
  prad += CUR_IMBALANCE;
  
  Serial.print("U: ");
  Serial.print(napiecie);
  Serial.print("mV,\tI: ");
  Serial.print(prad);
  Serial.println("A");
*/
  float Imcu = A[Imcu_ADC_INPUT]-AREF/2;
  Imcu /= Imcu_CUR_SENSIVITY;
  Imcu += Imcu_CUR_IMBALANCE;
  
  float Idrv = A[Idrv_ADC_INPUT]-AREF/2;
  Idrv /= Idrv_CUR_SENSIVITY;
  Idrv += Idrv_CUR_IMBALANCE;

  Serial.print("A0: ");
  Serial.print(A[0]);
  
  Serial.print("mV,\tA1: ");  
  Serial.print(A[1]);
  
  Serial.print("mV,\tA2: ");
  Serial.print(A[2]);
  
  Serial.print("mV,\tA3: ");
  Serial.print(A[3]);
  
  Serial.print("mV,\tA4: ");
  Serial.print(A[4]);

  Serial.print("mV,\tA5: ");
  Serial.print(A[5]);

  Serial.print("mV,\tA6: ");
  Serial.print(A[6]);
  
  Serial.print("mV,\tA7: ");
  Serial.print(A[7]);
  
  Serial.println("mV");

  delay(1);        // delay in between reads for stability
}
