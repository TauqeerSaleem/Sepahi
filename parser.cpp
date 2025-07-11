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

// Add ; for stuff like vectors
bool isBracedInitializer(const string& line) {
    return regex_match(line, regex(".*=\\s*\\{.*\\}\\s*$"));
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
        cerr << "Usage: " << argv[0] << " <output_file.cpp> or -" << endl;
        return 1;
    }

    istream& infile = cin;
    ostream* out;
    ofstream outfile;

    if (string(argv[1]) == "-") {
        out = &cout;
    } else {
        outfile.open(argv[1]);
        out = &outfile;
    }

    (*out) << "#include <iostream>\n"
           << "#include \"python_for_loop.cpp\"\n"
           << "using namespace std;\n\n";

    string line;
    stack<int> indentStack;
    stack<string> blockTypes;
    indentStack.push(0);
    blockTypes.push("root");

    while (getline(infile, line)) {
        int currentIndent = line.find_first_not_of(" \t");
        if (currentIndent == string::npos) currentIndent = 0;
        string trimmed = regex_replace(line, regex("^\\s+"), "");

        if (trimmed.empty()) {
            (*out) << endl; // preserve empty line
            continue;
        }

        string processed = line;

        // include <...> → #include <...>
        if (regex_match(trimmed, regex("^include\\s+\\S+$"))) {
            smatch match;
            regex_search(trimmed, match, regex("^include\\s+(\\S+)$"));
            string name = match[1].str();
            
            if (regex_match(name, regex(".*\\.(h|hpp|cpp)$"))) {
                processed = "#include \"" + name + "\"";
            } else {
                processed = "#include <" + name + ">";
            }
        }

        // main() → int main()
        else if (regex_match(trimmed, regex("^main\\s*\\(\\s*\\)\\s*$"))) {
            processed = "int main()";
        }
        // if / while / for / else if (Python-style)
        else if (regex_match(trimmed, regex("^(if|while|for|else if)\\s+.+$"))) {
            smatch match;
            regex_search(trimmed, match, regex("^(if|while|for|else if)\\s+(.+)$"));
            processed = match[1].str() + " (" + match[2].str() + ")";
        }
        // else — leave as-is

        // Handle indentation decrease
        while (currentIndent < indentStack.top()) {
            indentStack.pop();
            string blockType = blockTypes.top(); blockTypes.pop();
            if (blockType == "class" || blockType == "struct" || blockType == "enum" || blockType == "union") {
                (*out) << string(indentStack.top(), ' ') << "};" << endl;
            } else {
                (*out) << string(indentStack.top(), ' ') << "}" << endl;
            }
        }

        // Handle indentation increase (open block)
        if (currentIndent > indentStack.top()) {
            (*out) << "{" << endl;
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
        bool isElseOrElseIf = regex_match(clean, regex("^else(\\s+if.*)?$"));
        bool endsWithBlockBrace = regex_match(clean, regex(".*[;{}]\\s*$"));

        if (!isPreprocessor && !isUsingNamespace && !isIncludeDirective &&
            !isFunctionDecl && !isClassOrStruct && !isElseOrElseIf) {
            if (isBracedInitializer(clean) || !endsWithBlockBrace) {
                processed += ";";
            }
        }

        (*out) << processed << endl;
    }

    // Close any remaining open blocks
    while (indentStack.size() > 1) {
        indentStack.pop();
        string blockType = blockTypes.top(); blockTypes.pop();
        if (blockType == "class" || blockType == "struct" || blockType == "enum" || blockType == "union") {
            (*out) << string(indentStack.top(), ' ') << "};" << endl;
        } else {
            (*out) << string(indentStack.top(), ' ') << "}" << endl;
        }
    }

    if (string(argv[1]) != "-")
        cout << "Converted output written to " << argv[1] << endl;

    return 0;
}