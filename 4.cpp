#include "mazegenerator.h"
#include "interpreter.h"

int n,m;
int drnum, mnnum, wlnum;

intpair light_source_pos;
vector<intpair> mnpos, drpos;

intpair drmove(vec2d(char)& grid, intpair drpos);
intpair mnmove(vec2d(char)& grid, intpair drpos);

int main(){

    importer(false, -1, -1, 1, 1);
    vec2d(char) grid(2 * n + 1, vector<char>(2 * m + 1));
    mazegenerator(grid);

    for(int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            int x = 2 * i + 1;
            int y = 2 * j + 1;
            switch(grid[x][y]){
                case 'S':
                    light_source_pos = {x, y};
                    break;
                case 'M':
                    mnpos.push_back({x, y});
                    break;
                case 'D':
                    drpos.push_back({x, y});
                    break;
            }
        }
    }

    bool win = false;
    bool ended = false;

    bool ended[drnum] = { };
    int remain_dr = drnum;
    
    while(remain_dr>0){
        
        vector<int> declare;
        // Draftsmen move one by one
        for(int d=0; d<drnum; d++){
            if(ended[d]) continue;
            drpos[d] = drmove(grid, drpos[d]);
            if(drpos[d] == light_source_pos){
                ended[d] = true;
                declare.push_back(d);
                remain_dr--;
            }
        }

        // Hence, monsters move one by one
        for(int p=0; p<mnnum; p++){
            mnpos[p] = mnmove(grid, mnpos[p]);
        }

        print_the_status(grid);

        for(auto s:declare) cout << "congrats player #" << s << "! You got out of this hell!\n";
    }

    return 0;
}

intpair drmove(vec2d(char)& grid,intpair drpos){       //returns the new coordinate
    char move;
    bool wants_to_rest = false;
    int new_x = drpos.first;
    int new_y = drpos.second;
    while(new_x == drpos.first && new_y == drpos.second && !wants_to_rest){
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
                wants_to_rest = true;
                break;
            default:
                cout << "Invalid move! Please enter W, A, S, or D." << endl;
                new_x = drpos.first;
                new_y = drpos.second;
                continue;
            }

            //check if the player wants to go out of bounds
            if(new_x < 1 || new_x >= 2*n || new_y < 1 || new_y >= 2*m){
                cout << "Move out of bounds! Try again." << endl;
                new_x = drpos.first;
                new_y = drpos.second;
                continue;
            }
            
            // check if the move is blocked by a wall
            if(grid[(drpos.first + new_x)/2][(drpos.second + new_y)/2] == '#'){
                cout << "Move blocked by a wall! Try again." << endl;
                new_x = drpos.first;
                new_y = drpos.second;
            }

            // check if there aren't any monsters in the new position
            else if(grid[new_x][new_y] == 'M'){
                cout << "Do you wanna be the snack of a monster? Try again." << endl;
                new_x = drpos.first;
                new_y = drpos.second;
            }
    }
    if(!wants_to_rest){
        // update the draftsman's position
        grid[drpos.first][drpos.second] = ' ';
        grid[new_x][new_y] = 'D';
    }

}


intpair mnmove(vec2d(char)& grid, intpair drpos){
    for (int l=0; l<2; l++){
            // if monster reaches the draftsman
            if(mnx == new_x && mny == new_y){
                ended = true;
                break;
            }
            //If it can close horizantelly...
            if(drpos.second < mny && grid[mnx][mny - 1] != '#' && mny>2){
                mny -= 2;
                continue;
            }
            if(drpos.second > mny && grid[mnx][mny + 1] != '#' && mny<(2*m-2)){
                mny += 2;
                continue;
            }
            //else closes vertically...
            if(drpos.first < mnx && grid[mnx - 1][mny] != '#' && mnx>2){
                mnx -= 2;
                continue;
            }
            if(drpos.first > mnpos.first && grid[mnx + 1][mny] != '#' && mnx<(2*n-2)){
                mnx += 2;
                continue;
            }
        }

        // update the monster's position
        mnpos == light_source_pos ? grid[mnpos.first][mnpos.second] = 'S' : grid[mnpos.first][mnpos.second] = ' ';
        mnpos = {mnx, mny};
        grid[mnpos.first][mnpos.second] = 'M';
        print_the_status(grid);
        
        drpos = {new_x, new_y};

        if(mnpos == drpos){
            ended = true;
        }
}