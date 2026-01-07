#ifndef GAMESAVER
#define GAMESAVER

#include "includer.h"


int number_of_games(){
    FILE* fptr = fopen("data.txt", "r");
    char* numberofgames;
    fgets(numberofgames,5,fptr);
    return atoi(numberofgames);
}

status load_the_game(int a){
    FILE* fptr = fopen("data.txt", "r");
    string linebuf;

    fgets(linebuf, fptr);       //number of saved games
    for(int i=0; i<9*a-9; i++) fgets(linebuf, LINEMAX, fptr);
    fgets(linebuf, fptr);      //index of the game
    is_saved_game = true;
    status savedgame;
    
    // 1. The number of round
    fgets(linebuf, fptr);
    savedgame.round = atoi(linebuf);

    // 2. The last player that it was his turn
    fgets(linebuf, fptr);
    savedgame.round = atoi(linebuf);


    // 3. The numbers of the draftsmen and their infos: 
    // <id,x,y,temp_token,token_limit,winned,defeated>
    fscanf(fptr, "%d", &drnum);
    int id, x, y, temp_token, token_limit, winned, defeated;
    for(int i=0; i<drnum; i++){
        fscanf(fptr, "<%d,%d,%d,%d,%d,%d,%d> ",
        &id, &x, &y, &temp_token,
        &token_limit, &winned, &defeated)
        savedgame.drs.push_back({id, x, y, temp_token, token_limit, winned, defeated});
    }

    // 4. The numbers of the monsters and their coordinates: <x,y>
    fscanf(fptr, "%d", &mnnum);
    for(int i=0; i<mnnum; i++){
        fscanf(fptr, "<%d,%d> ",savedgame.mns[i].first,savedgame.mns[i].second);
    }

    // 5. The coordinate of the light source: <x,y>
    fscanf(fptr, "<x,y>\n", savedgame.lighpos.first, savedgame.lighpos.second);

    // 6. The numbers of walls and their coordinates: <x,y>
    fscanf(fptr, "%d ", &wlnum);
    for(int i=0; i<wlnum; i++) {
        fprintf(fptr, "<%d,%d> ",&x , &y);
        savedgame.walls.push_back({x,y});
    }

    // 7. The numerbs of 2-round walls and their coordinates: <x,y>
    int temp2num;
    fscanf(fptr, "%d ", &temp2num);
    for(int i=0; i<temp2num; i++){
        fscanf(fptr, "<%d,%d> ", &x, &y);
        savedgame.temp2.push_back({x,y});
    }

    // 8. The numbers of 1-round walls and their coordinates: <x,y>
    int temp1num;
    fscanf(fptr, "%d ", &temp1num)
    for(int i=0; i<temp1num; i++){
        fscanf(fptr, "<%d,%d> ",&x, &y);
        savedgame.temp1.push_back({x,y});
    }

    // 9. The numbers of the chance cubes and their coordinates: <x,y>
    int chancecubenum;
    fscanf(fptr, "%d ", &chnacecubenum);
    for(int i=0; i<chancecubenum; i++){
        fscanf(fptr, "<%d,%d> ", &x, &y);
        savedgame.chancecubes.push_back({x,y})
    }
    fclose(fptr);
    return savedgame;
}

void update_number_of_games(int numberofgames){
    FILE* datap = fopen("data.txt","r");
    FILE* tempp = fopen("data.txt", "w");
    string linebuf;
    fgets(linebuf, MAXLINE, datap);
    fprintf(tempp, itoa(numberofgames));
    while(fgets(linebuf, LINEMAX, datap)){
        fprintf(tempp, linebuf);
    }
    fclose(datap);
    fclose(tempp);
}

int save_the_game(status& game){    //returns the index of the last-appended game
    /*
    The format of 'data' file:
    before the statuses, the number of status is written
    every status's first information is the number of it that begins
    every line contains (in order) this infos:
    */
    int gamenum = number_of_games();
    
    update_number_of_games(gamenum);

    FILE* fptr = fopen("data.txt", "w");
    if(fptr==NULL)return -1;

    // 1. The number of round
    fprintf(fptr, "%d\n", game.round);
    
    // 2. The last player that it was his turn
    fprintf(fptr, "%d\n", game.lastplayer);

    // 3. The numbers of the draftsmen and their infos: 
    // <id,x,y,temp_token,token_limit,winned,defeated>
    fprintf(fptr, "%d ", game.drs.size());
    for(int i=0; i<dr.size(); i++){
        fprintf(fptr, "<%d,%d,%d,%d,%d,%d,%d> ",
            game.drs[i].id, games.drs[i].x, game.drs[i].y, game.drs[i].temp_token,
            game.drs[i].token_limit, game.drs[i].winned, game.drs[i].defeated)
    }
    fprintf(fptr, "\n");

    // 4. The numbers of the monsters and their coordinates: <x,y>
    fprintf(fptr, "%d ", mnnum);
    for(int i=0; i<mnpos.size(); i++){
        fprintf(fptr, "<%d,%d> ",game.mns[i].first,game.mns[i].second);
    }
    fprintf(fptr, "\n");
    
    // 5. The coordinate of the light source: <x,y>
    fprintf(fptr, "<x,y>\n", game.lighpos.first, game.lighpos.second);

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


    fclose(fptr);
    retrun 1;
}

#endif //GAMESAVER