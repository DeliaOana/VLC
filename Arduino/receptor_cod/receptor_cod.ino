#define SETTING 5 
#define DELAY 25 
#define BITLENGTH 8 
#define DIFFERENCE 10 
#define S1 4 
#define ID 180 
#define sensorPin A0 

int t, m, s, duration, d;
int sensorValue = 0; 
int middleValue; 
int inputByte; 
void checkMiddle(); 
int readByte(); 
int j; 
float temperature; 
long counter=millis(); 
int count=0; 
float freq;

void setup() { 
Serial.begin(115200); 
checkMiddle();//Setting the value between Logic 0 and Logic 1 
} 

void loop() { 
do 
{ 
if(digitalRead(S1)==HIGH) 
{ 
checkMiddle();
//middleValue = analogRead(sensorPin); 
Serial.println("Sensor is calibrated "); 
}
sensorValue = analogRead(sensorPin); 
}while(sensorValue<=middleValue); 
 
{ 
//Reading the transmitted values
//temperature
t=readByte(); 
//motion
m = readByte();
//sound
s = readByte();
//irradiance
duration = readByte();
//dust density
d = readByte();

//Calculating and displaying measured values 

//temperature
temperature = (float)t*5/1024; 
temperature = temperature * 100;
Serial.print("\n");
Serial.print(++count); // Sending the value of temperature 
Serial.print (" : The tempereature is: "); 
Serial.print(temperature); // Sending the value of temperature 
Serial.println(" 'C"); 

//motion
//Serial.print("m=");
//Serial.print(m);
//float motion = (float)m*5/1024;
//Serial.print(" The voltage at the PIR output is: ");
//Serial.print(motion);
//Serial.println("V");
if (m>0) {Serial.println("Motion has been detected");}
else {Serial.println("Motion has not been detected");}

//sound
//Serial.print("Sunet:");
//Serial.println(s);
if(s == 0){Serial.println("Noise has not been detected");}
else {Serial.println("Noise has been detected");}

//irradiance
//Serial.print("Durata perioada: ");
//Serial.println(duration);
freq = 1/(float)duration * 100; /// pt a transforma din M in k
//Serial.print("Frecventa: ");
//Serial.println(freq);
Serial.print("The irradiance is: ");
Serial.print(freq);
Serial.println(" microW/cm2");

//dust density
float calcVoltage = (float)d*3.3/1024;
float dustDensity = 0.17 * calcVoltage - 0.1;
//Serial.print("Tensiune dust:");
//Serial.println(calcVoltage);
if (dustDensity < 0) { dustDensity = 0;}
Serial.print("Dust density is: ");
Serial.print(dustDensity);
Serial.println(" mg/cm3");
Serial.println(" ");
 

if (count >256) 
{ 
count=0; 
} 
} 
} 
 
void checkMiddle() 
{ 
int difference; 
middleValue = analogRead(sensorPin); //Cehcking initial voltage on the sensor 
do{ 
sensorValue=analogRead(sensorPin);//Checking second voltage on sensor 
difference= sensorValue - middleValue;//Comparing the values of the voltage 
delay(SETTING); 
}while(difference<DIFFERENCE && difference>-DIFFERENCE); 
//Calculating the middle value between Logic 1 and Logic 0 
middleValue = (sensorValue + middleValue)/2; 
} 
///

///
int readByte() 
{ 
int i,input=0;
delay(DELAY); 
for(i=0;i<BITLENGTH;i++) 
{//Repeat for each bit 
sensorValue=analogRead(sensorPin);//Read the sensor Value 
//Serial.print("val citita de la senzor ");
//Serial.println(sensorValue);
//Serial.print("val prag ");
//Serial.println(middleValue);
if(sensorValue<=middleValue) 
{ 
input++; 
Serial.print("1");
} 
else{Serial.print("0");}
//Serial.print("pana la cat a crescut input ");
//Serial.println(input);
//Binary shift to store another bit 
input=input<<1; 
//Serial.print("input dupa shiftare ");
//Serial.println(input);
delay(DELAY); 
} 
//Return the value if input 
input=input>>1; 
//Serial.print("!!input dupa shiftare ");
//Serial.println(input);
delay(1000);
return input; 
}
