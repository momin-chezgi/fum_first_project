#ifndef importer_H
#define importer_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
using namespace std;
extern int n, m;
extern int drnum, mnnum, wlnum;

void importer(){
    cout << "Enter the dimensions of the grid (n,m): ";
    cin >> n >> m;
    while(n <= 2 || m <= 2 || n > 50 || m > 50){
        cout << "Invalid dimensions! Please re-enter n and m (2 < n,m <= 50):" << endl;
        cin >> n >> m;
    }
    cout << "remember that rules:" << endl;
    cout << "1. The sum of draftsmen and monsters should be at least 1 and at most (n*m)/9." << endl;
    cout << "2. The number of walls should be at least 0 and at most n*m - n - m." << endl;
    cout << "So enter the numbers of draftsmen, monsters and walls(in order): " << endl;
    cin >> drnum >> mnnum >> wlnum;
    while(drnum + mnnum < 1 || drnum + mnnum > (n*m)/9 || wlnum < 0 || wlnum > n*m - n - m){
        cout << "Invalid numbers! Please re-enter the numbers of draftsmen, monsters and walls:" << endl;
        cin >> drnum >> mnnum >> wlnum;
    }
    cout << "Valid inputs!" << endl;
    cout << string(50, '-') << endl;
}

#endif // importer_H