
# BattleShip-C_ncurses

## Welcome
Welcome to BattleShip-C_ncurses, a school project developed in May 2024.

## Description
This project is implemented in C and utilizes the ncurses library to create a Text-based User Interface (TUI). It also uses the socket library in C to enable communication with another computer on the same network. 

As always, it is designed to be compiled and run on Linux systems.

## Project Overview

Battleship is a classic game that tests players' strategy and logic. In this project, we are developing a simplified version of Battleship. The game is played in console mode, featuring random ship placement and player-computer interaction.

The goal of the game is to move a character named Pac-Man through a maze to eat all the small yellow pellets present. Within this maze, there are 1 to 4 ghosts that are the enemies of Pac-Man. The user must avoid being caught by any of these ghosts.

## Key Features to Implement

### 1 - Game Board
- Implement a 10x10 grid to represent the ocean.
- Use characters to display ship positions, successful hits, missed hits, etc.

### 2 - Ships
- Create a structure to represent a ship with attributes such as position (coordinates), size, and a sunk indicator.
- Generate three ships of size 3 randomly on the board.

### 3 - Random Placement
- Ensure ships are placed randomly on the board without overlapping.

### 4 - Game Turn
- Implement game logic allowing the player to specify coordinates to shoot at the board.
- Display messages indicating if the shot hit, missed, or sank a ship.

### 5 - Game End
- The game ends when all ships have been sunk.
- Display the number of turns required to win.

## Suggestions for Improvement (Bonus)

### A - Graphical Interface
- Add a graphical interface to enhance the user experience using ncurses.

### B - Difficulty Levels
- Integrate difficulty levels by adjusting ship placement or limiting the number of shots.

### C - Statistics
- Record and display player statistics, such as the number of games played and won.

### D - Customizable Ships
- Allow players to specify ship positions at the beginning of the game.

### E - Multiplayer Mode
- Add a multiplayer mode where two players can play on separate grids.

### F - Bonus : Multiplayer Local Game
- Using C sockets library

## Screenshots

![App Screenshot](https://i.ibb.co/zftHRmR/Screenshot-2024-11-28-215727.png)

![App Screenshot](https://i.ibb.co/gz8YMSM/Screenshot-2024-11-28-215804.png)

![App Screenshot](https://i.ibb.co/Nyq1QsM/Screenshot-2024-11-28-215837.png)

![App Screenshot](https://i.ibb.co/JHtvP8X/Screenshot-2024-11-28-215924.png)

![App Screenshot](https://i.ibb.co/pygnbMb/Screenshot-2024-04-02-225126.png)

## Installation
(Before proceeding, please install ncurses.h on Linux or via WSL on Windows)

To compile and run this project, follow these steps :
1. Clone the repository using `git clone` or Download ZIP.
2. Navigate to the project directory using `cd BattleShip-C_ncurses/src`.
3. Compile the project using :

```
gcc main.c solo.c multi.c client.c serveur.c -o main -lncurses && ./main 
```

## Status Update

Everything was done. 
