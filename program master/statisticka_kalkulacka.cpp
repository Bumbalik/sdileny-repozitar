

#include "statisticka_kalkulacka.h"
#include <stdio.h>    
#include <string.h>  
#include <math.h>     
#include <ctype.h>   


#define MAX_DATA_VELKOST 1000  
#define MAX_TEXT_VELKOST 5000    
#define MAX_DLZKA_SLOVA 30     
#define HISTOGRAM_TRIEDY 10    


void vycistiBuffer();


void sekciaCiselna();
int nacitajCislaZTextovehoSuboru(double data[], int maxVelkost);
double spocitajPriemer(double data[], int pocet);
double spocitajVazenyPriemer(double data[], int pocet);
void spocitajMedian(double data[], int pocet, double* median);
void spocitajRozptyl(double data[], int pocet, double priemer, double* rozptyl, double* smerOdchylka);
void utriedPole(double data[], int pocet);
void zobrazHistogramCisiel(double data[], int pocet);
void ulozCiselneVysledky(const char* nazovSuboru, double priemer, double v_priemer, double median, double rozptyl, double smerOdchylka);


void sekciaTextova();
int nacitajTextZoSuboru(char text[], int maxVelkost);
int spocitajZnaky(const char* text);
int spocitajSlova(const char* text);
void zobrazHistogramPismen(const char* text);
void zobrazHistogramDlzkySlov(const char* text);
void ulozTextoveVysledky(const char* nazovSuboru, int pocetZnakov, int pocetSlov);

void vycistiBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



void sekciaCiselna() {

    double data[MAX_DATA_VELKOST];
    int pocetHodnot = 0;
    int volbaVstupu = 0;
    char nazovSuboru[100];

    printf("\n--- SEKCIA 1: CISELNE HODNOTY ---\n");
    printf("Zvolte zdroj dat:\n");
    printf("1. Vstup z konzoly\n");
    printf("2. Vstup z textoveho suboru\n");
    printf("Vasa volba: ");
    scanf_s("%d", &volbaVstupu);
    vycistiBuffer();

    if (volbaVstupu == 1) {
        printf("Zadajte pocet hodnot (max %d): ", MAX_DATA_VELKOST);
        scanf_s("%d", &pocetHodnot);
        vycistiBuffer();

        if (pocetHodnot <= 0 || pocetHodnot > MAX_DATA_VELKOST) {
            printf("Neplatny pocet hodnot.\n");
            return;
        }
        printf("Zadajte %d cisel (oddelene medzerou alebo Enterom):\n", pocetHodnot);
        for (int i = 0; i < pocetHodnot; i++) { 
            scanf_s("%lf", &data[i]);
        }
        vycistiBuffer();
    }
    else if (volbaVstupu == 2) { 
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


    double priemer = 0.0, v_priemer = 0.0, median = 0.0, rozptyl = 0.0, smerOdchylka = 0.0;

    priemer = spocitajPriemer(data, pocetHodnot);
    v_priemer = spocitajVazenyPriemer(data, pocetHodnot);
    spocitajMedian(data, pocetHodnot, &median);
    spocitajRozptyl(data, pocetHodnot, priemer, &rozptyl, &smerOdchylka);

    
    printf("\n--- VYSLEDKY ANALYZY ---\n");
    printf("Pocet hodnot:    %d\n", pocetHodnot);
    printf("Priemer:         %.4f\n", priemer);
    printf("Vazeny priemer:  %.4f\n", v_priemer);
    printf("Median:          %.4f\n", median);
    printf("Rozptyl:         %.4f\n", rozptyl);
    printf("Smer. odchylka:  %.4f\n", smerOdchylka);

    
    zobrazHistogramCisiel(data, pocetHodnot);

   
    printf("\nZadat nazov suboru pre ulozenie vysledkov (alebo nechajte prazdne pre neulozenie): ");
    fgets(nazovSuboru, 100, stdin);
    nazovSuboru[strcspn(nazovSuboru, "\n")] = 0; 

    if (strlen(nazovSuboru) > 0) {
        ulozCiselneVysledky(nazovSuboru, priemer, v_priemer, median, rozptyl, smerOdchylka);
    }
}


 
int nacitajCislaZTextovehoSuboru(double data[], int maxVelkost) {
    char nazovSuboru[100];
    FILE* f; 
        int pocet = 0;

    printf("Zadajte nazov vstupneho suboru (napr. cisla.txt): ");
    fgets(nazovSuboru, 100, stdin);
    nazovSuboru[strcspn(nazovSuboru, "\n")] = 0;

    
        if (fopen_s(&f, nazovSuboru, "r") != 0 || f == NULL) {
            printf("Chyba: Subor '%s' sa nepodarilo otvorit.\n", nazovSuboru);
            return 0;
        }

   
    while (pocet < maxVelkost && fscanf_s(f, "%lf", &data[pocet]) == 1) {
        pocet++;
    }

    fclose(f);
        return pocet;
}


double spocitajPriemer(double data[], int pocet) {
    if (pocet == 0) return 0.0;
    double suma = 0.0;
    for (int i = 0; i < pocet; i++) {
        suma += data[i];
    }
    return suma / pocet;
}


double spocitajVazenyPriemer(double data[], int pocet) {
    if (pocet == 0) return 0.0;

   
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


void spocitajMedian(double data[], int pocet, double* median) {
    if (pocet == 0) {
        *median = 0.0;
        return;
    }

    
    double tempPole[MAX_DATA_VELKOST];
    for (int i = 0; i < pocet; i++) {
        tempPole[i] = data[i];
    }

    
    utriedPole(tempPole, pocet);

    if (pocet % 2 == 0) {
        
        *median = (tempPole[pocet / 2 - 1] + tempPole[pocet / 2]) / 2.0;
    }
    else {
        
        *median = tempPole[pocet / 2];
    }
}


void utriedPole(double data[], int pocet) {
    int i, j;
    double temp;
    for (i = 0; i < pocet - 1; i++) {
        
        for (j = 0; j < pocet - i - 1; j++) {
           
            if (data[j] > data[j + 1]) {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}


void spocitajRozptyl(double data[], int pocet, double priemer, double* rozptyl, double* smerOdchylka) {
    if (pocet < 2) { 
        *rozptyl = 0.0;
        *smerOdchylka = 0.0;
        return;
    }

    double sumaStvorcov = 0.0;
    for (int i = 0; i < pocet; i++) {
        sumaStvorcov += (data[i] - priemer) * (data[i] - priemer);
    }

    
    *rozptyl = sumaStvorcov / (pocet - 1);
    *smerOdchylka = sqrt(*rozptyl);
}


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

    int triedy[HISTOGRAM_TRIEDY] = { 0 }; 
    double sirkaTriedy = (max - min) / HISTOGRAM_TRIEDY;

    for (int i = 0; i < pocet; i++) {
        int indexTriedy = (int)((data[i] - min) / sirkaTriedy);
        
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


void ulozCiselneVysledky(const char* nazovSuboru, double priemer, double v_priemer, double median, double rozptyl, double smerOdchylka) {
    FILE* f;
    
        if (fopen_s(&f, nazovSuboru, "w") != 0 || f == NULL) {
            printf("Chyba: Subor '%s' sa nepodarilo otvorit pre zapis.\n", nazovSuboru);
            return;
        }

   
    fprintf(f, "--- Vysledky statistickej analyzy (cisla) ---\n");
    fprintf(f, "Priemer:         %.4f\n", priemer);
    fprintf(f, "Vazeny priemer:  %.4f\n", v_priemer);
    fprintf(f, "Median:          %.4f\n", median);
    fprintf(f, "Rozptyl:         %.4f\n", rozptyl);
    fprintf(f, "Smer. odchylka:  %.4f\n", smerOdchylka);

    fclose(f); 
    printf("Vysledky boli uspesne ulozene do suboru '%s'.\n", nazovSuboru);
}




 
void sekciaTextova() {
    char text[MAX_TEXT_VELKOST];
    int volbaVstupu = 0;
    char nazovSuboru[100];
    int pocetZnakov = 0;
    int pocetSlov = 0;

    printf("\n--- SEKCIA 2: TEXTOVE HODNOTY ---\n");
    printf("Zvolte zdroj dat:\n");
    printf("1. Vstup z konzoly\n");
    printf("2. Vstup z textoveho suboru\n");
    printf("Vasa volba: ");
    scanf_s("%d", &volbaVstupu);
    vycistiBuffer();

    if (volbaVstupu == 1) { 
        printf("Zadajte text (max %d znakov, ukoncite stlacenim Enter):\n", MAX_TEXT_VELKOST - 1);
        fgets(text, MAX_TEXT_VELKOST, stdin);
    }
    else if (volbaVstupu == 2) { 
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

    
    pocetZnakov = spocitajZnaky(text);
    pocetSlov = spocitajSlova(text);

    
    printf("\n--- VYSLEDKY ANALYZY TEXTU ---\n");
    printf("Pocet znakov (vratane medzier): %d\n", pocetZnakov);
    printf("Pocet slov:                    %d\n", pocetSlov);

    
    zobrazHistogramPismen(text);
    zobrazHistogramDlzkySlov(text);

   
    printf("\nZadat nazov suboru pre ulozenie vysledkov (alebo nechajte prazdne pre neulozenie): ");
    fgets(nazovSuboru, 100, stdin);
    nazovSuboru[strcspn(nazovSuboru, "\n")] = 0; 

    if (strlen(nazovSuboru) > 0) {
        ulozTextoveVysledky(nazovSuboru, pocetZnakov, pocetSlov);
    }
}


int nacitajTextZoSuboru(char text[], int maxVelkost) {
    char nazovSuboru[100];
    FILE* f;
    size_t pocetPrecitanych = 0;

    printf("Zadajte nazov vstupneho suboru (napr. text.txt): ");
    fgets(nazovSuboru, 100, stdin);
    nazovSuboru[strcspn(nazovSuboru, "\n")] = 0;

    
    if (fopen_s(&f, nazovSuboru, "rb") != 0 || f == NULL) {
        printf("Chyba: Subor '%s' sa nepodarilo otvorit.\n", nazovSuboru);
        return 0;
     }

    
    pocetPrecitanych = fread(text, 1, maxVelkost - 1, f);

    
    text[pocetPrecitanych] = '\0';

    fclose(f);
    return (int)pocetPrecitanych;
}


int spocitajZnaky(const char* text) {
   
    return (int)strlen(text);
}


int spocitajSlova(const char* text) {
    int pocet = 0;
    int vSlove = 0; 

    for (int i = 0; text[i] != '\0'; i++) {
      
        if (isspace(text[i])) {
            vSlove = 0;
        }
        else {
           
            if (vSlove == 0) {
                vSlove = 1;
                pocet++;
            }
        }
    }
    return pocet;
}


void zobrazHistogramPismen(const char* text) {
  
    int poctyPismen[26] = { 0 };
    int celkovoPismen = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (isalpha(c)) { 
            c = tolower(c); 
            int index = c - 'a'; 
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
        
        printf("%c: %d\n", 'A' + i, poctyPismen[i]);
    }
}


void zobrazHistogramDlzkySlov(const char* text) {
    int poctyDlzok[MAX_DLZKA_SLOVA] = { 0 };
    int aktualnaDlzka = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) { 
            aktualnaDlzka++;
        }
        else {
            
            if (aktualnaDlzka > 0) {
                if (aktualnaDlzka < MAX_DLZKA_SLOVA) {
                    poctyDlzok[aktualnaDlzka]++;
                }
                else {
                   
                    poctyDlzok[MAX_DLZKA_SLOVA - 1]++;
                }
                aktualnaDlzka = 0; 
            }
        }
    }
   
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


void ulozTextoveVysledky(const char* nazovSuboru, int pocetZnakov, int pocetSlov) {
    FILE* f;
    if (fopen_s(&f, nazovSuboru, "w") != 0 || f == NULL) { 
        printf("Chyba: Subor '%s' sa nepodarilo otvorit pre zapis.\n", nazovSuboru);
        return;
    }

    fprintf(f, "--- Vysledky statistickej analyzy (text) ---\n");
        fprintf(f, "Pocet znakov (celkom): %d\n", pocetZnakov);
    fprintf(f, "Pocet slov:           %d\n", pocetSlov);

    fclose(f);
        printf("Vysledky boli uspesne ulozene do suboru '%s'.\n", nazovSuboru);
}

int main() {
    int volba = 0;


    do {
        printf("\n--- STATISTICKA KALKULACKA  ---\n");
        printf("1. Sekcia 1: Ciselne hodnoty \n");
        printf("2. Sekcia 2: Textove hodnoty \n");
        printf("3. Koniec\n");
        printf("Vasa volba: ");


        if (scanf_s("%d", &volba) != 1) {
            volba = 0;
        }
        vycistiBuffer();


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

