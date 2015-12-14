#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "AVL.h"

using namespace std;

enum Error { USAGE, INVALID_FILE, INVALID_COMMAND };

string add(AVL<string>& tree, string value) {
    stringstream ss;
    tree.insert(value);
    ss << "add " << value;
    return ss.str();
}

string remove(AVL<string>& tree, string value) {
    stringstream ss;
    tree.remove(value);
    ss << "remove " << value;
    return ss.str();
}

string find(AVL<string>& tree, string value) {
    stringstream ss;
    bool found = tree.find(value);
    ss << "find " << value << " ";
	if (found) ss << "true";
	else ss << "false";
    return ss.str();
}

string clear(AVL<string>& tree) {
    tree.clear();
    return "clear";
}

string print(AVL<string>& tree) {
    return "print" + tree.toString();
}

string executeCommand(AVL<string>& tree, string line) {
    stringstream command(line);
    stringstream ss;
    
    string type;
    command >> type;
    if (type == "add") {
        string value;
        command >> value;
        ss << add(tree, value) << endl;
    }
    else if (type == "remove") {
        string value;
        command >> value;
        ss << remove(tree, value) << endl;
    }
    else if (type == "find") {
        string value;
        command >> value;
        ss << find(tree, value) << endl;
    }
    else if (type == "clear") {
        ss << clear(tree) << endl;
    }
    else if (type == "print") {
        ss << print(tree) << endl;
    }
    else throw INVALID_COMMAND;
    
    return ss.str();
}

// void test() {
// 	AVL<int>* tree = new AVL<int>();
//
// 	tree->insert(8);
// 	tree->insert(4);
// 	tree->insert(2);
// 	tree->insert(1);
// 	tree->insert(3);
// 	tree->insert(6);
// 	tree->insert(5);
// 	tree->insert(7);
// 	tree->insert(12);
// 	tree->insert(10);
// 	tree->insert(9);
// 	tree->insert(11);
// 	tree->insert(14);
// 	tree->insert(13);
// 	tree->insert(15);
//
// 	cout << tree->toStringInorder() << endl;
// 	cout << tree->toStringPreorder() << endl;
// 	cout << tree->toString() << endl;
//
// 	tree->remove(8);
// 	tree->remove(5);
// 	tree->remove(7);
// 	tree->remove(6);
// 	tree->remove(9);
// 	tree->remove(10);
// 	tree->remove(11);
// 	tree->remove(12);
// 	tree->remove(13);
// 	tree->remove(4);
// 	tree->remove(14);
// 	tree->remove(2);
// 	tree->remove(3);
// 	tree->remove(15);
// 	tree->remove(1);
//
// 	cout << tree->toStringInorder() << endl;
// 	cout << tree->toStringPreorder() << endl;
// 	cout << tree->toString() << endl;
//
// 	delete tree;
// }

void checkArgs(int argc, char* argv[]) {
	if (argc != 3) throw USAGE;
}

int main(int argc, char* argv[]) {

    try {
        checkArgs(argc, argv);
        
        AVL<string> tree;
        
        ifstream inFile(argv[1]);
        if (!inFile.is_open()) throw INVALID_FILE;
        
        ofstream outFile(argv[2]);
        if (!outFile.is_open()) throw INVALID_FILE;
            
        string line;
        while (getline(inFile, line)) {
            outFile << executeCommand(tree, line);
        }
    }
    catch (Error e) {
        if (e == USAGE) cout << "ERROR - USAGE: ./AVL [command_file] [output_file]" << endl;
		else if (e == INVALID_FILE) cout << "ERROR - INVALID FILE" << endl;
		else if (e == INVALID_COMMAND) cout << "ERROR - INVALID COMMAND" << endl;
    }
	catch (out_of_range e) {
		cout << "ERROR - INDEX OUT OF RANGE" << endl;
	}
	
	return 0;
}