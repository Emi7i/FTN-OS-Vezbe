/*  ZADATAK
    Koristeći konzolu i nasumično generisane slučajne brojeve
    po normalnoj distribuciji, vizuelizovati Gausovu krivu
    koristeći zvezdice. Gausova kriva po X osi ima moguće
    vrednosti koje su generisane, a po Y osi koliko su vrednosti
    u tom delu X ose zastupljene, procentualno, u svim
    generisanim vrednostima. Za opseg Y uzmite od 0 do
    najveće izmerene vrednosti od svih Y vrednosti,
    dok za X uzmite plus-minus 2.5 standardne devijacije
    od srednje vrednosti. Veličina uzorka slučajnih vrednosti,
    tj. koliko da generišete pre nego probate neka vam je
    8192.

    Gausova kriva za srednju vrednost 5
    i standardnu devijaciju 2 izlgeda otprilike ovako ako se
    posmatra rotirano za 90 stepeni, kao u zadatku pod A:
    0-1: *
    1-2: ****
    2-3: *********
    3-4: ***************
    4-5: ******************
    5-6: *******************
    6-7: ***************
    7-8: ********
    8-9: ****
    9-10: *
    Gausova kriva za srednju vrednost 0 i SD 1, nacrtana
    uspravno izgleda otprilike ovako:
                                        * *
                                     *  ***
                                     * ***** *
                                  *  **********
                                  *  ********** *
                              ***************** *
                            * *******************
                            *********************
                           ************************ **
                          ****************************
                         *******************************
                       * *******************************
                      ********************************** *
                    * ********************************** *
                    ****************************************
                    ******************************************
                 * *******************************************
                ************************************************
                **************************************************
            *******************************************************
            **********************************************************
       * * ************************************************************
   *  ********************************************************************
   **************************************************************************
********************************************************************************
    Odabrati jedan:
    A) (Lakše) Prikazati Gausovu krivu rotiranu za 90 stepeni.
    B) (Teže) Prikazati je normalno
*/
#include <iostream>
#include <random>
#include <functional>
#include <chrono>

#define BROJ_POGADJANJA 8192
#define MAX_VISINA 24
#define MAX_SIRINA 40  // 40 je neka optimalna vrednost koju sam izabrala

using namespace std;

typedef chrono::high_resolution_clock hrc_t;

hrc_t::time_point start = hrc_t::now();

// Postavlja zvezdice u matricu matrix na osnovu broja zvezdica u matixID array-u
void matrixSetStars(string matrix[MAX_VISINA][80], double *matrixID);

// Smanjuje ukupan broj zvezdica tako da sve stanu pri ispisu u terminal
// Radi po formulama : 
// buckets[i] = ceil(buckets[i] * MAX_SIRINA / maxNumBuckets);  
// matrix[i] = ceil(matrix[i] * MAX_VISINA / maxNumMatrix);
void setNumberOfStars(double *buckets, double *matrix);

// Broji u koji bucket je x upalo
// Ako je x upalo u i-ti bucket inkremetuje: buckets[i]++
// Racuna gde ce upasti po formuli: id = (x + 5)/10 * maxValue;
void splitIntoBuckets(double x, double *starNum, int maxValue);

int main(){
  
  default_random_engine generator;
  hrc_t::duration d = hrc_t::now() - start;
  generator.seed(d.count());
  normal_distribution<double> rn(0, 1.5);
  auto postaviTacku = bind(rn, generator);

  double bucketIDs[10] = {};        // = {} da bi sve vrednosti bile 0 na pocetku
  string buckets[10];
  string matrixString[MAX_VISINA][80];     // 24 je visina, 80 sirina
  double matrixStarNum[80] = {};
  

  for(int i = 0; i < BROJ_POGADJANJA; i++){
    splitIntoBuckets(postaviTacku(), bucketIDs, 9);
    splitIntoBuckets(postaviTacku(), matrixStarNum, 79);
  }

  setNumberOfStars(bucketIDs, matrixStarNum);

  for (int id = 0; id < 10; id++){ 
    buckets[id] = string(bucketIDs[id], '*');
    cout << buckets[id] << endl;
  }

  matrixSetStars(matrixString, matrixStarNum); 

  for (int i = 0; i < MAX_VISINA; i++){
    for(int j = 0; j < 80; j++){
      if (matrixString[i][j] == "")
        cout << ' ';
      else
        cout << matrixString[i][j];
    }
    cout << endl;
  }
}


void matrixSetStars(string matrix[MAX_VISINA][80], double *matrixID){
  for (int i = MAX_VISINA - 1; i >= 0; i--)
    for(int j = 0; j < 80; j++)
      if(matrixID[j] >= MAX_VISINA - i) // Ako je broj * > broja visine, stavi zvezdicu
        matrix[i][j] += '*';   
}


void setNumberOfStars(double *buckets, double *matrix){
  //  Znamo da ce najvise zvezdica upasti u jednu od ove 2
  int maxNumBuckets = (buckets[4] > buckets[5]) ? buckets[4] : buckets[5];   
  int maxNumMatrix = (matrix[40] > matrix[41]) ? matrix[40] : matrix[41];

  for(int i = 0; i < 10; i++){
    // Ovo nam daje maximalno MAX_SIRINA (u mom slucaju 50) zvezdica
    buckets[i] = ceil(buckets[i] * MAX_SIRINA / maxNumBuckets);  
    if(buckets[i] < 1 && buckets[i] > 0)
      buckets[i] = 1;
    // Safeguard ako se desi da je neka druga vrednost osim buckets[4] ili buckets[5] najveca.
    if(i > 3 && i < 7)
      if(buckets[i] > MAX_SIRINA)
        buckets[i] = MAX_SIRINA;
  }

  for(int i = 0; i < 80; i++){
    // Ovo nam daje maximalno MAX_VISINA (u mom slucaju 24) zvezdica
    matrix[i] = ceil(matrix[i] * MAX_VISINA / maxNumMatrix);
    if(matrix[i] < 1 && matrix[i] > 0)
      matrix[i] = 1;
    // Safeguard ako se desi da je neka druga vrednost osim matrix[40] ili matrix[41] najveca.
    if(i > 40 && i < 50)
      if(matrix[i] > MAX_VISINA)
        matrix[i] = MAX_VISINA;
  }

  // NOTE: Ne moramo koristiti ove safeguardove ako prodjemo kroz sve elemente i vidimo koji je najveci,
  // ali ja mislim da je brze ovako nego da prolazimo kroz svaki elemenat.
}


void splitIntoBuckets(double x, double *starNum, int maxValue){
  // Radim ovo da ne bih morala da pisem if-else 80 puta
  // Cak iako su vrednosti < -5 i > 5, stavljacemo ih sve u isti bucket pa nam nije toliko vazno ako 
  // vrednosti budu ogranicene na -5 i 5
  if(x < -5)
    x = -5;
  else if (x > 5)
    x = 5;
  // Ovo ce da dodeli id od [0, 79]
  int id = (x + 5)/10 * maxValue;
  starNum[id]++; 
}


/* Raspodela za standardnu vrednost 0, a devijaciju 1.5 izgleda ovako:
----------------------------------------------------
*
****
***************
*******************************
****************************************
******************************
***************
****
*
*
----------------------------------------------------

                                      * *
                                    *****
                                  ****** **
                                  **********
                                  ***********
                            * * **************
                            * **************** *
                            ********************
                          ***********************
                          ***********************
                          ************************
                        **************************
                        **************************
                      *******************************
                      *******************************
                      **********************************
                    *************************************
                    *************************************
                  * ************************************** *
                  *******************************************
              ************************************************ *
            *****************************************************  *
        * **********************************************************
*******************************************************************************
*/