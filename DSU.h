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
#endif // DSU_H