#define CLKW  LOW
#define CCLKW HIGH
#define SPEED_INTERVAL 10
#define SPEED_DEFAULT 200

int PUL = 7; //define Pulse pin
int DIR = 6; //define Direction pin
int ENA = 5; //define Enable Pin
String User_input;
int Speed = 200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin( 9600 );
  
  pinMode ( PUL, OUTPUT );
  pinMode ( DIR, OUTPUT );
  pinMode ( ENA, OUTPUT );
  digitalWrite( DIR, CLKW );
  digitalWrite( ENA, LOW );
  Speed = SPEED_DEFAULT;
}

void loop() {
    if ( Serial.available() > 0 ) 
    {   
      User_input = Serial.readString();
    }

    if ( User_input == "left" ) digitalWrite( DIR, CCLKW );
    if ( User_input == "right" ) digitalWrite( DIR, CLKW );
    if ( User_input == "on" ) digitalWrite( ENA, HIGH );
    if ( User_input == "off" ) digitalWrite( ENA, LOW );
    if ( User_input == "up" )
    {
      digitalWrite( ENA, LOW ); 
      Speed = Speed - SPEED_INTERVAL;
      if ( Speed <= 20 )
      {
        Speed = 20;
      }
      digitalWrite( ENA, HIGH );
    }
    if ( User_input == "down" )
    {
      digitalWrite( ENA, LOW ); 
      Speed = Speed + SPEED_INTERVAL;
      digitalWrite( ENA, HIGH ); 
    }
    
    if ( User_input == "reset" ) 
    {
      digitalWrite( ENA, LOW );
      digitalWrite( DIR, CLKW );  
      Speed = SPEED_DEFAULT;
    }

    User_input = "";
    
    digitalWrite( PUL, HIGH );
    delayMicroseconds( Speed );
    digitalWrite( PUL,LOW );
    delayMicroseconds( Speed );
}
