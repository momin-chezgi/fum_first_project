#include "interpreter.h"
#include "mazegenerator.h"

int main()
{
    importer();
    vec2d(char) shown_grid(n, vector<char>(m));
    mazegenerator(shown_grid);
    bool end_the_game = false;
    vector<intpair> draftsmen;
    vector<intpair> monsters;
    intpair light_source;
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            switch (shown_grid[2*i+1][2*j+1])
            {
            case 'S':
                light_source = intpair(i,j);
                break;
            case 'M':
                monsters.push_back(intpair(i,j));
                break;
            case 'D':
                draftsmen.push_back(intpair(i,j));
                break;
            default:
                break;
            }
        }
    }
    while(!end_the_game)
    {
        for (int player=0; player<drnum; player++)
        {

        }
    }
    return 0;
}