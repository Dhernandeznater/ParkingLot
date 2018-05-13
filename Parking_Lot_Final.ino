#include <LPD8806.h>
#include <Time.h>
#include <TimeLib.h>
#include <ParkingSpace.h>

//Initialize all objects and variables
LPD8806 lightStrip(32, 43, 42);
ParkingSpace lot[10];
time_t timeReserved[10];
time_t buttonTime;
int reservedSpots[10];
int colors[10][3]={ {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
                  {255, 255, 255},{255, 255, 0} , {128, 100, 50},
                  {102, 0, 204}, {46, 151, 88}, {1, 255, 255}, 
                  {32, 97, 128} };
uint8_t ledPins[10][3]={{44,45,46}, {A13,A14,A15}, {A10,A11,A12},
                        {A7,A8,A9}, {A4,A5,A6}, {A1,A2,A3},
                        {11,12,A0}, {8,9,10}, {5,6,7}, {2,3,4}};
bool isButtonPressed=false;
bool isSpaceAvailable=true;
const int CUT_OFF_NUMBER=27;
int tempRed;
int tempGreen;
int tempBlue;
long duration;
int distance;
int thirdHand;
int fourthHand;

void setup() 
{
   //Serial port for testing and debugging
  Serial.begin(9600);

   #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  // Set up the parking spots
  for(int x=0; x<10; x++)
  {
    lot[x].setReserved(false);
  }
  // Start up the LED strip
  lightStrip.begin();

  // Update the strip, to start they are all 'off'
  lightStrip.show();

  for(int x=0; x<10; x++)
    reservedSpots[x]=-1;

  pinMode(22, OUTPUT);
  pinMode(23, INPUT);
  pinMode(24, OUTPUT);
  pinMode(25, INPUT);
  pinMode(26, OUTPUT);
  pinMode(27, INPUT);
  pinMode(28, OUTPUT);
  pinMode(29, INPUT);
  pinMode(30, OUTPUT);
  pinMode(31, INPUT);
  pinMode(32, OUTPUT);
  pinMode(33, INPUT);
  pinMode(34, OUTPUT);
  pinMode(35, INPUT);
  pinMode(36, OUTPUT);
  pinMode(37, INPUT);
  pinMode(38, OUTPUT);
  pinMode(39, INPUT);
  pinMode(40, OUTPUT);

}

void updateDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;

}

// Chase one dot down the part of the strip.
void colorChase(uint32_t c, uint8_t wait, int spot) {
  int i;
  
  // Start by turning all pixels off:
  for(i=0; i<lightStrip.numPixels(); i++) lightStrip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<spot*2; i++) {
    lightStrip.setPixelColor(i, c); // Set new pixel 'on'
    lightStrip.show();              // Refresh LED states
    lightStrip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    checkButton();
    delay(wait);
    if(i==spot*2-1)
      delay(100);
  }

  lightStrip.show(); // Refresh to turn off last pixel
}


void showStrip()
{
  for(int x=0; x<10; x++)
   {
     if(reservedSpots[x]!=-1)
     {
      setColor(colors[x][0], colors[x][1], colors[x][2], reservedSpots[x]);
      colorChase(lightStrip.Color(colors[x][0], colors[x][1], colors[x][2]), 60, reservedSpots[x]+1);
      Serial.print("Spot");
      Serial.println(x);
     }
   }
}

void checkButton()
{
  if(digitalRead(13)==LOW&&now-buttonTime>5)
    isButtonPressed=true;
    
}

void setColor(int red, int green, int blue, uint8_t spot)
{
  
  
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif

  analogWrite(ledPins[spot][0], red);
  analogWrite(ledPins[spot][1], green);
  analogWrite(ledPins[spot][2], blue);  
}

void loop() 
{
  checkButton();
  
  Serial.println(digitalRead(13));
  
  //loop function until the entry button is pressed
  while(!isButtonPressed)
  {
    //Check to see which spots are taken
    for(int x=0; x<10; x++)
    {
      int spot=x+11;

      //Update distance for each spot
      updateDistance(2*spot, (2*spot)+1);

     
      
      checkButton();
      //Check if there is car in spot by distance
      if(x==3)
        distance++;
      if(distance<CUT_OFF_NUMBER&&!lot[x].getTaken())
      {
        Serial.println("taken is true");
        //Set spot as taken if there is a car, remove reserved tag
        lot[x].setTaken(true);
        lot[x].setReserved(false);
        for(int y=0; y<10; y++)
        {
          if(reservedSpots[y]==x)
          {
            reservedSpots[y]=-1;
            setColor(0,0,0,x);
            checkButton();
          }
        }
      }
      checkButton();
       //Print for testing and debugging
      Serial.print(distance);
      Serial.print(" is the distance for spot ");
      Serial.print(x+1);
      Serial.print(lot[x].getReserved());
      Serial.println(lot[x].getTaken());
      delay(1);
      
      checkButton();
      
      //Label spot as not taken if there isn't a car in spot
      if(distance>=CUT_OFF_NUMBER)
      {
        lot[x].setTaken(false);
        checkButton();
      }
    }
    showStrip();
  }

    

  //Check to see if there is an open spot in the lot
  for(int x=0; x<10; x++)
  {
    //Print for testing and debugging
    Serial.print("Spot ");
    Serial.print(x+1);
    Serial.print(": Taken: ");
    Serial.println(lot[x].getTaken());
    Serial.print("Reserved: ");
    Serial.println(lot[x].getReserved());
    delay(1);

    //If spot is found, break loop and set space available as true
    if(!lot[x].getTaken()&&!lot[x].getReserved())
    {
      //printing for testing and debugging
      Serial.print("Spot ");
      Serial.print(x+1);
      Serial.println(" is not Taken or Reserved");
      delay(1);
      isSpaceAvailable=true;

      //Adding spot to list of reserved spots
      for(int y=0; y<10; y++)
      {
        if(reservedSpots[y]==-1)
        {

          //Printing for testing and debugging
          Serial.print("Adding spot ");
          Serial.println(x+1);
          delay(1);
          reservedSpots[y]=x;
          lot[x].setReserved(true);
          isButtonPressed=false;
          break;
        }
      }
      break;
    }

    //If no spots are found, the value is set to false
    Serial.println("No spots are available");
    delay(1);
    isSpaceAvailable=false;
  }

  Serial.print("Is space available? ");
  Serial.println(isSpaceAvailable);
  showStrip(); 
  //Restart loop() function if there are no spaces
  if(!isSpaceAvailable)
  {
    Serial.println("No available spots, restarting loop");
    delay(1);
    isButtonPressed=false;
    
  }
}


