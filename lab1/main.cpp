#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "bst/BST.h"
using namespace std;

#pragma region File Definition
//INPUTS
ifstream input_file("inputs/input.txt");
ifstream input_keywords("inputs/reserved_keywords");

//OUTPUTS
ofstream output_file("outputs/output.txt");
ofstream output_ts("outputs/output_ts.txt");
ofstream output_fip("outputs/output_fip.txt");
#pragma endregion

#pragma region Variable declaration
vector<string> code_lines;
vector<string> parsed_code;
string errors = "";
map<string, int> key_words;
vector<pair<int,int>> fip;
BinarySearchTree ts;
#pragma endregion

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

    ts.inorder_output(ts.root, output_ts);

    for(auto element : fip) {
        if(element.second == -1) output_fip << element.first << " : " << "-" << "\n";
        else output_fip << element.first << " : " << element.second << "\n";
    }

    if(!errors.empty()) cout<<errors << '\n';

    output_file.close();
    output_ts.close();
}

void parse_code_file() {
    for (string line: code_lines) {
        if (!line.empty()) {
            istringstream stream(line);
            string atom;

            while (stream >> atom) {
                bool is_semicolon = false;
                bool is_paranthesis = false;

                if (atom[atom.size() - 1] == ',') atom.pop_back();
                else if (atom[atom.size() - 1] == ';') {
                    atom.pop_back();
                    is_semicolon = true;
                } else if (atom[atom.size() - 1] == ')' && atom.substr(0, 4) != "main" && atom.substr(0, 4) != "read" &&
                           atom.substr(0, 5) != "write") {
                    atom.pop_back();
                    is_paranthesis = true;
                } else if (atom[0] == '(') {
                    parsed_code.push_back("(");
                    atom.erase(0, 1);
                }

                if (!atom.empty())
                    parsed_code.push_back(atom);
                if (is_semicolon) parsed_code.push_back(";");
                if (is_paranthesis) parsed_code.push_back(")");
            }
        }
    }
}

void create_ts() {
    for (int i = 0; i < parsed_code.size(); i++) {
        if (parsed_code[i] == "int" || parsed_code[i] == "float" || parsed_code[i] == "string" || parsed_code[i] == "<-") {
            if (i + 1 < parsed_code.size()) {
                i++;
                if (key_words.count(parsed_code[i]) == 0 && ts.get_index(parsed_code[i]) == -1)
                    ts.insert(parsed_code[i]);
                else errors += "Error - '" + parsed_code[i] + "' is not a valid variable/const\n";
            }
            else break;
        }
    }
}

void create_fip() {
    if(parsed_code[0] != "main()") errors += "Error - Program does not start with 'main()'\n";
    if(parsed_code[1] != "{") errors += "Error - Missing '{' after 'main()'\n";
    if(parsed_code[parsed_code.size() - 1] != "}") errors += "Error - Missing '}' at the end of the file\n";

    for(int i = 0; i < parsed_code.size(); i++) {
        if(parsed_code[i] != "{" && parsed_code[i] != "}" && parsed_code[i] != "{" && parsed_code[i] != ")") {
            if(parsed_code[i] == "if") {
                fip.push_back(make_pair(key_words[parsed_code[i]], -1));
                if(parsed_code[i + 1] != "(") errors += "Error - Missing '(' after 'if'\n";
                else i++;
            }
            if(parsed_code[i] == "while") {
                fip.push_back(make_pair(key_words[parsed_code[i]], -1));
                if(parsed_code[i + 1] != "(") errors += "Error - Missing '(' after 'while'\n";
                else i++;
            }
            if(ts.get_index(parsed_code[i]) != -1) {
                if(parsed_code[i - 1] == "<-") fip.push_back(make_pair(key_words["CONST"], ts.get_index(parsed_code[i])));
                else fip.push_back(make_pair(key_words["ID"], ts.get_index(parsed_code[i])));
            }
            else fip.push_back(make_pair(key_words[parsed_code[i]], -1));
        }
    }
}

int main() {
    //Cod BB
    try {
        read_input_lines();
        read_reserved_keywords();
    } catch (const exception &e) {
        cout << e.what();
        output_file.close();
        return 1;
    }

    parse_code_file();
    create_ts();
    create_fip();

    output();
    return 0;
}
