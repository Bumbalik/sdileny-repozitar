
#include "modul.h"


void vycistiBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void utriedPole(double data[], int pocet) {
    // Bublinkové triedenie
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

double spocitajPriemer(double data[], int pocet) {
    double suma = 0.0;
    for (int i = 0; i < pocet; i++) {
        suma += data[i];
    }
    return (pocet > 0) ? suma / pocet : 0.0;
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
        if (scanf_s("%lf", &vahy[i]) != 1) { vahy[i] = 0.0; }
        sucetVah += vahy[i];
        sucetVazenychHodnot += data[i] * vahy[i];
    }
    // Buffer sa čisti v main.c po vstupe, aby nedošlo k chybe
    
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

    printf("\n--- Histogram cisel (Rozdelenie do %d tried) ---\n", HISTOGRAM_TRIEDY);
    for (int i = 0; i < HISTOGRAM_TRIEDY; i++) {
        double zaciatokTriedy = min + i * sirkaTriedy;
        double koniecTriedy = zaciatokTriedy + sirkaTriedy;
        printf("[%.2f - %.2f]: %d\n", zaciatokTriedy, koniecTriedy, triedy[i]);
    }
}

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
        } else {
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

int citajDataZoSuboru(double data[], int maxPocet) {
    FILE* f;
    int pocet = 0;
    if (fopen_s(&f, "../../../vstup.txt", "r") != 0 || f == NULL) {
        printf("Chyba: Subor 'vstup.txt' sa nepodarilo otvorit pre citanie na ceste '../../../'.\n");
        return 0;
    }

    printf("Citanie dat zo suboru '../../../vstup.txt'...\n");

    while (pocet < maxPocet && fscanf_s(f, "%lf", &data[pocet]) == 1) {
        pocet++;
    }

    fclose(f);
    printf("Nacitano %d ciselnych hodnot zo suboru.\n", pocet);
    return pocet;
}

void ulozCiselneVysledky(double priemer, double v_priemer, double median, double rozptyl, double smerOdchylka) {
    
    FILE* f;
    if (fopen_s(&f, "../../../vysledky.txt", "w") != 0 || f == NULL) {
        printf("Chyba: Subor 'vysledky.txt' sa nepodarilo otvorit pre zapis na ceste '../../../'.\n");
        return;
    }

    fprintf(f, "--- Vysledky statistickej analyzy (cisla) ---\n");
    fprintf(f, "Priemer:         %.4f\n", priemer);
    fprintf(f, "Vazeny priemer:  %.4f\n", v_priemer);
    fprintf(f, "Median:          %.4f\n", median);
    fprintf(f, "Rozptyl:         %.4f\n", rozptyl);
    fprintf(f, "Smer. odchylka:  %.4f\n", smerOdchylka);

    fclose(f);
    printf("Vysledky boli uspesne ulozene do suboru 'vysledky.txt' na ceste '../../../'.\n");
}

void ulozTextoveVysledky(int pocetZnakov, int pocetSlov) {
    
    FILE* f;
    if (fopen_s(&f, "../../../vysledky.txt", "w") != 0 || f == NULL) {
        printf("Chyba: Subor 'vysledky.txt' sa nepodarilo otvorit pre zapis na ceste '../../../'.\n");
        return;
    }

    fprintf(f, "--- Vysledky statistickej analyzy (text) ---\n");
    fprintf(f, "Pocet znakov (celkom): %d\n", pocetZnakov);
    fprintf(f, "Pocet slov:           %d\n", pocetSlov);

    fclose(f);
    printf("Vysledky boli uspesne ulozene do suboru 'vysledky.txt' na ceste '../../../'.\n");
}