#include "mazegenerator.h"
#include "interpreter.h"

int n,m, k;
int drnum, mnnum, wlnum;

intpair light_source_pos;
vector<intpair> mnpos, drpos;

intpair drmove(vec2d(char)& grid, intpair drpos, const int id, vector<int> deservedid,int* temp_token, const int round);
intpair mnmove(vec2d(char)& grid, intpair mnpos, vec2d(int)& has_seen);
intpair Im_hungry(vec2d(char)& grid, intpair mnpos, vec2d(int)& has_seen);

//issues temp walls.

int main(){

    importer();

    n>3 && m>3?(n>m?k=m:k=n):k=1;

    vec2d(char) grid(2 * n + 1, vector<char>(2 * m + 1));

    mazegenerator(grid);

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
                    drpos.push_back({x, y});
                    break;
            }
        }
    }

    bool ended[drnum];
    for (int i=0; i<drnum; i++) ended[i] = false;
    int remain_dr = drnum;
    vector<int> winners;
    vector<int> losers;
    vector<intpair> temp1;
    vector<intpair> temp2;
    int round = 1;
    vector<int> deservedid;
    int temp_token[drnum] = { };

    //while there's a draftsman, still working
    while(remain_dr > 0){

        // Draftsmen move one by one
        for(int d=0; d<drnum; d++){
            if(ended[d]) continue;
            
            intpair new_coo;
            d==0 ? new_coo = drmove(grid, drpos[d], d, deservedid, temp_token ,round)
                 : new_coo = drmove(grid, drpos[d], d, deservedid, temp_token ,0);
            
            if(new_coo.first%2 == new_coo.second%2)drpos[d] = new_coo;
            else{(temp2.push_back(new_coo));} 
 
            deservedid.clear();

            //When the draftsman reaches the light source, he wins...
            if(drpos[d] == light_source_pos){
                grid[drpos[d].first][drpos[d].second] = 'S';
                ended[d] = true;
                winners.push_back(d);
                deservedid.push_back(d+1);
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
            if(ended[d]) continue;
            if(grid[drpos[d].first][drpos[d].second] == 'M'){
                ended[d] = true;
                losers.push_back(d);
                deservedid.push_back(-1-d);
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
    // Announce the final results...
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
    return 0;
}

intpair drmove(vec2d(char)& grid, const intpair drpos, const int id, vector<int> deservedid,int* temp_token, const int round){       //returns the new coordinate
    grid[drpos.first][drpos.second] = 'd';
    print_the_status(grid);
    intpair dr2pos = get_the_move(grid,drpos.first, drpos.second, id, deservedid,temp_token, round);
    if(dr2pos != drpos){
        if(dr2pos.first%2 != dr2pos.second%2){
            grid[dr2pos.first][dr2pos.second] = '2';
            grid[drpos.first][drpos.second] = 'D';
        }
        else{
            grid[drpos.first][drpos.second] = ' ';
            grid[dr2pos.first][dr2pos.second] = 'D';
        }
    }
    else{
        grid[drpos.first][drpos.second] = 'Z';
    }
    return dr2pos;
}


intpair mnmove(vec2d(char)& grid,const intpair mnpos, vec2d(int)& has_seen){
    int mnx = mnpos.first;
    int mny = mnpos.second;
    intpair drpos = Im_hungry(grid, mnpos, has_seen);
    for (int l=0; l<2; l++){
        //If it can close horizantelly...
        if(drpos.second < mny && grid[mnx][mny - 1] == ' ' && mny>2 && grid[mnx][mny-2] != 'S' && grid[mnx][mny-2] != 'M'){
            mny -= 2;
            continue;
        }
        if(drpos.second > mny && grid[mnx][mny + 1] == ' ' && mny<(2*m-2) && grid[mnx][mny+2] != 'S' && grid[mnx][mny+2] != 'M'){
            mny += 2;
            continue;
        }
        //else closes vertically...
        if(drpos.first < mnx && grid[mnx - 1][mny] == ' ' && mnx>2 && grid[mnx-2][mny] != 'S' && grid[mnx-2][mny] != 'M'){
            mnx -= 2;
            continue;
        }
        if(drpos.first > mnpos.first && grid[mnx + 1][mny] == ' ' && mnx<(2*n-2) && grid[mnx+2][mny] != 'S' && grid[mnx+2][mny] != 'M'){
            mnx += 2;
            continue;
        }
    }

    // update the monster's position
    grid[mnpos.first][mnpos.second] = ' ';
    grid[mnx][mny] = 'M';

    return {mnx,mny};
}


intpair Im_hungry(vec2d(char)& grid, intpair mnpos, vec2d(int)& has_seen){
    vector<intpair> breadth1, breadth2;
    breadth1.push_back(mnpos);
    while(!breadth1.empty()){
        for(auto neigh : breadth1){
            if(grid[neigh.first][neigh.second] == 'D' || grid[neigh.first][neigh.second] == 'd' || grid[neigh.first][neigh.second] == 'Z'){
                return neigh;
            }
            int x = neigh.first;
            int y = neigh.second;
            if(has_seen[x][y]==1) continue;
            has_seen[x][y] = 1;
            if(grid[x+1][y]==' ') breadth2.push_back({x+2,y});
            if(grid[x][y+1]==' ') breadth2.push_back({x,y+2});
            if(grid[x-1][y]==' ') breadth2.push_back({x-2,y});
            if(grid[x][y-1]==' ') breadth2.push_back({x,y-2});
        }
        breadth1 = breadth2;
        breadth2.clear();
    }
    return {-1,-1};
}