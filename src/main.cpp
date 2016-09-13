/*
 * main.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: pavel
 */

#include <iostream>
#include <fstream>
#include "Promise.h"

using namespace std;
using namespace oldarm;

Promise openFile(const string& filename) {
    return Promise([&filename](Resolve res, Reject err) {
        ifstream file(filename);
        std::string content;
        file >> content;
        res();
    });
}

int main() {
    try {
        openFile("test.txt").then([]() {
            cout << "file content " << endl;
        }, []() {
            cerr << "can't open file " << endl;
        }).then([] {
            cout << "2 " << endl;
        });

        return 0;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}

