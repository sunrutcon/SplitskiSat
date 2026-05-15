# SplitskiSat - LED Matrix Word Clock

Projekt pametnog tekstualnog sata zasnovan na Arduino platformi koji prikazuje vrijeme na specifičnom splitskom/dalmatinskom narječju (tzv. "baba sat") te u klasičnom digitalnom formatu.

## 📋 Pregled sustava

Sustav koristi Arduino mikrokontroler koji u stvarnom vremenu sinkronizira vrijeme s hardverskim RTC modulom. Ovisno o odabranom načinu rada, vrijeme se ispisuje tekstualno (kombiniranjem jedinstvenih stringova za svaku minutu i sat iz rječnika) ili brojčano. Prikaz se vrši na LED matrici velike gustoće uz automatsko prilagođavanje veličine fonta ovisno o duljini generiranog teksta.

---

## 🛠️ Hardverska konfiguracija


| Komponenta | Opis / Specifikacije | Komunikacija / Pinovi |
| :--- | :--- | :--- |
| **Mikrokontroler** | Arduino (ATmega328P - npr. Uno / Nano) | Glavni procesor |
| **RTC Modul** | DS1307 Real-Time Clock | I2C sabirnica (`SDA`/`SCL`) |
| **LED Matrica** | Rezolucija 64x16 piksela | Namjenski pinovi (`LEDMatrix` pokretač) |
| **Tipke (4x)** | Fizička tipkala s internim *pull-up* otpornicima | Analogni/Digitalni pinovi (`A0`, `A1`, `A2`, `A3`) |

---

## ⚙️ Funkcionalnost i logika rada

### 1. Načini prikaza (Modes)
Korisnik može mijenjati modove prikaza u stvarnom vremenu pritisakom na tipku za odabir vizuala:
*   **Mod 1 (Slovčani/Tekstualni):** Prikazuje vrijeme riječima (npr. *"podne i kvarat"*, *"tri i pol"*). Sustav za svaku minutu u satu povlači specifičan izraz definiran unutar datoteke `dictionary.h`.
*   **Mod 2 (Brojčani/Digitalni):** Prikazuje vrijeme u standardnom 24-satnom formatu `HH:MM:SS` preko cijele visine ekrana.

### 2. Upravljanje fontovima i pametno skaliranje
Projekt sadrži tri skupa fontova podijeljena u zasebne datoteke:
*   `font_5x7.h`: Primarni font za tekstualni prikaz (širina 5, visina 7 piksela).
*   `font_4x7.h`: Uži pomoćni font (širina 4, visina 7 piksela).
*   `font_7x16_num.h`: Veliki font namijenjen isključivo prikazu brojki digitalnog sata.

**Algoritam za automatsko skaliranje:**
Sustav inicijalno pokušava ispisati tekstualno vrijeme pomoću čitljivijeg `5x7` fonta. Ako je generirani niz riječi predug da bi stao u dva reda na matrici širine 64 piksela, algoritam automatski prebacuje ispis na uži `4x7` font kako bi se spriječilo rezanje teksta.

---

## 🚨 Uočeni problemi u trenutnoj arhitekturi (v0.2)

Tijekom analize koda uočena su kritična uska grla koja uzrokuju **treperenje zaslona** i **loš odaziv tipki** (gubitak pritisaka):

1.  **Blokirajući I/O operacije (Serial preopterećenje):**
    Unutar funkcije `update_time()` u svakom ciklusu izvršava se masovni ispis podataka na `Serial` port pri brzini od samo 9600 bauda. To uzrokuje punjenje serijskog međuspremnika i hardversko zaustavljanje mikrokontrolera dok se tekst ne pošalje.
2.  **Prečesto čitanje RTC-a preko I2C-a:**
    Komunikacija s DS1307 modulom događa se u svakom prolazu kroz `loop()` petlju. I2C komunikacija je spora i troši dragocjene cikluse procesora stotinama puta u sekundi bez ikakve potrebe.
3.  **Pogrešno skeniranje tipki unutar ISR-a:**
    Funkcija `handleButtons()` poziva se unutar prekidne rutine tajmera (`timer_isr`). Korištena biblioteka `Button` oslanja se na funkciju `millis()` za uklanjanje titranja kontakata (*debouncing*). Budući da su unutar ISR-a svi drugi prekidi (uključujući i onaj koji broji `millis()`) zamrznuti, logika tipki potpuno otkazuje ili radi nepredvidivo.
4.  **Nedostatak ključne riječi `volatile`:**
    Polje `btnEvent[]` koristi se za prijenos podataka iz prekida u glavnu petlju, ali nije definirano kao `volatile`. Kompajler ga stoga optimizira u registre, zbog čega glavna petlja često uopće ne primijeti da je tipka pritisnuta.

---

## 🚀 Prijedlozi za budući refaktoring

Za potpunu modernizaciju projekta i postizanje fluidnog rada (60+ FPS prikaz bez blokiranja), planirane su sljedeće nadogradnje:

### 🧱 1. Razdvajanje arhitekture (Separation of Concerns)
*   Podijeliti kod u jasne module: Logika vremena (`ClockState`), Unos (`InputManager`) i Prikaz (`DisplayDriver`).
*   Maknuti bilo kakvu logiku obrade vremena ili provjere tipki iz datoteke koja upravlja ispisom na matricu.

### ⏱️ 2. Upravljanje vremenom bez blokiranja (Non-blocking design)
*   Implementirati Konačni automat (FSM) za upravljanje stanjima sata.
*   Ukloniti sve instance funkcije `delay()` i blokirajućih `while` petlji.
*   Ograničiti čitanje RTC-a preko I2C-a na maksimalno 2 do 4 puta u sekundi pomoću `millis()` provjere.

### 🧼 3. Optimizacija ISR-a (Prekidne rutine)
*   Svesti `timer_isr()` isključivo na izvršavanje funkcije `matrix.scan()`. Sve ostale operacije (uključujući skeniranje tipki) moraju se izvršavati unutar glavne `loop()` petlje.

### 💾 4. Optimizacija memorije (RAM i Flash)
*   Premjestiti sve statičke tekstove i rječnike (`sati`, `minuti` iz `dictionary.h`) u Flash memoriju pomoću `PROGMEM` i `F()` makroa kako bi se oslobodio kritični dinamički RAM (SRAM) na ATmega328P čipu.
*   Ukloniti tešku funkciju `sprintf` i zamijeniti je laganim manipulacijama pokazivača (*pointers*).
