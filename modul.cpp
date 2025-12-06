#include "modul.h"

const double PI = 3.14159265358979323846;

void vycistiBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int citajDataZoSuboru(std::vector<double>& data, const std::string& cesta) {
    std::ifstream vstup(cesta);
    if (!vstup.is_open()) {
        std::cout << "Chyba: subor '" << cesta << "' sa nepodarilo otvorit.\n";
        return 0;
    }

    double hodnota;
    while (vstup >> hodnota) {
        data.push_back(hodnota);
        if (static_cast<int>(data.size()) >= MAX_DATA_VELKOST) {
            break;
        }
    }

    std::cout << "Nacitanych " << data.size() << " ciselnych hodnot zo suboru.\n";
    return static_cast<int>(data.size());
}

bool citajTextZoSuboru(std::string& text, const std::string& cesta) {
    std::ifstream vstup(cesta);
    if (!vstup.is_open()) {
        std::cout << "Chyba: subor '" << cesta << "' sa nepodarilo otvorit.\n";
        return false;
    }

    std::ostringstream buffer;
    buffer << vstup.rdbuf();
    text = buffer.str();
    return true;
}

std::vector<double> nacitajCiselneHodnotyZRiadku(int pocetHodnot) {
    std::vector<double> data;
    data.reserve(pocetHodnot);

    for (int i = 0; i < pocetHodnot; ++i) {
        double hodnota;
        std::cout << "Hodnota " << (i + 1) << ": ";
        while (!(std::cin >> hodnota)) {
            std::cin.clear();
            vycistiBuffer();
            std::cout << "Neplatny vstup, skuste znova: ";
        }
        data.push_back(hodnota);
    }
    vycistiBuffer();
    return data;
}

double spocitajPriemer(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    double suma = 0.0;
    for (double hodnota : data) {
        suma += hodnota;
    }
    return suma / data.size();
}

double spocitajVazenyPriemer(const std::vector<double>& data, const std::vector<double>& vahy) {
    if (data.empty() || vahy.size() != data.size()) return 0.0;

    double sucetVah = 0.0;
    double sucet = 0.0;
    for (size_t i = 0; i < data.size(); ++i) {
        sucet += data[i] * vahy[i];
        sucetVah += vahy[i];
    }
    if (sucetVah == 0.0) return 0.0;
    return sucet / sucetVah;
}

double spocitajMedian(std::vector<double> data) {
    if (data.empty()) return 0.0;
    std::sort(data.begin(), data.end());
    size_t stred = data.size() / 2;
    if (data.size() % 2 == 0) {
        return (data[stred - 1] + data[stred]) / 2.0;
    }
    return data[stred];
}

std::pair<double, double> spocitajRozptyl(const std::vector<double>& data, double priemer) {
    if (data.size() < 2) return {0.0, 0.0};

    double suma = 0.0;
    for (double hodnota : data) {
        double odchylka = hodnota - priemer;
        suma += odchylka * odchylka;
    }
    double rozptyl = suma / (data.size() - 1);
    double smerOdchylka = std::sqrt(rozptyl);
    return {rozptyl, smerOdchylka};
}

std::vector<int> vytvorHistogramCisiel(const std::vector<double>& data, int triedy, double& min, double& max) {
    std::vector<int> bins(triedy, 0);
    if (data.empty()) return bins;

    min = *std::min_element(data.begin(), data.end());
    max = *std::max_element(data.begin(), data.end());
    if (min == max) {
        bins[0] = static_cast<int>(data.size());
        return bins;
    }

    double sirkaTriedy = (max - min) / triedy;
    for (double hodnota : data) {
        int index = static_cast<int>((hodnota - min) / sirkaTriedy);
        if (index >= triedy) index = triedy - 1;
        bins[index]++;
    }
    return bins;
}

static std::vector<int> vypocitajGaussoveBinHodnoty(const std::vector<int>& bins, double min, double max, double priemer, double smerOdchylka) {
    std::vector<int> gauss(bins.size(), 0);
    if (bins.empty() || smerOdchylka == 0.0) return gauss;

    double sirkaTriedy = (max - min) / bins.size();
    std::vector<double> raw(bins.size(), 0.0);
    double maxHodnota = 0.0;
    for (size_t i = 0; i < bins.size(); ++i) {
        double stredTriedy = min + (i + 0.5) * sirkaTriedy;
        double exponent = -0.5 * std::pow((stredTriedy - priemer) / smerOdchylka, 2);
        double hodnota = std::exp(exponent) / (smerOdchylka * std::sqrt(2 * PI));
        raw[i] = hodnota;
        if (hodnota > maxHodnota) maxHodnota = hodnota;
    }

    int maxHistogram = *std::max_element(bins.begin(), bins.end());
    for (size_t i = 0; i < gauss.size(); ++i) {
        if (maxHodnota == 0.0) {
            gauss[i] = 0;
        } else {
            gauss[i] = static_cast<int>(std::round(raw[i] / maxHodnota * maxHistogram));
        }
    }
    return gauss;
}

void vykresliHistogramSGaussom(const std::vector<int>& bins, double min, double max, double priemer, double smerOdchylka) {
    if (bins.empty()) return;

    auto gauss = vypocitajGaussoveBinHodnoty(bins, min, max, priemer, smerOdchylka);
    double sirkaTriedy = (max - min) / bins.size();

    std::cout << "\n--- Histogram cisel s Gaussovym rozlozenim ---\n";
    for (size_t i = 0; i < bins.size(); ++i) {
        double zaciatok = min + i * sirkaTriedy;
        double koniec = zaciatok + sirkaTriedy;
        std::cout << "[" << std::fixed << std::setprecision(2) << zaciatok << " - " << koniec << "]: ";

        std::cout << std::string(bins[i], '#');
        if (gauss[i] > 0) {
            if (!bins.empty()) std::cout << ' ';
            std::cout << std::string(gauss[i], '*');
        }
        std::cout << '\n';
    }
}

void ulozCiselneVysledky(const std::string& cesta, double priemer, double vazenyPriemer, double median, double rozptyl, double smerOdchylka) {
    std::ofstream vystup(cesta);
    if (!vystup.is_open()) {
        std::cout << "Chyba: subor '" << cesta << "' sa nepodarilo otvorit pre zapis.\n";
        return;
    }

    vystup << "--- Vysledky statistickej analyzy (cisla) ---\n";
    vystup << "Priemer:         " << priemer << '\n';
    vystup << "Vazeny priemer:  " << vazenyPriemer << '\n';
    vystup << "Median:          " << median << '\n';
    vystup << "Rozptyl:         " << rozptyl << '\n';
    vystup << "Smer. odchylka:  " << smerOdchylka << '\n';

    std::cout << "Vysledky boli ulozene do suboru '" << cesta << "'.\n";
}

int spocitajZnaky(const std::string& text) {
    return static_cast<int>(text.size());
}

int spocitajSlova(const std::string& text) {
    int pocet = 0;
    bool vSlove = false;
    for (char c : text) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            vSlove = false;
        } else {
            if (!vSlove) {
                ++pocet;
                vSlove = true;
            }
        }
    }
    return pocet;
}

std::array<int, 26> vytvorHistogramPismen(const std::string& text) {
    std::array<int, 26> pocty{};
    for (char c : text) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            pocty[lower - 'a']++;
        }
    }
    return pocty;
}

std::vector<int> vytvorHistogramDlzkySlov(const std::string& text, int maxDlzka) {
    std::vector<int> pocty(maxDlzka, 0);
    int dlzka = 0;
    for (char c : text) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            dlzka++;
        } else {
            if (dlzka > 0) {
                int index = std::min(dlzka, maxDlzka - 1);
                pocty[index]++;
                dlzka = 0;
            }
        }
    }
    if (dlzka > 0) {
        int index = std::min(dlzka, maxDlzka - 1);
        pocty[index]++;
    }
    return pocty;
}

void vypisHistogramPismen(const std::array<int, 26>& pocty) {
    std::cout << "\n--- Histogram pismen (A-Z) ---\n";
    for (size_t i = 0; i < pocty.size(); ++i) {
        if (pocty[i] > 0) {
            std::cout << static_cast<char>('A' + i) << ": " << std::string(pocty[i], '#') << " (" << pocty[i] << ")\n";
        }
    }
}

void vypisHistogramDlzkySlov(const std::vector<int>& pocty) {
    std::cout << "\n--- Histogram dlzky slov ---\n";
    for (size_t i = 1; i < pocty.size(); ++i) {
        if (pocty[i] > 0) {
            if (i == pocty.size() - 1) {
                std::cout << "Dlzka " << i << "+ : " << std::string(pocty[i], '#') << " (" << pocty[i] << ")\n";
            } else {
                std::cout << "Dlzka " << i << "  : " << std::string(pocty[i], '#') << " (" << pocty[i] << ")\n";
            }
        }
    }
}

void ulozTextoveVysledky(const std::string& cesta, int pocetZnakov, int pocetSlov) {
    std::ofstream vystup(cesta);
    if (!vystup.is_open()) {
        std::cout << "Chyba: subor '" << cesta << "' sa nepodarilo otvorit pre zapis.\n";
        return;
    }

    vystup << "--- Vysledky statistickej analyzy (text) ---\n";
    vystup << "Pocet znakov (vratane medzier): " << pocetZnakov << '\n';
    vystup << "Pocet slov:                    " << pocetSlov << '\n';

    std::cout << "Vysledky boli ulozene do suboru '" << cesta << "'.\n";
}

