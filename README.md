# NIDS
Naomi Interior Design Simulator. A simple interior design sim I had written in GameMaker Studio 1.4, which is deprecated. Rewritten in SDL2!

# MASTER BRANCH
This branch contains the source code for the game. The game was written in C++/SDL2 on a GNU/Linux machine, and therefore the makefile works for linux only.
To build this, simply clone the repository and use the `make` command to build the project. Thats it!

# Linux Installation
Clone the repository to whereever you would like the game installed. Run `make` in the base directory. An executable called `NIDS` will be created. You must have SDL2, SDL2_image, and SDL2_ttf. 

# Windows Installation
The windows release package is available [here](https://github.com/CoralRocker/NIDS/releases). Unzip the zip file that contains the latest release of the game, and extract the contents to wherever you'd like the game directory to be. The zip file contains folders and DLL files which allow the game to run correctly. PLEASE DONT MOVE THESE. If you'd like to have the game on your desktop, make a shortcut. The NIDS.exe file is statically linked to the C and C++ windows libraries, so you will not have to download any extraneous DLLs or redistributeables. However, zlib1 and libpng16 are not currently statically linked to the game, so DLLs are provided within the zip file. 

# TODO
* Add more objects
* More in-game options, including movement amount and animation speeds
* Player-Created objects
* Add an undo buffer
* Recreate the color selection UI to be more communicative
* Add color modification mode option?
* Refactor object menu selection
* Finish Maggie
* Remove glitching wall bug when placing modular table
* Improve performance?
