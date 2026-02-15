+Authors:
Mohsen Afgahi Zadeh      @mohsenfqz
Mohammad Amin Chezgi     @momin-chezgi

+Translation:
Darftsman instead of runner
Monster instead of hunter

+Phases:
1.Phase1, Printing the maze(with given data)
2.Phase2, Generating a valid maze
3.Phase3, Minimal gaming 
4.Phase4, intermediate gaming
5.Phase5, advanced gaming
6.Phase6, saving and loading

+Explainations:
1.Phase1:
  The base of saving the status is the 2-dimension vector of chars. grid[2*n+1][2*m+1].
  In the box below, we're going to intro the char standards that are used in the representing the game status. This standards wasn't declared at the first time completely, but at time passed, we completed all of the char signs.
.............chars sign standards:
  '#': Shows either a corner or a wall.
  'S': The place of lightsource.
  'D': The place of a draftsman.
  'M': The place of a monster.
  'd': The place of the current draftsman/player which is his turn to move
  'C': The place of Chance cube
  '2'\'1': A temprorary wall that the number shows the reminded turns to be disappeared.
...............
  It's obvious that this part doesn't need so much tests.
2.Phase2:
  The toughest pahse! The heart of the generating a valid maze is the spanning tree. It was implemented by DSU algorithm. Beside the maze-generator, a secure interpreter was needed to not allow the user to enter the harmful/unvalid inputs. This is the flow of that phase:
  place_the_lightsource()->put_the_walls()->[create_the_spanning_tree()]->place_the_draftsmen()->place_the_monsters();
  I should mention that after this phase, we breaked down the code to several headers and one main file.
..........limits of the given parameters:
    n,m : [2,50]
    drnum+mnnum : [1 , (n*m)/9]
      why? because each draftman/monster
                threatens up to 8 cells around it that 
                can't contain monsters/draftsmen.
    wlnum : [0 , n*m - n - m]
.............
  In the giant inputs, the wall numbers isn't the matter of the time. But when we increase the numbers of draftsmen and monsters(especially when they're so close) the time increases significantly! At the worst test case that we had, the time of rendering the map was about 10 minutes!
3&4.Phase3&4:
  Sadly the movement of monster was not definitly clear. So we made a logic for monster move: every monster after a round, looks around them, the nearest draftsman be detected by Im_hungry()(In phase 4). And other things that was mentioned in the .pdf document.
  tests were passed successfully.
5.Phase5:
  A minor but significantly change was happened in the monsters. Before it, a monster was looking on the 'previous' position and decided where to move. But after this any monster would decide after the draftsman move. And it became a deadful update for draftsmen! Because catching a draftsman would be much easier and faster! And of course temprorary walls and chance cubes helped them. 
  Anohter update of monsters move was for being able to pass the light source or not. Before it monsters could jump over a light source. But we've finished that and decided to be as a block for mondters.
  bunches of tests with a lot of bugs have been. You can see the commit messages to see some of them. 
6.Phase 6:
  I haven't use streams in C++ for file handling. Indstead of that, we used C fuctions like fgets() and sscanf(). First, the file line be written in the buffer and after that be parsed. The two data.txt and temp.txt file.








