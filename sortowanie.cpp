#include <iostream>
#include <cstdlib>
#include <cstring>
#include "sortowanie.hpp"
#include "testowanie.hpp"

using namespace std;

int main(){
	srand(time(NULL));
	//na poczatku usuwamy pliki utworzone w poprzednim wywolaniu programu, jesli sa.
	//dane w trakcie dzialania programu sa do pliku doklejane, kazde nowe wywolanie programu
	//powinno dzialac na nowym pliku, pustym pliku
	remove("przedzial1.txt");
	remove("przedzial2.txt");
	remove("przedzial3.txt");
	remove("przedzial4.txt");
	//wielkosc tablicy, dla ktorej bedziemy szukac optymalnego wspolczynnika dla sortowania grzebieniowego.
	//Jest ona stosunkowo niska ze wzgledu na ilosc sortowan, ktore zostana wykonane
	//w celu znalezienia optymalanego wspolczynnika
	int tabWspolczynnikWielkosc = 1000;
	//ta zmienna przechowuje maksymalna wielkosc tablicy, dla ktorej bedziemy porownywac sortowania grzebieniowe i babelkowe.
	//jest ona wieksza, aby nie uzyskiwac zerowych wynikow czasowych
	int tabSortowanieWielkosc = 15000;
	//tablica uzywana do poszukiwania optymalnego wspolczynnika dla sortowania grzebieniowego
	int tabWspolczynnik[tabWspolczynnikWielkosc];
	//tablica uzywana do wykonania porownac obu sortowan
	int tabSortowanie[tabSortowanieWielkosc];
	//wypelniamy obie tablicy danymi
	wypelnijTablice(tabWspolczynnik,tabWspolczynnikWielkosc);
	wypelnijTablice(tabSortowanie,tabSortowanieWielkosc);
	cout << "Szukanie optymalnego wspolczynnika dla sortowania grzebieniowego(moze troche potrwac)... " << endl << endl;
	//szukamy optymalna wartosc wspolczynnika
	double wspolczynnik = wybierzSredniOptymalnyWspolczynnikZPrzedzialow(tabWspolczynnik,tabWspolczynnikWielkosc,100);
	cout << "Wybrany optymalny wspolczynnik: " << wspolczynnik << endl << endl;
	cout << "Porownywanie czasow dzialania algorytmow sortowania babelkowego i grzebieniowego..." << endl;
	//wykonujemy porownanie sortowan z wykorzystaniem znalezionego wspolczynnika. W ostatnim argumencie funkcji okreslony
	//jest maksymalny rozmiar tablicy, dla ktorej wykonane maja zostac porownania.
	porownajSortowania(tabSortowanie,tabSortowanieWielkosc,wspolczynnik,15000);
}
