
#include "main.h" 
#include "modul.h" 

void sekciaCiselna() {
    double data[MAX_DATA_VELKOST]; 
    int pocetHodnot = 0;
    int volbaVstupu = 0; 

    printf("\n--- SEKCIA 1: CISELNE HODNOTY ---\n");
    printf("Vyberte zdroj vstupu:\n");
    printf("1. Vstup z konzoly\n");
    printf("2. Vstup zo suboru '../../../vstup.txt'\n");
    printf("Vasa volba: ");
    if (scanf_s("%d", &volbaVstupu) != 1) { volbaVstupu = 0; }
    vycistiBuffer();

    if (volbaVstupu == 1) {
        printf("Zadajte pocet hodnot (max %d): ", MAX_DATA_VELKOST);
        if (scanf_s("%d", &pocetHodnot) != 1) { pocetHodnot = 0; }
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

    } else if (volbaVstupu == 2) {
        pocetHodnot = citajDataZoSuboru(data, MAX_DATA_VELKOST);

        if (pocetHodnot == 0) {
            printf("Analýza sa neuskutoční.\n");
            return;
        }
    } else {
        printf("Neplatna volba. Navrat do hlavneho menu.\n");
        return;
    }

    double priemer = spocitajPriemer(data, pocetHodnot);
    double v_priemer = spocitajVazenyPriemer(data, pocetHodnot);
    double median; spocitajMedian(data, pocetHodnot, &median);
    double rozptyl, smerOdchylka; spocitajRozptyl(data, pocetHodnot, priemer, &rozptyl, &smerOdchylka);

    printf("\n--- VYSLEDKY ANALYZY ---\n");
    printf("Pocet hodnot:    %d\n", pocetHodnot);
    printf("Priemer:         %.4f\n", priemer);
    printf("Vazeny priemer:  %.4f\n", v_priemer);
    printf("Median:          %.4f\n", median);
    printf("Rozptyl:         %.4f\n", rozptyl);
    printf("Smer. odchylka:  %.4f\n", smerOdchylka);


    printf("\nStlacte Enter pre ulozenie vysledkov (do suboru 'vysledky.txt' o 3 urovne vyssie).\n");
    getchar(); 

    ulozCiselneVysledky(priemer, v_priemer, median, rozptyl, smerOdchylka);
}


void sekciaTextova() {
    char text[MAX_TEXT_VELKOST];

    printf("\n--- SEKCIA 2: TEXTOVE HODNOTY ---\n");
    printf("Vstup dat bude z konzoly.\n");

    printf("Zadajte text (max %d znakov, ukoncite Enterom):\n", MAX_TEXT_VELKOST - 1);
    fgets(text, MAX_TEXT_VELKOST, stdin); 

    int pocetZnakov = spocitajZnaky(text);
    int pocetSlov = spocitajSlova(text);

    printf("\n--- VYSLEDKY ANALYZY TEXTU ---\n");
    printf("Pocet znakov (vratane medzier): %d\n", pocetZnakov);
    printf("Pocet slov:                    %d\n", pocetSlov);
    printf("\nStlacte Enter pre ulozenie vysledkov (do suboru 'vysledky.txt' o 3 urovne vyssie).\n");
    getchar(); 

    ulozTextoveVysledky(pocetZnakov, pocetSlov);
}

int main() {
    int volba = 0;

    do {
        printf("\n--- STATISTICKA KALKULACKA (Projekt 5) ---\n");
        printf("1. Sekcia 1: Ciselne hodnoty\n");
        printf("2. Sekcia 2: Textove hodnoty\n");
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