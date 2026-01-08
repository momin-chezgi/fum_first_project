#ifndef MOVES_H
#define MOVES_H

#include "includer.h"
#include "interpreter.h"

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

intpair drmove(vec2d(char)& grid, draftsman& dr, vector<int> deservedid, const int round){       //returns the new coordinate
    grid[dr.x][dr.y] = 'd';
    print_the_status(grid);
    intpair dr2pos = get_the_move(grid, dr, deservedid, round);
    if(dr2pos != make_pair(dr.x,dr.y)){
        if(dr2pos == make_pair(0,0)){     //save the game
            return {0,0};
        }
        if(dr2pos.first%2 != dr2pos.second%2){
            grid[dr2pos.first][dr2pos.second] = '2';
            grid[dr.x][dr.y] = 'D';
        }
        else{
            grid[dr.x][dr.y] = ' ';
            dr.x = dr2pos.first, dr.y = dr2pos.second;
            char a = grid[dr2pos.first][dr2pos.second];
            grid[dr2pos.first][dr2pos.second] = 'D';
            if(a == 'C'){
                return {-dr2pos.first, -dr2pos.second};
            }
        }
    }
    else{
        grid[dr.x][dr.y] = 'Z';
    }
    return {dr2pos.first, dr2pos.second};
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
        if(grid[mnx][mny] == 'D' || grid[mnx][mny] == 'd' || grid[mnx][mny] == 'Z'){
            break;
        }
    }
    // update the monster's position
    grid[mnpos.first][mnpos.second] = ' ';
    grid[mnx][mny] = 'M';

    return {mnx,mny};
}

void earthquake(vec2d(char)& grid, vector<draftsman>& draftsman,vector<intpair>& mnpos, int remain_dr){
    srand(time(0));
    for(int d=0; d<remain_dr; d++){
        int newx = draftsman[d].x, newy = draftsman[d].y;
        if(draftsman[d].winned || draftsman[d].defeated) continue;
        
        vector<intpair> neighs;
        if(grid[newx][newy+1] == ' '){
            if(grid[newx][newy+2] == ' ') neighs.push_back({newx, newy+2});
        }
        if(grid[newx-1][newy] == ' '){
            if(grid[newx-2][newy] == ' ') neighs.push_back({newx-2, newy});
        }
        if(grid[newx][newy-1] == ' '){
            if(grid[newx][newy-2] == ' ') neighs.push_back({newx, newy-2});
        }
        if(grid[newx+1][newy] == ' '){
            if(grid[newx+2][newy] == ' ') neighs.push_back({newx+2, newy});
        }
        
        if(neighs.empty()) continue;
        
        int i = rand()%neighs.size();
        auto neigh = neighs[i];
        char a = 'D';
        if(grid[draftsman[d].x][draftsman[d].y] == 'Z') a = 'Z';
        grid[draftsman[d].x][draftsman[d].y] = ' ';
        grid[neigh.first][neigh.second] = a;
        draftsman[d].x = neigh.first, draftsman[d].y = neigh.second;
    }

    for(int mn=0; mn<mnpos.size(); mn++){
        int newx = mnpos[mn].first, newy = mnpos[mn].second;
        vector<intpair> neighs;
        if(grid[newx][newy+1] == ' '){
            if(grid[newx][newy+2] == ' ') neighs.push_back({newx, newy+2});
        }
        if(grid[newx-1][newy] == ' '){
            if(grid[newx-2][newy] == ' ') neighs.push_back({newx-2, newy});
        }
        if(grid[newx][newy-1] == ' '){
            if(grid[newx][newy-2] == ' ') neighs.push_back({newx, newy-2});
        }
        if(grid[newx+1][newy] == ' '){
            if(grid[newx+2][newy] == ' ') neighs.push_back({newx+2, newy});
        }
        if(neighs.empty()) continue;
        
        int i = rand()%neighs.size();
        auto neigh = neighs[i];
        grid[mnpos[mn].first][mnpos[mn].second] = ' ';
        grid[neigh.first][neigh.second] = 'M';
        mnpos[mn].first = neigh.first, mnpos[mn].second = neigh.second;
    }
}

#endif //MOVES_H