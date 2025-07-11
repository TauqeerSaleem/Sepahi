#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <regex>

using namespace std;

// Detects control structures (not currently altered, but reserved)
bool isControlStatement(const string& line) {
    return regex_search(line, regex("^\\s*(if|else if|else|for|while)\\b"));
}

// Detects function declarations
bool isFunctionDeclaration(const string& line) {
    return regex_match(line, regex("^\\s*(int|void|float|double|char|auto|long|short|bool)?\\s*\\w+\\s*\\(.*\\)\\s*$"));
}

// Detects class/struct/enum/union
bool isClassLikeDeclaration(const string& line) {
    return regex_match(line, regex("^\\s*(class|struct|union|enum)\\b.*$"));
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
    stack<string> blockTypes;
    indentStack.push(0);
    blockTypes.push("root");

    while (getline(infile, line)) {
        int currentIndent = line.find_first_not_of(" \t");
        if (currentIndent == string::npos) currentIndent = 0;
        string trimmed = regex_replace(line, regex("^\\s+"), "");
        string processed = line;

        // Handle: include <...> → #include <...>
        if (regex_match(trimmed, regex("^include\\s*<.*>$"))) {
            processed = "#" + trimmed;
        }
        // Handle: main → int main()
        else if (regex_match(trimmed, regex("^main\\s*\\(\\s*\\)\\s*$"))) {
            processed = "int main()";
        }


        // Handle indentation decrease
        while (currentIndent < indentStack.top()) {
            indentStack.pop();
            string blockType = blockTypes.top(); blockTypes.pop();
            if (blockType == "class" || blockType == "struct" || blockType == "enum" || blockType == "union") {
                outfile << string(indentStack.top(), ' ') << "};" << endl;
            } else {
                outfile << string(indentStack.top(), ' ') << "}" << endl;
            }
        }

        // Handle indentation increase (open block)
        if (currentIndent > indentStack.top()) {
            outfile << " {" << endl;
            indentStack.push(currentIndent);
            if (isClassLikeDeclaration(trimmed)) {
                blockTypes.push(trimmed.substr(0, trimmed.find(' ')));  // e.g., "class"
            } else {
                blockTypes.push("code");
            }
        }

        // SEMICOLON LOGIC
        string clean = regex_replace(processed, regex("^\\s+|\\s+$"), "");
        bool isPreprocessor = regex_match(clean, regex("^#.*"));
        bool isUsingNamespace = regex_match(clean, regex("^using\\s+namespace.*"));
        bool isIncludeDirective = regex_match(clean, regex("^include\\s+<.*>$"));
        bool isClassOrStruct = isClassLikeDeclaration(clean);
        bool isFunctionDecl = isFunctionDeclaration(clean);
        bool endsWithBlockBrace = regex_match(clean, regex(".*[;{}]\\s*$"));

        if (!isPreprocessor && !isUsingNamespace && !isIncludeDirective &&
            !isFunctionDecl && !isClassOrStruct && !endsWithBlockBrace) {
            processed += ";";
        }

        // Output processed line
        outfile << processed << endl;
    }

    // Close any remaining open blocks
    while (indentStack.size() > 1) {
        indentStack.pop();
        string blockType = blockTypes.top(); blockTypes.pop();
        if (blockType == "class" || blockType == "struct" || blockType == "enum" || blockType == "union") {
            outfile << string(indentStack.top(), ' ') << "};" << endl;
        } else {
            outfile << string(indentStack.top(), ' ') << "}" << endl;
        }
    }

    cout << "Converted output written to " << argv[1] << endl;
    return 0;
}