#ifndef importer_H
#define importer_H

#include "includer.h"

void importer(bool wellcom = true, int given_n = -1, int given_m = -1,
                  int given_drnum = -1, int given_mnnum = -1){
    
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
    cout << string(50, '-') << endl;;
}

#endif // importer_H