#ifndef SAVEANDLOAD
#define SAVEANDLOAD

#include "includer.h"
#include <cstring>

int number_of_games(){
    FILE* fptr = fopen("data.txt", "r");
    int numofgames;
    fscanf(fptr, "%d", &numofgames);
    fclose(fptr);
    return numofgames;
}

bool read_int(FILE* fptr, int* out){
    char linebuf[LINEMAX];
    if(!fgets(linebuf, LINEMAX, fptr)) return false;
    return sscanf(linebuf, "%d", out) == 1;
}

bool read_intpairs(FILE* fptr, vector<intpair>& out){
    char linebuf[LINEMAX];
    if(!fgets(linebuf, LINEMAX, fptr)) return false;

    char* p = linebuf;
    int count = 0;
    sscanf(p, "%d", &count);
    p = strchr(p, '<');
    for(int i=0; i<count; i++){
        int x, y;
        if(sscanf(p, " <%d,%d>", &x, &y) != 2) return false;
        out.push_back({x,y});
        p = strchr(p, '>') + 1;
    }
    return true;
}

bool read_draftsmen(FILE* fptr, vector<draftsman>& out){
    char linebuf[LINEMAX];
    if(!fgets(linebuf, LINEMAX, fptr)) return false;

    char* p = linebuf;
    int count = 0;
    sscanf(p, "%d", &count);
    p = strchr(p, '<')-1;
    for(int i=0; i<count; i++){
        int id, x, y, temp_token, token_limit, winned, defeated;
        if(sscanf(p, " <%d,%d,%d,%d,%d,%d,%d>",
                    &id, &x, &y, &temp_token, &token_limit, &winned, &defeated ) != 7) return false;
        out.emplace_back(draftsman{id,x,y,temp_token,token_limit,(bool)winned,(bool)defeated});
        p = strchr(p, '>') + 1;
    }
    return true;
}

inline void skip_games(FILE* fptr, int index){
    char linebuf[LINEMAX];
    for(int i=0; i<11*index+1; i++){
        fgets(linebuf, LINEMAX, fptr);
    }
}

status load_the_game(int index){
    is_saved_game = true;
    status savedg;

    FILE* fptr = fopen("data.txt", "r"); 
    char linebuf[LINEMAX];

    skip_games(fptr, index);

    
    // 1. The number of round
    read_int(fptr, &savedg.round);

    // 2. The last player that it was his turn
    read_int(fptr, &savedg.lastplayer);

    // 3. The numbers of the draftsmen and their infos: 
    // <id,x,y,temp_token,token_limit,winned,defeated>
    read_draftsmen(fptr, savedg.drs);

    // 4. The numbers of the monsters and their coordinates: <x,y>
    read_intpairs(fptr, savedg.mns);

    // 5. The coordinate of the light source: <x,y>
    read_intpairs(fptr, savedg.lighpos);

    // 6. The numbers of walls and their coordinates: <x,y>
    read_intpairs(fptr, savedg.walls);

    // 7. The numerbs of 2-round walls and their coordinates: <x,y>
    read_intpairs(fptr, savedg.temp2);

    // 8. The numbers of 1-round walls and their coordinates: <x,y>
    read_intpairs(fptr, savedg.temp1);

    // 9. The numbers of the chance cubes and their coordinates: <x,y>
    read_intpairs(fptr, savedg.chancecubes);

    // 10,11. dimensions of the grid
    read_int(fptr, &savedg.n);
    read_int(fptr, &savedg.m);
    

    fclose(fptr);

    return savedg;
}

int update_number_of_games() {
    FILE* datap = fopen("data.txt", "r");
    FILE* tempp = fopen("temp.txt", "w");
    if(datap == NULL || tempp == NULL){
        cerr << "Error: Files don't open...\n";
        exit(-1);
    }
    char linebuf[LINEMAX];
    
    // Get the number of games
    fgets(linebuf, LINEMAX, datap);
    int saved_gamenum = atoi(linebuf);

    // Write INCREMENTED count to temp.txt
    fprintf(tempp, "%d\n", saved_gamenum + 1);  
    
    // Copy the rest of data.txt to temp.txt
    while(fgets(linebuf, LINEMAX, datap)) {
        fprintf(tempp, "%s", linebuf);
    }
    
    fclose(datap);
    fclose(tempp);

    // Copy back from temp.txt to data.txt
    datap = fopen("data.txt", "w");
    tempp = fopen("temp.txt", "r");
    while(fgets(linebuf, LINEMAX, tempp)) {
        fprintf(datap, "%s", linebuf);
    }

    fclose(datap);
    fclose(tempp);
    
    return saved_gamenum;  // Returns the OLD count
}

int save_the_game(status& game){    //returns the index of the last-appended game
    /*
    The format of 'data' file:
    before the statuses, the number of status is written
    every status's first information is the number of it that begins
    every line contains (in order) this infos:
    */

    int gamenum = update_number_of_games();

    FILE* fptr = fopen("data.txt", "a");
    if(fptr==NULL)return -1;

    // 1. The number of round
    fprintf(fptr, "%d\n", game.round-1);
    
    // 2. The last player that it was his turn
    fprintf(fptr, "%d\n", game.lastplayer);

    // 3. The numbers of the draftsmen and their infos: 
    // <id,x,y,temp_token,token_limit,winned,defeated>
    fprintf(fptr, "%d ", game.drs.size());
    for(int i=0; i<dr.size(); i++){
        fprintf(fptr, "<%d,%d,%d,%d,%d,%d,%d> ",
            game.drs[i].id, game.drs[i].x, game.drs[i].y, game.drs[i].temp_token,
            game.drs[i].token_limit, game.drs[i].winned, game.drs[i].defeated);
    }
    fprintf(fptr, "\n");

    // 4. The numbers of the monsters and their coordinates: <x,y>
    fprintf(fptr, "%d ", mnnum);
    for(int i=0; i<mnpos.size(); i++){
        fprintf(fptr, "<%d,%d> ",game.mns[i].first,game.mns[i].second);
    }
    fprintf(fptr, "\n");

    // 5. The coordinate of the light source: <x,y>
    fprintf(fptr, "1 <%d,%d>\n", game.lighpos[0].first, game.lighpos[0].second);

    // 6. The numbers of walls and their coordinates: <x,y>
    fprintf(fptr, "%d ", wlnum);
    for(int i=0; i<game.walls.size(); i++) fprintf(fptr, "<%d,%d> ",game.walls[i].first, game.walls[i].second);
    fprintf(fptr, "\n");

    // 7. The numerbs of 2-round walls and their coordinates: <x,y>
    fprintf(fptr, "%d ", game.temp2.size());
    for(int i=0; i<game.temp2.size(); i++) fprintf(fptr, "<%d,%d> ",game.temp2[i].first, game.temp2[i].second);
    fprintf(fptr, "\n");

    // 8. The numbers of 1-round walls and their coordinates: <x,y>
    fprintf(fptr, "%d ", game.temp1.size());
    for(int i=0; i<game.temp1.size(); i++) fprintf(fptr, "<%d,%d> ",game.temp1[i].first, game.temp1[i].second);
    fprintf(fptr, "\n");

    // 9. The numbers of the chance cubes and their coordinates: <x,y>
    fprintf(fptr, "%d ", game.chancecubes.size());
    for(int i=0; i<game.chancecubes.size(); i++) fprintf(fptr, "<%d,%d> ", game.chancecubes[i].first, game.chancecubes[i].second);
    fprintf(fptr, "\n");

    // 10,11. dimensions of the grid
    fprintf(fptr, "%d\n", game.n);
    fprintf(fptr, "%d\n", game.m);

    fclose(fptr);

    return gamenum;
}

#endif //SAVEANDLOAD