/*
    Din ce am observat, Serial.available() nu returneaza numarul de caractere pe care le poate citi ci daca poate citi
    Solutia pentru problema asta e o clasa ReadingBuffer in care citeste cate in caracter de fiecare data cand este disponibil
    Mai departe, dupa ce am umplut un buffer de 9 elemente (adica daca am citit o fata), o stochez
    Cum fac asta? Verific cate fete am citit pana acum
    
*/

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

//folosesc functia asta sa convertesc din caractere in numere la citire
short chToNb (char ch)
{
    return (short)(ch - '0');
}

//folosesc functia asta sa afisez caractere cand afisez fetele 
char shToCh(short number) //short to ch ~ pentru afisare
{
    switch (number)
    {
        case WHITE:
            return 'W';
            break;

        case YELLOW:
            return 'Y';
            break;

        case RED:
            return 'R';
            break;

        case GREEN:
            return 'G';
            break;

        case BLUE:
            return 'B';
            break;

        case ORANGE:
            return 'O';
            break;
    
        default:
            return '-';
            break;
    }
}


class Face 
{
  public:
    short sqr[SQ_NB];           //folosesc short ca sa economisesc memorie
    short color;  
    int up, down, left, right;  //  fetele care se modifica la o rotire a fetei curente
    Face()
    {
        for(int i=0; i<SQ_NB; i++)
            sqr[i] = -1;
        color = -1;     
    }
    
    int fillColors(short col[])
    {
        color = col[CENTER];
        for(int i = 0; i< SQ_NB; i++ )
            sqr[i] = col[i];
        switch(color) 
        {
          case WHITE:
          {
            up = ORANGE;
            down = RED;
            left = GREEN;
            right = BLUE;
            break;
          }
          case YELLOW:
          {
            up = RED;
            down = ORANGE;
            left = GREEN;
            right = BLUE;
            break;
          }
          case RED:
          {
            up = WHITE;
            down = YELLOW;
            left = GREEN;
            right = BLUE;
            break;
          }
          
          case GREEN:
          {
            up = WHITE;
            down = YELLOW;
            left = ORANGE;
            right = RED;
            break;
          }
          case BLUE:
          {
            up = WHITE;
            down = YELLOW;
            left = RED;
            right = ORANGE;
            break;
          }
        }
    }

    void afisFace() 
    {
        for(int  i=0; i<3; i++)
            {
                for(int j=0; j<3;j++)
                    Serial.print (shToCh (sqr[ 3 * i + j]));
                Serial.println();   // '\n'
            }
    }
    void copyFace(Face f)
    {
      for(int i = 0; i < SQ_NB; i++)
        sqr[i] = f.sqr[i];
    }

    void rotateCW()
    {
      short newColors[SQ_NB];
      newColors[0] = sqr[6];
      newColors[1] = sqr[3];
      newColors[2] = sqr[0];
      newColors[3] = sqr[7];
      newColors[4] = sqr[4];
      newColors[5] = sqr[1];
      newColors[6] = sqr[8];
      newColors[7] = sqr[5];
      newColors[8] = sqr[2];
      for(int i = 0 ; i < SQ_NB; i++)
        sqr[i] = newColors[i];
    }

    void rotateCCW()
    {
      short newColors[SQ_NB];
      newColors[0] = sqr[2];
      newColors[1] = sqr[5];
      newColors[2] = sqr[8];
      newColors[3] = sqr[1];
      newColors[4] = sqr[4];
      newColors[5] = sqr[7];
      newColors[6] = sqr[0];
      newColors[7] = sqr[3];
      newColors[8] = sqr[6];
      for(int i = 0 ; i < SQ_NB; i++)
        sqr[i] = newColors[i];
      
    }
    

};

class ReadingBuffer
{
    short size;
    short poz;
    short buff[20];
public:
    void init(int sz)
    {
        size = sz;
        poz = 0;
    }

    bool filled()
    {
        return (poz == size || poz == 19);
    }

    void readData()
    {
        if( !filled() && Serial.available() )
        {
            buff[poz++] = chToNb(Serial.read());
            int x; 
            x=poz-1;
            Serial.println(buff[x]);
        }
    }

    short* getBuffer()
    {
        return buff;
    }

    void reset()
    {
        poz = 0;
    }


};

class Cube 
{
    Face F[FACES];
    ReadingBuffer buff;
    short filled[FACES];  ///vector care spune ce fata am citit          
public:
    Cube()
    {
        for(int  i = 0; i < FACES; i++)
            filled[i] = false;
        
        buff.init(SQ_NB);
    }

    bool readData()         //functia returneaza true daca inca nu a citit toate datele din cub
    {
        if(cubeFilled()) return false;  // daca cubul e plin, nu mai citesc date
        
        buff.readData();
        if(buff.filled())
        {
            setFaces(buff.getBuffer());
            buff.reset();
        }
        return true;
    }

    short setFaces(short data[])
    {
        if(!filled[data[CENTER]]) //daca nu am citit fata cu indicele in centrul setului de data
            filled[data[CENTER]] = true;
        else return FACES;          //nu returnez -1, ci 6 pentru eroare. idk if needed

        
        F[data[CENTER]].fillColors(data);
        F[data[CENTER]].afisFace();

        return data[CENTER];
        
    }

    void printFace()
    {
        for(int i=0; i< FACES; i++)
            F[i].afisFace();
        Serial.println();
    }

    bool cubeFilled()
    {
        for(int i=0; i < FACES; i++)
            if(filled[i] == false) return false;
        return true;
    }

    void upCW()
    {
      F[YELLOW].rotateCW();
      
      int r0, r1, r2;
      r0 = F[RED].sqr[0];
      r1 = F[RED].sqr[1];
      r2 = F[RED].sqr[2];

      //RED FACE UPDATE
      F[RED].sqr[0] = F[GREEN].sqr[0];
      F[RED].sqr[1] = F[GREEN].sqr[1];
      F[RED].sqr[2] = F[GREEN].sqr[2];

      //GREEN FACE UPDATE
      F[GREEN].sqr[0] = F[ORANGE].sqr[0];
      F[GREEN].sqr[1] = F[ORANGE].sqr[1];
      F[GREEN].sqr[2] = F[ORANGE].sqr[2];

      //ORANGE FACE UPDATE
      F[ORANGE].sqr[0] = F[BLUE].sqr[0];
      F[ORANGE].sqr[1] = F[BLUE].sqr[1];
      F[ORANGE].sqr[2] = F[BLUE].sqr[2];

      //BLUE FACE UPDATE
      F[BLUE].sqr[0] = r0;
      F[BLUE].sqr[1] = r1;
      F[BLUE].sqr[2] = r2;
    }

    void downCW()
    {
      F[WHITE].rotateCW();
      
      int r6, r7, r8;
      r6 = F[RED].sqr[6];
      r7 = F[RED].sqr[7];
      r8 = F[RED].sqr[8];

      //RED FACE UPDATE
      F[RED].sqr[6] = F[BLUE].sqr[6];
      F[RED].sqr[7] = F[BLUE].sqr[7];
      F[RED].sqr[8] = F[BLUE].sqr[8];

      //BLUE FACE UPDATE
      F[BLUE].sqr[6] = F[ORANGE].sqr[6];
      F[BLUE].sqr[7] = F[ORANGE].sqr[7];
      F[BLUE].sqr[8] = F[ORANGE].sqr[8];

      //ORANGE FACE UPDATE
      F[ORANGE].sqr[6] = F[GREEN].sqr[6];
      F[ORANGE].sqr[7] = F[GREEN].sqr[7];
      F[ORANGE].sqr[8] = F[GREEN].sqr[8];

      //GREEN FACE UPDATE
      F[GREEN].sqr[6] = r6;
      F[GREEN].sqr[7] = r7;
      F[GREEN].sqr[8] = r8;
    }

    void leftCW()
    {
      F[BLUE].rotateCW();    
      // THE OTHER FACES
      int r0, r3, r6;
      r0 = F[RED].sqr[0];
      r3 = F[RED].sqr[3];
      r6 = F[RED].sqr[6];

      //RED FACE UPDATE
      F[RED].sqr[0] = F[YELLOW].sqr[0];
      F[RED].sqr[3] = F[YELLOW].sqr[3];
      F[RED].sqr[6] = F[YELLOW].sqr[6];

      //YELLOW FACE UPDATE
      F[YELLOW].sqr[0] = F[ORANGE].sqr[8];
      F[YELLOW].sqr[3] = F[ORANGE].sqr[5];
      F[YELLOW].sqr[6] = F[ORANGE].sqr[2];

      //ORANGE FACE UPDATE
      F[ORANGE].sqr[2] = F[WHITE].sqr[6];
      F[ORANGE].sqr[5] = F[WHITE].sqr[0];
      F[ORANGE].sqr[8] = F[WHITE].sqr[3];

      //WHITE FACE UPDATE
      F[WHITE].sqr[0] = r0;
      F[WHITE].sqr[3] = r3;
      F[WHITE].sqr[6] = r6;
      
    }
    void rightCW()
    {
      F[GREEN].rotateCW();
      
      int r2, r5, r8;
      r2 = F[RED].sqr[2];
      r5 = F[RED].sqr[5];
      r8 = F[RED].sqr[8];

      //RED FACE UPDATE
      F[RED].sqr[2] = F[WHITE].sqr[2];
      F[RED].sqr[5] = F[WHITE].sqr[5];
      F[RED].sqr[8] = F[WHITE].sqr[8];

      //WHITE FACE UPDATE
      F[WHITE].sqr[2] = F[ORANGE].sqr[6];
      F[WHITE].sqr[5] = F[ORANGE].sqr[3];
      F[WHITE].sqr[8] = F[ORANGE].sqr[0];

      //ORANGE FACE UPDATE
      F[ORANGE].sqr[0] = F[YELLOW].sqr[8];
      F[ORANGE].sqr[3] = F[YELLOW].sqr[5];
      F[ORANGE].sqr[6] = F[YELLOW].sqr[2];

      //YELLOW FACE UPDATE
      F[YELLOW].sqr[2] = r2;
      F[YELLOW].sqr[5] = r5;
      F[YELLOW].sqr[8] = r8;
    }
    void frontCW()
    {
      F[RED].rotateCW();

      int y6, y7, y8;
      y6 = F[YELLOW].sqr[6];
      y7 = F[YELLOW].sqr[7];
      y8 = F[YELLOW].sqr[8];

      //YELLOW FACE UPDATE
      F[YELLOW].sqr[6] = F[BLUE].sqr[8];
      F[YELLOW].sqr[7] = F[BLUE].sqr[5];
      F[YELLOW].sqr[8] = F[BLUE].sqr[2];

      //BLUE FACE UPDATE
      F[BLUE].sqr[2] = F[WHITE].sqr[0];
      F[BLUE].sqr[5] = F[WHITE].sqr[1];
      F[BLUE].sqr[8] = F[WHITE].sqr[2];

      //WHITE FACE UPDATE
      F[WHITE].sqr[0] = F[GREEN].sqr[6];
      F[WHITE].sqr[1] = F[GREEN].sqr[3];
      F[WHITE].sqr[2] = F[GREEN].sqr[0];

      //GREEN FACE UPDATE
      F[GREEN].sqr[0] = y6;
      F[GREEN].sqr[3] = y7;
      F[GREEN].sqr[6] = y8;
      
    }

    void backCW()
    {
      F[ORANGE].rotateCW();
      
      int y0, y1, y2;
      y0 = F[YELLOW].sqr[0];
      y1 = F[YELLOW].sqr[1];
      y2 = F[YELLOW].sqr[2];

      //YELLOW FACE UPDATE
      F[YELLOW].sqr[0] = F[GREEN].sqr[2];
      F[YELLOW].sqr[1] = F[GREEN].sqr[5];
      F[YELLOW].sqr[2] = F[GREEN].sqr[8];

      //GREEN FACE UPDATE
      F[GREEN].sqr[2] = F[WHITE].sqr[8];
      F[GREEN].sqr[5] = F[WHITE].sqr[7];
      F[GREEN].sqr[8] = F[WHITE].sqr[6];

      //WHITE FACE UPDATE
      F[WHITE].sqr[6] = F[BLUE].sqr[0];
      F[WHITE].sqr[7] = F[BLUE].sqr[3];
      F[WHITE].sqr[8] = F[BLUE].sqr[6];

      //BLUE FACE UPDATE
      F[BLUE].sqr[0] = y2;
      F[BLUE].sqr[3] = y1;
      F[BLUE].sqr[6] = y0; 
    }

    void upCCW()
    {
      F[YELLOW].rotateCCW();
      
      int r0, r1, r2;
      r0 = F[RED].sqr[0];
      r1 = F[RED].sqr[1];
      r2 = F[RED].sqr[2];

      //RED FACE UPDATE
      F[RED].sqr[0] = F[BLUE].sqr[0];
      F[RED].sqr[1] = F[BLUE].sqr[1];
      F[RED].sqr[2] = F[BLUE].sqr[2];

      //BLUE FACE UPDATE
      F[BLUE].sqr[0] = F[ORANGE].sqr[0];
      F[BLUE].sqr[1] = F[ORANGE].sqr[1];
      F[BLUE].sqr[2] = F[ORANGE].sqr[2];

      //ORANGE FACE UPDATE
      F[ORANGE].sqr[0] = F[GREEN].sqr[0];
      F[ORANGE].sqr[1] = F[GREEN].sqr[1];
      F[ORANGE].sqr[2] = F[GREEN].sqr[2];

      //GREEN FACE UPDATE
      F[GREEN].sqr[0] = r0;
      F[GREEN].sqr[1] = r1;
      F[GREEN].sqr[2] = r2;
    }

    void downCCW()
    {
      F[WHITE].rotateCCW();
      
      int r6, r7, r8;
      r6 = F[RED].sqr[6];
      r7 = F[RED].sqr[7];
      r8 = F[RED].sqr[8];

      //RED FACE UPDATE
      F[RED].sqr[6] = F[GREEN].sqr[6];
      F[RED].sqr[7] = F[GREEN].sqr[7];
      F[RED].sqr[8] = F[GREEN].sqr[8];

      //GREEN FACE UPDATE
      F[GREEN].sqr[6] = F[ORANGE].sqr[6];
      F[GREEN].sqr[7] = F[ORANGE].sqr[7];
      F[GREEN].sqr[8] = F[ORANGE].sqr[8];

      //ORANGE FACE UPDATE
      F[ORANGE].sqr[6] = F[BLUE].sqr[6];
      F[ORANGE].sqr[7] = F[BLUE].sqr[7];
      F[ORANGE].sqr[8] = F[BLUE].sqr[8];

      //BLUE FACE UPDATE
      F[BLUE].sqr[6] = r6;
      F[BLUE].sqr[7] = r7;
      F[BLUE].sqr[8] = r8;
    }

    void leftCCW()
    {
      F[BLUE].rotateCCW();

      int r0, r3, r6;
      r0 = F[RED].sqr[0];
      r3 = F[RED].sqr[3];
      r6 = F[RED].sqr[6];

      //RED FACE UPDATE
      F[RED].sqr[0] = F[WHITE].sqr[0];
      F[RED].sqr[3] = F[WHITE].sqr[3];
      F[RED].sqr[6] = F[WHITE].sqr[6];

      //WHITE FACE UPDATE
      F[WHITE].sqr[0] = F[ORANGE].sqr[8];
      F[WHITE].sqr[3] = F[ORANGE].sqr[5];
      F[WHITE].sqr[6] = F[ORANGE].sqr[2];

      //ORANGE FACE UPDATE
      F[ORANGE].sqr[2] = F[YELLOW].sqr[2];
      F[ORANGE].sqr[5] = F[YELLOW].sqr[3];
      F[ORANGE].sqr[8] = F[YELLOW].sqr[0];

      //YELLOW FACE UPDATE
      F[YELLOW].sqr[0] = r0;
      F[YELLOW].sqr[3] = r3;
      F[YELLOW].sqr[6] = r6;
     
    }
    void rightCCW()
    {
      F[GREEN].rotateCCW();
      
      int r2, r5, r8;
      r2 = F[RED].sqr[2];
      r5 = F[RED].sqr[5];
      r8 = F[RED].sqr[8];

      //RED FACE UPDATE
      F[RED].sqr[2] = F[YELLOW].sqr[2];
      F[RED].sqr[5] = F[YELLOW].sqr[5];
      F[RED].sqr[8] = F[YELLOW].sqr[8];

      //YELLOW FACE UPDATE
      F[YELLOW].sqr[2] = F[ORANGE].sqr[6];
      F[YELLOW].sqr[5] = F[ORANGE].sqr[3];
      F[YELLOW].sqr[8] = F[ORANGE].sqr[0];

      //ORANGE FACE UPDATE
      F[ORANGE].sqr[0] = F[WHITE].sqr[8];
      F[ORANGE].sqr[3] = F[WHITE].sqr[5];
      F[ORANGE].sqr[6] = F[WHITE].sqr[2];

      //WHITE FACE UPDATE
      F[WHITE].sqr[2] = r2;
      F[WHITE].sqr[5] = r5;
      F[WHITE].sqr[8] = r8;
    }
    void backCCW()
    {
      F[ORANGE].rotateCCW();
      
      int y0, y1, y2;
      y0 = F[YELLOW].sqr[0];
      y1 = F[YELLOW].sqr[1];
      y2 = F[YELLOW].sqr[2];

      //YELLOW FACE UPDATE
      F[YELLOW].sqr[0] = F[BLUE].sqr[6];
      F[YELLOW].sqr[1] = F[BLUE].sqr[3];
      F[YELLOW].sqr[2] = F[BLUE].sqr[0];

      //BLUE FACE UPDATE
      F[BLUE].sqr[0] = F[WHITE].sqr[6];
      F[BLUE].sqr[3] = F[WHITE].sqr[7];
      F[BLUE].sqr[6] = F[WHITE].sqr[8];

      //WHITE FACE UPDATE
      F[WHITE].sqr[6] = F[GREEN].sqr[8];
      F[WHITE].sqr[7] = F[GREEN].sqr[5];
      F[WHITE].sqr[8] = F[GREEN].sqr[2];

      //GREEN FACE UPDATE
      F[GREEN].sqr[2] = y0;
      F[GREEN].sqr[5] = y1;
      F[GREEN].sqr[8] = y2; 
    }
    void frontCCW()
    {
      F[RED].rotateCCW();

      int y6, y7, y8;
      y6 = F[YELLOW].sqr[6];
      y7 = F[YELLOW].sqr[7];
      y8 = F[YELLOW].sqr[8];

      //YELLOW FACE UPDATE
      F[YELLOW].sqr[6] = F[GREEN].sqr[0];
      F[YELLOW].sqr[7] = F[GREEN].sqr[3];
      F[YELLOW].sqr[8] = F[GREEN].sqr[6];

      //GREEN FACE UPDATE
      F[GREEN].sqr[0] = F[WHITE].sqr[2];
      F[GREEN].sqr[3] = F[WHITE].sqr[1];
      F[GREEN].sqr[6] = F[WHITE].sqr[0];

      //WHITE FACE UPDATE
      F[WHITE].sqr[0] = F[BLUE].sqr[2];
      F[WHITE].sqr[1] = F[BLUE].sqr[5];
      F[WHITE].sqr[2] = F[BLUE].sqr[8];

      //BLUE FACE UPDATE
      F[BLUE].sqr[2] = y8;
      F[BLUE].sqr[5] = y7;
      F[BLUE].sqr[8] = y6;  
    }

};


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
