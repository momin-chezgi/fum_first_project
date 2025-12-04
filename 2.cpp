#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

int n, m, drnum, mnnum, wlnum;


int main(){
    cin >> n >> m;
    cin >> drnum >> mnnum >> wlnum;
    char grid[2 * n + 1][2 * m + 1];
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            grid[2 * i][2 * j] = '#';
        }
    }
    return 0;
}

void createMaze();