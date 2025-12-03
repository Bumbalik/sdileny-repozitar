#pragma once

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_DATA_VELKOST 1000     
#define MAX_TEXT_VELKOST 5000     
#define MAX_DLZKA_SLOVA 30        
#define HISTOGRAM_TRIEDY 10       

void vycistiBuffer();
double spocitajPriemer(double data[], int pocet);
double spocitajVazenyPriemer(double data[], int pocet);
void utriedPole(double data[], int pocet); 
void spocitajMedian(double data[], int pocet, double* median);
void spocitajRozptyl(double data[], int pocet, double priemer, double* rozptyl, double* smerOdchylka);
void zobrazHistogramCisiel(double data[], int pocet);

void ulozCiselneVysledky(double priemer, double v_priemer, double median, double rozptyl, double smerOdchylka); 
int spocitajZnaky(const char* text);
int spocitajSlova(const char* text);
void zobrazHistogramPismen(const char* text);
void zobrazHistogramDlzkySlov(const char* text);
void ulozTextoveVysledky(int pocetZnakov, int pocetSlov);

int citajDataZoSuboru(double data[], int maxPocet);