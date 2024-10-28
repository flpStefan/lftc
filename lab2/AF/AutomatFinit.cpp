#include "AutomatFinit.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

void AutomatFinit::console_read() {
    cout << "Initial state: ";
    getline(cin, entry);

    cout << "Exit states: ";
    string exit_states;
    getline(cin, exit_states);

    string exit_state;
    istringstream stream(exit_states);
    while (stream >> exit_state)
        this->exit_states.insert(exit_state);

    cout << "Transitions (enter a 0 to exit): ";
    while (true) {
        string transition;
        getline(cin, transition);
        if (transition == "0") return;

        istringstream parser(transition);
        string state1, state2;
        parser >> state1 >> state2;
        states.insert(state1);
        states.insert(state2);

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

vector<string> AutomatFinit::longest_prefix(const string &sequence) {
    istringstream stream(sequence);
    vector<string> input_symbols;
    string input_symbol;
    while (stream >> input_symbol)
        input_symbols.push_back(input_symbol);

    string current_state = this->entry;
    vector<string> prefix;
    for (const auto &symbol: input_symbols) {
        bool longest_prefix = true;

        if (transitions.contains(current_state)) {
            const auto &transition_mapping = transitions[current_state];

            for (const auto &[to_state, transition_symbols]: transition_mapping) {
                for (const auto &transition_symbol: transition_symbols) {
                    if (symbol == transition_symbol) {
                        current_state = to_state;
                        prefix.push_back(symbol);
                        longest_prefix = false;
                    }
                }

                if (!longest_prefix) break;
            }
        }

        if (longest_prefix) break;
    }

    if(exit_states.contains(current_state)) return prefix;
    return {};
}


///Flags
bool AutomatFinit::is_deterministic() {
    for (const auto &[source_state, transition_map]: transitions) {
        map<string, string> used_symbols;

        for (const auto &[to_state, symbols]: transition_map)
            for (const string &symbol: symbols)
                if (used_symbols.contains(symbol)) {
                    if (used_symbols[symbol] != to_state) return false;
                    used_symbols[symbol] = to_state;
                }
    }

    return true;
}

bool AutomatFinit::is_valid_sequence(const string &sequence) {
    istringstream stream(sequence);
    vector<string> symbols;
    string sym;

    while (stream >> sym) {
        if (!alphabet.contains(sym)) return false;
        symbols.push_back(sym);
    }

    string current_state = this->entry;
    for (const string &symbol: symbols) {
        bool is_valid = false;

        if (transitions.contains(current_state)) {
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
