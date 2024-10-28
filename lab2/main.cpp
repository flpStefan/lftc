#include <iostream>
#include "AF/AutomatFinit.h"
AutomatFinit af;

void menu() {
    cout << "Choose from the options below:\n";
    cout << "1. Get states\n";
    cout << "2. Get the alphabet\n";
    cout << "3. Get transitions\n";
    cout << "4. Get end transitions\n";
    cout << "5. Verify sequence\n";
    cout << "6. Longest prefix\n";
    cout << "0. Exit\n";
}

void print_states() {
    cout << "All States:\n";
    for (string state: af.get_states()) {
        cout << state << " ";
    }
    cout << '\n';
}

void print_alphabet() {
    cout << "Alphabet:\n";
    for (string alphabet: af.get_alphabet()) {
        cout << alphabet << " ";
    }
    cout << '\n';
}

void print_exit_state() {
    cout << "Exit states:\n";
    for (string state: af.get_exit_states()) {
        cout << state << " ";
    }
    cout << '\n';
}

void print_transitions() {
    cout << "Transitions:\n";
    for (auto [from_state, transition_map]: af.get_transitions()) {
        for (auto [to_state, symbols]: transition_map) {
            cout << from_state << " -> " << to_state << ": ";
            for (auto symbol: symbols) {
                cout << symbol << " ";
            }
            cout << '\n';
        }
    }
}

void verify_sequence() {
    if (af.is_deterministic()) {
        cout << "Introduce sequence: ";
        string sequence;
        getline(cin, sequence);
        getline(cin, sequence);

        if (af.is_valid_sequence(sequence)) cout << "Valid sequence\n";
        else cout << "Invalid sequence\n";
    } else cout << "AF not deterministic, cannot compute.\n";
}

void longest_prefix() {
    if (af.is_deterministic()) {
        cout << "Introduce sequence: ";
        string sequence;
        getline(cin, sequence);
        getline(cin, sequence);

        if (af.is_valid_sequence(sequence)) {
            cout << "Longest prefix: ";
            for (const string &symbol: af.longest_prefix(sequence)) cout << symbol;
            cout << '\n';
        } else cout << "Invalid sequence, cannot compute.\n";
    } else cout << "AF not deterministic, cannot compute.\n";
}

int main() {
    af.file_read("D:\\Facultate\\lftc\\lab2\\inputs\\input.txt");

    while (true) {
        menu();
        cout << '\n';
        int option;
        cout << ">>>";
        cin >> option;

        switch (option) {
            case 1:
                print_states();
                break;
            case 2:
                print_alphabet();
                break;
            case 3:
                print_transitions();
                break;
            case 4:
                print_exit_state();
                break;
            case 5:
                verify_sequence();
                break;
            case 6:
                longest_prefix();
                break;

            case 0:
            default:
                cout << "Quitting..";
                return 0;
        }

        cout << '\n';
    }
}
