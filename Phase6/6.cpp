#include "mazegenerator.h"
#include "moves.h"
#include "saveandload.h"

int n,m, k;
int drnum, mnnum, wlnum;
bool is_saved_game;

intpair light_source_pos;
vector<intpair> mnpos; 
vector<draftsman> dr;

void prepare_the_grid(vec2d(char)& grid, status& saved_status){
    if(!is_saved_game){
        mazegenerator(grid);
        int p=0;
        // Make vectors of coordinates
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
                        dr.push_back({p++, x, y, 0, k, 0, 0});
                        break;
                }
            }
        }
    }
    else{
        n = saved_status.n;
        m = saved_status.m;
        dr = saved_status.drs;
        mnpos = saved_status.mns;
        light_source_pos = saved_status.lighpos;
        //bounds...
        for(int i=0; i<=2*n; i++) grid[i][0] = grid[i][2*n] = '#';
        for(int j=0; j<=2*m; j++) grid[0][j] = grid[2*m][j] = '#';
        //vertices...
        for(int i=2; i<2*n; i++){
            for(int j=2; j<2*m; j++){
                if(i%2 != j%2) grid[i][j] = '#';
            }
        }
        
        //monsters, draftsman, chance cubes, walls, temporary walls...
        for(auto mn:mnpos) grid[mn.first][mn.second]='M';
        for(auto d:dr) grid[d.x][d.y] = '#';
        for(auto c:saved_status.chancecubes) grid[c.first][c.second] = 'C';
        for(auto w:saved_status.walls) grid[w.first][w.second] = '#';
        for(auto t2:saved_status.temp2) grid[t2.first][t2.second] = '#';
        for(auto t1:saved_status.temp1) grid[t1.first][t1.second] = '#';
    }

}

int play_the_game(vector<int>& winners, vector<int>& losers, vec2d(char)& grid, status savedstatus = status{}){
    int remain_dr = drnum;
    vector<intpair> temp1, temp2;
    vector<int> announceid;
    int round = 1;
    bool good_luck = false;
    int d;
    is_saved_game ? d = savedstatus.lastplayer : d = 0;
    //while there's a draftsman, still working
    while(remain_dr > 0){

        // Draftsmen move one by one
        for(; d<drnum; d++){
            if(dr[d].defeated || dr[d].winned) continue;
            
            intpair new_coo;
            d==0 && !good_luck? new_coo = drmove(grid, dr[d], announceid ,round++)
                 : new_coo = drmove(grid, dr[d], announceid ,0);
            
            good_luck = false;
            
            // saving the game for playing another time...
            if(new_coo == make_pair(0,0)){
                vector<intpair> chancecubes, walls;
                for(int i=1; i<2*n; i+=2)
                    for(int j=1; j<2*m; j+=2)
                        if(grid[i][j]=='C')chancecubes.push_back({i,j});
                for(int i=2; i<2*n-1; i++)
                    for(int j=2; j<2*m-1; j++)
                        if( (i-j)%2 != 0 && grid[i][j]=='#')walls.push_back({i,j});
                status savinggame = {round, d, dr, mnpos, light_source_pos, walls, temp1, temp2, chancecubes, n, m};
                int a = save_the_game(savinggame);
                cout << "The game was saved by the id " << a+1 << endl;
                return 1;       // The game has been saved
            }

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

        d=0;
    }
    return 0;   // The game has been ended
}

inline void init_k(){
    // k is the first limit of temperoray wall
    // k = max( min(n,m), 1)
    n>3 && m>3?(n>m?k=m/3:k=n/3):k=1;
}

int main(){

    status saved_status = importer();

    vec2d(char) grid(2 * n + 1, vector<char>(2 * m + 1));
    vector<int> winners, losers;

    init_k();
    
    prepare_the_grid(grid, saved_status);
    
    play_the_game(winners, losers, grid);

    print_the_status(grid);

    print_the_ranking(winners, losers);

    return 0;
}