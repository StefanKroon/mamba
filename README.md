# My personal MAMBA remake (1997)

When I was 17 years old, I made my own implementation on MS-DOS of one of my favorite games, [Mamba](https://nl.wikipedia.org/wiki/Mamba_(spel)) (made by Bert Uffen). Mamba is a snake-like video game with over [200](https://www.mobygames.com/group/3173/snake-variants/sort:date/page:1/) different variants. I used Turbo C++ from Borland for coding, compiling and building the game.

I found the game on a old hard disk with file timestamps still preserved. I have been creating two variants of the game. The first one is an as-close-as-possible copy, called MAMBAS. It was finished in June 1997. The second version was a variant with cheat codes in it. It is called MAMBAC. It was completed in December 1997.

Part of the files I found are corrupt. Therefore I didn't try to recompile the games, but I have added the final executables to this repository. I have also added the original game into the [EXE-directory](EXE/). Small differences can be noticed between the original game and the MAMBAS: slightly different sounds, and the level number in the top bar is indicated by `01` instead of `1`.

## How to run
Download your [DOSBox Frontend](https://www.dosbox.com/download.php?main=1), download the EXE-file from the [EXE-directory](EXE/) and start playing.

## Cheat mode (works with MAMBAC only)

|Code |Name | Description                                                                             |
|-----|-----|-----------------------------------------------------------------------------------------|
|SSSK |Activate Cheat Mode | Needs to be activate before other cheat codes will work                  |
|G    |God Mode | The Mamba will automatically turn into another direction when a wall or tail is hit |
|B    |Bonus | The amount of bonuses will increase by one                                             |
|A    |Autosearch | The Mamba will automatically search for food
|D    | Dead end detection | The Mamba will automatically try to avoid to go into a dead end
|+    | Level Up | Level will go up by 1
