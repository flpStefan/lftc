#include "AutomatFinit.h"

#include <fstream>
#include <iostream>
#include <sstream>

void AutomatFinit::citesteDeLaTastatura() {
    cout << "Introduceti starea initiala: ";
    getline(cin, stareInitiala);

    string stariFinaleInput;
    cout << "Introduceti starile finale separate prin spatiu: ";
    getline(cin, stariFinaleInput);

    istringstream ss(stariFinaleInput);
    string stare;
    while (ss >> stare) {
        stariFinale.insert(stare);
    }

    cout << "Introduceti tranzitiile (tip: stare1 stare2 simbol1 simbol2 ...) urmate de un Enter:\n";

    while (true) {
        string tranzitie;
        getline(cin, tranzitie);

        if (tranzitie.empty()) break;

        istringstream ss(tranzitie);
        string stare1, stare2;
        ss >> stare1 >> stare2;

        stari.insert(stare1);
        stari.insert(stare2);

        vector<string> simboluri;
        string simbol;
        while (ss >> simbol) {
            simboluri.push_back(simbol);
            alfabet.insert(simbol);
        }

        tranzitii[stare1][stare2] = simboluri;
    }
}

void AutomatFinit::citesteDinFisier(const std::string& numeFisier) {
    ifstream fisier(numeFisier);
    if (!fisier) {
        cerr << "Eroare la deschiderea fisierului." << endl;
        return;
    }

    string line;
    getline(fisier, line);
    istringstream ss(line);
    ss >> stareInitiala;

    string stare;
    while (ss >> stare) {
        stariFinale.insert(stare);
    }

    while (getline(fisier, line)) {
        istringstream ss(line);
        string stare1, stare2;
        ss >> stare1 >> stare2;

        stari.insert(stare1);
        stari.insert(stare2);

        vector<string> simboluri;
        string simbol;
        while (ss >> simbol) {
            simboluri.push_back(simbol);
            alfabet.insert(simbol);
        }

        tranzitii[stare1][stare2] = simboluri;
    }
}

void AutomatFinit::afiseazaStari() const {
    cout << "Multimea starilor: ";
    for (const auto& stare : stari) {
        cout << stare << " ";
    }
    cout << std::endl;
}

void AutomatFinit::afiseazaAlfabet() const {
    cout << "Alfabet: ";
    for (const auto& simbol : alfabet) {
        cout << simbol << " ";
    }
    cout << std::endl;
}

void AutomatFinit::afiseazaTranzitii() const {
    cout << "Tranzitii:\n";
    for (const auto& [stare1, mapTranzitii] : tranzitii) {
        for (const auto& [stare2, simboluri] : mapTranzitii) {
            for (const auto& simbol : simboluri) {
                cout << stare1 << " " << simbol << " " << stare2 << "\n";
            }
        }
    }
}

void AutomatFinit::afiseazaStariFinale() const {
    cout << "Stari finale: ";
    for (const auto& stare : stariFinale) {
        cout << stare << " ";
    }
    cout << std::endl;
}

bool AutomatFinit::esteDeterminist() const {
    for (const auto& [stareSursa, tranzitiiStare] : tranzitii) {

        std::map<std::string, std::string> simbolToStareDestinatie;


        for (const auto& [stareDestinatie, simboluri] : tranzitiiStare) {

            for (const std::string& simbol : simboluri) {

                if (simbolToStareDestinatie.find(simbol) != simbolToStareDestinatie.end()) {

                    if (simbolToStareDestinatie[simbol] != stareDestinatie) {
                        std::cout << "Automatul nu este determinist. Starea " << stareSursa
                                  << " are mai multe tranzitii pentru simbolul '" << simbol
                                  << "' catre starile " << simbolToStareDestinatie[simbol]
                                  << " si " << stareDestinatie << ".\n";
                        return false;
                    }
                } else {
                    simbolToStareDestinatie[simbol] = stareDestinatie;
                }
            }
        }
    }


    return true;
}

bool AutomatFinit::verificaSecventa(const string& secventa) const {
    istringstream ss(secventa);  //pentru a crea un vector cu fiecare caracter din secventa data de la tastatura
    vector<string> simboluri;
    string simbol;
    while (ss >> simbol) {
        simboluri.push_back(simbol);
    }

    string stareCurenta = stareInitiala;

    for (const string& simbol : simboluri) {
        bool tranzitieGasita = false;

        if (tranzitii.find(stareCurenta) != tranzitii.end()) { //verific daca exista tranzitii din starea curenta
            const auto& tranzitiiDinStare = tranzitii.at(stareCurenta);

            for (const auto& [stareDestinatie, simboluriTranzitie] : tranzitiiDinStare) {
                for (const auto& s : simboluriTranzitie) {
                    if (s == simbol) {
                        stareCurenta = stareDestinatie;
                        tranzitieGasita = true;
                        break;
                    }
                }
                if (tranzitieGasita) break;
            }
        }

        if (!tranzitieGasita) {
            return false;
        }
    }

    return stariFinale.find(stareCurenta) != stariFinale.end();
}

vector<string> AutomatFinit::celMaiLungPrefix(const string& secventa) const {
    istringstream ss(secventa);
    vector<string> simboluri;
    string simbol;
    while (ss >> simbol) {
        simboluri.push_back(simbol);
    }

    string stareCurenta = stareInitiala;
    vector<string> prefixAcceptat;

    for (const string& simbol : simboluri) {
        bool tranzitieGasita = false;

        if (tranzitii.find(stareCurenta) != tranzitii.end()) {
            const auto& tranzitiiDinStare = tranzitii.at(stareCurenta);

            for (const auto& [stareDestinatie, simboluriTranzitie] : tranzitiiDinStare) {
                for (const auto& s : simboluriTranzitie) {
                    if (s == simbol) {
                        stareCurenta = stareDestinatie;
                        prefixAcceptat.push_back(simbol);
                        tranzitieGasita = true;
                        break;
                    }
                }
                if (tranzitieGasita) break;
            }
        }

        if (!tranzitieGasita) {
            break;
        }
    }

    if (stariFinale.find(stareCurenta) != stariFinale.end()) {
        return prefixAcceptat; // Returnăm prefixul acceptat
    } else {
        return {}; // Dacă starea curentă nu este finală, returnăm un vector gol
    }
}