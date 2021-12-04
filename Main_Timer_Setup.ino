//GPIO ASSIGNMENT VARIABLES
const byte systemPump = 2;		//	GPIO D2 is being assigned to control Solid State Relay I that powers a System Pump [14.5VDC @ 10A]
const byte valve1 = 3;			//	GPIO D3 is being used to control a Solid State Relay (SSR II) that powers the 12VDC Misting Solenoid Valve 1
//const byte valve2 = 4;			//	GPIO D3 is being used to control a Solid State Relay (SSR II) that is a spare
const byte ledPin = 13;			//	INDICATOR ORANGE LED ON CHIP AS TROUBLESHOOTING INDICATOR

//ON TIME PARAMETERS DEFINED GLOBALLY
const unsigned long eventOnInterval = 2000;								//RECYCLE TIMER: ON TIME RANGE [(0.5s) - 360,0000 ms (1 hr.)]	//ON Time in Milliseconds LIMITED TO 0.5s?
unsigned long previousOnMillis = 0;										//NOTE:Make sure any other variables involved are the same variable class. Make sure integer constants have a "UL" [ie. time = previoustime / 100UL]

//OFF TIME VARIABLES DEFINED GLOBALLY
const unsigned long eventOffInterval = 600000;							//RECYCLE TIMER: OFF TIME RANGE [60,000 (1 min.) - 86,400,000ms (24 hr.)]					//5000 (5s) | 600000(10m) | 900000 (15m);     //OFF Time in Milliseconds (15m) LIMITED to 60s? 
unsigned long previousOffMillis = 0;

//VALVE STATE VARIABLE
bool statusValve1 = false;												//STARTS IN THE OFF(FALSE) STATUS

void setup()  // put your setup code here, to run once:
{
pinMode(systemPump,OUTPUT);												//D2
pinMode(valve1,OUTPUT);													//D3    | PA06
//pinMode(valve2,OUTPUT);													//D4    | PA07
pinMode(ledPin,OUTPUT);													//D13   | PA17

//INITIALIZE THE SERIAL PORT @ 9600 BAUD RATE
Serial.begin(9600);
while (!Serial);														//Wait while serial is initialized


const unsigned short onTime = eventOnInterval / 1000;					//Pint out the timing variable
Serial.print("Event On Time: ");
Serial.print(onTime);
Serial.println(" s");
Serial.print("Event Off Time: ");
const unsigned short offTime = eventOffInterval / 1000;
Serial.print(offTime);
Serial.println(" s");
Serial.println("RECYCLE TIMER SETUP COMPLETE");							//Set Up Complete

digitalWrite(ledPin,HIGH);												//BUILT IN LED TURNS ON TO INDICATE COMPLETION OF SETUP
digitalWrite(systemPump,HIGH);											//TURNS ON THE SSR FOR THE MAIN SYSTEM PUMP. TURN OFF WHEN PROTECT IS REQUIRED								
}

void loop()  // put your main code here, to run repeatedly:
{
//RECYCLE ON/OFF TIMER FUNCTION
digitalWrite(ledPin,LOW);													//BUILT IN LED TURNS OFF TO INDICATE THE START OF THE MAIN LOOP

unsigned long currentOffMillis = millis();								//READ TO COUNTER AND ASSIGN IT AS CURRENT OFF TIME.
unsigned long currentOnMillis = millis();								//READ TO COUNTER AND ASSIGN IT AS CURRENT ON TIME.

  if((statusValve1 == true) && (currentOnMillis - previousOnMillis >= eventOnInterval))		//EXPIRATION OF THE ON TIME**
	{
		digitalWrite(valve1,LOW);								//OPENS RELAY CIRCUIT THE CIRUIT BY WRITING THE GPIO HIGH
		statusValve1 = false;											//CHANGES VALVE STATUS TO CLOSED (FALSE)
		Serial.print("Valve 1 Status:");
		Serial.println(statusValve1);
		//RESET AND PAUSE THE EXPIRATION TIMERS
		previousOffMillis = currentOffMillis;							//ASSIGN CURRENT COUNTER VALUE TO RESET THE PREVIOUS OFFTIME
		previousOnMillis = 0;											//THIS IS SET TO CONSTANT TO PAUSE THE EXPIRATION CONDITION**
		digitalWrite(ledPin,HIGH);    
	}

  //EXPIRATION OF THE OFF-TIME
  if((statusValve1 == false) && (currentOffMillis - previousOffMillis >= eventOffInterval))//OFFTIMER EXPIRATION
	{
		digitalWrite(valve1,HIGH);											//CLOSED 3.3VDC RELAY INPUT CIRCUIT THE CIRUIT BY WRITING THE GPIO LOW AND SINKING 7MA OF CURRENT TO D4
		statusValve1 = true;											//CHANGES VALVE STATUS TO CLOSED (TRUE)
		Serial.print("Valve 1 Status:");
		Serial.println(statusValve1);
		//RESET AND PAUSE THE EXPIRATION TIMERS
		previousOnMillis = currentOnMillis;								//ASSIGN CURRENT COUNTER VALUE TO REMEMBER THE TIME
		previousOffMillis = 0;											//DISABLES THE
		digitalWrite(ledPin,HIGH);
		delay(200);														
	}
	
delay(100);																//DELAY TIME FOR LOOP LED STROBE
}
