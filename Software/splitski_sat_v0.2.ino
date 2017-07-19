#include "Time.h"
#include "LEDMatrix.h"
#include <TimerOne.h>
#include "dictionary.h"
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Button.h>
#include "font_5x7.h"
#include "font_4x7.h"
#include "font_7x16_num.h"

#define WIDTH   64
#define HEIGHT  16

#define FONT57 5
#define FONT46 4

// LEDMatrix(a, b, c, d, oe, r1, stb, clk);
LEDMatrix matrix(4, 5, 6, 7, 8, 9, 10, 11);



// Display Buffer 128 = 64 * 16 / 8
uint8_t displaybuf[WIDTH *HEIGHT / 8] = {
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0xE0, 0x07, 0x8F, 0xC7, 0xC7, 0xC7, 0xE0,
    0x00, 0x40, 0x0C, 0xCE, 0x6C, 0x6C, 0x6C, 0xE0, 0x00, 0xE0, 0x0C, 0x0C, 0x6C, 0x6C, 0x6C, 0x60,
    0x01, 0xF0, 0x07, 0x8C, 0x6F, 0xEF, 0xEC, 0x60, 0x23, 0xF8, 0x00, 0xCC, 0x6C, 0x0C, 0x0C, 0x60,
    0x33, 0xF8, 0x0C, 0xCE, 0x6C, 0x6C, 0x6C, 0xE0, 0x3B, 0xF8, 0x07, 0x8F, 0xC7, 0xC7, 0xC7, 0xE0,
    0x3B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x1B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
    0x0B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

uint8_t displaybuf_new[WIDTH *HEIGHT / 8] = {
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0xE0, 0x07, 0x8F, 0xC7, 0xC7, 0xC7, 0xE0,
    0x00, 0x40, 0x0C, 0xCE, 0x6C, 0x6C, 0x6C, 0xE0, 0x00, 0xE0, 0x0C, 0x0C, 0x6C, 0x6C, 0x6C, 0x60,
    0x01, 0xF0, 0x07, 0x8C, 0x6F, 0xEF, 0xEC, 0x60, 0x23, 0xF8, 0x00, 0xCC, 0x6C, 0x0C, 0x0C, 0x60,
    0x33, 0xF8, 0x0C, 0xCE, 0x6C, 0x6C, 0x6C, 0xE0, 0x3B, 0xF8, 0x07, 0x8F, 0xC7, 0xC7, 0xC7, 0xE0,
    0x3B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x1B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
    0x0B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

struct display_line {
  char line[20];
  uint8_t font;
};

Button button[4] = {
  Button(A0, PULLUP), 
  Button(A1, PULLUP), 
  Button(A2, PULLUP), 
  Button(A3, PULLUP)
};

uint8_t btnEvent[] = {0, 0, 0, 0};

tmElements_t tm;

int view = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  free_ram();
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");

  //setTime(23,44,20,10,3,2016);
  free_ram();

  Serial.println("matrix.begin");
  matrix.begin(displaybuf, displaybuf_new, WIDTH, HEIGHT);

  free_ram();
  Serial.println("matirx.clear");
  matrix.clear();
  
  free_ram();
  Serial.println("Timer.initialize");
  Timer1.initialize(1200);
  Timer1.attachInterrupt(timer_isr);

/*
  free_ram();
  Serial.println("matrix.reverse first time");
  
  matrix.reverse();
    delay(300);
  matrix.reverse();
    delay(300);
  matrix.reverse();
    delay(300);
  matrix.reverse();
    delay(300);
  /* */
}

void timer_isr() {
  matrix.scan();
  handleButtons();
}

void loop() {
  run();
  handleButtonEvents();
}

void run() {
  
   if(update_time() == 0) {
    free_ram();

    if(view == 0) {
      matrix.clear();
      show_time_split(tm);
      matrix.flush();
    }
    else {
      matrix.clear();
      show_time_num(tm);
      matrix.flush();
    }
    delay(1);
  }
  else {
    matrix.clear();
    print_string("#error       reading RTC", font_4x7);
    matrix.flush();
    {
      static long last_millis = millis();
      while(millis() - last_millis < 3000 && !btnEvent[0]) {
        handleButtons();
      }
    }
    //delay(3000);
  }
}
int free_ram () {
  int fr = 0;
  extern int __heap_start, *__brkval; 
  int v; 
   
  fr = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
  Serial.print("free RAM = ");
  Serial.println(fr);
  return fr;
}

void handleButtons() {
  for(int i = 0; i < 4; i++) {
    if(button[i].uniquePress()) {
      btnEvent[i]++;
    }
  }
}

void handleButtonEvents() {
  if(btnEvent[0]>0) {
    view = (view + btnEvent[0])%2;
    btnEvent[0] = 0;
  }
  if(btnEvent[1]>0) {
    Serial.println("Hour++");
    btnEvent[1] = 0;
    tm.Hour = (tm.Hour + 1)%24;
    if(RTC.setHour(tm.Hour)) {
    } else {
      Serial.println("Error setting time: Hour");
    }
  }
  if(btnEvent[2]>0) {
    Serial.println("Minute++");
    btnEvent[2] = 0;
    tm.Minute = (tm.Minute + 1)%60;
    if(RTC.setMinute(tm.Minute)) {
    } else {
      Serial.println("Error setting time: Minute");
    }
  }
  if(btnEvent[3]>0) {
    Serial.println("Second++");
    btnEvent[3] = 0;
    tm.Second = (tm.Second + 1)%60;
    if(RTC.setSecond(tm.Second)) {
    } else {
      Serial.println("Error setting time: Second");
    }
  }
}
int update_time() {
  if (RTC.read(tm)) { 
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.println(tmYearToCalendar(tm.Year));
    return 0;
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    return -1;
  }
//  delay(100);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void print_string_scale(char * tt) {
  const uint8_t * font = font_5x7;
  int max_pos = font57_max_pos;
  int pos = 0;
  char *token;
  int tt_length = my_strlen_utf8_c(tt);
  int frowlen = 0;
  
  do {
    token = strsep(&tt, " ");
    if(token == NULL)
      break;
      
    // if next token does not fit in first line, pad line rigth with spaces
    if(pos < max_pos && max_pos - pos < my_strlen_utf8_c(token)) {
      for(int k=pos; k<max_pos; k++) {
        print_letter(k,' ', font);
      }
      // mesure length of string in first line
      frowlen = pos;
      pos = max_pos;
    }
    else if(pos == max_pos) {
      // if first row is filled with text to the last characrer, add one imaginary space to the first line
      frowlen = max_pos+1;
    }
    
    // * we are in second row 
    if(pos == max_pos) {
      int length_of_rest = tt_length - frowlen;

      Serial.print("############################## TOTAL LEN ####################   ");
      Serial.println(tt_length);
      Serial.print("############################## FROW LEN ####################   ");
      Serial.println(frowlen);
      Serial.print("################################# length_of_rest #################   ");
      Serial.println(length_of_rest);

      // if string length for second line is to large to print in with font, get smaler font
      if(length_of_rest > max_pos) {
        max_pos = font47_max_pos;
        pos = max_pos;
        font = font_4x7;
      }
    }
    
    for(int i = 0; i < strlen(token); i++){
      if(i>=20) break;
      // skip first byte of utf8 characters with two bytes
      if((uint8_t)token[i] == 0xC4 || (uint8_t)token[i] == 0xC5) {
        continue;
      }
      print_letter(pos, token[i], font);
      pos++;
    }
    
    // add space between tokens/words in string
    if(pos != max_pos) {
      print_letter(pos,' ', font);
      pos++;
    }
    
    // fill rest of row with spaces
    if(pos < 2*max_pos) {
      for(int k = pos; k < 32; k++) {
       print_letter(k, ' ', font);
      }
    }
  } while (token != NULL );
}

void print_led1 (char * tt) {

  int pos = 0;
  char *token;
  Serial.println();
  Serial.print("[");
  Serial.print(tt);
  Serial.println("]");
  do
  {
    token = strsep(&tt, " ");
    if(token == NULL)
      break;
    if(pos <= 10 && 10-pos < my_strlen_utf8_c(token)){
      
      for(int j=pos; j<10; j++) {
        print_letter_5x7(j,' ');
//        print_letter_4x6(j,' ');
      }
      pos = 10;
    }
    for(int i = 0; i < my_strlen_utf8_c(token); i++) {
      if(pos>=20) break;
      if((uint8_t)token[i] == 0xC4 || (uint8_t)token[i] == 0xC5) {
        
        continue;
      }
      print_letter_5x7(pos,token[i]);
//      print_letter_4x6(pos,token[i]);
      pos++;
    }
    if(token!=NULL && pos != 10) {
      print_letter_5x7(pos,' ');
//      print_letter_4x6(pos,' ');
      pos++;
    }
  } while (token != NULL );
  if(pos < 20) {
    for(int k=pos; k<20; k++){
      print_letter_5x7(k,' ');
//      print_letter_4x6(pos,' ');
    }
  }
}

void print_line(char * line_string, uint8_t line, uint8_t font) {
  /* printing font 5x7 */
  uint8_t line_length = my_strlen_utf8_c(line_string);
  if(font == 5) {
    /* fill the rest of line with spaces */
    if(my_strlen_utf8_c(line_string) < 10) {
      memset(line_string, ' ', 10 - line_length);
    }
    /* printing line 1 */
    if(line == 0) {
      for(int pos = 0; pos < 10; pos++) {
        print_letter_5x7(pos, line_string[pos]);
      }
    }
    /* printing line 2 */
    else if(line == 1) {
      for(int pos = 0; pos < 10; pos++) {
        print_letter_5x7(pos + 10, line_string[pos]);
      }
    }
  }
  else if(font = 4) {
    /* fill the rest of line with spaces */
    if(my_strlen_utf8_c(line_string) < 12) {
      memset(line_string, ' ', 12 - line_length);
    }
    /* printing line 1 */
    if(line == 0) {
      for(int pos = 0; pos < 12; pos++) {
        print_letter_5x7(pos, line_string[pos]);
      }
    }
    /* printing line 2 */
    else if(line == 1) {
      for(int pos = 0; pos < 12; pos++) {
        print_letter_5x7(pos + 12, line_string[pos]);
      }
    }
  }
}

int my_strlen_utf8_c(char *s) {
   int i = 0, j = 0;
   while (s[i]) {
     if ((s[i] & 0xc0) != 0x80) j++;
     i++;
   }
   return j;
}

void hpp(char c) {
    Serial.println();
    Serial.print(c);
    Serial.print(" ");
    Serial.println((uint8_t)c, HEX);
}

void transpose(int chrno, uint8_t * outchr) {
  int i,j;
  for(i=0; i<7; i++) {
    outchr[i] = 0;
    for(j=0; j<5; j++) {
      outchr[i]|=((font_5x7[chrno*5+j]>>i)&(0x01))<<(7-j);
    }
  }
}

void show_time_split(tmElements_t tm) {
  static char hourText[40];

  int hrs = tm.Hour;
  int mns = tm.Minute;
  
  /*  
  hrs = 10;
  mns = 45;
  /* */
  
  memset(hourText, 0, 40);

  Serial.println("show_time_split");
  hrs = mns < 38 ? hrs : (hrs + 1) > 23 ? 0 : (hrs + 1);
  sprintf(hourText,"%s%s", sati[hrs], minuti[mns]);
  //print_led1(hourText);
  print_string_scale(hourText);
}

void show_time_num(tmElements_t tm) {
  /* HH */
  print_letter_7x16(0,tm.Hour/10);
  print_letter_7x16(1,tm.Hour%10);
  
  /* : */
  print_letter_7x16(2,10);
  
  /* MM */
  print_letter_7x16(3,tm.Minute/10);
  print_letter_7x16(4,tm.Minute%10);
  
  /* : */
  print_letter_7x16(5,10);
  
  /* SS */
  print_letter_7x16(6,tm.Second/10);
  print_letter_7x16(7,tm.Second%10);
}

/* PRINT STRING */

void print_string(char inString[], const uint8_t * font) {
  int pos_correction = 0;
  for(int i = 0; i < my_strlen_utf8_c(inString); i++) {
    if((uint8_t)inString[i] == 0xC4 || (uint8_t)inString[i] == 0xC5) {
      pos_correction++;
      continue;
    }
    print_letter(i - pos_correction, inString[i], font);
  }
}

/* PRINT LETTER */

void print_letter(int segment, int inCharNo, const uint8_t * font) {
  if(font == font_5x7) {
    print_letter_5x7(segment, inCharNo);
  }
  else if(font == font_4x7) {
    print_letter_4x7(segment, inCharNo);
  }
  else if(font == font_7x16) {
    print_letter_7x16(segment, inCharNo);
  }
}

/* ****** PRINT LETTER 7x16 ********* */

void print_letter_7x16(int pos, int inum) {

  const uint8_t * font = font_7x16;

  /* for each byte */
  for(int i = 0; i < 16; i++) {
    /* shift it and get lsb */
    for(int j = 0; j < 8; j ++) {
      uint8_t tmpch = pgm_read_word_near(font + inum*16+i);      
      matrix.drawPoint(j+8*pos, i, (tmpch>>(7-j))&1);
    }
  }
}

/* ****** PRINT LETTER 5X7 ********* */

void print_letter_5x7(int segment, int inCharNo) {
  int inum = 0;
  const uint8_t * font = font_5x7;
  uint8_t charNo = inCharNo - 32;
  
  if(charNo < 0 || charNo >= sizeof(font_4x7)/5){
    font = font_5x7_special;
    Serial.print("{");
    Serial.print(charNo,HEX);
    Serial.print("}");
    if(charNo == 0x6D  || charNo == 0x6C) // č
      inum = 1;
    else if(charNo == 0x67 || charNo == 0x66) //ć
      inum = 2;
    else if(charNo == 0x9E || charNo == 0x9D) //ž
      inum = 3;
    else if(charNo == 0x81 || charNo == 0x80) //š
      inum = 5;
    else if(charNo == 0x71 || charNo == 0x70) //đ
      inum = 4;
    else
      inum = 0;
  }
  else
    inum = charNo;

  if(segment < 10) { //first row
    for(int i = 0; i < 5; i++) {
      for(int j = 0; j < 7; j++) {
        uint8_t tmpch = pgm_read_word_near(font + inum*5+i);
        matrix.drawPoint(segment * 6 + i, j,tmpch>>j&1);
      }
    }
  }
  else if(segment < 20) { // second row
    for(int i = 0; i < 5; i++) {
      for(int j = 0; j < 7; j++) {
        uint8_t tmpch = pgm_read_word_near(font + inum*5+i);
        matrix.drawPoint((segment - 10) * 6 + i, 8 + j,tmpch>>j&1);
      }
    }
  }
}


/* ****** PRINT LETTER 4X7 ********* */

void print_letter_4x7(int segment, int inCharNo) {
  int inum = 0;
  int p1 = 4;
  int p2 = 13;
  int p3 = p1 + 1;
  const uint8_t * font = font_4x7;
  int row = segment < p2 ? 0 : 1;
  uint8_t charNo = inCharNo - 32;

  if(charNo < 0 || charNo >= sizeof(font_4x7)/5){
    font = font_4x7_special;
    if(charNo == 0x6D  || charNo == 0x6C) // č
      inum = 1;
    else if(charNo == 0x67 || charNo == 0x66) //ć
      inum = 2;
    else if(charNo == 0x9E || charNo == 0x9D) //ž
      inum = 3;
    else if(charNo == 0x81 || charNo == 0x80) //š
      inum = 5;
    else if(charNo == 0x71 || charNo == 0x70) //đ
      inum = 4;
    else
      inum = 0;
  }
  else
    inum = charNo;
  if(segment < 2*p2) {
    
    /* printing single character to matrix */
    for(int i = 0; i < p1; i++) {
      for(int j = 0; j < 7; j++) {
        uint8_t tmpch = pgm_read_word_near(font + inum*5+i);
        matrix.drawPoint((segment - p2 * row) * p3 + i, j + (row * 8),tmpch>>j&1);
      }
    }
    
    /* adding spacing - one column width */
    if(segment!=p2-1 && segment!=2*p2-1) {
      for(int k = 0; k < 7; k++) {
        matrix.drawPoint((segment - p2 * row) * p3 + p1, k + (row * 8),0);
      }
    }
  }  
}
