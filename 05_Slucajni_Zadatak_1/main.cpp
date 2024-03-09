/*
    Ako zamislite jedinični kvadrat centriran oko koordinatnog početka i krug upisan
    u taj kvadrat. Verovatnoća da će nasumično odabrana tačka u okviru kvadrata
    biti i u krugu, pod uslovom da je distribucija ravnomerna, je ravna odnosu njihovih
    površina, tj, ravna je PI/4. Iskoristiti ovaj podataka da kroz Monte Karlo simulaciju
    izračunate vrednost broja PI tako što probate ovaj eksperiment veliki broj puta i vodite
    računa o tome koliko puta dobijete tačku u krugu, a koliko puta van kruga, te na kraju
    simulacije izračunate izmerenu verovatnoću i na osnovu nje, sračunate vrednost broja pi.
*/

#include <iostream>
#include <random>
#include <functional>
#include <chrono>

// Rezultat za 100000000 pogadjanja je oko: 3.14174
// Rezultat za 10000000 pogadjanja je oko: 3.1417
// Rezultat za 1000000 pogadjanja je oko: 3.14342
// Rezultat za 10000 pogadjanja je oko: 3.1456
#define BROJ_POGADJANJA 1000000

using namespace std;

typedef chrono::high_resolution_clock hrc_t;

hrc_t::time_point start = hrc_t::now();

int main(){
    
    default_random_engine generator;
    hrc_t::duration d = hrc_t::now() - start;
    generator.seed(d.count());

    double x, y;
    int krugCounter = 0, kvadratCounter = 0;

    uniform_real_distribution<double> rdist(0, 1);
    auto postaviTacku = bind(rdist, generator);

    for (int i = 0; i < BROJ_POGADJANJA; i++)  {
        x = postaviTacku();
        y = postaviTacku();

        // Ovde sam racunala i sa ivicom kruga
        // x^2  + y^2 <= 1 -- Da li tacka upada u krug
        if(x*x + y*y <= 1) 
            krugCounter++;
    }
    
    cout << "Broj tacaka koji je upao u krug: " << krugCounter << endl;
    cout << "Broj tacaka ukupno: " << BROJ_POGADJANJA << endl;

    // Formula za pi = 4 * krugCounter / kvadratCounter     tj    pi = 4 * r / n
    cout << "Izracunata vrednost: " << (double) 4 * krugCounter/BROJ_POGADJANJA << endl;
    return 0;
}
