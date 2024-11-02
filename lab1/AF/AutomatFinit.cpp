#include "AutomatFinit.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

///Inputs
void AutomatFinit::file_read(const string &file_name) {
    ifstream input(file_name);
    if (!input)
        throw runtime_error("File could not be opened");

    //read entry
    string line;
    getline(input, line);
    this->entry = line;

    //read exits
    getline(input, line);
    istringstream stream(line);
    string state;
    while (stream >> state) {
        exit_states.insert(state);
    }

    //read transitions
    while (getline(input, line)) {
        istringstream parser(line);

        //read states
        string state1, state2;
        parser >> state1 >> state2;
        states.insert(state1);
        states.insert(state2);

        //read transitions
        vector<string> symbols;
        string symbol;
        while (parser >> symbol) {
            symbols.push_back(symbol);
            alphabet.insert(symbol);
        }
        transitions[state1][state2] = symbols;
    }
}


///Outputs
set<string> AutomatFinit::get_states() {
    return this->states;
}

set<string> AutomatFinit::get_alphabet() {
    return this->alphabet;
}

set<string> AutomatFinit::get_exit_states() {
    return this->exit_states;
}

map<string, map<string, vector<string> > > AutomatFinit::get_transitions() {
    return this->transitions;
}



///Flags
bool AutomatFinit::is_deterministic() {
    for (const auto &[source_state, transition_map]: transitions) {
        map<string, string> used_symbols;

        for (const auto &[to_state, symbols]: transition_map)
            for (const string &symbol: symbols)
                if (used_symbols.count(symbol)) {
                    if (used_symbols[symbol] != to_state) return false;
                    used_symbols[symbol] = to_state;
                }
    }

    return true;
}

bool AutomatFinit::is_valid_sequence(const string &sequence) {
    vector<string> symbols;
    for(const char c : sequence) {
        string sym(1,c);
        symbols.push_back(sym);
    }

    string current_state = this->entry;
    for (const string &symbol: symbols) {
        bool is_valid = false;

        if (transitions.count(current_state)) {
            const auto &transition_map = transitions[current_state];

            for (const auto &[to_state, transition_symbols]: transition_map) {
                for (const auto &transition_symbol: transition_symbols) {
                    if (transition_symbol == symbol) {
                        current_state = to_state;
                        is_valid = true;
                        break;
                    }
                }

                if (is_valid) break;
            }
        }

        if (!is_valid) return false;
    }

    return true;
}
