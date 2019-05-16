//
//  main.cpp
//  Route Planner
//
//  Created by Kun Lee on 2019-05-14.
//  Copyright © 2019 Kun Lee. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State {kEmpty, kObstacle};

vector<State> parseLine(string line) {
    int n;
    char s;
    vector<State> row;
    istringstream streamLine(line);
    while (streamLine >> n >> s) {
        if (n == 0) row.push_back(State::kEmpty);
        else row.push_back(State::kObstacle);
    }
    return row;
}

vector<vector<State>> readBoardFile(string fileName) {
    ifstream file(fileName);
    vector<vector<State> > board;
    if (file) {
        string line;
        while (getline(file, line)) {
            board.push_back(parseLine(line));
        }
    }
    return board;
}

string cellString(State state) {
    switch (state) {
        case State::kEmpty:
            return "0   ";
        case State::kObstacle:
            return "⛰️   ";
    }
}

void printBoard(const vector<vector<State>> board) {
    for (vector<State> i: board) {
        for (State j: i) {
            cout << cellString(j);
        }
        cout << "\n";
    }
}

int main() {
    vector<vector<State>> Board = readBoardFile("1.board");
    printBoard(Board);
}
