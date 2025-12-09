#include "DSU.h"
#include "importer.h"
#define cell Node

using namespace std;

int n, m;
int drnum, mnnum, wlnum;
/*
 limits of  this variables:
    drnum+mnnum : [1 , (n*m)/9]
        why? because each draftman/monster
                  threatens up to 8 cells around it that 
                  can't contain monsters/draftsmen.
    wlnum : [0 , n*m - n - m]
*/

// main chain of functions:
pair<int,int> place_the_lightsource(vector<vector<char>>& shown_grid);
void put_the_walls(pair<int, int> light_source_pos, vector<vector<char>>& shown_grid);
void place_the_draftsmen();
void place_the_monsters();

// sub-algorithms:
void fix_the_wrong_walls(int x, int y, int *direction);

int main(){
    importer();

    cell grid[n][m];
    vector<vector<char>> shown_grid(2 * n + 1, vector<char>(2 * m + 1));

    // initialize the corners
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            shown_grid[2 * i][2 * j] = '#';
        }
    }
    pair<int,int> light_source_pos = place_the_lightsource(shown_grid);

    return 0;
}

pair<int,int> place_the_lightsource(vector<vector<char>>& shown_grid){
    int i = rand() % n;
    int j = rand() % m;
    shown_grid[2 * i + 1][2 * j + 1] = 'S';
    return {i, j};
}

//Check it !
void put_the_walls(pair<int, int> light_source_pos, vector<vector<char>>& shown_grid){
    int walls_placed = 0;
    while (walls_placed < wlnum) {
        int i1 = rand() % n;
        int j1 = rand() % m;
        int direction = rand() % 4;
        // 0 : right       1: up        2: left       3: down
        // now  how to prevent from placing walls in the edge of grid?
        direction = fix_the_wrong_walls(i1, j1, direction);
    }
}

int  fix_the_wrong_walls(int i1, int j1, int direction){
    if (i1 == 0)
    {
        if (j1 == 0 && (direction == 1 || direction == 2))
        {
            direction == 1 ? direction = 0 : direction = 3;
        }
        else if (j1 == n - 1 && (direction == 0 || direction == 1))
        {
            direction == 0 ? direction = 2 : direction = 3;
        }
        else
        {
            if (direction == 1)
                direction = 3;
        }
    }

    if (i1 == n - 1)
    {
        if (j1 == 0 && (direction == 2 || direction == 3)){
            direction == 2 ? direction = 0 : direction = 1;
        }
        else if (j1 == n - 1 && (direction == 0 || direction == 3))
        {
            direction == 0 ? direction = 2 : direction = 1;
        }
        else
        {
            if (direction == 3)
                direction = 1;
        }
    }

    if (j1 == 0 && direction == 2)
    {
        direction = 0;
    }

    if (j1 == m - 1 && direction == 0)
    {
        direction = 2;
    }
    return direction;
}
