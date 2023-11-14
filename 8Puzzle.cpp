#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>

using namespace std;

struct PuzzleState {
    int board[3][3];        // board state
    int g;                  // cost from initial board state
    int h;                  // heuristic cost
    int f;                  // total cost
    int x, y;               // empty tile location
    PuzzleState* parent;    // parent puzzle state
};

// struct with overloading operator for prioritisng lower f cost
struct CompareCost {
    bool operator()(PuzzleState const& ps1, PuzzleState const& ps2) {
        return ps1.f > ps2.f; // priority queue compares incorrectly if its ps1.f < ps2.f
    }
};

// goal state
int goal[3][3] = {{1, 2, 3}, 
                  {8, 0, 4}, 
                  {7, 6, 5}};

// calculate heuristic for number of tiles in the wrong position excluding empty tile
int calcHeuristic(int board[3][3]) {
    int heuristic = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 0 && board[i][j] != goal[i][j]) {
                heuristic+=1;
            }
        }
    }
    return heuristic;
}

// check if board matches goal board
bool isGoal(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != goal[i][j])
                return false;
        }
    }
    return true;
}

// generate the successors (next puzzle state)
vector<PuzzleState> generateSuccessors(PuzzleState currentPuzzle) {
    vector<PuzzleState> successors;
    // get empty tile 
    int x = currentPuzzle.x;
    int y = currentPuzzle.y;

    // initialise possible moves for tile (top,bottom,left,right)
    int moveX[] = {-1, 1, 0, 0};
    int moveY[] = {0, 0, -1, 1};

    // generate successors / possible moves 
    for (int i = 0; i < 4; i++) {
        // move empty tile
        int newMoveX = x + moveX[i];
        int newMoveY = y + moveY[i];

        // check move is within bounds
        if (newMoveX >= 0 && newMoveX < 3 && newMoveY >= 0 && newMoveY < 3) {
            PuzzleState successor = currentPuzzle; // make successor
            swap(successor.board[x][y], successor.board[newMoveX][newMoveY]); // swap empty tile with the move
            successor.x = newMoveX;
            successor.y = newMoveY;
            successor.parent = new PuzzleState(currentPuzzle); // set parent as currentPuzzle
            successor.g = successor.parent->g + 1; // parent distance + distance between parent and successor
            successor.h = calcHeuristic(successor.board);
            successor.f = successor.g + successor.h; // compute total f cost
            
            successors.push_back(successor); // add successor
        }
    }
    return successors;
}

// Store the puzzle board as a string for tracking visits
string stringPuzzleBoard(PuzzleState currentPuzzle) {
    string stringBoard = "";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) 
            stringBoard += to_string(currentPuzzle.board[i][j]); // convert to int to string and append it
    }
    return stringBoard;
}

// print board
void printBoard(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// print solution path
void printSolutionPath(PuzzleState currentPuzzle) {
    cout << "Printing Solution Path" << endl;
    // add solution path of the puzzle through parent puzzle nodes
    vector<PuzzleState> solutionPath;
    while (currentPuzzle.parent != nullptr) { // add parent puzzle nodes to a vector until initial node reached
        solutionPath.push_back(currentPuzzle);
        currentPuzzle = *currentPuzzle.parent; 
    }
    solutionPath.push_back(currentPuzzle);

    // print solution path from initial to goal
    for (int i = solutionPath.size() - 1; i >= 0; i--) {
        if (i == solutionPath.size() - 1) 
            cout << "Initial State:" << endl;
        else 
            cout << "Move " << solutionPath.size() - i - 1 << ":" << endl;
        printBoard(solutionPath[i].board);
        cout << endl;
    }
}

// A* Search Algorithm 
void aStarSearch(int board[3][3]) {
    // Initialise Puzzle State
    PuzzleState initialPuzzle;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            initialPuzzle.board[i][j] = board[i][j];
        }
    }
    initialPuzzle.g = 0;
    initialPuzzle.h = calcHeuristic(initialPuzzle.board);
    initialPuzzle.f = 0; // leave total cost as 0
    initialPuzzle.parent = nullptr;

    // locate empty tile
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                initialPuzzle.x = i;
                initialPuzzle.y = j;
                break;
            }
        }
    }

    priority_queue<PuzzleState, vector<PuzzleState>, CompareCost> openList;
    unordered_map<string, bool> closedList; // visited
    openList.push(initialPuzzle);

    while (!openList.empty()) {
        PuzzleState currentPuzzle = openList.top();
        openList.pop();
    
        // check if is in goal states
        if (isGoal(currentPuzzle.board)) { 
            printSolutionPath(currentPuzzle);
            return;
        }
        
        // skip if already visited
        if (closedList[stringPuzzleBoard(currentPuzzle)]) { // convert board to string 
            continue;
        }
        closedList[stringPuzzleBoard(currentPuzzle)] = true;

        // Generate successors and possible moves
        vector<PuzzleState> successors = generateSuccessors(currentPuzzle);
        for (int i = 0; i < successors.size(); i++) {
            PuzzleState successor = successors[i];
            if (!closedList[stringPuzzleBoard(successor)])
                openList.push(successor);
        }
    }
    cout << "No Solution Found" << endl;
    return;
}

int main() {
    int initial[3][3] = {{5, 7, 2}, 
                         {0, 8, 6}, 
                         {4, 1, 3}};
    aStarSearch(initial);

    return 0;
}