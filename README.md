# Pac-Man
A Pac-Man type game built using C++

## 2D Console Game - Coin Collection Game

A console-based 2D game built with C++ that demonstrates Object-Oriented Programming (OOP) principles. Navigate a grid, collect coins, and avoid an intelligent enemy that chases you!

### Features

- **Player-Controlled Character**: Move using WASD keys (W=up, A=left, S=down, D=right)
- **Enemy AI**: Intelligent enemy that automatically chases the player toward them
- **Collectible Items**: Coins scattered across the grid worth 10 points each
- **Obstacles**: Static blockades that block movement for both player and enemy
- **Score Tracking**: Real-time score display and persistent high score system
- **Win/Lose Conditions**:
  - Win: Collect all coins on the map
  - Lose: Get caught by the enemy
- **High Score Persistence**: Game saves the highest score to `highest_score.txt`

### Game Mechanics

- **Grid-Based Gameplay**: 15×10 grid with random coin and blockade placement
- **Collision Detection**: Prevents overlapping of coins, blockades, player, and enemy
- **Smart Movement**: Obstacles block pathfinding for all entities
- **Game Loop**: Continuous turn-based gameplay until win/lose condition

### Technical Details

- **Language**: C++ with STL (Standard Template Library)
- **OOP Concepts**:
  - **Base Class**: `Entity` for all game objects
  - **Inheritance**: `Player2D` and `Enemy2D` extend Entity
  - **Templates**: `Collectible<T>` template class for flexible item management
  - **Encapsulation**: Private member variables with public getters/setters
- **Data Structures**: Vectors for dynamic storage, pairs for 2D coordinates
- **File I/O**: Persistent high score storage using file streams

### Classes

- **Entity**: Base class for game objects (x, y position, symbol)
- **Player2D**: Player character with movement and score tracking
- **Enemy2D**: AI-controlled enemy with chase logic
- **Blockades**: Obstacle management and collision detection
- **Collectible<T>**: Template class for collectible items (coins)

### How to Play

1. Compile and run the program
2. Enter your name when prompted
3. Use **W/A/S/D** to move the player ('P') around the grid
4. Collect all coins ('C') to win
5. Avoid the enemy ('E') that chases you
6. Type **Q** to quit at any time
7. High score is automatically saved when beaten

### Files

- OOP-Project.cpp - Complete game source code with all classes and logic
- `highest_score.txt` - Auto-generated file storing highest score and player name

### Game Symbols

- **P** - Player character
- **E** - Enemy
- **C** - Collectible coin
- **|** - Blockade/Obstacle
- **_** - Empty space

### Requirements

- C++11 or later
- Standard C++ library (iostream, vector, string, fstream)
- Windows system (uses `system("cls")` for console clearing)

### Notes

- Educational project demonstrating OOP principles and game development basics
- Randomized coin and blockade placement for replay value
- Console-based UI with real-time grid updates
