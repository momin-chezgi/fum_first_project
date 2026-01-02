#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "includer.h"
#include "structures.h"

inline void clear_the_screen()
{
    for(int i=0; i<80; i++){
        cout << endl;
    }
}

void importer(bool wellcom = true, int given_n = -1, int given_m = -1,
                  int given_drnum = -1, int given_mnnum = -1){
    clear_the_screen();
    if(wellcom){
    cout << "Wellcome to 'The Tale of Labyrinth' game! choose an option:" << endl;
    string chosen = "o";
    while(chosen!="1" && chosen!="2"){
        cout << "1.Play a new game              2.Play a saved game" << endl;
        cin >> chosen;
    }
    if(chosen == "2"){
        is_saved_game = true;
        vector<status> games = saved_games();
        int whichgame = 0;
        cout << "Which one do you play?"<<endl;
        for(int i=0; i<games.size(); i++) cout << i+1 << ", ";
        cout << endl;
        while(whichgame == 0){
            cin >> whichgame;
            if(whichgame>=games.size())whichgame=0;
        }

    }
    cout << "remember that rules(n and m are dimensions):" << endl;
    cout << "1. The sum of draftsmen and monsters should be at least 1 and at most (n*m)/9." << endl;
    cout << "2. The number of walls should be at least 0 and at most n*m - n - m." << endl;
    cout << "So enter the numbers of draftsmen, monsters and walls(in order): " << endl;
    }

    if(given_n != -1){
        if(given_n > 50 || given_n <= 2){
            cerr << "Invalid n dimension! It should be in range (2,50]. Exiting..." << endl;
            exit(1);
        }
        n = given_n;
        if(given_m != -1){
            if(given_m > 50 || given_m <= 2){
                cerr << "Invalid m dimension! It should be in range (2,50]. Exiting..." << endl;
                exit(1);
            }
            m = given_m;
        }
        else{
            cout << "enter the value of m to generate the maze with " << n << "*m dimension: ";
            cin >> m;
            while( m <= 2 || m > 50){
            cout << "Invalid dimensions! Please re-enter and m (2 < m <= 50):" << endl;
            cin >>  m;
            }
        }
    }
    
    else if(given_m != -1){
        if(given_m > 50 || given_m <= 2){
            cerr << "Invalid m dimension! It should be in range (2,50]. Exiting..." << endl;
            exit(1);
        }
        m = given_m;
        cout << "enter the value of n to generate the maze with n*" << m << " dimension: ";
        cin >> n;
        while(n <= 2  || n > 50){
            cout << "Invalid dimensions! Please re-enter n (2 < n <= 50):" << endl;
            cin >> n ;
        }
    }
        
    else if(given_n == -1 && given_m == -1){
        cout << "Enter the dimensions of the grid (n,m): ";
        cin >> n >> m;
        while(n <= 2 || m <= 2 || n > 50 || m > 50){
        cout << "Invalid dimensions! Please re-enter n and m (2 < n,m <= 50):" << endl;
        cin >> n >> m;
    }
    }

    if(given_drnum != -1) {
        drnum = given_drnum;
        if(given_mnnum != -1) mnnum = given_mnnum;
        else{
            cout << "enter the number of monsters to generate the maze with " << drnum << " draftsmen: ";
            cin >> mnnum;
        }
    }

    else{
        if(given_mnnum != -1){
            mnnum = given_mnnum;
            cout << "enter the number of draftsmen to generate the maze with " << mnnum << " monsters: ";
            cin >> drnum;
        }
        else{
            cout << "enter the numbers of draftsmen and monsters to generate the maze: ";
            cin >> drnum >> mnnum;
        }
    }

    cout << "enter the number of walls to generate the maze: ";
    cin >> wlnum;

    if (drnum + mnnum < 1 || drnum + mnnum > (n * m) / 9 || wlnum < 0 || wlnum > n * m - n - m){
        cerr << "Invalid numbers of draftsmen, monsters or walls! Exiting..." << endl;
        exit(1);
    }

    cout << "Valid inputs!" << endl;
    cout << string(50, '-') << endl;
}

intpair get_the_move(vec2d(char)& grid, draftsman& dr , vector<int>& announcedid, const int round=0){ //returns the coordinate of moved draftsman
    bool wants_to_rest = false;
    char move, move2;
    int new_x = dr.x, new_y = dr.y;
    
    if(!announcedid.empty()){
        for(auto d:announcedid)    
            d>0 ? cout << "congrats player #" << d << "! You got out of this hell!\n" 
            : cout << "Ops! player #" << -d  << "! God bless you!\n";
    }

    if(round!=0) cout << "Round " << round <<":==========================\n";
    cout << "Player #" << dr.id+1 << ":  coordinate(" << (dr.x+1)/2 << ", " << (dr.y+1)/2 << ")" << "  remained walls: " << dr.token_limit-dr.temp_token << endl;
    while(new_x == dr.x && new_y == dr.y && !wants_to_rest){
        cout << "Enter your move (W/A/S/D),Z for sleeping and TW/TA/TS/TD for building temporary walls: ";
        cin >> move;
        if(move == 'T' || move == 't') cin >> move2;
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
            case 'T':
            case 't':
                switch(move2){
                    case 'W':
                    case 'w':
                        new_x -= 1;
                        break;
                    case 'A':
                    case 'a':
                        new_y -= 1;
                        break;
                    case 'S':
                    case 's':
                        new_x += 1;
                        break;
                    case 'D':
                    case 'd':
                        new_y += 1;
                        break;
                    default:
                        cout << "Invlaid move! Please enter  TW, TA, TS, or TD." << endl;
                        new_x = dr.x;
                        new_y = dr.y;
                        continue;
                }
                break;
            default:
                cout << "Invalid move! Please enter W, A, S, or D." << endl;
                new_x = dr.x;
                new_y = dr.y;
                continue;
        }

        //check if the player wants to build a temporary wall
        if(move == 'T' || move == 't'){
            if(dr.temp_token <= dr.token_limit){
                dr.temp_token++;
                if(grid[new_x][new_y] == '#' || grid[new_x][new_y] == '2' || grid[new_x][new_y] == '1'){
                    cout << "You can't build the temporary wall there! Try again." << endl;
                    new_x = dr.x;
                    new_y = dr.y;
                    continue;
                }
            }
            else{
                cout << "Your temporary walls token has been limited! Try again." << endl;
                new_x = dr.x;
                new_y = dr.y;
                continue;
            }
        }

        //check if the player wants to go out of bounds
        if(new_x < 1 || new_x >= 2*n || new_y < 1 || new_y >= 2*m){
            cout << "Move out of bounds! Try again." << endl;
            new_x = dr.x;
            new_y = dr.y;
            continue;
        }
        
        // check if the move is blocked by a wall
        if(grid[(dr.x + new_x)/2][(dr.y + new_y)/2] == '#'){
            cout << "Move blocked by a wall! Try again." << endl;
            new_x = dr.x;
            new_y = dr.y;
            continue;
        }

        // check if there aren't any monsters or draftsman in the new position
        if(grid[new_x][new_y] == 'M'){
            cout << "Do you want to be the snack of a monster? Try again." << endl;
            new_x = dr.x;
            new_y = dr.y;
        }
        else {
            if(grid[new_x][new_y] == 'D' || grid[new_x][new_y] == 'd'){
                cout << "Sorry! every room hasn't got the place for you both. Try again." << endl;
                new_x = dr.x;
                new_y = dr.y;
            }
            else if(grid[new_x][new_y] == 'Z'){
                cout << "He's asleep! Don't make him wake up. Try again." << endl;
                new_x = dr.x;
                new_y = dr.y;
            }
        }
    }
    return {new_x, new_y};
}

int coor2id4monster(int x, int y, vector<intpair>& mnpos){
    for(int mn=0; mn<mnpos.size(); mn++){
        if(mnpos[mn] == make_pair(x,y)) return mn;
    }
    return -1;
}

void magic_transfer(vector<intpair>& mnpos, vec2d(char)& grid){
    int x, y;
    int newx = x, newy = y;
    cout << "Magic Transport! Write the coordinate of the monster that you want to move him(splitted by space):";
    cin >> x >> y;
    x = 2*x-1, y = 2*y-1;
    int mnid = coor2id4monster(x,y,mnpos);
    while(newx==x && newy==y){
        cout << "Try again!(splitted by space):";
        cin >> x >> y;
        x = 2*x-1, y = 2*y-1;
        int mnid = coor2id4monster(x,y,mnpos);
    }
    newx = x, newy = y;
    while(newx==x && newy==y){
        cout << "In which direction?(W/A/S/D) ";
        char move;
        cin >> move;
        switch(move){
            case 'W':
            case 'w':
                newx -= 2;
                break;
            case 'A':
            case 'a':
                newy -= 2;
                break;
            case 'S':
            case 's':
                newx += 2;
                break;
            case 'D':
            case 'd':
                newy += 2;
                break;
            default:
                cout << "Invalid move! Please enter W, A, S, or D." << endl;
                newx = x;
                newy = y;
        }
        
        //check if the player wants to go out of bounds
        if(newx < 1 || newx >= 2*n || newy < 1 || newy >= 2*m){
            cout << "Move out of bounds! Try again." << endl;
            newx = x;
            newy = y;
            continue;
        }
        
        // check if the move is blocked by a wall
        if(grid[(x + newx)/2][(y + newy)/2] == '#'){
            cout << "Cell blocked by a wall! Try again." << endl;
            newx = x;
            newy = y;
            continue;
        }

        // check if there aren't any monsters or draftsmen in the new position
        if(grid[newx][newy] == 'M'){
            cout << "Sorry! every room hasn't got the place for you both. Try again." << endl;
            newx = x;
            newy = y;
        }
    }
    grid[x][y] = ' ';
    grid[newx][newy] = 'M';
    mnpos[mnid].first = newx, mnpos[mnid].second = newy;
}

void print_the_status(vec2d(char)& shown_grid){
    clear_the_screen();
    //print the index of rows and columns
    cout << "   ";
    for(int j=1; j<=m; j++){
        if(j>9)cout << (j-j%10)/10 << " ";
        else cout << "  ";
    }
    cout << endl << "   ";
    for(int j=1; j<=m; j++) cout << j%10 << ' ';
    cout << endl;
    for(int i = 0; i <= 2*n; i++){
        if(i%2==1) (i+1)/2 > 9 ? cout << (i+1)/2 : cout << (i+1)/2 << " ";
        else cout << "  ";
        for( int j =0; j <= 2*m; j++){
            cout << shown_grid[i][j];
        }
        cout << endl;
    }
}

void print_the_ranking(vector<int>& winners, vector<int>& losers){
    cout << "----------------------------------------" << endl;
    cout << "Ranking:" << endl;
    int w=0;
    for(; w<winners.size(); w++){
        cout << w+1 << ".player #" << winners[w]+1 << endl;
    }
    for(int l=losers.size()-1; l>=0; l--){
        cout << ++w << ".player #" << losers[l]+1 << " XXX" << endl;
    }
    cout << "press 'e' and hit the Enter to end the game!";
    string meaningless;
    cin >> meaningless;
}

#endif // iNTERPRETER_H