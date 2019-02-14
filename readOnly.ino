
#include "ClassFace.h"
#include "ClassCube.h"
#include "Buffer.h"

#include "Cross.h"


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
int matrixStepper[6][4]={
  {50, 51, 52, 53},
  {46, 47, 48, 49},
  {42, 43, 44, 45},
  {38, 39, 40, 41},
  {34, 35, 36, 37},
  {30, 31, 32, 33}
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
    for( int j = 0; j < 6; j++)
    for ( i = 0; i < 4; i++) 
    {
      pinMode (matrixStepper[j][i], OUTPUT);
    }
}

int mot = 0;
bool doneMot = false;

void loop()
{
//    input();
    if(doneMot == false) 
    {
    switch (mot) {
      case 0 : {ccw(degrees_180,RIGHT); mot++; Serial.println("right"); break;}
      case 1 : {ccw(degrees_180,LEFT); mot++; Serial.println("left");break;}
      case 2 : {ccw(degrees_180,BACK); mot++; Serial.println("back");break;}
      case 3 : {ccw(degrees_180,FRONT); mot++; Serial.println("front");break;}
      case 4 : {ccw(degrees_180,DOWN); mot++; Serial.println("down"); break;}
      case 5 : {ccw(degrees_180,UP); mot++; Serial.println("up"); break;}
      default : { mot = 0;
        //doneMot = true;
        break;}
    };
    delay(1000);
    }

//if(doneMot == false)
  //ccw(degrees_90,RIGHT);// mot++; Serial.println("right");
  //doneMot = true;
}
