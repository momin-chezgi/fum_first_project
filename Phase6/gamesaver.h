#ifndef GAMESAVER
#define GAMESAVER

#include "includer.h"


int number_of_games(){
    ifstream fin("data");
    int numofgames=0;
    fin >> numofgames;
    fin.close();
    return numofgames;
}

int str2int(char* str, int startid, char endch){
    int a = 0;
    int line_ln=startid;
    for(; str[line_ln]!=endch; line_ln++);
    for(int l=startid; l<line_ln; l++){
        int digit = str[l]-'0';
        a += digit*pow(10,line_ln-1-l);
    }
    return a;
}

void saved_game(int a){
    FILE* fptr = fopen("data", "r");
    char file_line[LINEMAX];
    char* lptr = file_line;

    int game_index=-1;
    while(game_index < a){
        fgets(file_line, LINEMAX, fptr);
        if(file_line[0]>='0' && file_line[0]<='9') game_index++;
        if(file_line[0]=='#' && file_line[1]=='e') fclose(fptr);
    }

    //The number of round...
    fgets(file_line, LINEMAX, fptr);
    saved_status.round = str2int(file_line,0,'\0');

    //The last player that was his turn...
    fgets(file_line, LINEMAX, fptr);
    saved_status.who_was_the_last = str2int(file_line,0,'\0');

    //The numbers of the draftsmen and the infos of them: <id,x,y,temp_token,token_limit,winned,defeated>
    fgets(file_line, LINEMAX, fptr);
    drnum = str2int(file_line,0,' ');
    lptr += (int)log10(drnum)+2;
    for(int d=0; d<drnum; d++){
        saved_status.drs[d] = str2int(lptr)
    }
    
    
}

int save_the_game(int last_player=0,
    vector<intpair>& walls,
    vector<intpair>& temp2,
    vector<intpair>& temp1,
    vector<intpair>& chancecubes
){
    /*
    The format of 'data' file:
    before the statuses, the number of status is written
    every status's first information is the number of it that begins
    every line contains (in order) this infos:
    */
    int gamenum = number_of_games();
    
    FILE* fptr = fopen("data", "w");
    if(fptr==NULL)return -1;

    // 1. The number of round
    fprintf(fptr, "%d\n", gamenum);
    
    // 2. The last player that it was his turn
    fprintf(fptr, "%d\n", last_player);

    // 3. The numbers of the draftsmen and their infos: 
    // <id,x,y,temp_token,token_limit,winned,defeated>
    fprintf(fptr, "%d ", dr.size());
    for(int i=0; i<dr.size(); i++){
        fprintf(fptr, "<%d,%d,%d,%d,%d,%d,%d> ",
            dr[i].id, dr[i].x, dr[i].y, dr[i].temp_token, dr[i].token_limit, dr[i].winned, dr[i].defeated)
    }
    fprintf(fptr, "\n");

    // 4. The numbers of the monsters and their coordinates: <x,y>
    fprintf(fptr, "%d ", mnnum);
    for(int i=0; i<mnpos.size(); i++){
        fprintf(fptr, "<%d,%d> ",mnpos[i].first,mnpos[i].second);
    }
    fprintf(fptr, "\n");
    
    // 5. The coordinate of the light source: <x,y>
    fprintf(fptr, "<x,y>\n", light_source_pos.first, light_source_pos.second);

    // 6. The numbers of the numbers of walls and their coordinates: <x,y>
    fprintf(fptr, "%d ", wlnum);
    for(int i=0; i<walls.size(); i++) fprintf(fptr, "<%d,%d> ",walls[i].first, walls[i].second);
    fprintf(fptr, "\n");

    // 7. The numerbs of 2-round walls and their coordinates: <x,y>
    fprintf(fptr, "%d ", temp2.size())
    for(int i=0; i<temp2.size(); i++) fprintf(fptr, "<%d,%d> ",temp2[i].first, temp2[i].second);
    fprintf(fptr, "\n");

    // 8. The numbers of 1-round walls and their coordinates: <x,y>
    fprintf(fptr, "%d ", temp1.size())
    for(int i=0; i<temp1.size(); i++) fprintf(fptr, "<%d,%d> ",temp1[i].first, temp1[i].second);
    fprintf(fptr, "\n");

    // 9. The numbers of the chance cubes and their coordinates: <x,y>
    fprintf(fptr, "%d ", chancecubes.size());
    for(int i=0; i<chancecubes.size(); i++) fprintf(fptr, "<%d,%d> ", chancecubes[i].first, chancecubes[i].second);
    
    //change the number of games in line 1(?)

    fclose(fptr);

    retrun 1;
}

#endif //GAMESAVER