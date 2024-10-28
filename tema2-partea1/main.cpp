#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <string>
#include "AutomatFinit.h"
using namespace std;
int main() {
    AutomatFinit automat;
    string numeFisier;
    string secventa;
    automat.citesteDinFisier("C:\\Users\\peter\\Desktop\\ALL\\UBB\\an3\\an3-sem1\\LFTC\\tema2-partea1\\input_file.txt");

    int optiune;
    do {
        cout << "\nAlegeti ce doriti sa afisati:\n";
        cout << "1. Multimea starilor\n";
        cout << "2. Alfabetul\n";
        cout << "3. Tranzitiile\n";
        cout << "4. Multimea starilor finale\n";
        cout << "5. Verifica daca o secventa este acceptata de automatul finit\n";
        cout << "6. Determina cel mai lung prefix care este acceptat de automatul finit\n";
        cout << "7. Iesi\n";
        cout << "Optiunea dvs.: ";
        cin >> optiune;
        cin.ignore();

        switch (optiune) {
            case 1:
                automat.afiseazaStari();
            break;
            case 2:
                automat.afiseazaAlfabet();
            break;
            case 3:
                automat.afiseazaTranzitii();
            break;
            case 4:
                automat.afiseazaStariFinale();
            break;
            case 5:
                if (automat.esteDeterminist())
                {
                    cout << "Introduceti secventa (de exemplu: 0 1 2 X A): ";
                    getline(cin, secventa);

                    if (automat.verificaSecventa(secventa)) {
                        cout << "Secventa este acceptata de automat." << endl;
                    } else {
                        cout << "Secventa nu este acceptata de automat." << endl;
                    }
                    }
                else cout<< "Automatul finit nu este determinist, nu putem verifica secventa data!";
            break;
            case 6:
                if (automat.esteDeterminist())
            {
                cout << "Introduceti secventa (de exemplu: 0 1 2 X A): ";
                getline(cin, secventa);

                vector<string> prefix = automat.celMaiLungPrefix(secventa);
                    if (!prefix.empty()) {
                        cout << "Cel mai lung prefix acceptat este: ";
                        for (const string& s : prefix) {
                            cout << s << " ";
                        }
                        cout << endl;
                    } else {
                        cout << "Nu exista un prefix acceptat." << endl;
                    }

            }
                else cout<< "Automatul finit nu este determinist, nu putem verifica secventa data!";
            break;

            case 7:
                cout << "Iesire...\n";
            break;
            default:
                std::cout << "Optiune invalida! Va rugam incercati din nou.\n";
        }
    } while (optiune != 7);

    return 0;
}