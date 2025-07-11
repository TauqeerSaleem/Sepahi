#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <regex>

using namespace std;

// Helper to check if a line starts with a control structure
bool isControlStatement(const string& line) {
    return regex_search(line, regex("^\\s*(if|else if|else|for|while)\\b"));
}

// Add parentheses to control conditions if missing
string normalizeControlStatement(const string& line) {
    smatch match;
    if (regex_match(line, match, regex("^\\s*(if|else if|for|while)\\s+(.+)$"))) {
        return match[1].str() + " (" + match[2].str() + ")";
    }
    return line;
}

// Adds semicolon if appropriate
bool needsSemicolon(const string& line) {
    string trimmed = regex_replace(line, regex("^\\s+|\\s+$"), "");
    if (trimmed.empty() || trimmed.back() == '}' || trimmed.back() == ';')
        return false;
    return !(isControlStatement(trimmed) || trimmed.back() == '{');
}

int main() {
    ifstream infile("input.indpp");  // input file
    ofstream outfile("output.cpp");  // output file
    string line;
    stack<int> indentStack;
    indentStack.push(0);

    while (getline(infile, line)) {
        int currentIndent = line.find_first_not_of(" \t");
        if (currentIndent == string::npos) currentIndent = 0;
        string trimmed = regex_replace(line, regex("^\\s+"), "");

        // Close blocks if indentation decreased
        while (currentIndent < indentStack.top()) {
            indentStack.pop();
            outfile << string(indentStack.top(), ' ') << "}" << endl;
        }

        // Open block if indentation increased
        if (currentIndent > indentStack.top()) {
            outfile << " {" << endl;
            indentStack.push(currentIndent);
        }

        // Process line content
        if (trimmed.empty()) {
            outfile << endl;
            continue;
        }

        string processed = line;

        if (isControlStatement(trimmed)) {
            processed = regex_replace(line, regex("^(\\s*)(if|else if|for|while)\\s+(.+)$"), "$1$2 ($3)");
        } else if (regex_match(trimmed, regex("^main\\s*$"))) {
            processed = regex_replace(line, regex("main"), "int main()");
        }

        if (needsSemicolon(processed)) {
            processed += ";";
        }

        outfile << processed << endl;
    }

    // Close remaining open blocks
    while (indentStack.size() > 1) {
        indentStack.pop();
        outfile << string(indentStack.top(), ' ') << "}" << endl;
    }

    cout << "Converted output written to output.cpp" << endl;
    return 0;
}
