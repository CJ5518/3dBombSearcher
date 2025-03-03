# 3d Bomb Searcher
Minesweeper, but 3d. Called Bomb Searcher because I think it's funny.

![](https://carsonrueber.com/images/3dBombSearcher/ingame.PNG)

The game includes options to set up boards of different size and bomb count, although larger sizes cause lag. Not because of bad rendering code, but because of bad code for checking if the mouse is over a cube.
The CubeEngine, which, while entirely written by me, takes inspiration from some of the ideas in the SFML code, only draws instanced cubes, but is quite quick to do so. 

# Building
The project depends on SFML (for windowing and audio), glm for math, glew for opengl extension wrangling, and imgui for the gui. All of these dependencies are provided in the `extern/` folder.
