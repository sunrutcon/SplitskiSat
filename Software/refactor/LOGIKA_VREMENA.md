# 🕒 Splitska Logika Vremena i Mapiranje Minuta

Ovaj dokument opisuje implementiranu matematičku i jezičnu logiku zaokruživanja vremena na autentično splitsko narječje ("baba sat"). 

Sustav koristi simetrične intervale u kojima se tekstualna promjena događa točno **2 minute prije** svakog koraka od 5 minuta.

## 📊 Tablica intervala i uvjeta


| Minuta (RTC) | Prikazani tekstualni blok | Ponašanje sata (`hrs`) | Primjer (za 16:XX) |
| :---: | :--- | :--- | :--- |
| **00 - 02** | *Samo sat (prazno)* | Trenutni sat | "pet" |
| **03 - 07** | ` i pet` | Trenutni sat | "pet i pet" |
| **08 - 12** | ` i deset` | Trenutni sat | "pet i deset" |
| **13 - 17** | ` i kvarat` | Trenutni sat | "pet i kvarat" |
| **18 - 22** | ` i dvaest` | Trenutni sat | "pet i dvaest" |
| **23 - 27** | ` i dvaestipet` | Trenutni sat | "pet i dvaestipet" |
| **28 - 37** | ` i po` | Trenutni sat | "pet i po" |
| **38 - 42** | ` manje dvaest` | **Idući sat (+1)** | "sest manje dvaest" |
| **43 - 47** | ` manje kvarat` | **Idući sat (+1)** | "sest manje kvarat" |
| **48 - 52** | ` manje deset` | **Idući sat (+1)** | "sest manje deset" |
| **53 - 57** | ` manje pet` | **Idući sat (+1)** | "sest manje pet" |
| **58 - 59** | *Samo sat (prazno)* | **Idući sat (+1)** | "sest" |

---

## 🛠️ Programski uvjet za skok sata

Kako bi se sat ispravno ponašao prema gornjoj tablici, promjena sata unaprijed izvršava se točno u **38. minuti** (kada nastupa izraz *manje dvaest*):

```cpp
// Ako je minuta manja od 38, koristi trenutni sat. 
// U suprotnom, prebaci na idući sat (uz modul 24 za prijelaz s 23h na ponoć).
hrs = (mns < 38) ? hrs : (hrs + 1) % 24;
```

## 💾 Optimizacija Memorije (PROGMEM)

Sve tekstualne fraze i tablica pokazivača od 60 elemenata pohranjeni su u **Flash memoriju (`PROGMEM`)** unutar `dictionary.h`. 

To omogućuje mikrokontroleru da trenutnu minutu dobivenu iz RTC-a (`tm.Minute`) iskoristi izravno kao indeks polja, čime se eliminiraju spore `if-else` petlje i štedi 100% dinamičkog RAM-a:

```cpp
// Izravno i sigurno povlačenje teksta iz Flash memorije
strcpy_P(privremeneMinute, (char*)pgm_read_word(&(minuti[mns])));
```
