#include "modul.h"

const std::string VSTUP_SUBOR = "vstup.txt";
const std::string VYSLEDKY_SUBOR = "vysledky.txt";

void sekciaCiselna() {
    std::vector<double> data;
    int volbaVstupu = 0;

    std::cout << "\n--- SEKCIA 1: CISELNE HODNOTY ---\n";
    std::cout << "Vyberte zdroj vstupu:\n";
    std::cout << "1. Vstup z konzoly\n";
    std::cout << "2. Vstup zo suboru '" << VSTUP_SUBOR << "'\n";
    std::cout << "Vasa volba: ";
    std::cin >> volbaVstupu;
    vycistiBuffer();

    if (volbaVstupu == 1) {
        int pocetHodnot = 0;
        std::cout << "Zadajte pocet hodnot (max " << MAX_DATA_VELKOST << "): ";
        std::cin >> pocetHodnot;
        vycistiBuffer();

        if (pocetHodnot <= 0 || pocetHodnot > MAX_DATA_VELKOST) {
            std::cout << "Neplatny pocet hodnot.\n";
            return;
        }
        data = nacitajCiselneHodnotyZRiadku(pocetHodnot);
    } else if (volbaVstupu == 2) {
        citajDataZoSuboru(data, VSTUP_SUBOR);
        if (data.empty()) {
            std::cout << "Analýza sa neuskutocni.\n";
            return;
        }
    } else {
        std::cout << "Neplatna volba. Navrat do hlavneho menu.\n";
        return;
    }

    std::cout << "\n--- Zadanie vah pre vazeny priemer ---\n";
    std::vector<double> vahy;
    vahy.reserve(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        double vaha;
        std::cout << "Vaha pre " << data[i] << ": ";
        while (!(std::cin >> vaha)) {
            std::cin.clear();
            vycistiBuffer();
            std::cout << "Neplatny vstup, skuste znova: ";
        }
        vahy.push_back(vaha);
    }
    vycistiBuffer();

    double priemer = spocitajPriemer(data);
    double vPriemer = spocitajVazenyPriemer(data, vahy);
    double median = spocitajMedian(data);
    auto [rozptyl, smerOdchylka] = spocitajRozptyl(data, priemer);

    std::cout << "\n--- VYSLEDKY ANALYZY ---\n";
    std::cout << "Pocet hodnot:    " << data.size() << '\n';
    std::cout << "Priemer:         " << priemer << '\n';
    std::cout << "Vazeny priemer:  " << vPriemer << '\n';
    std::cout << "Median:          " << median << '\n';
    std::cout << "Rozptyl:         " << rozptyl << '\n';
    std::cout << "Smer. odchylka:  " << smerOdchylka << '\n';

    double min = 0.0, max = 0.0;
    auto bins = vytvorHistogramCisiel(data, HISTOGRAM_TRIEDY, min, max);
    vykresliHistogramSGaussom(bins, min, max, priemer, smerOdchylka);

    std::cout << "\nStlacte Enter pre ulozenie vysledkov (do suboru '" << VYSLEDKY_SUBOR << "')." << std::endl;
    std::cin.get();
    ulozCiselneVysledky(VYSLEDKY_SUBOR, priemer, vPriemer, median, rozptyl, smerOdchylka);
}

void sekciaTextova() {
    std::string text;
    int volba = 0;

    std::cout << "\n--- SEKCIA 2: TEXTOVE HODNOTY ---\n";
    std::cout << "Vyberte zdroj vstupu:\n";
    std::cout << "1. Vstup z konzoly\n";
    std::cout << "2. Vstup zo suboru '" << VSTUP_SUBOR << "'\n";
    std::cout << "Vasa volba: ";
    std::cin >> volba;
    vycistiBuffer();

    if (volba == 1) {
        std::cout << "Zadajte text (ukoncite Enterom, max " << MAX_TEXT_VELKOST << " znakov):\n";
        std::getline(std::cin, text);
        if (text.size() > MAX_TEXT_VELKOST) {
            text = text.substr(0, MAX_TEXT_VELKOST);
        }
    } else if (volba == 2) {
        if (!citajTextZoSuboru(text, VSTUP_SUBOR)) {
            std::cout << "Analýza sa neuskutocni.\n";
            return;
        }
    } else {
        std::cout << "Neplatna volba. Navrat do hlavneho menu.\n";
        return;
    }

    int pocetZnakov = spocitajZnaky(text);
    int pocetSlov = spocitajSlova(text);
    auto histogramPismen = vytvorHistogramPismen(text);
    auto histogramDlziek = vytvorHistogramDlzkySlov(text, MAX_DLZKA_SLOVA);

    std::cout << "\n--- VYSLEDKY ANALYZY TEXTU ---\n";
    std::cout << "Pocet znakov (vratane medzier): " << pocetZnakov << '\n';
    std::cout << "Pocet slov:                    " << pocetSlov << '\n';
    vypisHistogramPismen(histogramPismen);
    vypisHistogramDlzkySlov(histogramDlziek);

    std::cout << "\nStlacte Enter pre ulozenie vysledkov (do suboru '" << VYSLEDKY_SUBOR << "')." << std::endl;
    std::cin.get();
    ulozTextoveVysledky(VYSLEDKY_SUBOR, pocetZnakov, pocetSlov);
}

int main() {
    int volba = 0;

    do {
        std::cout << "\n--- STATISTICKA KALKULACKA (Projekt 5) ---\n";
        std::cout << "1. Sekcia 1: Ciselne hodnoty\n";
        std::cout << "2. Sekcia 2: Textove hodnoty\n";
        std::cout << "3. Koniec\n";
        std::cout << "Vasa volba: ";

        std::cin >> volba;
        vycistiBuffer();

        switch (volba) {
        case 1:
            sekciaCiselna();
            break;
        case 2:
            sekciaTextova();
            break;
        case 3:
            std::cout << "Program sa ukoncuje.\n";
            break;
        default:
            std::cout << "Neplatna volba. Prosim, skuste znova.\n";
            break;
        }
    } while (volba != 3);

    return 0;
}

