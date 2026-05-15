void prikaziTekstualnoVrijeme() {
  char charbuffer[40]; // Spremnik za spajanje stringova
  
  int hrs = tm.Hour;
  int mns = tm.Minute;
  
  // Prebaci sat unaprijed ako je minuta 38 ili više (kada kreće "manje dvaest" i kasnije puni sat)
  hrs = (mns < 38) ? hrs : (hrs + 1) % 24;

  char privremeniSat[15];
  char privremeneMinute[25];
  
  // Sigurno povlačenje iz PROGMEM Flash memorije
  strcpy_P(privremeniSat, (char*)pgm_read_word(&(sati[hrs])));
  strcpy_P(privremeneMinute, (char*)pgm_read_word(&(minuti[mns])));
  
  // Spajanje u konačni tekst
  strcpy(charbuffer, privremeniSat);
  strcat(charbuffer, privremeneMinute);

  // Slanje teksta tvojoj funkciji za automatsko skaliranje i iscrtavanje ekrana
  print_string_scale(charbuffer);
}
