#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

const int MAX_DATA_VELKOST = 1000;
const int MAX_TEXT_VELKOST = 5000;
const int MAX_DLZKA_SLOVA = 30;
const int HISTOGRAM_TRIEDY = 10;

void vycistiBuffer();

int citajDataZoSuboru(std::vector<double>& data, const std::string& cesta);
bool citajTextZoSuboru(std::string& text, const std::string& cesta);

std::vector<double> nacitajCiselneHodnotyZRiadku(int pocetHodnot);
double spocitajPriemer(const std::vector<double>& data);
double spocitajVazenyPriemer(const std::vector<double>& data, const std::vector<double>& vahy);
double spocitajMedian(std::vector<double> data);
std::pair<double, double> spocitajRozptyl(const std::vector<double>& data, double priemer);
std::vector<int> vytvorHistogramCisiel(const std::vector<double>& data, int triedy, double& min, double& max);
void vykresliHistogramSGaussom(const std::vector<int>& bins, double min, double max, double priemer, double smerOdchylka);

void ulozCiselneVysledky(const std::string& cesta, double priemer, double vazenyPriemer, double median, double rozptyl, double smerOdchylka);

int spocitajZnaky(const std::string& text);
int spocitajSlova(const std::string& text);
std::array<int, 26> vytvorHistogramPismen(const std::string& text);
std::vector<int> vytvorHistogramDlzkySlov(const std::string& text, int maxDlzka);
void vypisHistogramPismen(const std::array<int, 26>& pocty);
void vypisHistogramDlzkySlov(const std::vector<int>& pocty);

void ulozTextoveVysledky(const std::string& cesta, int pocetZnakov, int pocetSlov);

