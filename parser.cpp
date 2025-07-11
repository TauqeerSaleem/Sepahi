#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <regex>

using namespace std;

bool isControlStatement(const string& line) {
    return regex_search(line, regex("^\\s*(if|else if|else|for|while)\\b"));
}

string normalizeControlStatement(const string& line) {
    smatch match;
    if (regex_match(line, match, regex("^\\s*(if|else if|for|while)\\s+(.+)$"))) {
        return match[1].str() + " (" + match[2].str() + ")";
    }
    return line;
}

bool isFunctionDeclaration(const string& line) {
    return regex_match(line, regex("^\\s*(int|void|float|double|char|auto|long|short|bool)?\\s*\\w+\\s*\\(.*\\)\\s*$"));
}

bool needsSemicolon(const string& line) {
    string trimmed = regex_replace(line, regex("^\\s+|\\s+$"), "");
    if (trimmed.empty() || trimmed.back() == '}' || trimmed.back() == ';')
        return false;
    return !(isControlStatement(trimmed) || trimmed.back() == '{');
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <output_file.cpp>" << endl;
        return 1;
    }

    istream& infile = cin;
    ofstream outfile(argv[1]);
    outfile << "#include <iostream>\nusing namespace std;\n\n";

    string line;
    stack<int> indentStack;
    indentStack.push(0);

    while (getline(infile, line)) {
        int currentIndent = line.find_first_not_of(" \t");
        if (currentIndent == string::npos) currentIndent = 0;
        string trimmed = regex_replace(line, regex("^\\s+"), "");

        while (currentIndent < indentStack.top()) {
            indentStack.pop();
            outfile << string(indentStack.top(), ' ') << "}" << endl;
        }

        if (currentIndent > indentStack.top()) {
            outfile << " {" << endl;
            indentStack.push(currentIndent);
        }

        if (trimmed.empty()) {
            outfile << endl;
            continue;
        }

        string processed = line;

        if (isControlStatement(trimmed)) {
            processed = regex_replace(line, regex("^(\\s*)(if|else if|for|while)\\s+(.+)$"), "$1$2 ($3)");
        } else if (regex_match(trimmed, regex("^main\\s*$"))) {
            processed = regex_replace(trimmed, regex("^main$"), "int main()");
        }

        // Use the latest version (after processing) to decide on semicolon
        string postTrimmed = regex_replace(processed, regex("^\\s+|\\s+$"), "");
        if (!isFunctionDeclaration(postTrimmed) && needsSemicolon(postTrimmed)) {
            processed += ";";
        }


        outfile << processed << endl;
    }

    while (indentStack.size() > 1) {
        indentStack.pop();
        outfile << string(indentStack.top(), ' ') << "}" << endl;
    }

    cout << "Converted output written to " << argv[1] << endl;
    return 0;
}