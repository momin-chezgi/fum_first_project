#ifndef INCLUDER_H
#define INCLUDER_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include <fstream>

#define cell Node
#define vec2d(type) vector<vector<type>>
#define intpair pair<int,int>
#ifdef _WIN32 
#include <windows.h>
#endif
using namespace std;

#define LINEMAX 37000


struct Node
{
    // Node* parent = this;
    // Node() : parent(this) {}
    // Node(const Node&) = delete;
    // Node& operator = (const Node&) = delete;
    int x = 0;
    int y = 0;
    int xp = 0;
    int yp = 0;

    void find(vec2d(cell)& grid) {
        if (xp == x && yp == y) return;  // Already root
        int rootx = x, rooty = y;
        while (xp != rootx || yp != rooty) {
            rootx = xp;
            rooty = yp;
            xp = grid[xp][yp].xp;
            yp = grid[xp][yp].yp;
        }
        // Now compress path: set this node's parent directly to root
        this->xp = rootx;
        this->yp = rooty;
    }
    bool unite(Node* n1, Node* n2, vec2d(cell)& grid) {
        n1->find(grid);
        n2->find(grid);
        int root2_x = n2->xp;
        int root2_y = n2->yp;
        if (n1->xp != root2_x || n1->yp != root2_y) {
            grid[root2_x][root2_y].xp = n1->xp;
            grid[root2_x][root2_y].yp = n1->yp;
            return true;
        } else {
            return false;
        }
    }

    // bool isConnected(Node* other)
    // {
    //     this->find();
    //     other->find();
    //     return this->parent == other->parent;
    // }
};

struct draftsman
{
    int id = -1;
    int x = -1;
    int y = -1;
    int temp_token = 0;
    int token_limit = k;
    bool winned = false;
    bool defeated = false;
};

struct status
{
    int round=-1;
    int who_was_the_last=0;
    vector<draftsman> drs;
    vector<intpair> mns;
    intpair lighpos = {-1,-1};
    vector<intpair> walls;
    vector<intpair> temp1;
    vector<intpair> temp2;
    vector<intpair> chancecubes;
};

extern int n, m, k;
extern int drnum, mnnum, wlnum;
extern bool is_saved_game;

extern intpair light_source_pos;
extern vector<intpair> mnpos;

extern vector<draftsman> dr;
extern status saved_status;

#endif // INCLUDER_H