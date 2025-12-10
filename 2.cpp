#include "DSU.h"
#include "importer.h"
#define cell Node
#define vec2d(type) vector<vector<type>>
#define intpair pair<int,int>
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
//1.
intpair place_the_lightsource(vec2d(char)& shown_grid);
//2.
void put_the_walls(intpair light_source_pos,vec2d(cell) &grid, vec2d(char)& shown_grid, vec2d(int) &connected);
//3.
void place_the_draftsmen();
//4.
void place_the_monsters();

// sub-algorithms:
void create_the_spanning_tree(vec2d(cell) & grid, intpair light_source_pos,vec2d(int) & connected, vec2d(char) & shown_grid);
int fix_the_wrong_walls(int i1, int j1, int direction);
intpair where_is_the_neighbor(int i, int j, int direction);
pair<intpair, intpair> shown2real_coord(int a, int b, char cell_type);

int main(){

    importer();

    vec2d(cell) grid(n, vector<cell>(m));
    vec2d(int) connected(n, vector<int>(m, 0));
    vec2d(char) shown_grid(2 * n + 1, vector<char>(2 * m + 1));

    // initialize cells, corners and the space between cells that will be placed by walls later
    for (int i = 0; i <= n; i++)
    {
        for(int j=0; j<= m; j++){
            shown_grid[2*i+1][2*j+1] = ' ';
            shown_grid[2 * i][2 * j] = '#';
            shown_grid[2 * i][2 * j + 1] = ' ';
            shown_grid[2 * i + 1][2 * j] = ' ';
        }
    }
    //1. place the light source
    intpair light_source_pos = place_the_lightsource(shown_grid);
    connected[light_source_pos.first][light_source_pos.second] = 1;
    //2. place the walls
    put_the_walls(light_source_pos, grid, shown_grid, connected);
    return 0;
}

intpair place_the_lightsource(vec2d(char)& shown_grid){
    int i = rand() % n;
    int j = rand() % m;
    shown_grid[2 * i + 1][2 * j + 1] = 'S';
    return {i, j};
}

//Check it !
void put_the_walls(intpair light_source_pos, 
    vec2d(cell) &grid, vec2d(char) &shown_grid,
    vec2d(int) &connected){
    
    int walls_placed = 0;
    // first, create the spanning tree to ensure connectivity
    create_the_spanning_tree(grid, light_source_pos, connected, shown_grid);
    
    vector<intpair> possible_walls; // Note: the coordinate is not the real one, it's the coordinate in shown_grid

    //then we can place additional walls randomly on the remaining possible wall positions
    // collect all possible wall positions
    //vertical walls
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m-1; j++){
            if(shown_grid[2*i +1][2*j +2] == ' '){
                possible_walls.push_back({2*i +1, 2*j +2});
            }
        }
    }
    //horizontal walls
    for(int i = 0; i < n-1; i++){
        for(int j = 0; j < m; j++){
            if(shown_grid[2*i +2][2*j +1] == ' '){
                possible_walls.push_back({2*i +2, 2*j +1});
            }
        }
    }
    // shuffle the possible walls
    random_device rd;          
    mt19937 g(rd());            
    shuffle(possible_walls.begin(), possible_walls.end(), g);
    // place walls until we reach wlnum
    for(int i = 0; i < possible_walls.size() && walls_placed < wlnum; i++){
        intpair wall_pos = possible_walls[i];
        shown_grid[wall_pos.first][wall_pos.second] = '#';
        walls_placed++;
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

intpair where_is_the_neighbor(int i, int j, int direction){
    switch(direction){
        case 0:
            if(j!=m-1)return {i, j+1};
        case 1:
            if(i!=0) return {i-1, j};
        case 2:
            if(j!=0)return {i, j-1};
        case 3:
            if(i!=n-1)return {i+1, j};
    }
    return {-1, -1}; // should not reach here
}

void create_the_spanning_tree(vec2d(cell) &grid, intpair light_source_pos,
    vec2d(int) &connected, vec2d(char) &shown_grid){
    vector<cell> check_list;
    check_list.push_back(grid[light_source_pos.first][light_source_pos.second]);

    while(!check_list.empty()){
        int list_size = check_list.size();
        int idx = rand() % list_size;
        cell current = check_list[idx];
        // remove current from check_list
        check_list[idx] = check_list.back();
        check_list.pop_back();

        // explore neighbors
        for(int direction = 0; direction < 4; direction++){
            // if not connected, add to
            if(where_is_the_neighbor(current.x, current.y, direction) != make_pair(-1, -1)){
                intpair neighbor_pos = where_is_the_neighbor(current.x, current.y, direction);
                cell* neighbor = &grid[neighbor_pos.first][neighbor_pos.second];
                // if already connected, skip
                if(connected[neighbor_pos.first][neighbor_pos.second]==1){
                    break;
                }
                current.unitewith(neighbor);
                // Update shown_grid to reflect the connection
                switch (direction) {
                    case 0: // right
                        shown_grid[2 * current.x + 1][2 * current.y + 2] = '. ';
                        break;
                    case 1: // up
                        shown_grid[2 * current.x][2 * current.y + 1] = '. ';
                        break;
                    case 2: // left
                        shown_grid[2 * current.x + 1][2 * current.y] = ' .';
                        break;
                    case 3: // down
                        shown_grid[2 * current.x + 2][2 * current.y + 1] = ' .';
                        break;
                }
                check_list.push_back(*neighbor);
                connected[neighbor_pos.first][neighbor_pos.second]=1;
                }
            }
    }
}



//How to convert between shown_grid and grid?
//  if we have shown_grid[a][b], iff a%2 != b%2 then this is a wall-place coordinate and [(a != 0||n-1) || (b != 0||m-1)]
//  To get the corresponding cell coordinates in grid:
//  case 1: vertical wall(a is odd): the wall is between [(a-1)/2][(b-2)/2] and [(a-1)/2][b/2]
//  case 2: horizontal wall(b is odd): the wall is between [(a-2)/2][(b-1)/2] and [a/2][(b-1)/2]
pair<intpair,intpair> shown2real_coord(int a, int b, char cell_type){
    switch (cell_type){
        case 'W': //wall
            if(a%2 ==1 && b%2 ==0){ // vertical wall
                return {{(a-1)/2, (b-2)/2}, {(a-1)/2, b/2}};
            }
            else if(a%2 ==0 && b%2 ==1){ // horizontal wall
                return {{(a-2)/2, (b-1)/2}, {a/2, (b-1)/2}};
            }
        break;
        case 'S': //source
        case 'D': //draftsman
        case 'M': //monster
            if(a%2 ==1 && b%2 ==1){ // cell
                return {{(a-1)/2, (b-1)/2}, { -1, -1}};
            }
        break;
    }
    
}