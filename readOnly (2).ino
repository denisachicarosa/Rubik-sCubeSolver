
#include "ClassFace.h"
#include "ClassCube.h"
#include "Buffer.h"


#define WHITE 0       //down
#define YELLOW 1      //up
#define RED 2         //front
#define GREEN 3       //left
#define BLUE 4        //right
#define ORANGE 5      //back

#define CENTER 4
#define FACES 6
#define SQ_NB 9 

#define degrees_180 250
#define degrees_90 125

#define UP  5 
#define DOWN 4
#define FRONT 3
#define BACK 2
#define LEFT 1
#define RIGHT 0



Cube C;
bool done = false;
int i;

void input() 
{
  if(!C.readData())
      if(done == false)
        {
          Serial.println("CUBE FILLED !");
          done = true;
        }
}


// pini steppere
int matrixStepper[5][4]={
  {50, 51, 52, 53},
  {46, 47, 48, 49},
  {42, 43, 44, 45},
  {38, 39, 40, 41},
  {34, 35, 36, 37}
};

// functii de miscare ale motoarelor
void write (int motor, int valA,int valB,int valC,int valD) 
{
  digitalWrite(matrixStepper[motor][0], valA);
  digitalWrite(matrixStepper[motor][1], valB);
  digitalWrite(matrixStepper[motor][2], valC);
  digitalWrite(matrixStepper[motor][3], valD);
  delay(2);
}

void cw(int degrees, int motor){
  for( i = 0; i < degrees; i++) 
  {
    write(motor, 1,1,0,0);
    write(motor, 0,1,1,0);
    write(motor, 0,0,1,1);
    write(motor, 1,0,0,1);
  }
  
}

void ccw(int degrees, int motor){
   for( i = 0; i < degrees; i++) 
   {
    write(motor, 1,0,0,1);
    write(motor, 0,0,1,1);
    write(motor, 0,1,1,0);
    write(motor, 1,1,0,0);
   }
}



void setup ()
{
    Serial.begin(9600);
    for ( i = 0; i < 4; i++) 
    {
      pinMode (matrixStepper[0][i], OUTPUT);
    }
}


void loop()
{
    //input();
    ccw(degrees_90,RIGHT);
}
