#include "sortowanie.hpp"
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#ifndef testowanie
#define testowanie
using namespace std;
//funkcja losujaca liczbe z przedzialu 0 do prawy-1
int losujLiczbe(int prawy){
	return rand() % prawy;
}
//funkcja wypelniajaca tablice losowymi liczbami
void wypelnijTablice(int tab[], int length){
	for (int i = 0; i < length; ++i)
	{
		tab[i] = losujLiczbe(length);
	}
}
//funkcja zerujaca tablice
void zerujTablice(double tab[], int length){
	for(int i=0;i<length;i++){
		tab[i] = 0;
	}
}
/*
	funkcja sprawdzajca czas sortowania babelkowego dla podanej tablicy.
	Sortowanie odbywa sie na kopii przekazanej tablicy. Funkcja zwraca czas sortowania.
*/
long testujBabelkowe(int tab[], int length){
	clock_t start, koniec;
	int copyBabelkowe[length];
	memcpy(copyBabelkowe, tab, length*sizeof(int));
	start = clock();
	babelkowe(copyBabelkowe,length);
	koniec = clock();
	return (long)koniec-start;
}
/*
	funkcja sprawdzajaca czas sortowania grzebieniowego dla podanej tablicy.
	Sortowanie odbywa sie na kopii przekazanej tablicy. Dodatkowo funkcja przyjmuje
	jako parametr wspolczynnik, ktory okresla przez jaka wartosc bedzie dzielona rozpietosc i ktory
	jest przekazywany do funkcji sortujacej. Zwraca czas sortowania. 
*/

long testujGrzebieniowe(int tab[], int length, double wspolczynnik){
	clock_t start, koniec;
	int copyGrzebieniowe[length];
	memcpy(copyGrzebieniowe, tab, length*sizeof(int));
	start = clock();
	grzebieniowe(copyGrzebieniowe,length, wspolczynnik);
	koniec = clock();
	return long(koniec - start);
}

//funkcja zwracajaca czas potrzebny na wykonanie okreslonej w ostatnim parametrze liczby kopiowan tablicy.
long memcpyTime(int tab[], int length, int number){
	clock_t start, koniec;
	int copyTab[length];
	start = clock();
	for(int i=0;i<number;i++){
		memcpy(copyTab, tab, length*sizeof(int));
	}
	koniec = clock();
	return long(koniec - start);
}
/*
	Funkcja szukajaca optymalnej wartosci wspolczynnika, przez ktory powinna byc dzielona rozpietosc przy sortowaniu grzebieniowym.
	Funkcja nie modyfikuje oryginalnej tablicy. Jako argumenty przyjmuje tablice, jej dlugosc, oraz numer przedzialu, ktory ma byc przeszukiwany.
  	Sprawdzane sa czasy sortowania grzebieniowego dla wszystkich wartosci wspolczynnika w podanym przedziale, z uwzglednieniem
	kroku jakim jest rozdzielczosc i zapamietywany jest najlepszy uzsykany dotychczas czas i wartosc wspolczynnika, dla ktorej ten czas zostal
	uzyskany. Na koncu czas ten jest zwracany.
*/
double szukajOptymalnegoWspolczynnikaWPrzedziale(int tab[], int length,  int przedzial){
    //plik, w ktorym bedziemy zapisywac wyniki
	fstream plik;
	//rozdzielczosc, o ktora bedziemy zwiekszac wspolczynnik w kazdym obrocie petli
	double rozdzielczosc;
	//lewy kraniec sprawdzanego przedzialu
	double leftMost; 
	//prawy kraniec sprawdzanego przedzialu
	double rightMost;
    //ustawiamy zmienne w zaleznosci od przeszukiwanego przedzialu
	switch(przedzial){
		case 1: 
			plik.open("przedzial1.txt", ios::out | ios::app);
			rozdzielczosc = 0.01;
			leftMost = 1.01;
			rightMost = 2.0;
			break;
		case 2: 
			plik.open("przedzial2.txt", ios::out | ios::app);
			rozdzielczosc = 0.02;
			leftMost = 2.01;
			rightMost = 3.0;
			break;	
		case 3: 
			plik.open("przedzial3.txt", ios::out | ios::app);
			rozdzielczosc = 0.05;
			leftMost = 3.01;
			rightMost = 5.0;
			break;
		case 4: 
			plik.open("przedzial4.txt", ios::out | ios::app);
			rozdzielczosc = 0.1;
			leftMost = 5.01;
			rightMost = 10.0;
			break;	
	}
	//lewa krancowa wartosc przedzialu jest wartoscia poczatkowa wspolczynnika
	double wspolczynnik = leftMost;
	//w tej zmiennej bedziemy przechowywac czas sortowania dla danego wspolczynnika
	double czas = 0;
	//w zmiennej najlepszy czas bedziemy przechowywac najlepszy uzyskany dotychczas czas
	double najlepszyCzas = 1000000;
	//w zmiennej najlepszy wspolczynnik bedziemy zapamietywac wartosc wspolczynnika, dla ktorej osiagnieto najlepszy dotychczas czas.
	double najlepszyWspolczynnik = wspolczynnik;
	/*
		wspolczynnik zerowego czasu jest uzywany w momencie kiedy sortowanie ukonczylo sie zbyt szybko i w efekcie zwrocony zostal zerowy czas.
		W takim wypadku program wchodzi do petli, ktora wykonuje tyle sortowan ile okreslone jest w zmiennej wspolczynnikaDlaZerowegoCzasu i mierzy
		laczny czas wykonanych sortowan. Jezeli nadal czas jest zerowy to wspolczynnik ten jest podwajany tak dlugo az osiagniety zostanie niezerowy czas. 
		Na koniec uzyskany czas dzielony jest przez ilosc wykonanych obrotow petli dla sprawdzenia, ile czasu zajelo jedno sortowanie.
	*/
	int wspolczynnikDlaZerowegoCzasu;
	//w petli bedziemy szukac najlepszej rozpietosci dla danego przedzialu
	if(plik.good()){
		plik << "a) wspolczynnik b) rozdzielczosc c) czas wykonania" << endl;
		while(wspolczynnik <= rightMost){
			wspolczynnikDlaZerowegoCzasu = 10;
			//mierzymy czas sortowania dla danego wspolczynnika
			czas = testujGrzebieniowe(tab,length,wspolczynnik);
			//jezeli czas jest rowny zero to znaczy, ze tablica zostala posortowana zbyt szybko
			//dlatego zmierzymy czas dla wiekszej liczby sortowan dla danego wspolczynnika, odejmujemy od tego czasu czas potrzebny na wykonanie
			//i-krotnej liczby kopiowan danej tablicy, ktore to pojedyncze kopiowanie jest wykonane dla pojedynczego wywolania funkcji sortujacej.
			//Na koncu uzyskany czas dzielimy przez liczbe sortowan, aby uzyskac czas pojedynczego sortowania.
			while(czas == 0){
				clock_t startTmp, koniecTmp;
				startTmp = clock();
				for(int i=0;i<wspolczynnikDlaZerowegoCzasu;i++){
					testujGrzebieniowe(tab,length,wspolczynnik);
				}
				koniecTmp = clock();
				czas = (koniecTmp-startTmp-memcpyTime(tab,length,wspolczynnikDlaZerowegoCzasu))/(double)wspolczynnikDlaZerowegoCzasu;
				wspolczynnikDlaZerowegoCzasu *=2;
			}
			plik << wspolczynnik << "  " << rozdzielczosc << "  " << czas << endl;
			//jesli uzyskany czas jest lepszy od dotychczas najlepszego to go zapisujemy wraz ze wspolczynnikiem
			if(czas < najlepszyCzas){
				najlepszyCzas = czas;
				najlepszyWspolczynnik = wspolczynnik;
				wspolczynnik += rozdzielczosc;
			}
			//jesli nie to tylko zwiekszamy wspolczynnik o zadana rozdzielczosc
			else{
				wspolczynnik += rozdzielczosc;
			}
		}
	}
	plik.close();
	return najlepszyWspolczynnik;
}
/*
	Funkcja obliczajca srednia wartosc uzyskanego optymalnego wspolczynnika dla podanej w przekazanym do funkcji parametrze
	ilosci sortowan. Dla kazdego nowego sortowania tablica wypelniana jest nowymi wartosciami. Zwracana jest srednia z optymalnych
	uzyskanych wspolczyniikow.
*/
double wybierzSredniOptymalnyWspolczynnikZPrzedzialow(int tab[], int length, int iloscSortowan){
    fstream plik("optymalneWspolczynniki.txt", ios::out);
	//zmienna okresla ilosc przedzialow, dla ktorej bedziemy szukac optymalnego wspolczynnika i
	//tworzy odpowiednio duza tablice dla przechowywania wynikow daneych przedzialow
	int iloscPrzedzialow = 4;
	//suma uzyskanych optymalnych wspolczynnikow dla danego przedzialu bedzie przechowywana w tej tablicy
	double sumaPrzedzialu[iloscPrzedzialow];
	//w tej tablicy bedzie przechowywana srednia wartosc optymalnego wspolczynnika dla danego przedzialu
	double sredniaPrzedialu[iloscPrzedzialow];
	//zerujemy obie tablice
	zerujTablice(sumaPrzedzialu,iloscPrzedzialow);
	zerujTablice(sredniaPrzedialu,iloscPrzedzialow);
	//do tej zmiennej zapiszemy srednia optymalny wspolczynnik
	double sredniOptymalnyWspolczynnik = 10;
	int i;
	//zapisujemy optymalne wspolczynniki dla danego przedzialu, z kazdej iteracji i sumujemy
	if(plik.good()){
	  	plik << "a) numer iteracji b) najlepsza znaleziona wartosc wspolczynnika c) numer przedzialu" << endl;
	  	double wspolczynnik;
	  	for (i = 0; i <= iloscSortowan-1; i++){
	   		wypelnijTablice(tab,length);
	   		wspolczynnik = szukajOptymalnegoWspolczynnikaWPrzedziale(tab,length,1);
		  	sumaPrzedzialu[0] += wspolczynnik;
		  	plik << i+1 << " " << wspolczynnik << " 1" << endl;
		  	wspolczynnik = szukajOptymalnegoWspolczynnikaWPrzedziale(tab,length,2);
		  	sumaPrzedzialu[1] += wspolczynnik;
		 	plik << i+1 << " " << wspolczynnik << " 2" << endl;
		  	wspolczynnik = szukajOptymalnegoWspolczynnikaWPrzedziale(tab,length,3);
		  	sumaPrzedzialu[2] += wspolczynnik;
		  	plik << i+1 << " " << wspolczynnik << " 3" << endl;
		  	wspolczynnik = szukajOptymalnegoWspolczynnikaWPrzedziale(tab,length,4);
		  	sumaPrzedzialu[3] += wspolczynnik;
		  	plik << i+1 << " " << wspolczynnik << " 4" << endl;
	 	}
  	}	
  	plik.close();
	//dzielimy sume optymalnych rozpietosci dla przedzialu przez ilosc wykonanych dodawan
	//aby uzyskac wartosc srednia
	for (int j = 0; j < iloscPrzedzialow;j++){
		sredniaPrzedialu[j] = (double)sumaPrzedzialu[j]/i;
		cout << "Optymalna wartosc wspolczynnika dla przedzialu " << j + 1 << " : " << sredniaPrzedialu[j] << endl; 
	}
	cout << endl;
	//wybieramy najbardziej optymalny wspolczynnik ze wszystkich przedzialow i zwracamy go
	for (int k = 0; k < iloscPrzedzialow; k++){
		if(sredniaPrzedialu[k] < sredniOptymalnyWspolczynnik){
			sredniOptymalnyWspolczynnik = sredniaPrzedialu[k];
		}
	}
	return sredniOptymalnyWspolczynnik;
	
}
/*
	Funkcja porownujaca sortowanie czas dzialania sortowania babelkowego i grzebieniowego.
	Oba sortowanie sortuja tablice o takich samych wartosciach. Kazda funkcja testujaca czas sortowania 
	wykonuje kopie przekazanej tablicy i na niej mierzy czas dzialania. Jako trzeci argument do funkcji 
	przekazywana jest wartosc wspolczynnika dla sortowanie grzebieniowego, przez ktory dzielona ma byc rozpietosc.
	Czwarty argument okresla maksymalna rozmiar tablicy jaka ma byc sortowana. Dla kazdego sortowania losowana
	jest nowa tablica. Wyniki zapisywane sa do pliku.
*/
void porownajSortowania(int tab[], int length,double wspolczynnik, int maksWielkoscTablicy){
	fstream plik("sortowania.txt", ios::out);
	double babelkowe, grzebieniowe;
	if(plik.good()){
		cout << "a) rozmiar tablicy: czas sortowania: b) babelkowego: c) grzebieniowego:" << endl;
		plik << "a) rozmiar tablicy: czas sortowania: b) babelkowego: c) grzebieniowego:" << endl;
		for(int i=500;i<maksWielkoscTablicy;i+=100){
			wypelnijTablice(tab,i);
			babelkowe = testujBabelkowe(tab,i);
			grzebieniowe = testujGrzebieniowe(tab,i,wspolczynnik);
			cout <<i<<"  " << (long)babelkowe << "  " << (long)grzebieniowe << endl;
			plik <<i<<"  " << (long)babelkowe << "  " << (long)grzebieniowe << endl;
		}
	}
	plik.close();
}
#endif
