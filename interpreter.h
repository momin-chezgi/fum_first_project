#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "includer.h"

void clear_the_screen()
{
    for(int i=0; i<80; i++){
        cout << endl;
    }
}

void importer(bool wellcom = true, int given_n = -1, int given_m = -1,
                  int given_drnum = -1, int given_mnnum = -1){
    clear_the_screen();
    if(wellcom){
    cout << "Wellcome to 'The Tale of Labyrinth' game! " << endl;
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

intpair get_the_move(vec2d(char)& grid,const int x,const int y, const int id){ //returns the coordinate of moved draftsman
    bool wants_to_rest = false;
    char move;
    int new_x = x;
    int new_y = y;
    cout << "Player #" << id+1 << "(" << (x+1)/2 << ", " << (y+1)/2 << "):"<< endl;
    while(new_x == x && new_y == y && !wants_to_rest){
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
                new_x = x;
                new_y = y;
                continue;
        }

        //check if the player wants to go out of bounds
        if(new_x < 1 || new_x >= 2*n || new_y < 1 || new_y >= 2*m){
            cout << "Move out of bounds! Try again." << endl;
            new_x = x;
            new_y = y;
            continue;
        }
        
        // check if the move is blocked by a wall
        if(grid[(x + new_x)/2][(y + new_y)/2] == '#'){
            cout << "Move blocked by a wall! Try again." << endl;
            new_x = x;
            new_y = y;
            continue;
        }

        // check if there aren't any monsters or draftsman in the new position
        if(grid[new_x][new_y] == 'M'){
            cout << "Do you wanna be the snack of a monster? Try again." << endl;
            new_x = x;
            new_y = y;
        }
        else {
            if(grid[new_x][new_y] == 'D'){
                cout << "Sorry! every room hasn't got the place for you both. Try again." << endl;
                new_x = x;
                new_y = y;
            }
            else if(grid[new_x][new_y] == 'Z'){
                cout << "He's asleep! Don't make him wake up. Try again." << endl;
                new_x = x;
                new_y = y;
            }
        }
    }
    return {new_x, new_y};
}


void print_the_status(vec2d(char)& shown_grid){
    clear_the_screen();
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


#endif // iNTERPRETER_H