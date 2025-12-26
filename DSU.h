#ifndef DSU_H
#define DSU_H
#include "includer.h"

struct Node
{
    // Node* parent = this;
    // Node() : parent(this) {}
    // Node(const Node&) = delete;
    // Node& operator = (const Node&) = delete;
    int x = 0;
    int y = 0;
    int xp = -1;
    int yp = -1;

    void find(vec2d(cell)& grid)
    {
        while(xp != x || yp != y){
            Node parent = grid[xp][yp];
            x = xp, y = yp;
            xp = grid[xp][yp].xp, yp = grid[xp][yp].yp;
        }
    }
    bool unite(Node* n1, Node* n2, vec2d(cell)& grid)
    {
        
        n1->find(grid), n2->find(grid);
        
        if(n1->xp != n2->xp || n1->yp != n2->yp){
            n2->xp = n1->xp, n2->yp = n1->yp;
            return true; // united
        }
        else{
            return false; // already united, no action taken
        }
    }

    // bool isConnected(Node* other)
    // {
    //     this->find();
    //     other->find();
    //     return this->parent == other->parent;
    // }
};
#endif // DSU_H