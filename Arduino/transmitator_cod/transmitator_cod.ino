#define DELAY 25
#define LED 2 //Output pin where LED is connected 
#define ID 180 //ID of Receiver 
#define BITLENGTH 8 //1 Byte 
#define FIRSTBIT pow(2,(BITLENGTH-1))//MSB Value 
#define TRUE 1 
#define TEMPERATURE A0 
#define PIR A1
#define SOUND 3
#define LIGHT 4
#define LED_GP2 12
#define GP2 A5

int j , t, m, s, d; 
int count=0; 
long Timer = millis(); 
float voltage; 
void writeByte(int); 
int counter=0; 
float duration;
float freq;
float calcVoltage;
float dustDensity;


void setup() { 
//Setting the LED as Output 
pinMode(LED,OUTPUT); 
Serial.begin(115200); 
pinMode(LIGHT, INPUT);
pinMode(LED_GP2, OUTPUT);
} 


void loop() 
{ 
if ((millis()-Timer)<20000) 
{ 

////Reading the output of the sensors
//temperature
t=analogRead(TEMPERATURE); //Reading the temperature(i.e. the voltage drop on the sensor) 
//motion
m=analogRead(PIR);
float motion = (float)m*5/1024;
if (motion > 0) {m=255;}
else{m=0;}
//sound
s=digitalRead(SOUND);
//irradiance
duration = pulseIn(LIGHT, HIGH);
//dust density
digitalWrite(LED_GP2,LOW); // power on the LED
delayMicroseconds(280);
d=analogRead(GP2);
delayMicroseconds(40);
digitalWrite(LED_GP2,HIGH); // turn the LED off

///Sending data to the receiver 
//temperature
writeByte(t);//sending the temperature to the receiver 
//motion
writeByte(m);
//sound
writeByte(s);
//irradiance
writeByte(duration);
//dust density
writeByte(d);
 

///Calculating and displaying measured values
Serial.print("\n");
Serial.print(++count); // Sending the value of temperature 

//temperature
float temperature = (float)t*5/1024; 
temperature = temperature * 100; 
Serial.print (" : The temperature is: "); 
Serial.print(temperature); // Sending the value of temperature 
Serial.println(" 'C"); 

//motion
//Serial.print("m=");
//Serial.print(m);
//float motion = (float)m*5/1024;
//Serial.print(" The voltage at the PIR output is: ");
//Serial.print(motion);
//Serial.println("V");
if (motion > 0) {Serial.println("Motion has been detected");}
else {Serial.println("Motion has not been detected");}


//sound
//Serial.print("Sunet:");
//Serial.println(s);
if(s == 0){Serial.println("Noise has not been detected");}
else {Serial.println("Noise has been detected");}

//irradinace
//Serial.print("Durata perioada: ");
//Serial.println(duration);
freq = 1/duration * 100; /// pt a transforma din M in k
//Serial.print("Frecventa: ");
//Serial.println(freq);
Serial.print("The irradiance is: ");
Serial.print(freq);
Serial.println(" microW/cm2");

//dustDensity
calcVoltage = (float)d*3.3/1024;
dustDensity = 0.17 * calcVoltage - 0.1;
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



//Sending 1byte number 
void writeByte(int decimal) 
{
//converting the decimal value to binary and sending 8 bit information from MSB to LSB 
int i,binary; 
digitalWrite(LED,LOW);
delay(DELAY);
for(i=0;i<BITLENGTH;i++) 
{ 
binary = (int)decimal/FIRSTBIT; //Getting the first binary bit value 
decimal= (decimal & ((int)FIRSTBIT -1));//Setting the first bit to zero 
decimal=decimal<<1; //Shift all bits by one to left 
if(binary==TRUE) 
{ 
digitalWrite(LED,HIGH); 
Serial.print("1"); 
}else 
{ 
digitalWrite(LED,LOW); 
Serial.print("0"); 
} 
delay(DELAY); 
} 
//Serial.println(); 
digitalWrite(LED,HIGH);
delay(1000); 
}
