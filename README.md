+Authors:
Mohsen Afgahi Zadeh      mohsenfqz
Mohammad Amin Chezgi     momin-chezgi

For translation, we prefered to use below words:
Draftsman for the player.
monster for the creatures that used to live under the ground in the maze.
Light-sourec for the final goal.

'1.cpp' is for the first phase.
'2.cpp' is for the second.
And so on.

Tips:
1. Regarding to the access of Mr.Abrishamy, we're using C++ files and compilers instead of C.
2. We use two differnt IDEs: VSCode and CLion.
3. Because of the different IDE, we added two folders for two IDEs proccesses such as compiler.(but actually they did, we didn't!)


1. First Phase, Printing the status:
  I used terminal for printing the status of the grid. (and it will soon be expanded for presenting the game)
this is a bit different from the document presentation, Because it prints corners, walls and cells.
Any sign has a meaning:
'#' shows that it's either a corner or a wall.
'.' shows an empty cell.
'S' shows the light-source.
'M' shows a monster.
'D' shows a draftman.
  +Testing status: passed! but requiers more test cases.

2. Second Phase, generating a random maze:
  We explained all of the steps in the code with commands. here you are the notes that is not there:
One thing remains that I should tell. We decided to write the codes completely(or maybe not) and after all of them, run tests.
It's owing to the semester tests and assuring to be a base code, whether it reaches at the final milestone or not.
code lines boosted and we decided to break them down into some files with .h suffix. they're DSU.h and importer.h(current interpreter.h)
  +Algorithm insights: We decided to lead our maze on the spanning tree idea. In the spanning tree, Any vertix represents a cell and 
any edge between two vertices shows that you can't add a wall between them. then a DSU algorithm runs and now all of the cells are connected together.
after that, we place walls randomly on the possible places for walls.
at the end, we place draftsmen and monsters with a greedy algorithm.(But at the worst case it maybe stuck to a loop)
  +Testing status: No tests

4. Third Phase, basic gaming:
   A significant change was in the file distrubting. so we didn't use 2.cpp for generating a maze and let it still unchanged.
we copied and edited 2.cpp to mazegenerator.h , edited interpreter.h and added includer.h.
  +Testing status: No tests
