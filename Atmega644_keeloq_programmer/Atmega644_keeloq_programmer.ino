#include <SD.h>
#include <SPI.h>
//#include "SdFat.h"

#include <TouchScreen.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>

#include <avr/pgmspace.h>
//#define PROGMEM

#include <Wire.h>
#include <EEPROM.h>

MCUFRIEND_kbv tft;
#define XP 6
#define YP A1
#define XM A2
#define YM 7
#define RESISTANCE 300//300

#define MINPRESSURE 100//5 200      // Touch screen minimum pressure to do something   
#define MAXPRESSURE 1000 //10000 10000    // Touch screen maximum pressure to do somet

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define SD_card 10
File monFichierSD;

int actx;
int acty;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, RESISTANCE);
const int TS_LEFT = 914, TS_RT = 136, TS_TOP = 897, TS_BOT = 124;
Adafruit_GFX_Button Enter_btn, SEND_btn, CLR_btn, END_btn, Haut_btn, Bas_btn, OK_btn, b_0btn, b_1btn, b_2btn, b_3btn, b_4btn, b_5btn, b_6btn, b_7btn, b_8btn,
                    b_9btn, b_Abtn, b_Bbtn, b_Cbtn, b_Dbtn, b_Ebtn, b_Fbtn, simple_btn, normal_btn, Hcs200_btn, Hcs300_btn, disc_btn, disc_Ser0_btn;
byte CENTER = 200;
///*
bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    actx = map(p.y, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
    acty = map(p.x, TS_TOP, TS_BOT, 0, tft.height());

  }
  //delay(5);
  return pressed;
}

//=================================== defines for color pallette ======================================
// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define DARK_GREY 0x03E0   //
#define GREY 0x7BEF
#define LIGHT_GREY   0xC618
#define BLUEL   0x105D//0xA3FA
//#define GREENL  0x21E5
#define TEAL    0x0438
#define ORANGE    0xFC00
//#define PINK      0xF81F
#define PURPLE    0x8010
//#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
//#define DKCYAN    0x03EF
//#define DKRED     0x6000
//#define DKMAGENTA 0x8008
//#define DKYELLOW  0x8400
#define DKORANGE  0x8200
//#define DKPINK    0x9009
//#define DKPURPLE  0x4010
#define DKGREY    0x4A49
//#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0x0D0A//0x0DAA//0xBFF7
//#define LTCYAN    0xC7FF
//#define LTRED     0xFD34
//#define LTMAGENTA 0xFD5F
//#define LTYELLOW  0xFFF8
//#define LTORANGE  0xFE73
//#define LTPINK    0xFDDF
//#define LTPURPLE  0xCCFF
//#define LTGREY    0xE71C


boolean mode_Enter = true;//true
boolean bool_choix_hcs200 = false;
boolean bool_choix_hcs201 = false;
boolean bool_choix_hcs300 = false;
boolean bool_choix_hcs301 = false;
boolean bool_choix_hcs320 = false;
boolean bool_choix_disc = false;
boolean bool_disc = false;
boolean bool_disc_Ser0 = false;
//boolean bool_choix_hcs = false;
boolean bool_choix_simple = true;
boolean bool_choix_normal = false;
boolean bool_choix_secure = false;
boolean bool_invalide_serial = false;
boolean bool_invalide_mancode = false;
boolean bool_invalide_seed = false;
boolean bool_page_deux = false;
boolean bool_OSC0 = false;
boolean bool_OSC1 = false;
boolean bool_OSC2 = false;
boolean bool_OSC3 = false;
boolean bool_Vlow = false;
boolean bool_BRS = false;
boolean bool_MTX4 = false;
boolean bool_TXEN = false;
boolean bool_S3SET = false;
boolean bool_XSER = false;
boolean bool_OVR0 = false;
boolean bool_OVR1 = false;
boolean bool_BSL0 = false;
boolean bool_BSL1 = false;
boolean EEPROM_Write_Error = false;
boolean bool_SD_serial_exist = false;
boolean bool_ASD = false;
boolean bool_EEPROM_Write_Succefull = false;
boolean bool_EEPROM_Write_Error = false;

byte mode = 0;
byte mode_config = 0;
byte Count_Config = 0;
byte mode_menu = 0;
byte OSC0 = 0;
byte OSC1 = 0;
byte OSC2 = 0;
byte OSC3 = 0;
byte VLOW = 0;
byte BRS = 0;
byte MTX4 = 0;
byte _TXEN = 0;
byte S3SET = 0;
byte XSER = 0;
byte OVR0 = 0;
byte OVR1 = 0;
byte BSL0 = 0;
byte BSL1 = 0;
byte EENC = 0;
byte q_button = 0;

File monFichier;

String value_hcs_dossier = "";
String value_hcs_serial = "";
String value = "";

// text box where numbers go
#define TEXT_X 5
#define TEXT_Y 5
#define TEXT_W 205
#define TEXT_H 20
#define TEXT_TSIZE 2
#define TEXT_TCOLOR MAGENTA

#define TEXT_LEN  16
char textfield[TEXT_LEN + 1] = ""; //"";
uint8_t textfield_i = 0;

#define TEXT_LEN_HUIT 8
char textfield_SERIAL[TEXT_LEN_HUIT + 1] = "";
uint8_t textfield_i_SERIAL = 0;

char textfield_SEED[TEXT_LEN_HUIT + 1] = "";
uint8_t textfield_i_SEED = 0;

#define TEXT_LEN_QUATRE  4
char textfield_DISC[TEXT_LEN_QUATRE + 1] = "";
uint8_t textfield_i_DISC = 0;

char textfield_SYNC[TEXT_LEN_QUATRE + 1] = "";
uint8_t textfield_i_SYNC = 0;

#define TEXT_LEN_UN 1
char textfield_CONFIG_Ads[TEXT_LEN_UN + 1] = "";
uint8_t textfield_i_CONFIG_Ads = 0;

char textfield_KEY0[TEXT_LEN_HUIT + 1] = "";
uint8_t textfield_i_KEY0 = 0;

char textfield_KEY1[TEXT_LEN_HUIT + 1] = "";
uint8_t textfield_i_KEY1 = 0;

char textfield_KEY_HIGH[TEXT_LEN_HUIT + 1] = "";
uint8_t textfield_i_KEY_HIGH = 0;

char textfield_KEY_LOW [TEXT_LEN_HUIT  + 1] = "";
uint8_t textfield_i_KEY_LOW  = 0;

// We have a status line for like, is FONA working
#define STATUS_X 5
#define STATUS_Y 5
/*
  #define BUTTON_X 40
  #define BUTTON_Y 185
  #define BUTTON_W 30
  #define BUTTON_H 25
  #define BUTTON_SPACING_X 5
  #define BUTTON_SPACING_Y 10
  #define BUTTON_TEXTSIZE 1
*/
char buttonlabels[16][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
uint16_t buttoncolors[16] = {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE};
Adafruit_GFX_Button buttons[16];

/******************* UI details */
/*
  #define BUTTON_X_labels1 40
  #define BUTTON_Y_labels1 100
  #define BUTTON_W_labels1 30
  #define BUTTON_H_labels1 25
  #define BUTTON_SPACING_X_labels1 5
  #define BUTTON_SPACING_Y_labels1 10
  #define BUTTON_TEXTSIZE_labels1 1
*/
uint16_t Color;
uint8_t TEXT_X_CONFIG;
uint8_t TEXT_Y_CONFIG;

#define EEPROM_SIZE 12

// ****** HCS TIME PROGRAMMING EQUATE ********
#define Tps 4          // PROGRAM MODE SETUP TIME 4mS (3,5mS min, 4,5 max)
#define Tph1 4          // HOLD TIME 1 4mS (3,5mS min)
#define Tph2 62          // HOLD TIME 2 62uS (50uS min)
#define Tpbw 3          // BULK WRITE TIME 3mS (2,2mS min)
#define Tclkh 35          // CLOCK HIGH TIME 35uS (25uS min)
#define Tclkl 35          // CLOCK LOW TIME 35uS (25uS min)
#define Twc 40          // PROGRAM CYCLE TIME 40mS (36mS min)
#define Tphold 100          // PROGRAM CYCLE TIME HCS201 100us

unsigned long KEY0 ;
unsigned long KEY1 ;
unsigned long KEY2 ;
unsigned long KEY3 ;
unsigned long KeyHigh;
unsigned long KeyLow;
unsigned long SER_0_SER_1;
unsigned long SEED;

word MCODE_0 ;
word MCODE_1 ;
word MCODE_2 ;
word MCODE_3 ;
word SEED_0 ;
word SEED_1 ;
word TEMPS_SEED_1 ;
word SER_0 ;
word SER_1 ;
word SYNC;
word AUTOFF;
word DISC;
word CONF_HI;
word ENV_KEY = 0x0000;

//keeloq stuff
#define KeeLoq_NLF    0x3A5C742E
#define bit(x,n)    (((x)>>(n))&1)
#define g5(x,a,b,c,d,e) (bit(x,a)+bit(x,b)*2+bit(x,c)*4+bit(x,d)*8+bit(x,e)*16)

#define disc70 (SER_0 & 0x00FF)
#define disc8 ((SER_0 & 0x0100)>>8)
#define disc9 ((SER_0 & 0x0200)>>9)
#define NUM_WRD 12                // NUMBER OF WORD TO PROGRAM INTO HCS
#define RES 0X0000                // RESERVED WORD
//#define ENV_KEY 0x0000            // ENVELOPE KEY (NOT USED FOR HCS200)

#define disc70_Disc (DISC & 0x00FF)
#define disc8_Disc ((DISC & 0x0100)>>8)
#define disc9_Disc ((DISC & 0x0200)>>9)

//HCS200
// normal mode
#define disc10_Hcs200 ((SER_0 & 0x0400)>>10)
#define disc11_Hcs200 ((SER_0 & 0x0800)>>11)
//secure mode
#define disc10_Disc_Hcs200 ((DISC & 0x0400)>>10)
#define disc11_Disc_Hcs200 ((DISC & 0x0800)>>11)

//###########################################################################################
//SYSTEM VARIABLES
//###########################################################################################
char TempStringWord[5];
char TempStringDoubleByte[16];
word TempWord;
word Key[4];
word EEPROM_Write_Buffer[12];
word EEPROM_Read_Buffer[12];


#define CLK 25               //(OUT PIN) Clock (S2) for Programming HCS //// White Wire
#define DATA 26//13              // (IN/OUT PIN) Data (PWM) for Programming HCS   //Blue Wire
#define HCSVDD 24            // (OUT PIN) HCS Vdd line      //Red Wire


unsigned long Keeloqdecrypt(unsigned long data, unsigned long keyHigh, unsigned long keyLow)
{
  unsigned long x = data;
  unsigned long r;
  int keyBitNo, index;
  unsigned long keyBitVal, bitVal;

  for (r = 0; r < 528; r++)
  {
    keyBitNo = (15 - r) & 63;
    if (keyBitNo < 32)
      keyBitVal = bitRead(keyLow, keyBitNo);
    else
      keyBitVal = bitRead(keyHigh, keyBitNo - 32);
    index = 1 * bitRead(x, 0) + 2 * bitRead(x, 8) + 4 * bitRead(x, 19) + 8 * bitRead(x, 25) + 16 * bitRead(x, 30);
    bitVal = bitRead(x, 31) ^ bitRead(x, 15) ^ bitRead(KeeLoq_NLF, index) ^ keyBitVal;
    x = (x << 1) ^ bitVal;
  }
  return x;
}


byte ReadKeeloq() {

  unsigned long TempSerNum1 = 0;
  unsigned long TempSerNum = 0;

  if (bool_choix_secure) {
    TempSerNum1 = SEED;
  }
  else {
    TempSerNum1 = SER_0_SER_1;
    TempSerNum1 |= 0x20000000;
  }

  KeyLow = Keeloqdecrypt(TempSerNum1, KEY0, KEY1);

  TempSerNum = SER_0_SER_1;

  if (bool_choix_secure) {
    TempSerNum |= 0x00000000;
  }
  else {
    TempSerNum |= 0x60000000;
  }
  KeyHigh = Keeloqdecrypt(TempSerNum, KEY0, KEY1);
}


void Serial_hcs() {
  value_hcs_serial.concat(value);
  value_hcs_serial.concat(".txt");
  char filename[value_hcs_serial.length() + 1];
  value_hcs_serial.toCharArray(filename, sizeof(filename));
  monFichier = SD.open(filename, FILE_WRITE);
  Serial.println(filename);
  // value_hcs_serial = "";
}


void conversion() {
  String value0x = "0x";
  value = "";
  value_hcs_dossier = "";
  value_hcs_serial = "";

  for (byte num = 0; num < 8; num++) {
    value0x = value0x + String(textfield_SERIAL[num]);
    value = value  + String(textfield_SERIAL[num]);
  }

  SER_0_SER_1 = strtol(value0x.c_str(), NULL, 0);

  value_hcs_dossier = "HCS200/";
  // value_hcs_dossier.concat(value);
  // value_hcs_dossier.concat(".txt");
  Sd_exist_compare();

  value_hcs_dossier = "HCS201/";
  // value_hcs_dossier.concat(value);
  // value_hcs_dossier.concat(".txt");
  Sd_exist_compare();

  value_hcs_dossier = "HCS300/";
  // value_hcs_dossier.concat(value);
  // value_hcs_dossier.concat(".txt");
  Sd_exist_compare();

  value_hcs_dossier = "HCS301/";
  // value_hcs_dossier.concat(value);
  // value_hcs_dossier.concat(".txt");
  Sd_exist_compare();

  value_hcs_dossier = "HCS320/";
  // value_hcs_dossier.concat(value);
  //  value_hcs_dossier.concat(".txt");
  Sd_exist_compare();
  //Serial.println(value_hcs_dossier);
  if (!bool_SD_serial_exist) {

    if (bool_choix_hcs200) {
      value_hcs_serial = "HCS200/";
      // value_hcs_serial.concat(value);
      // value_hcs_serial.concat(".txt");
      Serial_hcs();

    }
    if (bool_choix_hcs201) {
      value_hcs_serial = "HCS201/";
      // value_hcs_serial.concat(value);
      //  value_hcs_serial.concat(".txt");
      Serial_hcs();
    }
    if (bool_choix_hcs300) {
      value_hcs_serial = "HCS300/";
      // value_hcs_serial.concat(value);
      // value_hcs_serial.concat(".txt");
      Serial_hcs();
    }
    if (bool_choix_hcs301) {
      value_hcs_serial = "HCS301/";
      // value_hcs_serial.concat(value);
      //  value_hcs_serial.concat(".txt");
      Serial_hcs();
    }
    if (bool_choix_hcs320) {
      value_hcs_serial = "HCS320/";
      //  value_hcs_serial.concat(value);
      //  value_hcs_serial.concat(".txt");
      Serial_hcs();
    }
   // Serial.println(value_hcs_serial);
    value = "";

    if (monFichier) {

      if (bool_disc_Ser0) {
        value = "DISC_SERIAL";
      }
      if (bool_disc) {
        value = "DISC_SECURITE";
      }
      monFichier.println(value);
      // Serial.println(value);

      value = "";
      if (bool_choix_simple) {
        value = "SIMPLE";
      }
      if (bool_choix_normal) {
        value = "NORMAL";
      }
      if (bool_choix_secure) {
        value = "SECURITE";
      }

      monFichier.println(value);
      // Serial.println(value);
      value = "";
      for (byte num = 0; num < 8; num++) {
        value = value  + String(textfield_SERIAL[num]);
      }
      monFichier.println("SERIAL = " + value);
       // Serial.println("SERIAL = " + value);

      value = "";
      for (byte num = 0; num < 16; num++) {
        value = value + String(textfield[num]);
      }
      monFichier.println("MANCODE = " + value);
      // Serial.println("MANCODE = " + value);

      value = "";
      for (byte num = 0; num < 8; num++) {
        value = value + String(textfield_SEED[num]);
      }
      monFichier.println("SEED = " + value);
      // Serial.println("SEED = " + value);
    }

    if (bool_choix_secure) {
      value0x = "0x";
      for (byte num = 0; num < 8; num++) {
        value0x = value0x + String(textfield_SEED[num]);
      }
      SEED = strtol(value0x.c_str(), NULL, 0);
    }

    if (bool_choix_normal || bool_choix_secure) {
      value0x = "0x";
      for (byte num = 0; num < 8; num++) {
        value0x = value0x + String(textfield[num]);
      }
      KEY0 = strtol(value0x.c_str(), NULL, 0);

      value0x = "0x";
      for (byte num = 0 , num1 = 0; num < 8 , num1 < 8; num++ , num1++) {
        textfield_KEY0[num1] = textfield[num];
      }
      for (byte num = 0; num < 8; num++) {
        value0x = value0x + String(textfield_KEY0[num]);
      }

      KEY0 = strtoul (textfield_KEY0, 0, 16);

      value0x = "0x";
      for (byte num = 8 , num1 = 0; num < 16 , num1 < 8; num++ , num1++) {
        textfield_KEY1[num1] = textfield[num];
      }

      for (byte num = 0; num < 8; num++) {
        value0x = value0x + String(textfield_KEY1[num]);
      }
      KEY1 = strtoul (textfield_KEY1, 0, 16);

      ReadKeeloq();

      ltoa(KeyLow, textfield_KEY_LOW, 16);
      for (byte num = 0; num < 8; num++) {
      }

      String value_KEY_LOW = "";
      String value_KEY_HIGH = "";
      for (byte num = 0; num < 8; num++) {
        value_KEY_LOW = value_KEY_LOW + String(textfield_KEY_LOW[num]);
      }

      ltoa(KeyHigh, textfield_KEY_HIGH, 16);
      for (byte num = 0; num < 8; num++) {
      }

      for (byte num = 0; num < 8; num++) {
        value_KEY_HIGH = value_KEY_HIGH + String(textfield_KEY_HIGH[num]);
      }

      value_KEY_HIGH = value_KEY_HIGH + value_KEY_LOW;

      value = "";
      value = value + value_KEY_HIGH;
      if (monFichier) {
        monFichier.println("KEY_HIGH_LOW = " + value_KEY_HIGH);
      }

      value0x = "0x";
      for (byte num = 4; num < 8; num++) {
        value0x = value0x + String(textfield_KEY_HIGH[num]);
      }
      MCODE_2 = strtol(value0x.c_str(), NULL, 0);

      value0x = "0x";
      for (byte num = 0; num < 4; num++) {
        value0x = value0x + String(textfield_KEY_HIGH[num]);
      }
      MCODE_3 = strtol(value0x.c_str(), NULL, 0);

      value0x = "0x";
      for (byte num = 4; num < 8; num++) {
        value0x = value0x + String(textfield_KEY_LOW[num]);
      }
      MCODE_0 = strtol(value0x.c_str(), NULL, 0);

      value0x = "0x";
      for (byte num = 0; num < 4; num++) {
        value0x = value0x + String(textfield_KEY_LOW[num]);
      }
      MCODE_1 = strtol(value0x.c_str(), NULL, 0);
    }
  }
}

void Programm_Hcs() {


  Efface_tft();
  unsigned long HC201_OSC = 0;
  byte compteur_HCS201 = 0;
  int i = 0;
  String value = "";
  if (bool_choix_simple || bool_choix_normal || bool_choix_secure) {
    conversion();
  }

  if (!bool_SD_serial_exist) {
    Key[i] = MCODE_0;  i++;
    Key[i] = MCODE_1;  i++;
    Key[i] = MCODE_2;  i++;
    Key[i] = MCODE_3;  i++;

    if ( bool_choix_hcs300 || bool_choix_hcs301 || bool_choix_hcs320) {
      if (bool_disc_Ser0) {
        CONF_HI = ((EENC << 7) | (BSL1 << 6) | (BSL1 << 5) | (VLOW << 4) | (OVR1 << 3) | (OVR0 << 2) | (disc9 << 1) | disc8);
      }
      if (bool_disc) {
        CONF_HI = ((EENC << 7) | (BSL1 << 6) | (BSL1 << 5) | (VLOW << 4) | (OVR1 << 3) | (OVR0 << 2) | (disc9_Disc << 1) | disc8_Disc);
      }
    }
    if ( bool_choix_hcs200) {
      if (bool_disc_Ser0) {
        CONF_HI = CONF_HI = ((EENC << 7) | (BSL1 << 6) | (BSL0 << 5) | (VLOW << 4) | (disc11_Hcs200 << 3) | (disc10_Hcs200 << 2) | (disc9 << 1) | disc8);
      }
      if (bool_disc) {
        CONF_HI = ((EENC << 7) | (BSL1 << 6) | (BSL0 << 5) | (VLOW << 4) | (disc11_Disc_Hcs200 << 3) | (disc10_Disc_Hcs200 << 2) | (disc9_Disc << 1) | disc8_Disc);
      }
    }
    if ( bool_choix_hcs201) {
      CONF_HI = ((XSER << 9) | (S3SET << 8) | (_TXEN << 7) | (MTX4 << 6) | (BRS << 5) | (VLOW << 4) | (OSC2 << 3) | (OSC2 << 2) | (OSC1 << 1) | OSC0);
      if (bool_disc_Ser0 ) {
        ENV_KEY = ((disc11_Hcs200 << 3) | (disc10_Hcs200 << 2) | (disc9 << 1) | disc8);
        ENV_KEY = ((ENV_KEY << 8) | disc70);
      }
      if (bool_disc) {
        ENV_KEY = ((disc11_Disc_Hcs200 << 3) | (disc10_Disc_Hcs200 << 2) | (disc9_Disc << 1) | disc8);
        ENV_KEY = ((ENV_KEY << 8) | disc70_Disc);
      }
    }

    String Encryption_Key = "";

    // Serial.println("Encryption Key: ");
    for (int i = 3; i >= 0; i--) {
      sprintf(TempStringWord, " % .4X", Key[i]);
      // Serial.print(TempStringWord);
      Encryption_Key = Encryption_Key + TempStringWord;
    }
    if (monFichier) {
      monFichier.println("Encryption Key: " + Encryption_Key);
    }
    // Serial.println("");

    //Prepare EEPROM_Write_Buffer to write
    EEPROM_Write_Buffer[0] = Key[0];
    EEPROM_Write_Buffer[1] = Key[1];
    EEPROM_Write_Buffer[2] = Key[2];
    EEPROM_Write_Buffer[3] = Key[3];
    EEPROM_Write_Buffer[4] = SYNC;
    EEPROM_Write_Buffer[5] = 0x0000;
    EEPROM_Write_Buffer[6] = SER_0; //Serial Number Low Word
    if ( bool_choix_hcs300 || bool_choix_hcs301 || bool_choix_hcs320) {
      EEPROM_Write_Buffer[7] = (SER_1 | (AUTOFF << 15)); //Serial Number High Byte + SET THE AUTO SHUT-OFF TIMER
    }
    else {
      EEPROM_Write_Buffer[7] = (SER_1 ); //Serial Number High Byte + SET THE AUTO SHUT-OFF TIMER
    }
    EEPROM_Write_Buffer[8] = SEED_0;
    EEPROM_Write_Buffer[9] = SEED_1;
    EEPROM_Write_Buffer[10] = ENV_KEY;

    if ( bool_choix_hcs200 || bool_choix_hcs300 || bool_choix_hcs301 || bool_choix_hcs320) {
      if (bool_disc_Ser0) {
        EEPROM_Write_Buffer[11] = ((CONF_HI << 8) | disc70);
      }
      if (bool_disc) {
        EEPROM_Write_Buffer[11] = ((CONF_HI << 8) | disc70_Disc);
      }
    }
    else {
      EEPROM_Write_Buffer[11] = CONF_HI ;
    }

    // Serial.println("EEPROM_Write_Buffer: ");
    tft.setCursor(167 + 2, 144 + 2);
    tft.setTextColor(BLACK, BLACK);
    tft.setTextSize(1);
    tft.print(F("EEPROM Write Succefull!"));
    delay(100);
    tft.setCursor(167 + 2, 144 + 2);
    tft.setTextColor(TEAL, BLACK);
    tft.print(F("EEPROM_Write_Buffer: "));

    delay(100);

    Encryption_Key = "";

    for (int i = 11; i >= 0; i--) {
      sprintf(TempStringWord, " % .4X", EEPROM_Write_Buffer[i]);
      // Serial.print(TempStringWord); Serial.print(" ");
      Encryption_Key = Encryption_Key + TempStringWord;
    }
    if (monFichier) {
      value = "";
      value = value + Encryption_Key;
      monFichier.println("Buffer - EEprom_Write: " + Encryption_Key);
    }

    Encryption_Key = "";

    // Serial.println("");
    // Serial.println("Serial Number: ");
    sprintf(TempStringWord, " % .4X", EEPROM_Write_Buffer[7]);
    // Serial.print(TempStringWord); Serial.print(" ");
    Encryption_Key = Encryption_Key + TempStringWord;
    sprintf(TempStringWord, " % .4X", EEPROM_Write_Buffer[6]);
    Encryption_Key = Encryption_Key + TempStringWord;
    if (monFichier) {
      value = "";
      value = value + Encryption_Key;
      monFichier.println("Serial Number: " + Encryption_Key);
    }

    Encryption_Key = "0x";
    sprintf(TempStringWord, " % .4X", EEPROM_Write_Buffer[6]);

     //Serial.print(TempStringWord); Serial.print(" ");

    // Serial.println("");
    // Serial.println("EEPROM_Write_Buffer Config Word: ");
    sprintf(TempStringWord, " % .4X", EEPROM_Write_Buffer[11]);
    Encryption_Key = Encryption_Key + TempStringWord;
    // Serial.print("0x"); Serial.print(TempStringWord); Serial.print(" ");
    // Serial.println("");
    if (monFichier) {
      value = "";
      value = value + Encryption_Key;
      monFichier.println("EEPROM_Write_Buffer Config Word: " + Encryption_Key);
    }
    /*
      if (bool_choix_simple || bool_choix_normal || bool_choix_secure) {
      if (monFichier) {
      monFichier.close();
      }
      }
    */
    delay(100);

    //START M_PROG_INIT
    digitalWrite(DATA, LOW);
    digitalWrite(CLK, LOW);
    digitalWrite(HCSVDD, HIGH);
    delay(16); //delay 16milis
    //M_PROG_SETUP
    digitalWrite(CLK, HIGH);
    delay(Tps); //WAIT Program mode Setup Time (Tps)
    digitalWrite(DATA, HIGH);
    delay(Tph1);  //WAIT Program Hold Time 1 (Tph1)
    digitalWrite(DATA, LOW);
    delayMicroseconds(Tph2);     //WAIT Program Hold Time 2 (Tph2)
    //M_PROG_BULK_ER
    digitalWrite(CLK, LOW);
    delay(Tpbw);     //WAIT Program Bulk Write Time (Tpbw)
    // Serial.println("Writting EEPROM....");
    tft.setCursor(167 + 2, 144 + 2);
    tft.setTextColor(TEAL, BLACK);
    tft.setTextSize(1);
    tft.print(F("Writting EEPROM...."));
    for (byte i = 0; i < 12; i++) { //One word at time
      for (byte j = 0; j < 16; j++) { //One bit at time
        digitalWrite(CLK, HIGH);
        if ( bitRead(EEPROM_Write_Buffer[i], j) ) { //Read bit from word
          digitalWrite(DATA, HIGH);
        } else {
          digitalWrite(DATA, LOW);
        }
        delayMicroseconds(Tclkh);   // Tclkh
        digitalWrite(CLK, LOW);
        delayMicroseconds(Tclkl);   // Tclkl
      } // One bit at time
      digitalWrite(DATA, LOW);  //END OUTPUT WORD  DATA=0

      if ( bool_choix_hcs201) {
        pinMode(DATA, INPUT);
        digitalWrite(CLK, HIGH);
        while (compteur_HCS201 != 4) {
          HC201_OSC = pulseIn(DATA, HIGH, 810);
          if (HC201_OSC > 795 && HC201_OSC <= 800) {
            compteur_HCS201 ++;
          }
        }
        compteur_HCS201 = 0;
        pinMode(DATA, OUTPUT);
        digitalWrite(CLK, LOW);
        delayMicroseconds(Tphold);
      }
      else {
        delay(Twc); //WAIT FOR WORD Write Cycle Time (Twc) 40ms
      }
    } //One word at time
    //desligar linhas!
    tft.setCursor(167 + 2, 144 + 2);
    tft.setTextColor(TEAL, BLACK);
    tft.setTextSize(1);
    tft.print(F("Writting EEPROM Complete!"));
    //Serial.println("Writting EEPROM Complete!");
    pinMode(DATA, INPUT);
    delay(100);
    Verify_EEPROM();

    if (bool_EEPROM_Write_Error) {
      SD.remove(value_hcs_serial);
     // Serial.println(value_hcs_serial);
      monFichier.close();
    }

    else {
      if (bool_choix_simple || bool_choix_normal || bool_choix_secure) {
        if (monFichier) {
          monFichier.close();
        }
      }
    }
  }
  bool_SD_serial_exist = false;
  mode_menu = 2;
}
/*
  void software_Reset() {
  asm volatile ("  jmp 0");
  }
*/

void Verify_EEPROM()  {

  // Serial.println("Reading EEPROM...");
  tft.setCursor(167 + 2, 144 + 2);
  tft.setTextColor(BLACK, BLACK);
  tft.setTextSize(1);
  tft.print(F("Writting EEPROM Complete!"));
  tft.setCursor(167 + 2, 144 + 2);
  tft.setTextColor(DKTEAL, BLACK);
  tft.setTextSize(1);
  tft.print(F("Reading EEPROM..."));

  for (byte i = 0; i < 12; i++) { //One word at time
    EEPROM_Read_Buffer[i] = 0xFFFF; //Fill buffer with 0xFF
    for (byte j = 0; j < 16; j++) { //One bit at time
      delay(Twc); //WAIT FOR WORD Write Cycle Time (Twc)
      if (digitalRead(DATA)) {
        bitSet(EEPROM_Read_Buffer[i], j);
      }
      else {
        bitClear(EEPROM_Read_Buffer[i], j);
      }
      digitalWrite(CLK, HIGH);
      delayMicroseconds(Tclkh);   // Tclkh
      digitalWrite(CLK, LOW);
      delayMicroseconds(Tclkl);   // Tclkl
    } // One bit at time
  } //One word at time
  digitalWrite(HCSVDD, LOW);
  digitalWrite(CLK, LOW);
  // pinMode(HCSVDD, INPUT);
  // pinMode(CLK, INPUT);
  delay(100);
  /*
    // Serial.println("EEPROM_Read_Buffer: ");
    for (int i = 11; i >= 0; i--) {
    sprintf(TempStringWord, " % .4X", EEPROM_Read_Buffer[i]);
    // Serial.print(TempStringWord); delay(50); Serial.print(" ");
    }
  */
  // Serial.println("");
  for (byte i = 0; i < 12; i++) {
    if (EEPROM_Read_Buffer[i] != EEPROM_Write_Buffer[i])EEPROM_Write_Error = true;
  }
  if (EEPROM_Write_Error) {
    //  Serial.println("EEPROM Write Error!");
    tft.setCursor(167 + 2, 144 + 2);
    tft.setTextColor(RED, BLACK);
    tft.setTextSize(1);
    tft.print(F("EEPROM Write Error!"));
    bool_EEPROM_Write_Error = true;
    EEPROM_Write_Error = false;
  }
  else {
    //  Serial.println("EEPROM Write Succefull!");
    tft.setCursor(167 + 2, 144 + 2);
    tft.setTextColor(DKGREEN, BLACK);
    tft.setTextSize(1);
    tft.print(F("EEPROM Write Succefull!"));
    bool_EEPROM_Write_Succefull = true;
  }
}


void CheckFwd_Enter_Menu_Key_board() {

  bool down = Touch_getXY();

  b_0btn.press(down && b_0btn.contains(actx, acty));
  if (b_0btn.justReleased())
    b_0btn.drawButton();
  if (b_0btn.justPressed()) {
    b_0btn.drawButton(true);
    q_button = 0;
    Zero_un_button();
  }
  b_1btn.press(down && b_1btn.contains(actx, acty));
  if (b_1btn.justReleased())
    b_1btn.drawButton();
  if (b_1btn.justPressed()) {
    b_1btn.drawButton(true);
    q_button = 1;
    Zero_un_button();
  }
  if (!bool_ASD) {
    b_2btn.press(down && b_2btn.contains(actx, acty));
    if (b_2btn.justReleased())
      b_2btn.drawButton();
    if (b_2btn.justPressed()) {
      b_2btn.drawButton(true);
      q_button = 2;
      Keyboard_button();
    }
    b_3btn.press(down && b_3btn.contains(actx, acty));
    if (b_3btn.justReleased())
      b_3btn.drawButton();
    if (b_3btn.justPressed()) {
      b_3btn.drawButton(true);
      q_button = 3;
      Keyboard_button();
    }
    b_4btn.press(down && b_4btn.contains(actx, acty));
    if (b_4btn.justReleased())
      b_4btn.drawButton();
    if (b_4btn.justPressed()) {
      b_4btn.drawButton(true);
      q_button = 4;
      Keyboard_button();
    }
    b_5btn.press(down && b_5btn.contains(actx, acty));
    if (b_5btn.justReleased())
      b_5btn.drawButton();
    if (b_5btn.justPressed()) {
      b_5btn.drawButton(true);
      q_button = 5;
      Keyboard_button();
    }
    b_6btn.press(down && b_6btn.contains(actx, acty));
    if (b_6btn.justReleased())
      b_6btn.drawButton();
    if (b_6btn.justPressed()) {
      b_6btn.drawButton(true);
      q_button = 6;
      Keyboard_button();
    }
    b_7btn.press(down && b_7btn.contains(actx, acty));
    if (b_7btn.justReleased())
      b_7btn.drawButton();
    if (b_7btn.justPressed()) {
      b_7btn.drawButton(true);
      q_button = 7;
      Keyboard_button();
    }
    b_8btn.press(down && b_8btn.contains(actx, acty));
    if (b_8btn.justReleased())
      b_8btn.drawButton();
    if (b_8btn.justPressed()) {
      b_8btn.drawButton(true);
      q_button = 8;
      Keyboard_button();
    }
    b_9btn.press(down && b_9btn.contains(actx, acty));
    if (b_9btn.justReleased())
      b_9btn.drawButton();
    if (b_9btn.justPressed()) {
      b_9btn.drawButton(true);
      q_button = 9;
      Keyboard_button();
    }
    b_Abtn.press(down && b_Abtn.contains(actx, acty));
    if (b_Abtn.justReleased())
      b_Abtn.drawButton();
    if (b_Abtn.justPressed()) {
      b_Abtn.drawButton(true);
      q_button = 10;
      Keyboard_button();
    }
    b_Bbtn.press(down && b_Bbtn.contains(actx, acty));
    if (b_Bbtn.justReleased())
      b_Bbtn.drawButton();
    if (b_Bbtn.justPressed()) {
      b_Bbtn.drawButton(true);
      q_button = 11;
      Keyboard_button();
    }
    b_Cbtn.press(down && b_Cbtn.contains(actx, acty));
    if (b_Cbtn.justReleased())
      b_Cbtn.drawButton();
    if (b_Cbtn.justPressed()) {
      b_Cbtn.drawButton(true);
      q_button = 12;
      Keyboard_button();
    }
    b_Dbtn.press(down && b_Dbtn.contains(actx, acty));
    if (b_Dbtn.justReleased())
      b_Dbtn.drawButton();
    if (b_Dbtn.justPressed()) {
      b_Dbtn.drawButton(true);
      q_button = 13;
      Keyboard_button();
    }
    b_Ebtn.press(down && b_Ebtn.contains(actx, acty));
    if (b_Ebtn.justReleased())
      b_Ebtn.drawButton();
    if (b_Ebtn.justPressed()) {
      b_Ebtn.drawButton(true);
      q_button = 14;
      Keyboard_button();
    }
    b_Fbtn.press(down && b_Fbtn.contains(actx, acty));
    if (b_Fbtn.justReleased())
      b_Fbtn.drawButton();
    if (b_Fbtn.justPressed()) {
      b_Fbtn.drawButton(true);
      q_button = 15;
      Keyboard_button();
    }
  }
}


void Key_board() {
  b_0btn.initButton(&tft, 40, 185, 30, 25, WHITE, BLUE, WHITE, "0", 1);
  b_0btn.drawButton(false);
  b_1btn.initButton(&tft, 75, 185, 30, 25, WHITE, BLUE, WHITE, "1", 1);
  b_1btn.drawButton(false);
  b_2btn.initButton(&tft, 110, 185, 30, 25, WHITE, BLUE, WHITE, "2", 1);
  b_2btn.drawButton(false);
  b_3btn.initButton(&tft, 145, 185, 30, 25, WHITE, BLUE, WHITE, "3", 1);
  b_3btn.drawButton(false);
  b_4btn.initButton(&tft, 180, 185, 30, 25, WHITE, BLUE, WHITE, "4", 1);
  b_4btn.drawButton(false);
  b_5btn.initButton(&tft, 215, 185, 30, 25, WHITE, BLUE, WHITE, "5", 1);
  b_5btn.drawButton(false);
  b_6btn.initButton(&tft, 250, 185, 30, 25, WHITE, BLUE, WHITE, "6", 1);
  b_6btn.drawButton(false);
  b_7btn.initButton(&tft, 285, 185, 30, 25, WHITE, BLUE, WHITE, "7", 1);
  b_7btn.drawButton(false);
  b_8btn.initButton(&tft, 40, 220, 30, 25, WHITE, BLUE, WHITE, "8", 1);
  b_8btn.drawButton(false);
  b_9btn.initButton(&tft, 75, 220, 30, 25, WHITE, BLUE, WHITE, "9", 1);
  b_9btn.drawButton(false);
  b_Abtn.initButton(&tft, 110, 220, 30, 25, WHITE, BLUE, WHITE, "A", 1);
  b_Abtn.drawButton(false);
  b_Bbtn.initButton(&tft, 145, 220, 30, 25, WHITE, BLUE, WHITE, "B", 1);
  b_Bbtn.drawButton(false);
  b_Cbtn.initButton(&tft, 180, 220, 30, 25, WHITE, BLUE, WHITE, "C", 1);
  b_Cbtn.drawButton(false);
  b_Dbtn.initButton(&tft, 215, 220, 30, 25, WHITE, BLUE, WHITE, "D", 1);
  b_Dbtn.drawButton(false);
  b_Ebtn.initButton(&tft, 250, 220, 30, 25, WHITE, BLUE, WHITE, "E", 1);
  b_Ebtn.drawButton(false);
  b_Fbtn.initButton(&tft, 285, 220, 30, 25, WHITE, BLUE, WHITE, "F", 1);
  b_Fbtn.drawButton(false);
  CLR_btn.initButton(&tft, 40, 150, 30, 25, WHITE, DKGREEN, WHITE, "CLR", 1);
  CLR_btn.drawButton(false);
  Haut_btn.initButton(&tft, 75, 150, 30, 25, WHITE, DKORANGE, WHITE, "HAUT", 1);
  Haut_btn.drawButton(false);
  Bas_btn.initButton(&tft, 110, 150, 30, 25, WHITE, DKORANGE, WHITE, "BAS", 1);
  Bas_btn.drawButton(false);
  SEND_btn.initButton(&tft, 145, 150, 30, 25, WHITE, RED, WHITE, "SEND", 1);//DKGREY
  SEND_btn.drawButton(false);
  Enter_btn.initButton(&tft, 295, 120, 40, 25, WHITE, LTGREEN, WHITE, "RESET", 1);//PURPLE
  Enter_btn.drawButton(false);
}

void CheckFwdMenuButtons_label1() {

  bool down = Touch_getXY();

  CLR_btn.press(down && CLR_btn.contains(actx, acty));
  if (CLR_btn.justReleased())
    CLR_btn.drawButton();
  if (CLR_btn.justPressed()) {
    CLR_btn.drawButton(true);
    Efface_tft();
    if (mode == 0) {
      // bool_ASD = false;
      textfield[textfield_i] = 0;
      if (textfield > 0) {
        textfield_i--;
        if (textfield_i == 255  ) {
          textfield_i = 0;
        }
        textfield[textfield_i] = ' ';
      }
    }
    if (mode == 1) {
      // bool_ASD = false;
      textfield_SERIAL[textfield_i_SERIAL] = 0;
      if (textfield_SERIAL > 0) {
        textfield_i_SERIAL--;
        if (textfield_i_SERIAL == 255 ) {
          textfield_i_SERIAL = 0;
        }
        textfield_SERIAL[textfield_i_SERIAL] = ' ';
      }
    }
    if (mode == 2) {
      // bool_ASD = false;
      textfield_SEED[textfield_i_SEED] = 0;
      if (textfield_SEED > 0) {
        textfield_i_SEED--;
        if (textfield_i_SEED == 255 ) {
          textfield_i_SEED = 0;
        }
        textfield_SEED[textfield_i_SEED] = ' ';
      }
    }
    if (mode == 3) {
      //bool_ASD = false;
      textfield_DISC[textfield_i_DISC] = 0;
      if (textfield_DISC > 0) {
        textfield_i_DISC--;
        if (textfield_i_DISC == 255 ) {
          textfield_i_DISC = 0;
        }
        textfield_DISC[textfield_i_DISC] = ' ';
      }
    }
    if (mode == 4) {
      // bool_ASD = false;
      textfield_SYNC[textfield_i_SYNC] = 0;
      if (textfield_SYNC > 0) {
        textfield_i_SYNC--;
        if (textfield_i_SYNC == 255 ) {
          textfield_i_SYNC = 0;
        }
        textfield_SYNC[textfield_i_SYNC] = ' ';
      }
    }
    if (mode == 5) {
      // bool_ASD = true;
      textfield_CONFIG_Ads[textfield_i_CONFIG_Ads] = 0;
      if (textfield_CONFIG_Ads > 0) {
        textfield_i_CONFIG_Ads--;

        if (textfield_i_CONFIG_Ads == 255 ) {
          textfield_i_CONFIG_Ads = 0;
        }
        textfield_CONFIG_Ads[textfield_i_CONFIG_Ads] = ' ';
      }
    }
  }
  Haut_btn.press(down && Haut_btn.contains(actx, acty));
  if (Haut_btn.justReleased())
    Haut_btn.drawButton();
  if (Haut_btn.justPressed()) {
    Haut_btn.drawButton(true);
    Count_Config--;
    if (Count_Config <= 0 ) {
      Count_Config = 0;
    }
    if (Count_Config == 255 ) {
      Count_Config = 0;
    }
  }
  Bas_btn.press(down && Bas_btn.contains(actx, acty));
  if (Bas_btn.justReleased())
    Bas_btn.drawButton();
  if (Bas_btn.justPressed()) {
    Bas_btn.drawButton(true);
    Count_Config++;
    if (Count_Config >= 5) {
      Count_Config = 5;
    }
  }
  SEND_btn.press(down && SEND_btn.contains(actx, acty));
  if (SEND_btn.justReleased())
    SEND_btn.drawButton();
  if (SEND_btn.justPressed()) {
    SEND_btn.drawButton(true);
    if ( bool_choix_simple || bool_choix_normal || bool_choix_secure) {
      mode_menu = 3;//3
    }
  }
  Enter_btn.press(down && Enter_btn.contains(actx, acty));
  if (Enter_btn.justReleased())
    Enter_btn.drawButton();
  if (Enter_btn.justPressed()) {
    Enter_btn.drawButton(true);
    Parametres_remise_zero();
    mode_menu = 4;//0
  }
}

void Rectangles() {
  tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, WHITE);
  tft.drawRect(5, 30, 102, 20, WHITE);
  tft.drawRect(5, 55, 102, 20, WHITE);
  tft.drawRect(210, 30, 51, 20, WHITE);
  tft.drawRect(210, 55, 51, 20, WHITE);
  tft.drawRect(5, 80, 25, 20, WHITE); //ads
  tft.drawRect(165, 140, 155, 20, WHITE);//write eeprom
  tft.setTextSize(2);
  tft.setTextColor(YELLOW);
}

void Ads() {
  tft.setTextSize(2);
  tft.setTextColor(Color);
  tft.setCursor(32, 82);
  tft.print(F("ASD"));
}
/*
  void Write_hcs_def() {
  tft.setTextSize(1);
  tft.setTextColor(CYAN);
  tft.setCursor(243, 110);
  tft.print(F(" - - -"));
  }
*/
void Mancode() {
  tft.setTextSize(2);
  tft.setTextColor(Color);
  tft.setCursor(215, 10);
  tft.print(F("MANCODE"));
}
void Serial_Num() {
  tft.setTextSize(2);
  tft.setTextColor(Color);
  tft.setCursor(110, 32);
  tft.print(F("SERIAL"));
}
void Seed() {
  tft.setTextSize(2);
  tft.setTextColor(Color);
  tft.setCursor(110, 57);
  tft.print(F("SEED"));
}
void Disc() {
  tft.setTextSize(2);
  tft.setTextColor(Color);
  tft.setCursor(265, 32);
  tft.print(F("DISC"));
}
void Sync() {
  tft.setTextSize(2);
  tft.setTextColor(Color);
  tft.setCursor(265, 57);
  tft.print(F("SYNC"));
}


void transforme_char_uint64() {

  String val = "0";
  String value0x = "0x";

  if (mode == 0) {

    for (byte num = 12; num < 16; num++) {
      value0x = value0x + String(textfield[num]);
    }

    MCODE_0 = strtol(value0x.c_str(), NULL, 0);

    value0x = "0x";
    for (byte num = 8; num < 12; num++) {
      value0x = value0x + String(textfield[num]);
    }
    MCODE_1 = strtol(value0x.c_str(), NULL, 0);

    value0x = "0x";
    for (byte num = 4; num < 8; num++) {
      value0x = value0x + String(textfield[num]);
    }
    MCODE_2 = strtol(value0x.c_str(), NULL, 0);

    value0x = "0x";
    for (byte num = 0; num < 4; num++) {
      value0x = value0x + String(textfield[num]);
    }
    MCODE_3 = strtol(value0x.c_str(), NULL, 0);

    if (MCODE_3 <= 0x0FFF) {
      tft.setCursor(167 + 2, 144 + 2);
      tft.setTextColor(RED, BLACK);
      tft.setTextSize(1);
      tft.print(F("Invalid Mancode!"));
      bool_invalide_mancode = true;
    }
  }
  if (mode == 1) {

    value0x = "0x";
    for (byte num = 4; num < 8; num++) {
      value0x = value0x + String(textfield_SERIAL[num]);
    }
    SER_0  = strtol(value0x.c_str(), NULL, 0);

    value0x = "0x";
    for (byte num = 0; num < 4; num++) {
      value0x = value0x + String(textfield_SERIAL[num]);
    }

    SER_1  = strtol(value0x.c_str(), NULL, 0);

    if (SER_1 >= 0x1000) {
      tft.setCursor(167 + 2, 144 + 2);
      tft.setTextColor(RED, BLACK);
      tft.setTextSize(1);
      tft.print(F("Invalid Serial!"));
      bool_invalide_serial = true;
    }
  }
  if (mode == 2) {

    value0x = "0x";
    for (byte num = 4; num < 8; num++) {
      value0x = value0x + String(textfield_SEED[num]);
    }
    SEED_0  = strtol(value0x.c_str(), NULL, 0);
    value0x = "0x";
    for (byte num = 0; num < 4; num++) {
      value0x = value0x + String(textfield_SEED[num]);
    }
    SEED_1  = strtol(value0x.c_str(), NULL, 0);

    if (SEED_1 <= 0x01FF) {
      tft.setCursor(167 + 2, 144 + 2);
      tft.setTextColor(RED, BLACK);
      tft.setTextSize(1);
      tft.print(F("Invalid Seed!"));
      bool_invalide_seed = true;
    }
  }
  if (mode == 3) {

    value0x = "0x";
    for (byte num = 0; num < 4; num++) {
      value0x = value0x + String(textfield_DISC[num]);
    }
    DISC = strtol(value0x.c_str(), NULL, 0);
  }

  if (mode == 4) {

    value0x = "0x";
    for (byte num = 0; num < 4; num++) {
      value0x = value0x + String(textfield_SYNC[num]);
    }
    SYNC = strtol(value0x.c_str(), NULL, 0);
  }
  if (mode == 5) {

    val = "0";
    val = textfield_CONFIG_Ads[0];
    AUTOFF = strtol(val.c_str(), NULL, 0);
  }
}

void Choix_Config() {
  switch (Count_Config) {
    case 0:
      // byte line_number;
      mode = 0;
      Color = GREEN;
      Mancode();
      Color = YELLOW;
      Serial_Num();
      CheckFwd_Enter_Menu_Key_board();
      //Serial.println(textfield);
      tft.setCursor(TEXT_X + 2, TEXT_Y + 2);
      tft.setTextColor(TEXT_TCOLOR, BLACK);
      tft.setTextSize(TEXT_TSIZE);
      tft.print(textfield);
      CheckFwdMenuButtons_label1();
      if (textfield_i == 16) {
        transforme_char_uint64();
      }
      break;

    case 1 :
      //if ( !bool_ASD) {
      mode = 1;
      Color = YELLOW;
      Mancode();
      Seed();
      Color = GREEN;
      Serial_Num();
      CheckFwd_Enter_Menu_Key_board();
      // Serial.println(textfield_SERIAL);
      tft.setCursor(5 + 2, 30 + 2);
      tft.setTextColor(TEXT_TCOLOR, BLACK);
      tft.setTextSize(TEXT_TSIZE);
      tft.print(textfield_SERIAL);
      CheckFwdMenuButtons_label1();
      if (textfield_i_SERIAL == 8) {
        transforme_char_uint64();
      }
      //}
      break;

    case 2:
      // if ( !bool_ASD) {
      mode = 2;
      Color = YELLOW;
      Serial_Num();
      Disc();
      Color = GREEN;
      Seed();
      CheckFwd_Enter_Menu_Key_board();
      //Serial.println(textfield_SEED);
      tft.setCursor(5 + 2, 55 + 2);
      tft.setTextColor(TEXT_TCOLOR, BLACK);
      tft.setTextSize(TEXT_TSIZE);
      tft.print(textfield_SEED);
      CheckFwdMenuButtons_label1();
      if (textfield_i_SEED == 8) {
        transforme_char_uint64();
      }
      // }
      break;

    case 3:
      //if ( !bool_ASD) {
      mode = 3;
      Color = YELLOW;
      Seed();
      Sync();
      Color = GREEN;
      Disc();
      CheckFwd_Enter_Menu_Key_board();
      // Serial.println(textfield_DISC);
      tft.setCursor(210 + 2, 30 + 2);
      tft.setTextColor(TEXT_TCOLOR, BLACK);
      tft.setTextSize(TEXT_TSIZE);
      tft.print(textfield_DISC);
      CheckFwdMenuButtons_label1();
      if (textfield_i_DISC == 4) {
        transforme_char_uint64();
      }
      //}
      break;

    case 4:
      //bool_ASD = false;
      // if ( !bool_ASD) {
      mode = 4;
      Color = YELLOW;
      Disc();
      Ads();
      Color = GREEN;
      Sync();
      CheckFwd_Enter_Menu_Key_board();
      // Serial.println(textfield_SYNC);
      tft.setCursor(210 + 2, 55 + 2);
      tft.setTextColor(TEXT_TCOLOR, BLACK);
      tft.setTextSize(TEXT_TSIZE);
      tft.print(textfield_SYNC);
      CheckFwdMenuButtons_label1();
      if (textfield_i_SYNC == 4) {
        transforme_char_uint64();
      }
      // }
      break;

    case 5:
      // bool_ASD = true;
      mode = 5;
      Color = YELLOW;
      Sync();
      Color = GREEN;
      Ads();
      CheckFwd_Enter_Menu_Key_board();
      //Serial.println(textfield_CONFIG_Ads);
      tft.setCursor(10 + 2, 81 + 2);
      tft.setTextColor(TEXT_TCOLOR, BLACK);
      tft.setTextSize(TEXT_TSIZE);
      tft.print(textfield_CONFIG_Ads);
      CheckFwdMenuButtons_label1();
      if (textfield_i_CONFIG_Ads == 1) {
        transforme_char_uint64();
      }
      break;
  }
}


void config_choix() {
  Key_board();
  // Key_board_label1();
  Rectangles();
  Color = GREEN;
  Mancode();
  Color = YELLOW;
  Serial_Num();
  Disc();
  Seed();
  Sync();
  Ads();
  mode = 0;
  mode_config = 10;
  Count_Config = 0;
}


void Page_une_hcs() {

  tft.setTextSize(2);
  tft.setTextColor(YELLOW);
  tft.drawRect(5, 5, 314, 234, WHITE);
  tft.drawRect(5, 5, 135, 234, WHITE);//5, 5, 157, 234

  float a = 5;
  for (int x = 0; x <= 7 ; x++) {
    tft. drawLine(5, a, 318 , a , WHITE);
    a = a + 29.25;
  }
  tft.setCursor(10, 14.8125);//15  9,8125 + 5 = 14.8125
  tft.print(F("HCS200"));
  tft.setCursor(10, 41.5625);
  tft.print(F("HCS201"));
  tft.setCursor(10, 70.8125);
  tft.print(F("HCS300"));
  tft.setCursor(10, 100.0625);
  tft.print(F("HCS301"));
  tft.setCursor(10, 129.3125);
  tft.print(F("HCS320"));
  tft.setCursor(10, 158.5625);
  tft.setTextColor(WHITE);
  tft.print(F("HCS360"));
  tft.setCursor(10, 187.8125);
  tft.print(F("HCS361"));

  uint16_t  color1 = RED;
  uint16_t  color = BLUE;
  float y = 19.625;
  for ( int x = 0; x <= 6; x++) {//6
    tft.drawCircle(100, y, 9, color);
    tft.drawCircle(100, y, 8, color);
    tft.fillCircle(100, y, 7, color1);
    y = y + 29.25;
    //  Serial.println(y);
    if ( y >= 136.63) {
      // color = WHITE;
      color1 = WHITE;
    }
  }
  y = 48.875;
  color = BLUE;
  color1 = RED;
  for ( int x = 0; x <= 1; x++) {
    tft.drawCircle(279, y, 9, color);
    tft.drawCircle(279, y, 8, color);
    tft.fillCircle(279, y, 7, color1);
    y = y + 29.25;
    // Serial.println(y);
  }
  y = y + 29.25;
  for ( int x = 0; x <= 2; x++) {
    tft.drawCircle(279, y, 9, color);
    tft.drawCircle(279, y, 8, color);
    tft.fillCircle(279, y, 7, color1);
    y = y + 29.25;
    // Serial.println(y);
  }
  tft.setTextColor(BLUE);
  tft.fillRect(140, 6, 178, 28.25, ORANGE);
  tft.setCursor(145, 14.8125);
  tft.print(F("DISCRIMINATION"));
  tft.setTextColor(YELLOW);
  tft.setCursor(189, 41.5625);
  tft.print(F("DISC"));
  tft.setCursor(189, 70.8125);
  tft.print(F("SER_0"));
  tft.setTextColor(BLUE);
  tft.fillRect(140, 93.75, 178, 29, ORANGE);//93.75
  tft.setCursor(189, 100.0625);//145
  tft.print(F("LEARN"));
  tft.setTextColor(YELLOW);
  tft.setCursor(189, 129.3125);
  tft.print(F("SIMPLE"));
  tft.setCursor(189, 158.5625);
  tft.print(F("NORMAL"));
  tft.setCursor(189, 187.8125);
  tft.print(F("SECURE"));
  tft.fillRect(140, 210.75, 178, 28.25, BLUEL);//210.75
  //tft.setTextColor(GREEN);
  Parametres_remise_zero();
  actx = 0;
  acty = 0;
}

void Rectangles_choix_hcs() {
  actx = 0;
  acty = 0;
  bool down = Touch_getXY();

  if (actx > 92 && actx < 100) {
    if (acty > 8 && acty < 17)  {
      Parametres_hcs();
      tft.fillCircle(100, 19.625, 7, GREEN);//29.25
      bool_choix_hcs200 = true;
      // actx = 0;
      // acty = 0;
      goto fin;
    }
    if (acty > 38 && acty < 47)  {
      Parametres_hcs();
      tft.fillCircle(100, 48.88, 7, GREEN);
      bool_choix_hcs201 = true;
      // actx = 0;
      // acty = 0;
      goto fin;
    }
    if (acty > 70 && acty < 77)  {
      Parametres_hcs();
      tft.fillCircle(100, 78.12, 7, GREEN);
      bool_choix_hcs300 = true;
      // actx = 0;
      // acty = 0;
      goto fin;
    }
    if (acty > 99 && acty < 108)  {
      Parametres_hcs();
      tft.fillCircle(100, 107.37, 7, GREEN);
      bool_choix_hcs301 = true;
      // actx = 0;
      // acty = 0;
      goto fin;
    }
    if (acty > 131 && acty < 140)  {
      Parametres_hcs();
      tft.fillCircle(100, 136.63, 7, GREEN);
      bool_choix_hcs320 = true;
      // actx = 0;
      // acty = 0;
      goto fin;
    }
  }
  if (actx > 273 && actx < 284) {
    if (acty > 38 && acty < 47)  {
      Parametres_hcs_disc();
      tft.fillCircle(279, 48.88, 7, GREEN);
      bool_disc = true;
      //  actx = 0;
      //  acty = 0;
      goto fin;
    }
    if (acty > 70 && acty < 77)  {
      Parametres_hcs_disc();
      tft.fillCircle(279, 78.12, 7, GREEN);
      bool_disc_Ser0 = true;
      //  actx = 0;
      //  acty = 0;
      goto fin;
    }
    if (acty > 132 && acty < 139)  {
      Parametres_hcs_learn();
      tft.fillCircle(279, 136.63, 7, GREEN);
      bool_choix_simple = true;
      //  actx = 0;
      //  acty = 0;
      goto fin;
    }
    if (acty > 162 && acty < 172)  {
      Parametres_hcs_learn();
      tft.fillCircle(279, 165.88, 7, GREEN);
      bool_choix_normal = true;
      // actx = 0;
      //  acty = 0;
      goto fin;
    }
    if (acty > 194 && acty < 203)  {
      Parametres_hcs_learn();
      tft.fillCircle(279, 195.13, 7, GREEN);
      bool_choix_secure = true;
      //  actx = 0;
      //  acty = 0;
      goto fin;
    }
  }
  if ((bool_choix_hcs200 || bool_choix_hcs201 || bool_choix_hcs300 || bool_choix_hcs301 || bool_choix_hcs320) && (bool_disc || bool_disc_Ser0) && (bool_choix_simple || bool_choix_normal || bool_choix_secure)) {
    tft.setTextColor(GREEN);
    tft.setCursor(189, 217.0625);
    tft.print(F("OK"));
    bool_page_deux = true;
  }

  // }
  if (bool_page_deux) {
    if ((actx > 144 && actx < 310) && (acty > 220 && acty < 240)) {
      mode_menu = 1;
      tft.fillScreen(BLACK);
      Page_deux_hcs();
      //  actx = 0;
      //  acty = 0;
      goto fin;
    }
  }

fin:

  actx = 0;
  acty = 0;

  delay(50);
}

void Parametres_hcs() {
  float y = 19.625;
  //  Serial.println(y);
  for (int x = 0; x <= 4; x++) {
    tft.fillCircle(100, y, 7, RED);
    y = y + 29.25;
    // Serial.println(y);
  }
  bool_choix_hcs200 = false;
  bool_choix_hcs201 = false;
  bool_choix_hcs300 = false;
  bool_choix_hcs301 = false;
  bool_choix_hcs320 = false;
}

void Parametres_hcs_disc() {
  tft.fillCircle(279, 48.88, 7, RED);
  tft.fillCircle(279, 78.12, 7, RED);
  bool_disc = false;
  bool_disc_Ser0 = false;
}

void Parametres_hcs_learn() {
  tft.fillCircle(279, 136.63, 7, RED);
  tft.fillCircle(279, 165.88, 7, RED);
  tft.fillCircle(279, 195.13, 7, RED);
  bool_choix_simple = false;
  bool_choix_normal = false;
  bool_choix_secure = false;
}

void Page_deux_hcs() {
  int b = 0;
  tft.setTextSize(2);
  tft.setTextColor(YELLOW);
  tft.drawRect(5, 5, 315, 234, WHITE);
  tft.fillRect(6, 6, 313, 232, GREY);
  tft.fillRect(215, 209.75, 103, 28.25, BLUEL);
  tft.setCursor(230, 217.0625);
  tft.print(F("OK"));
  float a = 5;
  for (int x = 0; x <= 7 ; x++) {
    tft. drawLine(5, a, 318 , a , WHITE);
    a = a + 29.25;
  }
  a = 110;
  for (int x = 0; x <= 2 ; x++) {
    tft. drawLine(a, 34.25, a , 237 , WHITE);
    a = a + 105;
  }
  tft.fillRect(6, 6, 313, 28.25, ORANGE);
  tft.setCursor(50, 14.8125);
  tft.setTextColor(BLUE);
  tft.print(F("CONFIGURATION WORLD"));
  tft.setTextColor(YELLOW);
  if (bool_choix_hcs201) {
    tft.setCursor(10, 41.5625);
    tft.print(F("OSC0"));
    tft.setCursor(10, 70.8125);
    tft.print(F("OSC1"));
    tft.setCursor(10, 100.0625);
    tft.print(F("OSC2"));
    tft.setCursor(10, 129.3125);
    tft.print(F("OSC3"));
    tft.setCursor(10, 158.5625);
    tft.print(F("Vlow"));
    tft.setCursor(10, 187.8125);
    tft.print(F("BRS"));
    tft.setCursor(10, 217.0625);
    tft.print(F("MTX4"));
    tft.setCursor(115, 41.5625);
    tft.print(F("TXEN"));
    tft.setCursor(115, 70.8125);
    tft.print(F("S3SET"));
    tft.setCursor(115, 100.0625);
    tft.print(F("XSER"));
  }
  float y = 0;
  if (bool_choix_hcs200) {
    tft.setCursor(115, 158.5625);
    tft.print(F("Vlow"));
    tft.setCursor(115, 187.8125);
    tft.print(F("BSL0"));
    y = 165.87;
    b = 1;
    for ( int x = 0; x <= b; x++) {
      tft.drawCircle(195, y, 9, BLUE);
      tft.drawCircle(195, y, 8, BLUE);
      tft.fillCircle(195, y, 7, RED);
      y = y + 29.25;
    }
  }
  if (bool_choix_hcs201) {
    y = 48.875;
    b = 6;
    for ( int x = 0; x <= b; x++) {
      tft.drawCircle(90, y, 9, BLUE);
      tft.drawCircle(90, y, 8, BLUE);
      tft.fillCircle(90, y, 7, RED);
      y = y + 29.25;
    }
    y = 48.875;
    b = 2;
    for ( int x = 0; x <= b; x++) {
      tft.drawCircle(195, y, 9, BLUE);
      tft.drawCircle(195, y, 8, BLUE);
      tft.fillCircle(195, y, 7, RED);
      y = y + 29.25;
    }
  }
  if ((bool_choix_hcs300) || (bool_choix_hcs301) || (bool_choix_hcs320)) {
    tft.setCursor(115, 100.0625);
    tft.print(F("OVR0"));
    tft.setCursor(115, 129.3125);
    tft.print(F("OVR1"));
    tft.setCursor(115, 158.5625);
    tft.print(F("Vlow"));
    tft.setCursor(115, 187.8125);
    tft.print(F("BSL0"));
    tft.setCursor(115, 217.0625);
    tft.print(F("BSL1"));
    y = 107.37;
    b = 4;
    for ( int x = 0; x <= b; x++) {
      tft.drawCircle(195, y, 9, BLUE);
      tft.drawCircle(195, y, 8, BLUE);
      tft.fillCircle(195, y, 7, RED);
      y = y + 29.25;
    }
  }

}

void Parametres_config_world() {
  // actx = 0;
  // acty = 0;
  bool down = Touch_getXY();
  if (actx > 81 && actx < 91) {
    if (acty > 42 && acty < 52)  {
      if (bool_choix_hcs201) {
        bool_OSC0 = !bool_OSC0;
        if (bool_OSC0) {
          tft.fillCircle(90, 48.88, 7, GREEN);
          OSC0 = 1;
          goto fin;
        }
        if (!bool_OSC0) {
          tft.fillCircle(90, 48.88, 7, RED);
          OSC0 = 0;
          goto fin;
        }
      }
    }
    if (acty > 70 && acty < 79)  {
      if (bool_choix_hcs201) {
        bool_OSC1 = !bool_OSC1;
        if (bool_OSC1) {
          tft.fillCircle(90, 78.12, 7, GREEN);
          OSC1 = 1;
          goto fin;
        }
        if (!bool_OSC1) {
          tft.fillCircle(90, 78.12, 7, RED);
          OSC1 = 0;
          goto fin;
        }
      }
    }
    if (acty > 102 && acty < 112)  {
      if (bool_choix_hcs201) {
        bool_OSC2 = !bool_OSC2;
        if (bool_OSC2) {
          tft.fillCircle(90, 107.37, 7, GREEN);
          OSC2 = 1;
          goto fin;
        }
        if (!bool_OSC2) {
          tft.fillCircle(90, 107.37, 7, RED);
          OSC2 = 0;
          goto fin;
        }
      }
    }
    if (acty > 132 && acty < 142)  {
      if (bool_choix_hcs201) {
        bool_OSC3 = !bool_OSC3;
        if (bool_OSC3) {
          tft.fillCircle(90, 136.63, 7, GREEN);
          OSC3 = 1;
          goto fin;
        }
        if (!bool_OSC3) {
          tft.fillCircle(90, 136.63, 7, RED);
          OSC3 = 0;
          goto fin;
        }
      }
    }
    if (acty > 162 && acty < 172)  {
      if (bool_choix_hcs201) {
        bool_Vlow = !bool_Vlow;
        if (bool_Vlow) {
          tft.fillCircle(90, 165.88, 7, GREEN);
          VLOW = 1;
          goto fin;
        }
        if (!bool_Vlow) {
          tft.fillCircle(90, 165.88, 7, RED);
          VLOW = 0;
          goto fin;
        }
      }
    }
    if (acty > 194 && acty < 204)  {
      if (bool_choix_hcs201) {
        bool_BRS = !bool_BRS;
        if (bool_BRS) {
          tft.fillCircle(90, 195.13, 7, GREEN);
          BRS = 1;
          goto fin;
        }
        if (!bool_BRS) {
          tft.fillCircle(90, 195.13, 7, RED);
          BRS = 0;
          goto fin;
        }
      }
    }
    if (acty > 224 && acty < 234)  {
      if (bool_choix_hcs201) {
        bool_MTX4 = !bool_MTX4;
        if (bool_MTX4) {
          tft.fillCircle(90, 224.38, 7, GREEN);
          MTX4 = 1;
          goto fin;
        }
        if (!bool_MTX4) {
          tft.fillCircle(90, 224.38, 7, RED);
          MTX4 = 0;
          goto fin;
        }
      }
    }
  }
  //deuxieme colonne
  if (actx > 188 && actx < 198) {
    if (acty > 42 && acty < 52)  {
      if (bool_choix_hcs201) {
        bool_TXEN = !bool_TXEN;
        if (bool_TXEN) {
          tft.fillCircle(195, 48.88, 7, GREEN);
          _TXEN = 1;
          goto fin;
        }
        if (!bool_TXEN) {
          tft.fillCircle(195, 48.88, 7, RED);
          _TXEN = 0;
          goto fin;
        }
      }
    }
    if (acty > 70 && acty < 79)  {
      if (bool_choix_hcs201) {
        bool_S3SET = !bool_S3SET;
        if (bool_S3SET) {
          tft.fillCircle(195, 78.12, 7, GREEN);
          S3SET = 1;
          goto fin;
        }
        if (!bool_S3SET) {
          tft.fillCircle(195, 78.12, 7, RED);
          S3SET = 0;
          goto fin;
        }
      }
    }
    if (acty > 102 && acty < 112)  {
      if (bool_choix_hcs201) {
        bool_XSER = !bool_XSER;
        if (bool_XSER) {
          tft.fillCircle(195, 107.37, 7, GREEN);
          XSER = 1;
          goto fin;
        }
        if (!bool_XSER) {
          tft.fillCircle(195, 107.37, 7, RED);
          XSER = 0;
          goto fin;
        }
      }
      if ((bool_choix_hcs300) || (bool_choix_hcs301) || (bool_choix_hcs320)) {
        bool_OVR0 = !bool_OVR0 ;
        if (bool_OVR0) {
          tft.fillCircle(195, 107.37, 7, GREEN);
          OVR0 = 1;
          goto fin;
        }
        if (!bool_OVR0) {
          tft.fillCircle(195, 107.37, 7, RED);
          OVR0 = 0;
          goto fin;
        }
      }
    }
    if (acty > 131 && acty < 142)  {
      if ((bool_choix_hcs300) || (bool_choix_hcs301) || (bool_choix_hcs320)) {
        bool_OVR1 = !bool_OVR1 ;
        if (bool_OVR1) {
          tft.fillCircle(195, 136.62, 7, GREEN);
          OVR1 = 1;
          goto fin;
        }
        if (!bool_OVR1) {
          tft.fillCircle(195, 136.62, 7, RED);
          OVR1 = 0;
          goto fin;
        }
      }
    }
    if (acty > 162 && acty < 172)  {
      if ((bool_choix_hcs200) || (bool_choix_hcs300) || (bool_choix_hcs301) || (bool_choix_hcs320)) {
        bool_Vlow = !bool_Vlow ;
        if (bool_Vlow) {
          tft.fillCircle(195, 165.87, 7, GREEN);
          VLOW = 1;
          goto fin;
        }
        if (!bool_Vlow) {
          tft.fillCircle(195, 165.87, 7, RED);
          VLOW = 0;
          goto fin;
        }
      }
    }
    if (acty > 194 && acty < 204)  {
      if ((bool_choix_hcs200) || (bool_choix_hcs300) || (bool_choix_hcs301) || (bool_choix_hcs320)) {
        bool_BSL0 = !bool_BSL0 ;
        if (bool_BSL0) {
          tft.fillCircle(195, 195.12, 7, GREEN);
          BSL0 = 1;
          goto fin;
        }
        if (!bool_BSL0) {
          tft.fillCircle(195, 195.12, 7, RED);
          BSL0 = 0;
          goto fin;
        }
      }
    }
    if (acty > 224 && acty < 234)  {
      if ((bool_choix_hcs300) || (bool_choix_hcs301) || (bool_choix_hcs320)) {
        bool_BSL1 = !bool_BSL1 ;
        if (bool_BSL1) {
          tft.fillCircle(195, 224.37, 7, GREEN);
          BSL1 = 1;
          goto fin;
        }
        if (!bool_BSL1) {
          tft.fillCircle(195, 224.37, 7, RED);
          BSL1 = 0;
          goto fin;
        }
      }
    }

  }
  if ((actx > 220 && actx < 305) && (acty > 219 && acty < 238))  {
    tft.fillScreen(BLACK);
    config_choix();
    mode_menu = 2;
    goto fin;
  }

fin:
  actx = 0;
  acty = 0;
  delay(80);
}

void Keyboard_button() {
  if (mode == 0) {
    if (textfield_i < TEXT_LEN) {
      textfield[textfield_i] = buttonlabels[q_button][0];
      textfield_i ++;
    }
  }
  if (mode == 1) {
    if (textfield_i_SERIAL < TEXT_LEN_HUIT) {
      textfield_SERIAL[textfield_i_SERIAL] = buttonlabels[q_button][0];
      textfield_i_SERIAL++;
    }
  }
  if (mode == 2) {
    if (textfield_i_SEED < TEXT_LEN_HUIT ) {
      textfield_SEED [textfield_i_SEED ] =  buttonlabels[q_button][0];
      textfield_i_SEED ++;
    }
  }
  if (mode == 3) {
    if (textfield_i_DISC < TEXT_LEN_QUATRE ) {
      textfield_DISC [textfield_i_DISC] =  buttonlabels[q_button][0];
      textfield_i_DISC ++;
    }
  }
  if (mode == 4) {
    if (textfield_i_SYNC < TEXT_LEN_QUATRE ) {
      textfield_SYNC [textfield_i_SYNC ] =  buttonlabels[q_button][0];
      textfield_i_SYNC ++;
    }
  }
  if (mode == 5) {
    q_button = 1;
    if (textfield_i_CONFIG_Ads < TEXT_LEN_UN ) {
      textfield_CONFIG_Ads [textfield_i_CONFIG_Ads ]  =  buttonlabels[q_button][0];
      textfield_i_CONFIG_Ads ++;
    }
  }
}

void Zero_un_button() {
  if (mode == 0) {
    if (textfield_i < TEXT_LEN) {
      textfield[textfield_i] = buttonlabels[q_button][0];
      textfield_i ++;
    }
  }
  if (mode == 1) {
    if (textfield_i_SERIAL < TEXT_LEN_HUIT) {
      textfield_SERIAL[textfield_i_SERIAL] = buttonlabels[q_button][0];
      textfield_i_SERIAL++;
    }
  }
  if (mode == 2) {
    if (textfield_i_SEED < TEXT_LEN_HUIT ) {
      textfield_SEED [textfield_i_SEED ] =  buttonlabels[q_button][0];
      textfield_i_SEED ++;
    }
  }
  if (mode == 3) {
    if (textfield_i_DISC < TEXT_LEN_QUATRE ) {
      textfield_DISC [textfield_i_DISC] =  buttonlabels[q_button][0];
      textfield_i_DISC ++;
    }
  }
  if (mode == 4) {
    if (textfield_i_SYNC < TEXT_LEN_QUATRE ) {
      textfield_SYNC [textfield_i_SYNC ] =  buttonlabels[q_button][0];
      textfield_i_SYNC ++;
    }
  }
  if (mode == 5) {
    if (textfield_i_CONFIG_Ads < TEXT_LEN_UN ) {
      textfield_CONFIG_Ads [textfield_i_CONFIG_Ads ]  =  buttonlabels[q_button][0];
      textfield_i_CONFIG_Ads ++;
    }
  }
}

void Efface_tft() {
  tft.setCursor(167 + 2, 144 + 2);
  tft.setTextColor(BLACK, BLACK);
  tft.setTextSize(1);
  if (bool_invalide_serial ) {
    tft.print(F("Invalid Serial!"));
    bool_invalide_serial = false;
  }
  if (bool_invalide_mancode ) {
    tft.print(F("Invalid Mancode!"));
    bool_invalide_mancode = false;
  }
  if (bool_invalide_seed ) {
    tft.print(F("Invalid Seed!"));
    bool_invalide_seed = false;
  }
  if (bool_SD_serial_exist) {
    tft.print(F("SD Serial exist!"));
    bool_SD_serial_exist = false;
  }
  if (bool_EEPROM_Write_Error) {
    tft.print(F("EEPROM Write Error!"));
    bool_EEPROM_Write_Error = false;
  }
  if (bool_EEPROM_Write_Succefull) {
    tft.print(F("EEPROM Write Succefull!"));
    bool_EEPROM_Write_Succefull = false;
  }
}

void Parametres_remise_zero() {
  ENV_KEY = 0x0000;
  bool_OSC0 = false;
  bool_OSC1 = false;
  bool_OSC2 = false;
  bool_OSC3 = false;
  bool_Vlow = false;
  bool_BRS = false;
  bool_MTX4 = false;
  bool_TXEN = false;
  bool_S3SET = false;
  bool_XSER = false;
  bool_OVR0 = false;
  bool_OVR1 = false;
  bool_BSL0 = false;
  bool_BSL1 = false;
  // bool_choix_config = false;
  bool_choix_hcs200 = false;
  bool_choix_hcs201 = false;
  bool_choix_hcs300 = false;
  bool_choix_hcs301 = false;
  bool_choix_hcs320 = false;
  //  bool_choix_hcs360 = false;
  // bool_choix_hcs361 = false;
  bool_SD_serial_exist = false;
  bool_disc = false;
  bool_disc_Ser0 = false;
  bool_choix_simple = false;
  bool_choix_normal = false;
  bool_choix_secure = false;
  bool_invalide_serial = false;
  bool_invalide_mancode = false;
  bool_invalide_seed = false;
  bool_page_deux = false;

  OSC0 = 0;
  OSC1 = 0;
  OSC2 = 0;
  OSC3 = 0;
  VLOW = 0;
  BRS = 0;
  MTX4 = 0;
  _TXEN = 0;
  S3SET = 0;
  XSER = 0;
  OVR0 = 0;
  OVR1 = 0;
  BSL0 = 0;
  BSL1 = 0;

}

void Sd_exist_compare() {
  value_hcs_dossier.concat(value);
  value_hcs_dossier.concat(".txt");
  if (SD.exists(value_hcs_dossier)) {
    // Serial.println("SD exist = " + value_hcs_dossier);
    Efface_tft();
    tft.setCursor(167 + 2, 144 + 2);
    tft.setTextColor(RED, BLACK);
    tft.setTextSize(1);
    tft.print(F("SD Serial exist!"));
    bool_SD_serial_exist = true;
  }
  value_hcs_dossier = "";
}


void setup() {
   Serial.begin(115200);
  pinMode(DATA, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(HCSVDD, OUTPUT);
  //pinMode(PROG, INPUT_PULLUP);  //Energia: pinMode(PROG, INPUT_PULLUP);
  digitalWrite(DATA, LOW);
  digitalWrite(CLK, LOW);
  digitalWrite(HCSVDD, LOW);

  pinMode(SD_card, OUTPUT);
  // digitalWrite(SD_card, HIGH);

  delay(100);
  tft.reset();
  uint16_t identifier = tft.readID();
  identifier = 0x9341;
  tft.begin(identifier);
  tft.setRotation(1);
  tft.setTextSize(3);
  tft.fillScreen(BLACK);
  tft.setCursor(100, 25);
  tft.setTextColor(WHITE);
  tft.println(F("Arduino"));
  tft.setCursor(75, 50);
  tft.setTextColor(GREEN);
  tft.println(F("Programmer"));
  tft.setCursor(140, 80);
  tft.setTextColor(RED);
  tft.println(F("HCS"));
  int y, x;
  for (y = 110; y < 150; y += 2) {
    tft. drawLine(80 + x, y, 230 - x , y , YELLOW);
    x += 4;
  }

  delay(1000);
  tft.fillScreen(BLACK);

  //Serial.print(F("Init SD card... "));
  if (!SD.begin(SD_card)) {
   // Serial.println(F("FAIL"));
    tft.setCursor(80, 80);
    tft.setTextColor(RED);
    tft.print(F("NO SDCARD"));
  } else {
    if (!SD.exists("HCS200")) {
      SD.mkdir("HCS200");
    }
    if (!SD.exists("HCS201")) {
      SD.mkdir("HCS201");
    }
    if (!SD.exists("HCS300")) {
      SD.mkdir("HCS300");
    }
    if (!SD.exists("HCS301")) {
      SD.mkdir("HCS301");
    }
    if (!SD.exists("HCS320")) {
      SD.mkdir("HCS320");
    }
    digitalWrite(SD_card, LOW);

    tft.fillScreen(DKGREY);
    Page_une_hcs();
  }
}

void loop() {

  if (mode_menu == 0) {
    Rectangles_choix_hcs();
  }

  if (mode_menu == 1) {
    Parametres_config_world();
  }

  if (mode_menu == 2) {
    Choix_Config();
    CheckFwdMenuButtons_label1();
    //SD_check_serial();
  }

  if (mode_menu == 3) {
    // CheckFwdMenuButtons_label1();
    Programm_Hcs();
  }

  if (mode_menu == 4) {
    tft.fillScreen(DKGREY);
    Parametres_remise_zero();
    Page_une_hcs();
    mode_menu = 0;
  }

}
