#include "mazegenerator.h"
#include "interpreter.h"

int n,m;
int drnum, mnnum, wlnum;

intpair light_source_pos, monster_pos, draftsman_pos;

int main(){
    importer(false, -1, -1, 1, 1);
    vec2d(char) shown_grid(2 * n + 1, vector<char>(2 * m + 1));
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

    bool win = false;
    bool ended = false;

    int new_x = draftsman_pos.first;
    int new_y = draftsman_pos.second;
    int monster_new_x = monster_pos.first;
    int monster_new_y = monster_pos.second;
    while(!ended){
        new_x = draftsman_pos.first;
        new_y = draftsman_pos.second;
        char move;
        while(new_x == draftsman_pos.first && new_y == draftsman_pos.second){
            cout << "Enter your move (W/A/S/D) Or Z for nothing: ";
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
                case 'Z':
                case 'z':
                    break;
                default:
                    cout << "Invalid move! Please enter W, A, S, or D." << endl;
                    new_x = draftsman_pos.first;
                    new_y = draftsman_pos.second;
                    continue;
            }

            //check if the player wants to go out of bounds
            if(new_x < 1 || new_x >= 2*n || new_y < 1 || new_y >= 2*m){
                cout << "Move out of bounds! Try again." << endl;
                new_x = draftsman_pos.first;
                new_y = draftsman_pos.second;
                continue;
            }
            
            // check if the move is blocked by a wall
            if(shown_grid[(draftsman_pos.first + new_x)/2][(draftsman_pos.second + new_y)/2] == '#'){
                cout << "Move blocked by a wall! Try again." << endl;
                new_x = draftsman_pos.first;
                new_y = draftsman_pos.second;
            }

            // check if there aren't any monsters in the new position
            else if(shown_grid[new_x][new_y] == 'M'){
                cout << "Do you wanna be the snack of a monster? Try again." << endl;
                new_x = draftsman_pos.first;
                new_y = draftsman_pos.second;
            }

            // if the draftsman reaches the light source
            else if(shown_grid[new_x][new_y] == 'S'){
                win = ended = true;
            }
        }

        // update the draftsman's position
        shown_grid[draftsman_pos.first][draftsman_pos.second] = ' ';
        draftsman_pos = {new_x, new_y};
        shown_grid[draftsman_pos.first][draftsman_pos.second] = 'D';

        // move the monster
        monster_new_x = monster_pos.first;
        monster_new_y = monster_pos.second;
        
        for (int l=0; l<2; l++){
            // simple logic for monster movement: move towards the draftsman if possible
            int hdir = (draftsman_pos.second - monster_pos.second)/(int)abs(draftsman_pos.second - monster_pos.second);
            int vdir = (draftsman_pos.first - monster_pos.first)/(int)abs(draftsman_pos.first - monster_pos.first);
            
            if(shown_grid[monster_pos.first][monster_pos.second + hdir] != '#'){
                monster_new_y += 2*hdir;
                monster_new_y > 0 && monster_new_y < 2*n && shown_grid[monster_pos.first][monster_new_y] != 'S' ? draftsman_pos.second = monster_new_y : monster_new_y -= 2*hdir;
            }
            else if(shown_grid[monster_pos.first + vdir][monster_pos.second] != '#'){
                monster_new_x += 2*vdir;
                monster_new_x > 0 && monster_new_x < 2*n && shown_grid[monster_new_x][monster_pos.second] != 'S' ? draftsman_pos.first = monster_new_x : monster_new_x -= 2*vdir;
            }
        }

        // update the monster's position
        shown_grid[monster_pos.first][monster_pos.second] = ' ';
        monster_pos = {monster_new_x, monster_new_y};
        shown_grid[monster_pos.first][monster_pos.second] = 'M';
        
        // if monster reaches the draftsman
        if(monster_pos == draftsman_pos){
            ended = true;
        }
        // print the updated maze
        print_the_status(shown_grid);
        // check for win/lose conditions here and set ended = true if game ends
    }
    win ? cout << "Congrats! You could walk through this wild world! I recommand you to play the next release of this game.\n" : cout << "Ops! Sadly, it seems you get stuck in the maze!\n";
    return 0;
}