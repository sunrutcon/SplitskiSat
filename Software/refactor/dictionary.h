#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <Arduino.h>

// 1. Sati u PROGMEM-u (Autentični splitski izrazi)
const char sat_0[] PROGMEM = "ponoc";
const char sat_1[] PROGMEM = "jedan";
const char sat_2[] PROGMEM = "dva";
const char sat_3[] PROGMEM = "tri";
const char sat_4[] PROGMEM = "cetiri";
const char sat_5[] PROGMEM = "pet";
const char sat_6[] PROGMEM = "sest";
const char sat_7[] PROGMEM = "sedam";
const char sat_8[] PROGMEM = "osam";
const char sat_9[] PROGMEM = "devet";
const char sat_10[] PROGMEM = "deset";
const char sat_11[] PROGMEM = "jedanaest";
const char sat_12[] PROGMEM = "podne";

const char* const sati[] PROGMEM = {
  sat_0, sat_1, sat_2, sat_3, sat_4, sat_5, sat_6, sat_7, sat_8, sat_9, sat_10, sat_11,
  sat_12, sat_1, sat_2, sat_3, sat_4, sat_5, sat_6, sat_7, sat_8, sat_9, sat_10, sat_11
};

// 2. Skraćeni tekstualni blokovi za minute (prilagođeni širini ekrana)
const char m_prazno[] PROGMEM       = "";
const char m_i_pet[] PROGMEM        = " i pet";
const char m_i_deset[] PROGMEM      = " i deset";
const char m_i_kvarat[] PROGMEM     = " i kvarat";
const char m_i_dvaest[] PROGMEM     = " i dvaest";
const char m_i_dvaestipet[] PROGMEM = " i dvaestipet";
const char m_ipo[] PROGMEM          = " i po";
const char m_m_dvaest[] PROGMEM     = " manje dvaest";
const char m_m_kvarat[] PROGMEM     = " manje kvarat";
const char m_m_deset[] PROGMEM      = " manje deset";
const char m_m_pet[] PROGMEM        = " manje pet";

// 3. Tablica od točno 60 elemenata prema tvojim točnim minutama okidanja
const char* const minuti[] PROGMEM = {
  m_prazno, m_prazno, m_prazno,                                          // 00, 01, 02 min -> "puni sat"
  m_i_pet, m_i_pet, m_i_pet, m_i_pet, m_i_pet,                             // 03-07 min -> "i pet"
  m_i_deset, m_i_deset, m_i_deset, m_i_deset, m_i_deset,                   // 08-12 min -> "i deset"
  m_i_kvarat, m_i_kvarat, m_i_kvarat, m_i_kvarat, m_i_kvarat,             // 13-17 min -> "i kvarat"
  m_i_dvaest, m_i_dvaest, m_i_dvaest, m_i_dvaest, m_i_dvaest,             // 18-22 min -> "i dvaest"
  m_i_dvaestipet, m_i_dvaestipet, m_i_dvaestipet, m_i_dvaestipet, m_i_dvaestipet, // 23-27 min -> "i dvaestipet"
  m_ipo, m_ipo, m_ipo, m_ipo, m_ipo, m_ipo, m_ipo, m_ipo, m_ipo, m_ipo,   // 28-37 min -> "i po" (drži punih 10 minuta)
  m_m_dvaest, m_m_dvaest, m_m_dvaest, m_m_dvaest, m_m_dvaest,             // 38-42 min -> "manje dvaest" (odavde sat ide gore)
  m_m_kvarat, m_m_kvarat, m_m_kvarat, m_m_kvarat, m_m_kvarat,             // 43-47 min -> "manje kvarat"
  m_m_deset, m_m_deset, m_m_deset, m_m_deset, m_m_deset,                   // 48-52 min -> "manje deset"
  m_m_pet, m_m_pet, m_m_pet, m_m_pet, m_m_pet,                             // 53-57 min -> "manje pet"
  m_prazno, m_prazno                                                       // 58, 59 min -> "puni idući sat"
};

#endif
