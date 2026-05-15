#include <TimerOne.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Button.h>

#include "LEDMatrix.h"
#include "dictionary.h"
#include "font_5x7.h"
#include "font_4x7.h"
#include "font_7x16_num.h"

#define WIDTH 64
#define HEIGHT 16

// Konfiguracija hardvera
LEDMatrix matrix(4, 5, 6, 7, 8, 9, 10, 11);
uint8_t displaybuf[WIDTH * HEIGHT / 8];
uint8_t displaybuf_new[WIDTH * HEIGHT / 8];

Button btnMode(A0, PULLUP);
Button btnHour(A1, PULLUP);
Button btnMin(A2, PULLUP);
Button btnSec(A3, PULLUP);

// Stanja sustava (Konačni automat)
enum DisplayMode { MODE_WORD_CLOCK, MODE_DIGITAL_CLOCK, MODE_ERROR };
DisplayMode trenutniMod = MODE_WORD_CLOCK;

tmElements_t tm;
unsigned long zadnjeCitanjeRTC = 0;
const unsigned long intervalRTC = 500; // Čitaj RTC svakih 500ms

void setup() {
  Serial.begin(115200); // Brzi Serial ne blokira mikrokontroler
  
  matrix.begin(displaybuf, displaybuf_new, WIDTH, HEIGHT);
  matrix.clear();

  // Tajmer okida samo osvježavanje matrice svakih 1200us
  Timer1.initialize(1200);
  Timer1.attachInterrupt(timer_isr);
}

// ISR mora biti munjevito brz - bez ikakve druge logike!
void timer_isr() {
  matrix.scan();
}

void loop() {
  provjeriTipke();

  // Ne-blokirajuće čitanje vremena i ažuriranje ekrana
  unsigned long trenutniMillis = millis();
  if (trenutniMillis - zadnjeCitanjeRTC >= intervalRTC) {
    zadnjeCitanjeRTC = trenutniMillis;
    
    if (RTC.read(tm)) {
      if (trenutniMod != MODE_DIGITAL_CLOCK && trenutniMod != MODE_WORD_CLOCK) {
        trenutniMod = MODE_WORD_CLOCK; // Oporavak od greške
      }
      osvjeziPrikaz();
    } else {
      trenutniMod = MODE_ERROR;
      prikaziGresku();
    }
  }
}

void provjeriTipke() {
  if (btnMode.uniquePress()) {
    trenutniMod = (trenutniMod == MODE_WORD_CLOCK) ? MODE_DIGITAL_CLOCK : MODE_WORD_CLOCK;
    osvjeziPrikaz();
  }
  
  if (btnHour.uniquePress()) {
    tm.Hour = (tm.Hour + 1) % 24;
    RTC.write(tm);
    osvjeziPrikaz();
  }
  
  if (btnMin.uniquePress()) {
    tm.Minute = (tm.Minute + 1) % 60;
    RTC.write(tm);
    osvjeziPrikaz();
  }

  if (btnSec.uniquePress()) {
    tm.Second = 0; // Resetiranje sekundi na 0 je praktičnije nego +1
    RTC.write(tm);
    osvjeziPrikaz();
  }
}

void osvjeziPrikaz() {
  matrix.clear();
  if (trenutniMod == MODE_WORD_CLOCK) {
    prikaziTekstualnoVrijeme();
  } else if (trenutniMod == MODE_DIGITAL_CLOCK) {
    prikaziDigitalnoVrijeme();
  }
  matrix.flush();
}

void prikaziDigitalnoVrijeme() {
  // HH
  print_letter_7x16(0, tm.Hour / 10);
  print_letter_7x16(1, tm.Hour % 10);
  // :
  print_letter_7x16(2, 10);
  // MM
  print_letter_7x16(3, tm.Minute / 10);
  print_letter_7x16(4, tm.Minute % 10);
  // :
  print_letter_7x16(5, 10);
  // SS
  print_letter_7x16(6, tm.Second / 10);
  print_letter_7x16(7, tm.Second % 10);
}

void prikaziTekstualnoVrijeme() {
  charbuffer[45]; 
  
  int hrs = tm.Hour;
  int mns = tm.Minute;
  
  // Logika za "baba sat" - zaokruživanje sata prema minuti
  hrs = (mns < 38) ? hrs : (hrs + 1) % 24;

  // Sigurno čitanje stringova iz PROGMEM-a bez sprintf-a
  char privremeniSat[20];
  char privremeneMinute[25];
  
  strcpy_P(privremeniSat, (char*)pgm_read_word(&(sati[hrs])));
  strcpy_P(privremeneMinute, (char*)pgm_read_word(&(minuti[mns])));
  
  // Spajanje u jedan lokalni buffer
  strcpy(charbuffer, privremeniSat);
  strcat(charbuffer, privremeneMinute);

  // Poziv tvoje funkcije za pametno skaliranje i ispis na matricu
  print_string_scale(charbuffer);
}

void prikaziGresku() {
  matrix.clear();
  // Koristimo F() makro da statički tekst greške ne troši RAM
  print_string((char*)"RTC ERR", font_4x7);
  matrix.flush();
}

// --- Ovdje ispod prekopiraj svoje stare funkcije za crtanje slova ---
// (print_string_scale, print_letter, print_letter_5x7, print_letter_4x7, print_letter_7x16)
// NAPOMENA: Iz njih izbaci sve "Serial.print" naredbe kako bi radile maksimalnom brzinom.
