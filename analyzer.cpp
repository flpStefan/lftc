#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
using namespace std;

#pragma region File Definition
ifstream input_file("");
ofstream output_file("");
#pragma endregion

#pragma region Variable declaration
vector<string> lines;
vector<string> result;
map<string, int> appearances;
#pragma endregion

void read_lines() {
	if (!input_file)
		throw runtime_error("Error - Couldn't open the file!");

	string line;
	while (getline(input_file, line))
		lines.push_back(line);

	input_file.close();
}

void output() {
	output_file << "[";
	for (string atom : result) {
		output_file << "'" << atom << "'" << ", ";
	}
	output_file << "]";

	output_file.close();
}

void parse_file() {
	for (string line : lines) {
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
				}
				else if (atom[atom.size() - 1] == ')' && atom.substr(0, 4) != "main") {
					atom.pop_back();
					is_paranthesis = true;
				}
				else if (atom[0] == '(') {
					result.push_back("(");
					atom.erase(0, 1);
				}

				result.push_back(atom);
				if (is_semicolon) result.push_back(";");
				if (is_paranthesis) result.push_back(")");
			}
		}
	}
}

int main() { //Cod BB
	try {
		read_lines();
	}
	catch (const exception& e) {
		cout << e.what();
		output_file.close();
		return 1;
	}

	parse_file();
	output();
	return 0;
}