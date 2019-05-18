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
#include <algorithm>

using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State {kEmpty, kObstacle};
const int direction[4][2] {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

//compare f-value of two nodes
bool compare(const vector<int> node1, const vector<int> node2) {
    int f1 = node1[2] + node1[3]; //f = g + h
    int f2 = node2[2] + node2[3];
    return f1 > f2;
}

void cellSort(vector<vector<int>> *openNodes) {
    sort(openNodes->begin(), openNodes->end(), compare); //uses <algorithm> library
}

//a function of the current coordinates and the goal, h = (x, y)
int heuristic(int x, int y, int x2, int y2) {
    return abs(x - x2) + abs(y - y2);
}

bool checkValidCell(int x, int y, vector<vector<State>> &board) {
    bool boardX = (x >= 0 && x < board.size());
    bool boardY = (y >= 0 && y < board[x].size());
    if (boardX && boardY) return board[x][y] == State::kEmpty;
    return false;
}

//add a new node to the list of open nodes
void addToOpen(int x, int y, int g, int h, vector<vector<int>> &openNodes, vector<vector<State>> &board) {
    openNodes.push_back(vector<int> {x, y, g, h});
    board[x][y] = State::kClosed;
}

void expandNeighbors(const vector<int> &currentNode, int goal[2], vector<vector<int>> &openNodes, vector<vector<State>> &board) {
    int x = currentNode[0];
    int y = currentNode[1];
    int g = currentNode[2];
    
    for (auto i: direction) {
        int x2 = x + i[0];
        int y2 = y + i[1];
        if (checkValidCell(x2, y2, board)) {
            int h = heuristic(x2, y2, goal[0], goal[1]);
            addToOpen(x2, y2, g + 1, h, openNodes, board);
        }
    }
}

vector<vector<State>> search(vector<vector<State>> board, int init[2], int goal[2]) {
    vector<vector<int>> openNodes {}; //initialize an empty list of open nodes
    
    //initialize a starting node with the following:
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = heuristic(x, y, goal[0], goal[1]);
    
    addToOpen(x, y, g, h, openNodes, board); //add the new node to the list of open nodes
    
    while(!openNodes.empty()) {
        cellSort(&openNodes); //sort the open nodes by f-value and pop the optimal node
        vector<int> currentNode = openNodes.back();
        openNodes.pop_back();
        board[currentNode[0]][currentNode[1]] = State::kPath;
        if (currentNode[0] == goal[0] && currentNode[1] == goal[1]) {
            board[init[0]][init[1]] = State::kStart;
            board[goal[0]][goal[1]] = State::kStop;
            return board;
        }
        expandNeighbors(currentNode, goal, openNodes, board);
    }
    
    cout << "No path has been found\n";
    return vector<vector<State>> {};
}

//convert a string line from board to accessible states
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

//read the board from given file
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

//convert board states to user friendly items
string cellString(State state) {
    switch (state) {
        case State::kObstacle:
            return "Δ   ";
        case State::kPath:
            return "*   ";
        case State::kStart:
            return "S   ";
        case State::kStop:
            return "S   ";
        default: return "0   ";
    }
}

//print the board
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
    int init[2] = {2, 2};
    int goal[2] = {4, 5};
    vector<vector<State>> solution = search(board, init, goal);
    printBoard(solution);
}
