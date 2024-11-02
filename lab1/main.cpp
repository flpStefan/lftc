#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <AF/AutomatFinit.h>

#include "bst/BST.h"
using namespace std;

#pragma region File Definition
//INPUTS
string input_filename = R"(D:\Facultate\lftc\lftc\lab1\inputs\input.txt)";
string keywords_filename = R"(D:\Facultate\lftc\lftc\lab1\inputs\reserved_keywords.txt)";
string identifiers_filename = R"(D:\Facultate\lftc\lftc\lab1\inputs\af_identifiers.txt)";
string integers_filename = R"(D:\Facultate\lftc\lftc\lab1\inputs\af_integers.txt)";
string float_filename = R"(D:\Facultate\lftc\lftc\lab1\inputs\af_float.txt)";
ifstream input_file(input_filename);
ifstream input_keywords(keywords_filename);

//OUTPUTS
string output_filename = R"(D:\Facultate\lftc\lftc\lab1\outputs\output.txt)";
string output_ts_filename = R"(D:\Facultate\lftc\lftc\lab1\outputs\output_ts.txt)";
string output_fip_filename = R"(D:\Facultate\lftc\lftc\lab1\outputs\output_fip.txt)";
ofstream output_file(output_filename);
ofstream output_ts(output_ts_filename);
ofstream output_fip(output_fip_filename);
#pragma endregion

#pragma region Variable declaration
vector<string> code_lines;
vector<string> parsed_code;
string errors = "";
map<string, int> key_words;
vector<pair<int, int> > fip;
BinarySearchTree ts_constants;
BinarySearchTree ts_identifiers;
AutomatFinit identifiers;
AutomatFinit integer_constants;
AutomatFinit float_constants;
#pragma endregion

/// INPUT | OUTPUT
void read_input_lines() {
    if (!input_file)
        throw runtime_error("Error - Couldn't open the file!");

    string line;
    while (getline(input_file, line))
        code_lines.push_back(line);

    input_file.close();
}

void read_reserved_keywords() {
    if (!input_keywords)
        throw runtime_error("Error - Couldn't open the file!");

    string line;
    while (getline(input_keywords, line)) {
        istringstream stream(line);
        string word;

        bool is_key = true;
        string key;
        int value = -1;
        while (stream >> word) {
            if (is_key) {
                is_key = false;
                key = word;
            } else value = stoi(word);
        }
        key_words[key] = value;
    }

    input_keywords.close();
}

void output() {
    output_file << "List of parsed atoms: \n";
    output_file << "[";
    for (string atom: parsed_code) {
        output_file << "'" << atom << "'" << ", ";
    }

    output_ts << "Constants: \n";
    ts_constants.inorder_output(ts_constants.root, output_ts);
    output_ts << "Variables: \n";
    ts_constants.inorder_output(ts_constants.root, output_ts);

    for (auto element: fip) {
        if (element.second == -1) output_fip << element.first << " : " << "-" << "\n";
        else output_fip << element.first << " : " << element.second << "\n";
    }

    if (!errors.empty()) cout << errors << '\n';

    output_file.close();
    output_ts.close();
    output_fip.close();
}


/// CODE PROCESSING
bool is_alphanum(const char &c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

vector<string> parse_sequence(const string &sequence) {
    vector<string> tokens;

    for (int i = 0; i < sequence.size(); i++) {
        //form the word without any special characters
        string current_word;
        while (i < sequence.size()) {
            if (is_alphanum(sequence[i]) || sequence[i] == '_') {
                current_word += sequence[i];
            } else break;
            i++;
        }

        if (!current_word.empty()) tokens.push_back(current_word);

        //checking specific character structures
        if (sequence[i] == '<') {
            if (i + 1 < sequence.size()) {
                if (sequence[i + 1] == '-') {
                    tokens.push_back("<-");
                    i++;
                } else if (sequence[i + 1] == '=') {
                    tokens.push_back("<=");
                    i++;
                } else tokens.push_back("<");
            } else tokens.push_back("<");
        } else if (sequence[i] == '>') {
            if (i + 1 < sequence.size()) {
                if (sequence[i + 1] == '=') {
                    tokens.push_back(">=");
                    i++;
                } else tokens.push_back(">");
            } else tokens.push_back(">");
        } else if (sequence[i] == '=') {
            if (i + 1 < sequence.size()) {
                if (sequence[i + 1] == '=') {
                    tokens.push_back("==");
                    i++;
                } else tokens.push_back("=");
            } else tokens.push_back("=");
        } else if (sequence[i] == '!') {
            if (i + 1 < sequence.size()) {
                if (sequence[i + 1] == '=') {
                    tokens.push_back("!=");
                    i++;
                } else tokens.push_back("!");
            } else tokens.push_back("!");
        } else if (i < sequence.size() && !is_alphanum(sequence[i])) {
            string character;
            character += sequence[i];
            tokens.push_back(character);
        }
    }

    return tokens;
}

void parse_code() {
    int line_number = 0;
    int opened_brace = 0;
    int opened_paranthesis = 0;
    bool must_close_string = false;
    for (string line: code_lines) {
        line_number++;

        if (!line.empty()) {
            istringstream stream(line);
            string sequence;

            while (stream >> sequence) {
                vector<string> tokens = parse_sequence(sequence);

                for (const string &token: tokens) {
                    //paranthesis check
                    if (token == "(") opened_paranthesis++;
                    if (token == ")") opened_paranthesis--;
                    if (token == "{") opened_brace++;
                    if (token == "}") opened_brace--;

                    if (!token.empty()) {
                        parsed_code.push_back(token);

                        //if token is a reserved word
                        if (key_words.count(token) != 0) {
                            if(token == "\"" && must_close_string) must_close_string = false;

                            fip.push_back(make_pair(key_words[token], -1));
                        }
                        //if token is a constant/identifier
                        else {
                            if (ts_constants.get_index(token) == -1 && ts_identifiers.get_index(token) == -1) {
                                //checking if there is anything before
                                if (fip.size() > 0) {
                                    //if it is a variable it has to be declared
                                    int previous_code = fip[fip.size() - 1].first;
                                    if (previous_code == key_words["string"] || previous_code == key_words["int"] ||
                                        previous_code == key_words["float"]) {
                                        //we check the if the identifier is valid
                                        if (!identifiers.is_valid_sequence(token))
                                            errors += "Invalid identifier: " + token + " at line: " + to_string(
                                                line_number) + "\n";

                                        ts_identifiers.insert(token);
                                        fip.push_back(make_pair(0, ts_identifiers.get_index(token)));
                                    } else {
                                        //it must be a constant
                                        //if it doesn't have " before than it must be an integer or a float
                                        if (fip[fip.size() - 1].first != key_words["\""]) {
                                            if(!integer_constants.is_valid_sequence(token) && !float_constants.is_valid_sequence(token))
                                                errors += "Invalid constant: " + token + " at line: " + to_string(line_number) + "\n";
                                        }
                                        else must_close_string = true;
                                        ts_constants.insert(token);
                                        fip.push_back(make_pair(1, ts_constants.get_index(token)));
                                    }
                                } else {
                                    //othwerwise we throw an error
                                    errors += "Error - couldn't solve for symbol: " + token + " at line " + to_string(
                                        line_number) + "\n";
                                }
                            }
                            else {
                                if(ts_constants.get_index(token) != -1)
                                    fip.push_back(make_pair(1, ts_constants.get_index(token)));
                                else fip.push_back(make_pair(0, ts_identifiers.get_index(token)));
                            }
                        }
                    }

                    if (opened_paranthesis < 0)
                        errors += "Unexpected paranthesis at line " + to_string(line_number) +
                                "\n";
                    if (opened_brace < 0) errors += "Unexpected brace at line " + to_string(line_number) + "\n";
                }
            }

            if(must_close_string) {
                errors = "Error - string wasn't closed at line " + to_string(line_number) + "\n";
                must_close_string = false;
            }
        }
    }

    if(fip.size() > 0) {
        if(fip[0].first == key_words["main"]) errors = "Error at line 0, program should start with 'main'\n" + errors;
    }
}

/// MAIN
int main() {
    //Reading the inputs
    try {
        read_input_lines();
        read_reserved_keywords();
        identifiers.file_read(identifiers_filename);
        integer_constants.file_read(integers_filename);
        float_constants.file_read(float_filename);

        if (!identifiers.is_deterministic() || !integer_constants.is_deterministic() || !float_constants.
            is_deterministic()) {
            cout << "Error - Program cannot start with nedeterministic AFs";
            return 1;
        }
    } catch (const exception &e) {
        cout << e.what();
        output_file.close();
        return 1;
    }

    parse_code();
    output();

    return 0;
}
