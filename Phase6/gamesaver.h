#ifndef GAMESAVER
#define GAMESAVER

#include "includer.h"
#include "structures.h"

extern status saved_status;


int number_of_games(){
    int a = 0;
    FILE* fptr = fopen("data", "r");
    char file_line[LINEMAX];
    
    while(1){
        fgets(file_line, LINEMAX, fptr);
        if(file_line[0]<='9' && file_line[0]>='0') a++;
        if(file_line[0]=='#' && file_line[1]=='e'){
            fclose(fptr);
            return a;
        }
    }
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

int save_the_game(){

}



/*
The format of 'data' file:
data begins with #start and ends with #end
every status's first information is the number of it that begins
every line contains (in order) this infos:
1. The number of round
2. The last player that it was his turn
3. The numbers of the draftsmen and the infos of them: <id,x,y,temp_token,token_limit,winned,defeated>
4. The numbers of the monsters and the coordinates of them: <x,y>
5. The coordinate of the light source: <x,y>
6. The numbers of the numbers of walls and the coordinates of them: <x,y>
7. The numerbs of 2-round walls and the coordinates of them: <x,y>
8. The numbers of 1-round walls and the coordinates of them: <x,y>
9. The numbers of the chance cubes and the coordinate of them: <x,y>
*/

#endif //GAMESAVER