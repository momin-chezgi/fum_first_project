#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "DSU.h"
#include "interpreter.h"
#define cell Node
#define vec2d(type) vector<vector<type>>
#define intpair pair<int,int>
using namespace std;

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
void place_the_draftsmen(vec2d(char) & shown_grid);
//4.
void place_the_monsters(vec2d(char) & shown_grid);

// sub-algorithms:
void create_the_spanning_tree(vec2d(cell) & grid, intpair light_source_pos,vec2d(int) & connected, vec2d(char) & shown_grid);
int fix_the_wrong_walls(int i1, int j1, int direction);
intpair where_is_the_neighbor(int i, int j, int direction);
pair<intpair, intpair> shown2real_coord(int a, int b, char cell_type);
bool are_there_no_enemy_nearby(vec2d(char) & shown_grid, int i, int j, char cell_type);
inline void remove_dots(vec2d(char)& shown_grid);

int mazegenerator(vec2d(char)& shown_grid, int n_input  = n, int m_input = m,
                  int drnum_input = drnum, int mnnum_input = mnnum,
                  int wlnum_input = wlnum){
        // if you want to give just some parameters fill others with -1;

    n = n_input;
    m = m_input;
    drnum = drnum_input;
    mnnum = mnnum_input;
    wlnum = wlnum_input;

    vec2d(cell) grid(n, vector<cell>(m));
    vec2d(int) connected(n, vector<int>(m, 0));

    // initialize cells, corners and the space between cells that will be placed by walls later
    for (int i = 0; i < n; i++){
        for(int j=0; j< m; j++){
            shown_grid[2*i+1][2*j+1] = ' ';     
            shown_grid[2 * i][2 * j] = '#';
            shown_grid[2 * i][2 * j + 1] = ' ';
            shown_grid[2 * i + 1][2 * j] = ' ';
        }
    }
    for(int i=0; i<=2*n; i++){
        shown_grid[i][0] = shown_grid[i][2*m] = '#';
    }
    for(int j=0; j<=2*m; j++){
        shown_grid[0][j] = shown_grid[2*n][j] = '#';
    }
    
    //initialize the cells of grid[][]
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            grid[i][j].x = grid[i][j].xp = i;
            grid[i][j].y = grid[i][j].yp = j;
        }
    }

    //1. place the light source
    intpair light_source_pos = place_the_lightsource(shown_grid);
    connected[light_source_pos.first][light_source_pos.second] = 1;
    
    //2. place the walls
    put_the_walls(light_source_pos, grid, shown_grid, connected);
    
    //3. place the draftsmen
    place_the_draftsmen(shown_grid);
    
    //4. place the monsters
    place_the_monsters(shown_grid);

    remove_dots(shown_grid);

    //final output
    for(int i = 0; i < shown_grid.size(); i++){
        for(int j =0; j < shown_grid[0].size(); j++){
            cout << shown_grid[i][j];
        }
        cout << endl;
    }

    return 0;
}

intpair place_the_lightsource(vec2d(char)& shown_grid){
    srand(time(0));
    int i = rand() % n;
    int j = rand() % m;
    shown_grid[2 * i + 1][2 * j + 1] = 'S';
    return {i, j};
}

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

    // finally, replace all remaining '.' with ' ' to finalize the maze
    for (int i = 1; i < 2*n; i+=2){
        for (int j = 2; j< 2*m; j+=2){
            if(shown_grid[i][j] == '.'){
                shown_grid[i][j] = ' ';
            }
        }
    }
}

void place_the_draftsmen(vec2d(char) & shown_grid){
    int darftsmen_placed = 0;
    while(darftsmen_placed < drnum){
        srand(time(0));
        int i = rand() % n;
        int j = rand() % m;
        if(shown_grid[2*i +1][2*j +1] == ' '){
            shown_grid[2*i +1][2*j +1] = 'D';
            darftsmen_placed++;
        }
    }
}

void place_the_monsters(vec2d(char) & shown_grid){
    if(mnnum < drnum){
        int monsters_placed = 0;
        while(monsters_placed < mnnum){
            srand(time(0));
            int i = rand() % n;
            int j = rand() % m;
            if(shown_grid[2*i +1][2*j +1] == ' '  && are_there_no_enemy_nearby(shown_grid, i, j, 'M')){
                shown_grid[2*i +1][2*j +1] = 'M';
                monsters_placed++;
            }
        }    
    }
    if(mnnum >= drnum){
        int monsters_placed = 0;
        while(monsters_placed < mnnum){
            srand(time(0));
            int i = rand() % n;
            int j = rand() % m;
            if(shown_grid[2*i +1][2*j +1] == ' '  && are_there_no_enemy_nearby(shown_grid, i, j, 'D')){
                shown_grid[2*i +1][2*j +1] = 'M';
                monsters_placed++;
            }    
        }
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
        case 0: //right
            if(j!=m-1)return {i, j+1};
            break;
        case 1: //up
            if(i!=0) return {i-1, j};
            break;
        case 2: //left
            if(j!=0)return {i, j-1};
            break;
        case 3: //down
            if(i!=n-1)return {i+1, j};
            break;
        default:
            return {-1,-1};
    }
    return {-1, -1}; // should not reach here
}

void create_the_spanning_tree(vec2d(cell) &grid, intpair light_source_pos,
    vec2d(int) &connected, vec2d(char) &shown_grid){
    vector<cell*> check_list;
    check_list.push_back(&grid[light_source_pos.first][light_source_pos.second]);

    while(!check_list.empty()){
        int list_size = check_list.size();
        srand(time(0));
        int idx = rand() % list_size;
        cell* current = check_list[idx];

        // remove current from check_list
        check_list.erase(check_list.begin() + idx);

        // explore neighbors
        for(int direction = 0; direction < 4; direction++){
            intpair neighbor_pos = where_is_the_neighbor(current->x, current->y, direction);
            
            // if not connected and is valid, add to the tree
            if(neighbor_pos != make_pair(-1, -1)){
                cell* neighbor = &grid[neighbor_pos.first][neighbor_pos.second];
                
                // if already connected, skip
                if(connected[neighbor_pos.first][neighbor_pos.second]==1){
                    continue;
                }

                //add the neighbor to the tree and Update shown_grid to reflect the connection
                if(current->unite(current, neighbor, grid)) shown_grid[current->x + neighbor->x + 1][current->y + neighbor->y + 1] = '.';
                check_list.push_back(neighbor);
                connected[neighbor_pos.first][neighbor_pos.second]=1;
                }
        }
    }
}

inline void remove_dots(vec2d(char)& shown_grid){
    for(int i=1; i<2*n; i++){
        for(int j=1; j<2*m; j++){
            if(i%2==j%2) continue;
            if(shown_grid[i][j] == '.')shown_grid[i][j] = ' ';
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
    return {{-1,-1},{-1,-1}};    
}

bool are_there_no_enemy_nearby(vec2d(char) & shown_grid, int i, int j, char cell_type){
    char enemy;
    cell_type == 'D' ? enemy = 'M' : enemy = 'D';
    int ni = i;
    int nj = j;
    if(i > 0){ // up
        ni = i -1;
        if(shown_grid[2*ni +1][2*nj +1] == enemy) return false;
    }
    ni = i;
    if(i < n-1){ // down
        ni = i +1;
        if(shown_grid[2*ni +1][2*nj +1] == enemy) return false;
    }
    ni = i;
    if(j > 0){ // left
        nj = j -1;
        if(shown_grid[2*ni +1][2*nj +1] == enemy) return false;
    }
    nj = j;
    if(j < m-1){ // right
        nj = j +1;
        if(shown_grid[2*ni +1][2*nj +1] == enemy) return false;
    }
    return true;
}
#endif // MAZEGENERATOR_H