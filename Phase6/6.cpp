#include "mazegenerator.h"
#include "moves.h"
#include "structures.h"

int n,m, k;
int drnum, mnnum, wlnum;
bool is_saved_game;

intpair light_source_pos;
vector<intpair> mnpos;

intpair drmove(vec2d(char)& grid, draftsman& dr, vector<int> deservedid, const int round);
intpair mnmove(vec2d(char)& grid, intpair mnpos, vec2d(int)& has_seen);
intpair Im_hungry(vec2d(char)& grid, intpair mnpos, vec2d(int)& has_seen);


int main(){

    importer();

    n>3 && m>3?(n>m?k=m/3:k=n/3):k=1;

    vec2d(char) grid(2 * n + 1, vector<char>(2 * m + 1));
    vector<draftsman> dr(drnum);
    
    if(!is_saved_game) mazegenerator(grid);

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
            d==0 ? new_coo = drmove(grid, dr[d], announceid ,round++)
                 : new_coo = drmove(grid, dr[d], announceid ,0);
            
            bool good_luck = false;
            if(new_coo.first < 0 && new_coo.second < 0){
                good_luck = true;
                new_coo.first *= -1, new_coo.second *= -1;
            }

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
            
            //chance cube if it's the time
            if(good_luck){
                srand(time(0));
                switch(rand()%4){
                    case 0:
                        d--;
                        break;
                    case 1:
                        cout << "Your token limit for temporary walls increased!" << endl;
                        dr[d].token_limit += 2;
                        char meaningless;
                        cout << "press 'n' and enter to the next player ";
                        cin >> meaningless;
                        break;
                    case 2:
                        earthquake(grid, dr, mnpos, remain_dr);
                        break;
                    case 3:
                        magic_transfer(mnpos, grid);
                        break;
                }
                good_luck = false;
            }
        }

        // Then, monsters move one by one
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
