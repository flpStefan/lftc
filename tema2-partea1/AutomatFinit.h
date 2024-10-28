
#ifndef AUTOMATFINIT_H
#define AUTOMATFINIT_H
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class AutomatFinit {
private:
    string stareInitiala;
    set<string> stari;
    set<string> alfabet;
    map<string, map<string,vector<string>>> tranzitii;
    set<string> stariFinale;

public:
    void citesteDeLaTastatura();

    void citesteDinFisier(const string& numeFisier);

    void afiseazaStari() const;

    void afiseazaAlfabet() const;

    void afiseazaTranzitii() const;

    void afiseazaStariFinale() const;

    bool esteDeterminist() const;

    void afiseazaAutomatul() const;

    bool verificaSecventa(const string& secventa) const;

    vector<string> celMaiLungPrefix(const string &secventa) const;
};



#endif //AUTOMATFINIT_H
