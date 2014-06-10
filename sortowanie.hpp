#ifndef sortowanie
#define sortowanie

using namespace std;

void grzebieniowe(int tab[], int length, double wspolczynnik){
	int rozpietosc = length;
	bool zamieniono = true;
	while(rozpietosc > 1 || zamieniono){
		zamieniono = false;
		rozpietosc = (int)(rozpietosc/wspolczynnik);
			if(rozpietosc == 0){
				rozpietosc = 1;
			}
		for (int i=0; i+rozpietosc < length; i++){
			if(tab[i] > tab[i + rozpietosc]){
				int temp = tab[i];
				tab[i] = tab[i + rozpietosc];
				tab[i + rozpietosc] = temp;
				zamieniono = true;
			}
		}
	}
}

void babelkowe(int tab[], int length){
	for(int i=0;i<length;i++){
		for(int j=length-1;j>i;j--){
			if(tab[j]<tab[j-1]){
				int temp = tab[j];
				tab[j] = tab[j-1];
				tab[j-1] = temp;
			}
		}
	}
}
#endif
