#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>

int n,m, drnum, mnnum, wlnum;
using namespace std;

void printGrid(char *grid);
void clearScreen();

int main(void){
    int source[2];
    cin >> n >> m;
    char grid[2 * n+1][2 * m+1];
    for (int i = 0; i <= n; i++)
    {
        for(int j=0; j<= m; j++){
            grid[2*i+1][2*j+1] = ' ';
            grid[2 * i][2 * j] = '#';
            grid[2 * i][2 * j + 1] = ' ';
            grid[2 * i + 1][2 * j] = ' ';
        }
    }
    //printGrid(&grid[0][0]);
    cin >> source[0] >> source[1];
    grid[2*source[0]+1][2*source[1]+1] = 'S';
    //printGrid(&grid[0][0]);
    cin >> drnum;
    for (int i = 0; i<drnum; i++){
        int x, y;
        cin >> x >> y;
        grid[2*x+1][2*y+1] = 'D';
    }
    //printGrid(&grid[0][0]);
    cin >> mnnum;
    for (int i = 0; i<mnnum; i++){
        int x, y;
        cin >> x >> y;
        grid[2*x+1][2*y+1] = 'M';
    }
    //printGrid(&grid[0][0]);
    cin >> wlnum;
    for (int i = 0; i < wlnum; i++){
        int x, y;
        char wltype;
        cin >> x >> y >> wltype;
        if(wltype == 'H'){
            grid[2*x+2][2*y + 1] = '#';  
        }
        else{
            grid[2*x+1][2*y+2] = '#';
        }
    }
    printGrid(&grid[0][0]);
    return 0;
}

void printGrid(char *grid){
    for (int i = 0; i <= 2*n; i++)
    {
        for(int j=0; j <= 2*m; j++){
            cout << *(grid + (i * (2*m+1) + j));
        }
        cout << endl;
    }
}

