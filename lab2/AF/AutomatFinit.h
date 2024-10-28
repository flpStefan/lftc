#ifndef AUTOMATFINIT_H
#define AUTOMATFINIT_H
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

class AutomatFinit {
    string entry;
    set<string> exit_states;
    set<string> states;
    map<string, map<string, vector<string> > > transitions;
    set<string> alphabet;

public:
    AutomatFinit() = default;

    //Inputs
    void file_read(const string &file_name);

    void console_read();

    //Outputs
    set<string> get_states();

    set<string> get_alphabet();

    set<string> get_exit_states();

    map<string, map<string, vector<string> > > get_transitions();

    vector<string> longest_prefix(const string& sequence);

    bool is_valid_sequence(const string& sequence);

    bool is_deterministic();
};


#endif
