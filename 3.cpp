#include "mazegenerator.h"
#include "interpreter.h"

int main(){
    intpair light_source_pos, monster_pos, draftsman_pos;
    vec2d(char) shown_grid(2 * n + 1, vector<char>(2 * m + 1));

    importer(false, -1, -1, 1, 1);
    mazegenerator(shown_grid);

    for(int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            int x = 2 * i + 1;
            int y = 2 * j + 1;
            switch(shown_grid[x][y]){
                case 'S':
                    light_source_pos = {x, y};
                break;
                case 'M':
                    monster_pos = {x, y};
                break;
                case 'D':
                    draftsman_pos = {x, y};
                break;
            }
        }
    }


    bool ended = false;

    while(!ended){
        char move;

        int new_x = draftsman_pos.first;
        int new_y = draftsman_pos.second;

        while(new_x == draftsman_pos.first && new_y == draftsman_pos.second){
            cout << "Enter your move (W/A/S/D): ";
            cin >> move;
            switch(move){
                case 'W':
                case 'w':
                    new_x -= 2;
                    break;
                case 'A':
                case 'a':
                    new_y -= 2;
                    break;
                case 'S':
                case 's':
                    new_x += 2;
                    break;
                case 'D':
                case 'd':
                    new_y += 2;
                    break;
                default:
                    cout << "Invalid move! Please enter W, A, S, or D." << endl;
                    int new_x = draftsman_pos.first;
                    int new_y = draftsman_pos.second;
                    continue;
            }

            //check that the player won't go out of bounds
            if(new_x < 1 || new_x >= 2*n || new_y < 1 || new_y >= 2*m){
                cout << "Move out of bounds! Try again." << endl;
                int new_x = draftsman_pos.first;
                int new_y = draftsman_pos.second;
                continue;
            }
            
            // check if the move is blocked by a wall
            if(shown_grid[(draftsman_pos.first + new_x)/2][(draftsman_pos.second + new_y)/2] == '#'){
                cout << "Move blocked by a wall! Try again." << endl;
                int new_x = draftsman_pos.first;
                int new_y = draftsman_pos.second;
            }
            // check if there aren't any monsters in the new position
            else if(shown_grid[new_x][new_y] == 'M'){
                cout << "Move blocked by a monster! Try again." << endl;
                int new_x = draftsman_pos.first;
                int new_y = draftsman_pos.second;
            }
            // if the draftsman reaches the light source
            else if(shown_grid[new_x][new_y] == 'S'){
                ended = true;
            }
        }
        // update the draftsman's position
        shown_grid[draftsman_pos.first][draftsman_pos.second] = ' ';
        draftsman_pos = {new_x, new_y};
        shown_grid[draftsman_pos.first][draftsman_pos.second] = 'D';
        // move the monster
        int monster_new_x = monster_pos.first;
        int monster_new_y = monster_pos.second;
        for (int i = 0; i<2; i++){
            // simple logic for monster movement: move towards the draftsman if possible
            if(draftsman_pos.second < monster_pos.second && shown_grid[monster_pos.first][monster_pos.second - 1] != '#'){
                monster_new_y -= 2;
            }
            else if(draftsman_pos.second > monster_pos.second && shown_grid[monster_pos.first][monster_pos.second + 1] != '#'){
                monster_new_y += 2;
            }
            else if(draftsman_pos.first < monster_pos.first && shown_grid[monster_pos.first - 1][monster_pos.second] != '#'){
                monster_new_x -= 2;
            }
            else if(draftsman_pos.first > monster_pos.first && shown_grid[monster_pos.first + 1][monster_pos.second] != '#'){
                monster_new_x += 2;
            }
        }
        // update the monster's position
        shown_grid[monster_pos.first][monster_pos.second] = ' ';
        monster_pos = {monster_new_x, monster_new_y};
        shown_grid[monster_pos.first][monster_pos.second] = 'M';
        // if moster reaches the draftsman
        if(monster_pos == draftsman_pos){
            ended = true;
        }
        // print the updated maze
        print_the_status(shown_grid);
        // check for win/lose conditions here and set ended = true if game ends
    }
    return 0;
}