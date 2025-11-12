// statisticka_kalkulacka.cpp : Defines the entry point for the application.
//

#include "statisticka_kalkulacka.h"
#include <stdio.h>    // Pre vstup/výstup (printf, scanf_s, FILE, fopen_s, ...) [cite: 977, 1100]
#include <string.h>   // Pre prácu s reťazcami (strlen, ...) [cite: 983]
#include <math.h>     // Pre matematické funkcie (sqrt)
#include <ctype.h>    // Pre funkcie isalpha, isspace, tolower

// Definovanie pevných limitov namiesto dynamickej alokácie
#define MAX_DATA_VELKOST 1000     // Max. počet čísel, ktoré vieme spracovať
#define MAX_TEXT_VELKOST 5000     // Max. dĺžka textu
#define MAX_DLZKA_SLOVA 30        // Pre histogram dĺžky slov
#define HISTOGRAM_TRIEDY 10       // Počet "stĺpcov" v histograme

/* --- Deklarácie funkcií (dopredné) --- */
// Pomocná funkcia
void vycistiBuffer();

// Sekcia 1: Číselné štatistiky [cite: 516]
void sekciaCiselna();
int nacitajCislaZTextovehoSuboru(double data[], int maxVelkost);
double spocitajPriemer(double data[], int pocet);
double spocitajVazenyPriemer(double data[], int pocet);
void spocitajMedian(double data[], int pocet, double* median);
void spocitajRozptyl(double data[], int pocet, double priemer, double* rozptyl, double* smerOdchylka);
void utriedPole(double data[], int pocet);// Bublinkové triedenie [cite: 417]
void zobrazHistogramCisiel(double data[], int pocet);
void ulozCiselneVysledky(const char* nazovSuboru, double priemer, double v_priemer, double median, double rozptyl, double smerOdchylka);

// Sekcia 2: Textové štatistiky [cite: 517]
void sekciaTextova();
int nacitajTextZoSuboru(char text[], int maxVelkost);
int spocitajZnaky(const char* text);
int spocitajSlova(const char* text);
void zobrazHistogramPismen(const char* text);
void zobrazHistogramDlzkySlov(const char* text);
void ulozTextoveVysledky(const char* nazovSuboru, int pocetZnakov, int pocetSlov);


/* --- Hlavná funkcia programu --- */
int main() {
    int volba = 0;

    // Hlavné menu programu
    // Použijeme cyklus do-while (Prednáška č. 8) [cite: 367]
        do {
            printf("\n--- STATISTICKA KALKULACKA (Projekt 5) ---\n");
            printf("1. Sekcia 1: Ciselne hodnoty [cite: 516]\n");
            printf("2. Sekcia 2: Textove hodnoty [cite: 517]\n");
            printf("3. Koniec\n");
            printf("Vasa volba: ");

            // Použijeme bezpečné načítanie scanf_s (Prednáška č. 5) [cite: 1002]
                if (scanf_s("%d", &volba) != 1) {
                    volba = 0; // Reset v prípade neplatného vstupu
                }
            vycistiBuffer(); // Vyčistenie bufferu po scanf_s

            // Použijeme príkaz switch (Prednáška č. 6) [cite: 708]
                switch (volba) {
                case 1:
                    sekciaCiselna();
                    break;
                case 2:
                    sekciaTextova();
                    break;
                case 3:
                    printf("Program sa ukoncuje.\n");
                    break;
                default:
                    printf("Neplatna volba. Prosim, skuste znova.\n");
                    break;
                }
        } while (volba != 3);

    return 0;
}

/* --- Pomocné funkcie --- */

/**
 * @brief Čistí vstupný buffer (stdin) od zvyšných znakov (napr. '\n' po scanf_s).
 */
void vycistiBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


/*
 * =============================================================
 * [cite_start]SEKCIA 1: ČÍSELNÉ ŠTATISTIKY [cite: 516]
 * =============================================================
 */

 /**
  * @brief Hlavná funkcia pre spracovanie číselných štatistík.
  */
void sekciaCiselna() {
    // Namiesto dynamickej alokácie (Predn. 10) [cite: 12] [cite_start]použijeme statické pole (Predn. 4) [cite: 746]
    double data[MAX_DATA_VELKOST];
    int pocetHodnot = 0;
    int volbaVstupu = 0;
    char nazovSuboru[100];

    printf("\n--- SEKCIA 1: CISELNE HODNOTY ---\n");
    printf("Zvolte zdroj dat[cite: 514]:\n");
    printf("1. Vstup z konzoly\n");
    printf("2. Vstup z textoveho suboru\n");
    printf("Vasa volba: ");
    scanf_s("%d", &volbaVstupu);
    vycistiBuffer();

    if (volbaVstupu == 1) { // Vstup z konzoly [cite: 514]
        printf("Zadajte pocet hodnot (max %d): ", MAX_DATA_VELKOST);
        scanf_s("%d", &pocetHodnot);
        vycistiBuffer();

        if (pocetHodnot <= 0 || pocetHodnot > MAX_DATA_VELKOST) {
            printf("Neplatny pocet hodnot.\n");
            return;
        }
        printf("Zadajte %d cisel (oddelene medzerou alebo Enterom):\n", pocetHodnot);
        for (int i = 0; i < pocetHodnot; i++) { // Cyklus for (Predn. 8) [cite: 326]
            scanf_s("%lf", &data[i]);
        }
        vycistiBuffer();
    }
    else if (volbaVstupu == 2) { // Vstup zo súboru [cite: 514]
        pocetHodnot = nacitajCislaZTextovehoSuboru(data, MAX_DATA_VELKOST);
        if (pocetHodnot == 0) {
            printf("Nepodarilo sa nacitat ziadne data.\n");
            return;
        }
        printf("Uspesne nacitanych %d hodnot.\n", pocetHodnot);
    }
    else {
        printf("Neplatna volba.\n");
        return;
    }

    // Výpočty [cite: 516]
    double priemer = 0.0, v_priemer = 0.0, median = 0.0, rozptyl = 0.0, smerOdchylka = 0.0;

    priemer = spocitajPriemer(data, pocetHodnot);
    v_priemer = spocitajVazenyPriemer(data, pocetHodnot);
    spocitajMedian(data, pocetHodnot, &median);
    spocitajRozptyl(data, pocetHodnot, priemer, &rozptyl, &smerOdchylka);

    // Zobrazenie výsledkov
    printf("\n--- VYSLEDKY ANALYZY ---\n");
    printf("Pocet hodnot:    %d\n", pocetHodnot);
    printf("Priemer:         %.4f\n", priemer);
    printf("Vazeny priemer:  %.4f\n", v_priemer);
    printf("Median:          %.4f\n", median);
    printf("Rozptyl:         %.4f\n", rozptyl);
    printf("Smer. odchylka:  %.4f\n", smerOdchylka);

    // Zobrazenie histogramu [cite: 516]
    zobrazHistogramCisiel(data, pocetHodnot);

    // Uloženie výsledkov [cite: 518]
    printf("\nZadat nazov suboru pre ulozenie vysledkov (alebo nechajte prazdne pre neulozenie): ");
    fgets(nazovSuboru, 100, stdin);
    nazovSuboru[strcspn(nazovSuboru, "\n")] = 0; // Odstránenie '\n'

    if (strlen(nazovSuboru) > 0) {
        ulozCiselneVysledky(nazovSuboru, priemer, v_priemer, median, rozptyl, smerOdchylka);
    }
}

/**
 * @brief Načíta čísla z textového súboru.
 * Používa koncepty z Prednášky č. [cite_start]9[cite: 1097].
 */
int nacitajCislaZTextovehoSuboru(double data[], int maxVelkost) {
    char nazovSuboru[100];
    FILE* f; // Ukazovateľ na súbor [cite: 1103]
        int pocet = 0;

    printf("Zadajte nazov vstupneho suboru (napr. cisla.txt): ");
    fgets(nazovSuboru, 100, stdin);
    nazovSuboru[strcspn(nazovSuboru, "\n")] = 0;

    // Otvorenie súboru na čítanie ("r") [cite: 1108, 1114]
        if (fopen_s(&f, nazovSuboru, "r") != 0 || f == NULL) {
            printf("Chyba: Subor '%s' sa nepodarilo otvorit.\n", nazovSuboru);
            return 0;
        }

    // Čítanie čísel, kým sme na konci súboru (EOF) alebo nedosiahneme limit
    // Používame fscanf_s na formátované čítanie [cite: 1146]
    while (pocet < maxVelkost && fscanf_s(f, "%lf", &data[pocet]) == 1) {
        pocet++;
    }

    fclose(f);// Zatvorenie súboru [cite: 1115]
        return pocet;
}

/**
 * [cite_start]@brief Spočíta aritmetický priemer. [cite: 516]
 */
double spocitajPriemer(double data[], int pocet) {
    if (pocet == 0) return 0.0;
    double suma = 0.0;
    for (int i = 0; i < pocet; i++) {
        suma += data[i];
    }
    return suma / pocet;
}

/**
 * [cite_start]@brief Spočíta vážený priemer. [cite: 516]
 */
double spocitajVazenyPriemer(double data[], int pocet) {
    if (pocet == 0) return 0.0;

    // Použijeme ďalšie statické pole pre váhy
    double vahy[MAX_DATA_VELKOST];
    double sucetVah = 0.0;
    double sucetVazenychHodnot = 0.0;

    printf("\n--- Zadanie vah pre vazeny priemer ---\n");
    printf("Zadajte %d vah (pre kazde cislo jednu):\n", pocet);

    for (int i = 0; i < pocet; i++) {
        printf("Vaha pre %.2f: ", data[i]);
        scanf_s("%lf", &vahy[i]);
        sucetVah += vahy[i];
        sucetVazenychHodnot += data[i] * vahy[i];
    }
    vycistiBuffer();

    if (sucetVah == 0) return 0.0;
    return sucetVazenychHodnot / sucetVah;
}

/**
 * [cite_start]@brief Spočíta medián. [cite: 516]
 * Musí najprv utriediť dáta.
 */
void spocitajMedian(double data[], int pocet, double* median) {
    if (pocet == 0) {
        *median = 0.0;
        return;
    }

    // Musíme vytvoriť kópiu poľa, aby sme nezmenili pôvodné dáta
    // (pôvodné dáta potrebujeme nezotriedené pre histogram)
    double tempPole[MAX_DATA_VELKOST];
    for (int i = 0; i < pocet; i++) {
        tempPole[i] = data[i];
    }

    // Utriedime kópiu poľa pomocou Bublinkového triedenia (Prednáška č. 8) [cite: 417]
    utriedPole(tempPole, pocet);

    if (pocet % 2 == 0) {
        // Párny počet - priemer dvoch prostredných
        *median = (tempPole[pocet / 2 - 1] + tempPole[pocet / 2]) / 2.0;
    }
    else {
        // Nepárny počet - prostredný prvok
        *median = tempPole[pocet / 2];
    }
}

/**
 * [cite_start]@brief Implementácia Bublinkového triedenia (Prednáška č. 8) [cite: 417-420].
 */
void utriedPole(double data[], int pocet) {
    int i, j;
    double temp;
    for (i = 0; i < pocet - 1; i++) {
        // Prechádzame poľom
        for (j = 0; j < pocet - i - 1; j++) {
            // Ak je prvok väčší ako nasledujúci, vymeníme ich
            if (data[j] > data[j + 1]) {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

/**
 * [cite_start]@brief Spočíta rozptyl a smerodajnú odchýlku. [cite: 516]
 */
void spocitajRozptyl(double data[], int pocet, double priemer, double* rozptyl, double* smerOdchylka) {
    if (pocet < 2) { // Rozptyl z 1 alebo 0 hodnôt je 0
        *rozptyl = 0.0;
        *smerOdchylka = 0.0;
        return;
    }

    double sumaStvorcov = 0.0;
    for (int i = 0; i < pocet; i++) {
        sumaStvorcov += (data[i] - priemer) * (data[i] - priemer);
    }

    // Použijeme výberový rozptyl (delenie n-1)
    *rozptyl = sumaStvorcov / (pocet - 1);
    *smerOdchylka = sqrt(*rozptyl);
}

/**
 * [cite_start]@brief Zobrazí jednoduchý histogram v konzole. [cite: 516]
 */
void zobrazHistogramCisiel(double data[], int pocet) {
    if (pocet == 0) return;

    double min = data[0], max = data[0];
    for (int i = 1; i < pocet; i++) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
    }

    if (min == max) {
        printf("\n--- Histogram ---\n");
        printf("Vsetky hodnoty su rovnake (%.2f).\n", min);
        return;
    }

    int triedy[HISTOGRAM_TRIEDY] = { 0 }; // Inicializácia poľa na nuly
    double sirkaTriedy = (max - min) / HISTOGRAM_TRIEDY;

    for (int i = 0; i < pocet; i++) {
        int indexTriedy = (int)((data[i] - min) / sirkaTriedy);
        // Ošetrenie pre hraničnú hodnotu (max)
        if (indexTriedy >= HISTOGRAM_TRIEDY) indexTriedy = HISTOGRAM_TRIEDY - 1;
        triedy[indexTriedy]++;
    }

    printf("\n--- Histogram ---\n");
    for (int i = 0; i < HISTOGRAM_TRIEDY; i++) {
        double zaciatokTriedy = min + i * sirkaTriedy;
        double koniecTriedy = zaciatokTriedy + sirkaTriedy;
        printf("[%.2f - %.2f]: %d\n", zaciatokTriedy, koniecTriedy, triedy[i]);
    }
}

/**
 * [cite_start]@brief Uloží číselné výsledky do súboru. [cite: 518]
 * Používa koncepty z Prednášky č. [cite_start]9[cite: 1097].
 */
void ulozCiselneVysledky(const char* nazovSuboru, double priemer, double v_priemer, double median, double rozptyl, double smerOdchylka) {
    FILE* f;
    // Otvorenie súboru na zápis ("w") [cite: 1108, 1114]
        if (fopen_s(&f, nazovSuboru, "w") != 0 || f == NULL) {
            printf("Chyba: Subor '%s' sa nepodarilo otvorit pre zapis.\n", nazovSuboru);
            return;
        }

    // Zápis do súboru pomocou fprintf [cite: 1131]
    fprintf(f, "--- Vysledky statistickej analyzy (cisla) ---\n");
    fprintf(f, "Priemer:         %.4f\n", priemer);
    fprintf(f, "Vazeny priemer:  %.4f\n", v_priemer);
    fprintf(f, "Median:          %.4f\n", median);
    fprintf(f, "Rozptyl:         %.4f\n", rozptyl);
    fprintf(f, "Smer. odchylka:  %.4f\n", smerOdchylka);

    fclose(f); // Zatvorenie súboru [cite: 1115]
    printf("Vysledky boli uspesne ulozene do suboru '%s'.\n", nazovSuboru);
}


/*
 * =============================================================
 * [cite_start]SEKCIA 2: TEXTOVÉ ŠTATISTIKY [cite: 517]
 * =============================================================
 */

 /**
  * @brief Hlavná funkcia pre spracovanie textových štatistík.
  */
void sekciaTextova() {
    char text[MAX_TEXT_VELKOST];
    int volbaVstupu = 0;
    char nazovSuboru[100];
    int pocetZnakov = 0;
    int pocetSlov = 0;

    printf("\n--- SEKCIA 2: TEXTOVE HODNOTY ---\n");
    printf("Zvolte zdroj dat[cite: 514]:\n");
    printf("1. Vstup z konzoly\n");
    printf("2. Vstup z textoveho suboru\n");
    printf("Vasa volba: ");
    scanf_s("%d", &volbaVstupu);
    vycistiBuffer();

    if (volbaVstupu == 1) { // Vstup z konzoly [cite: 514]
        printf("Zadajte text (max %d znakov, ukoncite stlacenim Enter):\n", MAX_TEXT_VELKOST - 1);
        fgets(text, MAX_TEXT_VELKOST, stdin);// Načítanie reťazca [cite: 1141]
    }
    else if (volbaVstupu == 2) { // Vstup zo súboru [cite: 514]
        if (!nacitajTextZoSuboru(text, MAX_TEXT_VELKOST)) {
            printf("Nepodarilo sa nacitat text zo suboru.\n");
            return;
        }
        printf("Textovy subor uspesne nacitany.\n");
    }
    else {
        printf("Neplatna volba.\n");
        return;
    }

    // Výpočty [cite: 517]
    pocetZnakov = spocitajZnaky(text);
    pocetSlov = spocitajSlova(text);

    // Zobrazenie výsledkov
    printf("\n--- VYSLEDKY ANALYZY TEXTU ---\n");
    printf("Pocet znakov (vratane medzier): %d\n", pocetZnakov);
    printf("Pocet slov:                    %d\n", pocetSlov);

    // Zobrazenie histogramov [cite: 517]
    zobrazHistogramPismen(text);
    zobrazHistogramDlzkySlov(text);

    // Uloženie výsledkov [cite: 518]
    printf("\nZadat nazov suboru pre ulozenie vysledkov (alebo nechajte prazdne pre neulozenie): ");
    fgets(nazovSuboru, 100, stdin);
    nazovSuboru[strcspn(nazovSuboru, "\n")] = 0; // Odstránenie '\n'

    if (strlen(nazovSuboru) > 0) {
        ulozTextoveVysledky(nazovSuboru, pocetZnakov, pocetSlov);
    }
}

/**
 * @brief Načíta text zo súboru.
 * Používa koncepty z Prednášky č. [cite_start]9 (práca s binárnym súborom)[cite: 1159].
 */
int nacitajTextZoSuboru(char text[], int maxVelkost) {
    char nazovSuboru[100];
    FILE* f;
    size_t pocetPrecitanych = 0;

    printf("Zadajte nazov vstupneho suboru (napr. text.txt): ");
    fgets(nazovSuboru, 100, stdin);
    nazovSuboru[strcspn(nazovSuboru, "\n")] = 0;

    // Otvoríme súbor v režime "rb" (binárne čítanie) [cite: 1114]
        // Je to jednoduchšie pre začiatočníka na načítanie celého bloku
    if (fopen_s(&f, nazovSuboru, "rb") != 0 || f == NULL) {
        printf("Chyba: Subor '%s' sa nepodarilo otvorit.\n", nazovSuboru);
        return 0;
     }

    // Načítame celý súbor naraz (alebo koľko sa zmestí) [cite: 1159]
    pocetPrecitanych = fread(text, 1, maxVelkost - 1, f);

    // DÔLEŽITÉ: Ručne pridáme ukončovací znak reťazca (Prednáška č. 5) [cite: 898]
    text[pocetPrecitanych] = '\0';

    fclose(f);// [cite: 1115]
    return (int)pocetPrecitanych;
}

/**
 * [cite_start]@brief Spočíta celkový počet znakov v reťazci. [cite: 517]
 */
int spocitajZnaky(const char* text) {
    // Použijeme funkciu z <string.h> (Prednáška č. 5) [cite: 986]
    // Poznámka: strlen nepočíta '\0', ale počíta '\n'
    return (int)strlen(text);
}

/**
 * [cite_start]@brief Spočíta počet slov v texte. [cite: 517]
 */
int spocitajSlova(const char* text) {
    int pocet = 0;
    int vSlove = 0; // Príznak (flag): 0 = sme v medzere, 1 = sme v slove

    for (int i = 0; text[i] != '\0'; i++) {
        // isspace() kontroluje medzeru, tabulator, novy riadok
        if (isspace(text[i])) {
            vSlove = 0;
        }
        else {
            // Ak sme neboli v slove a teraz sme, našli sme nové slovo
            if (vSlove == 0) {
                vSlove = 1;
                pocet++;
            }
        }
    }
    return pocet;
}

/**
 * [cite_start]@brief Zobrazí histogram počtu písmen. [cite: 517]
 */
void zobrazHistogramPismen(const char* text) {
    // Pole pre 26 písmen abecedy (A-Z)
    int poctyPismen[26] = { 0 };
    int celkovoPismen = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (isalpha(c)) { // Kontroluje, či je to písmeno
            c = tolower(c); // Prevedie na malé písmeno
            int index = c - 'a'; // 'a' je 0, 'b' je 1, atď.
            poctyPismen[index]++;
            celkovoPismen++;
        }
    }

    printf("\n--- Histogram Pismen (A-Z) ---\n");
    if (celkovoPismen == 0) {
        printf("Text neobsahuje ziadne pismena.\n");
        return;
    }
    for (int i = 0; i < 26; i++) {
        // %c vypíše znak, 'A' + i dá správne písmeno
        printf("%c: %d\n", 'A' + i, poctyPismen[i]);
    }
}

/**
 * [cite_start]@brief Zobrazí histogram dĺžok slov. [cite: 517]
 */
void zobrazHistogramDlzkySlov(const char* text) {
    int poctyDlzok[MAX_DLZKA_SLOVA] = { 0 };
    int aktualnaDlzka = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) { // Považujeme za slovo len písmená
            aktualnaDlzka++;
        }
        else {
            // Sme na konci slova (medzera, číslo, bodka...)
            if (aktualnaDlzka > 0) {
                if (aktualnaDlzka < MAX_DLZKA_SLOVA) {
                    poctyDlzok[aktualnaDlzka]++;
                }
                else {
                    // Príliš dlhé slová idú do poslednej priehradky
                    poctyDlzok[MAX_DLZKA_SLOVA - 1]++;
                }
                aktualnaDlzka = 0; // Vynulujeme pre ďalšie slovo
            }
        }
    }
    // Započítame posledné slovo, ak text nekončil medzerou
    if (aktualnaDlzka > 0) {
        if (aktualnaDlzka < MAX_DLZKA_SLOVA) {
            poctyDlzok[aktualnaDlzka]++;
        }
        else {
            poctyDlzok[MAX_DLZKA_SLOVA - 1]++;
        }
    }

    printf("\n--- Histogram Dlzky Slov ---\n");
    for (int i = 1; i < MAX_DLZKA_SLOVA; i++) {
        if (poctyDlzok[i] > 0) {
            if (i == MAX_DLZKA_SLOVA - 1) {
                printf("Dlzka %2d+ : %d\n", i, poctyDlzok[i]);
            }
            else {
                printf("Dlzka %2d  : %d\n", i, poctyDlzok[i]);
            }
        }
    }
}

/**
 * [cite_start]@brief Uloží textové výsledky do súboru. [cite: 518]
 * Používa koncepty z Prednášky č. [cite_start]9[cite: 1097].
 */
void ulozTextoveVysledky(const char* nazovSuboru, int pocetZnakov, int pocetSlov) {
    FILE* f;
    if (fopen_s(&f, nazovSuboru, "w") != 0 || f == NULL) { // [cite: 1108, 1114]
        printf("Chyba: Subor '%s' sa nepodarilo otvorit pre zapis.\n", nazovSuboru);
        return;
    }

    fprintf(f, "--- Vysledky statistickej analyzy (text) ---\n");// [cite: 1131]
        fprintf(f, "Pocet znakov (celkom): %d\n", pocetZnakov);
    fprintf(f, "Pocet slov:           %d\n", pocetSlov);

    fclose(f);// [cite: 1115]
        printf("Vysledky boli uspesne ulozene do suboru '%s'.\n", nazovSuboru);
}

