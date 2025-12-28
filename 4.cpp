#include "mazegenerator.h"
#include "interpreter.h"

int n,m;
int drnum, mnnum, wlnum;

intpair light_source_pos;
vector<intpair> mnpos, drpos;


intpair drmove(vec2d(char)& grid, intpair drpos);
intpair mnmove(vec2d(char)& grid, intpair mnpos);
intpair Im_hungry(vec2d(char)& grid, intpair mnpos);


int main(){

    importer();

    vec2d(char) grid(2 * n + 1, vector<char>(2 * m + 1));

    mazegenerator(grid);

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

    bool ended[drnum] = { };
    int remain_dr = drnum;
    vector<int> winners;
    vector<int> losers;


    //while there's a draftsman, still working
    while(remain_dr > 0){

        // Draftsmen move one by one
        for(int d=0; d<drnum; d++){
            if(ended[d]) continue;

            drpos[d] = drmove(grid, drpos[d]);
            if(drpos[d] == light_source_pos){
                ended[d] = true;
                winners.push_back(d);
                remain_dr--;
            }
        }

        // Hence, monsters move one by one
        for(int p=0; p<mnnum; p++){
            mnpos[p] = mnmove(grid, mnpos[p]);
        }

        //Check if a/some draftsman/men is/are eaten...
        for(int d=0; d<remain_dr; d++){
            if(ended[d]) continue;
            if(grid[drpos[d].first][drpos[d].second] == 'M'){
                ended[d] = true;
                losers.push_back(d);
                remain_dr--;
            }
        }

        //status...
        print_the_status(grid);
        for(auto s:winners) cout << "congrats player #" << s << "! You got out of this hell!\n";
        for(auto s:losers) cout << "Ops! player #" << s << "! God bless you!\n";
    }


    // Announce the final results...
    cout << 50*'-' << endl;
    cout << "Ranking:" << endl;
    int w=0;
    for(; w<winners.size(); w++){
        cout << w+1 << ".player #" << winners[w] << endl;
    }
    for(int l=losers.size()-1; l>=0; l--){
        cout << w++ << ".player #" << losers[l] << endl;
    }


    return 0;
}

intpair drmove(vec2d(char)& grid, const intpair drpos, const int id){       //returns the new coordinate
    intpair dr2pos = get_the_move(grid,drpos.first, drpos.second, id);
    if(dr2pos == drpos){
        //Mark the reserved cell
        grid[dr2pos.first][dr2pos.second] = '.';
    }
    return dr2pos;
}


intpair mnmove(vec2d(char)& grid,const intpair mnpos){
    int mnx = mnpos.first;
    int mny = mnpos.second;
    intpair drpos = Im_hungry(grid, mnpos);
    for (int l=0; l<2; l++){
        //If it can close horizantelly...
        if(drpos.second < mny && grid[mnx][mny - 1] != '#' && mny>2 && grid[mnx][mny-2] == ' '){
            mny -= 2;
            continue;
        }
        if(drpos.second > mny && grid[mnx][mny + 1] != '#' && mny<(2*m-2) && grid[mnx][mny+2] == ' '){
            mny += 2;
            continue;
        }
        //else closes vertically...
        if(drpos.first < mnx && grid[mnx - 1][mny] != '#' && mnx>2 && grid[mnx-2][mny] == ' '){
            mnx -= 2;
            continue;
        }
        if(drpos.first > mnpos.first && grid[mnx + 1][mny] != '#' && mnx<(2*n-2) && grid[mnx+2][mny] == ' '){
            mnx += 2;
            continue;
        }
    }

    // update the monster's position
    grid[mnpos.first][mnpos.second] = ' ';
    grid[mnx][mny] = 'M';

    return {mnx,mny};
}


intpair Im_hungry(vec2d(char)& grid, intpair mnpos){
    vector<intpair> breadth1, breadth2;
    breadth1.push_back(mnpos);
    while(!breadth1.empty()){
        for(auto neigh : breadth1){
            if(grid[neigh.first][neigh.second] == 'D'){
                return neigh;
            }
            int x = neigh.first;
            int y = neigh.second;
            if(grid[x+1][y]!='#') breadth2.push_back({x+1,y});
            if(grid[x][y+1]!='#') breadth2.push_back({x,y+1});
            if(grid[x-1][y]!='#') breadth2.push_back({x-1,y});
            if(grid[x][y-1]!='#') breadth2.push_back({x,y-1});
        }
        breadth1 = breadth2;
        breadth2.clear();
    }
    return {-1,-1};
}