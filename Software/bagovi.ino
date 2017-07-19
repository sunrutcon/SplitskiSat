/*
BUGS

[fixed] 1. jedanest manje kvarat:
     - prvi red svake sekunde dobije neku crtu kratko po sredini, 
     - crta nije cijela već isprekidana, moguće da je neki font

[fixed] 2. jedanest i dvaestipet
     - zadnje slovo t ispisuje kvadratić
     - prvi red svake sekunde dobije neku crtu kratko po sredini, 

[fixed] 3. jedanaest manje deset
     - donji red svako par sekundi zatitra zadnji red ledica, kao da se ugase

[fixed] 4. RTC error - baterija se bila punila preko diode na RTC modulu i napon je bio prevelik za rad rct modula, 
                     - uklonjena dioda

[fixed] 5. greška u napajanju arduino nano 
          - LED matrix displej se napajao preko arduina, 
          - tekla je velika struja kroz diodu na ulazu i dioda je pregorila te je cijeli uređaj ostao bez napajanja
          - uklonjena dioda na aruino nano i prebrikana sa žicom
 
[fixed] 6. greška u zadnjem redu 5. matrice
          - ponekad nije svijetlio cijeli red, a ponekad slabo
          - prelemljeni pinovi matrice i chipova na tom dijelu
*/
