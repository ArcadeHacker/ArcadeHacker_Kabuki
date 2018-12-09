// Kabuki Desuicider 1.22 by arcadehacker.blogspot.com
#include <LiquidCrystal.h>

// lcd stuff
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int lcd_key       = 0;
int adc_key_in    = 0;
int adc_key_prev  = 0;
int c = -1;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//  Kabuki interface pins
#define BUSAK       0
#define WAIT        1
#define BUSRQ       2
#define RESET       3
#define M1          11 
#define RFSH        12
#define VCC         13

// delay amount
int time = 25;

// Kabuki game programming codes
unsigned char bytes[][11] = {

{ 0x65, 0x48, 0xFA, 0xC6, 0x88, 0x05, 0x39, 0x77, 0x24, 0xF0, 0x00 }, //Pang
{ 0x65, 0x48, 0xFA, 0xC6, 0x88, 0x05, 0x39, 0x77, 0x24, 0xF0, 0x00 }, //Buster Bros
{ 0x65, 0x48, 0xFA, 0xC6, 0x88, 0x05, 0x39, 0x77, 0x24, 0xF0, 0x00 }, //Pomping World
{ 0x58, 0x52, 0x97, 0x70, 0x53, 0x97, 0x70, 0x53, 0x43, 0x70, 0x00 }, //Super Pang World
{ 0x55, 0xAA, 0xDC, 0x14, 0xE5, 0x94, 0xA7, 0xB8, 0x5A, 0x70, 0x00 }, //Super Pang Japan
{ 0x21, 0x30, 0x97, 0x70, 0x53, 0x97, 0x70, 0x53, 0x12, 0x70, 0x00 }, //Super Buster Bros
{ 0x48, 0x54, 0xB1, 0xA2, 0x3E, 0xB1, 0xA2, 0x3E, 0x4F, 0x70, 0x00 }, //Super Marukin-Ban
{ 0x51, 0x51, 0xB0, 0xE6, 0x3A, 0x05, 0x39, 0x77, 0x51, 0xFF, 0x00 }, //Warriors of Fate
{ 0x43, 0x43, 0x53, 0x02, 0xEF, 0xFA, 0xC6, 0x88, 0x43, 0xFF, 0x00 }, //Cadillacs & Dinosaurs
{ 0x22, 0x22, 0xF5, 0x9C, 0x14, 0xDE, 0x54, 0x43, 0x22, 0xFF, 0x00 }, //Punisher
{ 0x31, 0x31, 0xD6, 0x34, 0x47, 0xB1, 0xA2, 0x3E, 0x19, 0xFF, 0x00 }, //Slam Masters
{ 0x00, 0x02, 0xD1, 0x0F, 0x59, 0x0A, 0x62, 0xEF, 0x01, 0x70, 0x00 }, //Block Block
{ 0xAA, 0x55, 0x05, 0x39, 0x77, 0xFA, 0xC6, 0x88, 0xA5, 0x70, 0x00 }, //Mhjong Gauken 2
{ 0xAA, 0x55, 0x05, 0x39, 0x77, 0xFA, 0xC6, 0x88, 0xA5, 0x70, 0x00 }, //Poker Ladies
{ 0xAA, 0x55, 0x05, 0x39, 0x77, 0xFA, 0xC6, 0x88, 0xA5, 0x70, 0x00 }, //Dokaben
{ 0xAA, 0x55, 0x05, 0x39, 0x77, 0xFA, 0xC6, 0x88, 0xA5, 0x70, 0x00 }, //Dokaben 2 NOT DUMPED, PLACEHOLDER KEY
{ 0x65, 0x48, 0xFA, 0xC6, 0x88, 0x05, 0x39, 0x77, 0x24, 0x70, 0x00 }, //Capcom Baseball
{ 0x57, 0x51, 0x82, 0x9C, 0xBB, 0x10, 0xD5, 0x9F, 0x43, 0x70, 0x00 }, //Capcom World
{ 0x57, 0x51, 0x97, 0x70, 0x53, 0x97, 0x70, 0x53, 0x43, 0x70, 0x00 }, //Adventure Quiz 2
{ 0x11, 0x11, 0x29, 0xCB, 0xB8, 0x29, 0xCB, 0xB8, 0x11, 0x70, 0x00 }, //Quiz Tonosama
{ 0xAA, 0x55, 0x05, 0x39, 0x77, 0xFA, 0xC6, 0x88, 0xA5, 0x70, 0x00 }, //Ashita Tenki NOT DUMPED, PLACEHOLDER KEY
{ 0x18, 0x28, 0x4E, 0x5D, 0xC1, 0x4E, 0x5D, 0xC1, 0x18, 0x70, 0x00 }  //Quiz Sangokushi 

};

unsigned char mask = 1; // Bit mask
unsigned char bits[8];

// Display game list
char* GameList[] = 
  { 
  "Pang            ",
  "Buster Bros     ",
  "Pomping World   ",
  "Super Pang World",
  "Super Pang Japan",
  "Super BusterBros",
  "Super MarukinBan",
  "Warriors of Fate",
  "Cadillacs and D.",
  "Punisher        ",
  "Slam Masters    ",
  "Block Block     ",
  "Mhjong Gauken 2 ",
  "Poker Ladies    ",
  "Dokaben         ",
  "Dokaben 2       ",
  "Capcom Baseball ",
  "Capcom World    ",
  "Adventure Quiz 2",
  "Quiz Tonosama   ",
  "Ashita Tenki    ",
  "Quiz Sangokushi "
  };
  

// Initialize
void setup() {
  digitalWrite(RFSH, HIGH);
  digitalWrite(VCC, HIGH);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Kabuki Desuicide");
  lcd.setCursor(0,1);
  lcd.print("By ArcadeHacker");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print("Select a game:  ");
  lcd.setCursor(0,1);
  lcd.print(" --press down-- ");

}

void busrqclk()
{
    digitalWrite(BUSRQ, HIGH); delay(time);
    digitalWrite(BUSRQ, LOW); delay(time);
}

void busakclk()
{
    digitalWrite(BUSAK, LOW); delay(time);
    digitalWrite(BUSAK, HIGH); delay(time);
}

void m1clk()
{
    digitalWrite(M1, LOW); delay(time);
    digitalWrite(M1, HIGH); delay(time);
}

void program_unlock()
{

    digitalWrite(M1, HIGH);
    busakclk();
    digitalWrite(M1, LOW);
    busakclk();
    digitalWrite(M1, HIGH);
    busakclk();
    busakclk();
    digitalWrite(M1, LOW);
    busakclk();
    digitalWrite(M1, HIGH);
    busakclk();
    busakclk();
    busakclk();
    busakclk();
    digitalWrite(M1, LOW);
    busakclk();
    digitalWrite(M1, HIGH);
    busakclk();    
    digitalWrite(M1, LOW);
    busakclk();
    busakclk();
    digitalWrite(M1, HIGH);
    busakclk();        
    digitalWrite(M1, LOW);
    busakclk();
    busakclk();
    busakclk();
    digitalWrite(M1, HIGH);
    busakclk();    
    digitalWrite(M1, LOW);
    busakclk();
    digitalWrite(M1, HIGH);
    busakclk();    
} 

void ProgramKabuki(int prg)
                {
                int i;
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("Preparing Kabuki");                
 
                pinMode(RESET, OUTPUT); pinMode(WAIT, OUTPUT); pinMode(BUSRQ, OUTPUT);
                digitalWrite(RESET, LOW); digitalWrite(WAIT, LOW); delay(1000); 
                pinMode(BUSAK, OUTPUT); pinMode(M1, OUTPUT);
                digitalWrite(BUSAK, HIGH); digitalWrite(M1, HIGH); delay(1000);
                lcd.setCursor(0,1);
                lcd.print("Door knocking...");                     
        
               
                for(i=0; i<30; i++)
                  {
                  busrqclk();
                  }
                lcd.setCursor(0,1);
                lcd.print("Unlocking...    ");                  
              

                for(int i=0; i<30; i++)
                  {            
                  busakclk();
                  }   
               
                program_unlock();      
                
                for(i=0; i<10; i++)        
                  {                   
                    m1clk();  
                  }
                lcd.setCursor(0,0);
                lcd.print("Programming     ");
                lcd.setCursor(0,1);
                lcd.print("Kabuki...       ");    
                     

                for(i=0; i<20; i++)        
                  {        
                    digitalWrite(M1, LOW);                
                    busakclk();  
                  }  

                  
                for(i=0; i<11; i++)        
                  {      
                    for (int b = 7; b > -1; b--) {  
               
                     bits[b] = (bytes[prg][i] & (mask << b)) != 0;
                     printf("%d",bits[b]);
       
                     if (bits[b] == 1) {
                      digitalWrite(M1, HIGH);}
                      else { 
                      digitalWrite(M1, LOW);}                  
                    busakclk();  

                   }
                  }
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("Done!           ");                   
                delay(1000);       
                busrqclk();
                busrqclk();
                busrqclk();
                digitalWrite(WAIT, HIGH);
                busrqclk();
                busrqclk();
                busrqclk();
                pinMode(BUSAK, INPUT); pinMode(M1, INPUT);
                pinMode(RESET, INPUT); pinMode(WAIT, INPUT); pinMode(BUSRQ, INPUT);
        
                while (analogRead (0) != 638) { 
                 lcd.setCursor(0,0); lcd.print("--insert Kabuki ");   lcd.setCursor(0,1); lcd.print("in cpu socket--");
                }
}

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);   
  delay(5);
  int k = (analogRead(0) - adc_key_in); 
  if (5 < abs(k)) return btnNONE;  

  if (adc_key_in > 1000) return btnNONE; 
  if (adc_key_in < 50)   return btnRIGHT;  
  if (adc_key_in < 195)  return btnUP; 
  if (adc_key_in < 380)  return btnDOWN; 
  if (adc_key_in < 555)  return btnLEFT; 
  if (adc_key_in < 790)  return btnSELECT;   
  return btnNONE;  
}                

// main loop
void loop()  
{

  adc_key_prev = lcd_key ;     
  lcd_key = read_LCD_buttons();

  if (adc_key_prev != lcd_key)
  {

    lcd.setCursor(0,1);          
  
    switch (lcd_key)            
    {
    case btnRIGHT:
      {
        break;
      }
    case btnLEFT:
      {
        break;
      }
    case btnUP:
      {
     if (c != 0) {c--;}
     lcd.print(GameList[c]);
        break;
      }
    case btnDOWN:
      {
     if (c != sizeof(GameList)/sizeof(GameList[0])-1) {c++;}
     lcd.print(GameList[c]);
        break;
      }
    case btnSELECT:
      {
       if (c != -1) 
         {
           ProgramKabuki(c);                
           lcd.setCursor(0,0);
           lcd.print("Select a game:  ");
           lcd.setCursor(0,1);
           lcd.print(GameList[c]);
         }
        break;
      }
    case btnNONE:
      {
        break;
      }
    }
  }
}
