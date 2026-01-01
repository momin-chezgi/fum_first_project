#include "mazegenerator.h"
#include "moves.h"
#include "draftsman.h"

int n,m, k;
int drnum, mnnum, wlnum;

intpair light_source_pos;
vector<intpair> mnpos, drpos;

intpair drmove(vec2d(char)& grid, intpair drpos, const int id, vector<int> announceid,int* temp_token, const int round);
intpair mnmove(vec2d(char)& grid, intpair mnpos, vec2d(int)& has_seen);
intpair Im_hungry(vec2d(char)& grid, intpair mnpos, vec2d(int)& has_seen);


int main(){

    importer();

    n>3 && m>3?(n>m?k=m:k=n):k=1;

    vec2d(char) grid(2 * n + 1, vector<char>(2 * m + 1));
    draftsman dr[drnum];

    mazegenerator(grid);

    int p=0;
    // Make vectors of coordinates of any type
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
                    dr[p].id = p;
                    dr[p].x = x, dr[p].y = y;
                    p++;
                    break;
            }
        }
    }

    int remain_dr = drnum;
    vector<int> winners, losers;
    vector<intpair> temp1, temp2;
    vector<int> announceid;
    int round = 1;

    //while there's a draftsman, still working
    while(remain_dr > 0){

        // Draftsmen move one by one
        for(int d=0; d<drnum; d++){
            if(dr[d].defeated || dr[d].winned) continue;
            
            intpair new_coo;
            d==0 ? new_coo = drmove(grid, dr[d], announceid ,round)
                 : new_coo = drmove(grid, dr[d], announceid ,0);
            
            if(new_coo.first%2 != new_coo.second%2) temp2.push_back(new_coo);
 
            announceid.clear();

            //When the draftsman reaches the light source, he wins...
            if(make_pair(dr[d].x, dr[d].y) == light_source_pos){
                grid[dr[d].x][dr[d].y] = 'S';
                dr[d].winned = true;
                winners.push_back(d);
                announceid.push_back(d+1);
                remain_dr--;
            }
        }

        // Hence, monsters move one by one
        for(int p=0; p<mnnum; p++){
            vec2d(int) has_seen(2*n+1, vector<int>(2*m+1, 0));
            mnpos[p] = mnmove(grid, mnpos[p], has_seen);
        }

        //Check if a/some draftsman/men is/are eaten...
        for(int d=0; d<drnum; d++){
            if(dr[d].defeated || dr[d].winned) continue;
            if(grid[dr[d].x][dr[d].y] == 'M'){
                dr[d].defeated = true;
                losers.push_back(d);
                announceid.push_back(-1-d);
                remain_dr--;
            }
        }

        // Reduce the number of temporary walls...
        for(auto s:temp2) grid[s.first][s.second] = '1';
        for(auto s:temp1) grid[s.first][s.second] = ' ';
        temp1 = temp2;
        temp2.clear();
    }

    print_the_status(grid);
    print_the_ranking(winners, losers);
    return 0;
}
