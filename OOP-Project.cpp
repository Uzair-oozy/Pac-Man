#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <fstream>
using namespace std;

// Blockade class for obstacles
class Blockades 
{
private:
    vector<pair<int, int>> blocks;
    char symbol;
public:
    Blockades(char s = '|') : symbol(s) {}

    void addBlock(int x, int y) 
    {
        blocks.push_back({x, y});
    }

    bool isBlock(int x, int y) const 
    {
        for (const auto& b : blocks) 
        {
            if (b.first == x && b.second == y)
            {
                return true;
            }
        }
        return false;
    }

    void draw(vector<string>& grid) const 
    {
        for (const auto& b : blocks) 
        {
            grid[b.second][b.first] = symbol;
        }
    }
    // Getter for block positions
    const vector<pair<int, int>>& getBlocks() const 
    { 
        return blocks; 
    }
};


// Base class for all entities in the game
class Entity 
{
private:
    int x, y;
    char symbol;
public:
    Entity(int x, int y, char s) : x(x), y(y), symbol(s) {}

    virtual void draw(vector<string>& grid) const 
    {
        grid[y][x] = symbol;
    }

    void setX(int x)
    {
        this->x = x;
    }

    void setY(int y)
    {
        this->y = y;
    }

    int getX() const 
    { 
        return x; 
    }

    int getY() const 
    { 
        return y; 
    }

    virtual ~Entity() {}
};

// Player class for 2D game
class Player2D : public Entity 
{
private:
    int score;
    string name;
public:
    Player2D(int x, int y, string n) : Entity(x, y, 'P'), score(0), name(n) {}

    void move(char dir, int width, int height, const Blockades& blockades)
    {
        int y = getY();
        int x = getX();
        int nextX = x, nextY = y;
        if (dir == 'w' && y > 0)
        {
            nextY--;
        }
        else if (dir == 's' && y < height - 1)
        {
            nextY++;
        }
        else if (dir == 'a' && x > 0)
        {
            nextX--;
        }
        else if (dir == 'd' && x < width - 1)
        {
            nextX++;
        }
        if (!blockades.isBlock(nextX, nextY))
        {
            setY(nextY);
            setX(nextX);
        }
    }

    void addScore(int s) 
    { 
        score += s; 
    }

    int getScore() const 
    { 
        return score; 
    }

    string getName() const 
    { 
        return name; 
    }

    void draw(vector<string>& grid) const override 
    {
        grid[getY()][getX()] = 'P';
    }
};

// Enemy class for 2D game
class Enemy2D : public Entity 
{
public:
    Enemy2D(int x, int y) : Entity(x, y, 'E') {}

    void chase(const Player2D& player, int width, int height, const Blockades& blockades)
    {
        int ex = getX();
        int ey = getY();
        int targetX = ex, targetY = ey;
        if (ex < player.getX() && ex < width - 1 && !blockades.isBlock(ex + 1, ey))
        {
            targetX = ex + 1;
        }
        else if (ex > player.getX() && ex > 0 && !blockades.isBlock(ex - 1, ey))
        {
            targetX = ex - 1;
        }

        if (ey < player.getY() && ey < height - 1 && !blockades.isBlock(targetX, ey + 1))
        {
            targetY = ey + 1;
        }
        else if (ey > player.getY() && ey > 0 && !blockades.isBlock(targetX, ey - 1))
        {
            targetY = ey - 1;
        }
        setX(targetX);
        setY(targetY);
    }

    void draw(vector<string>& grid) const override 
    {
        grid[getY()][getX()] = 'E';
    }
};

// Template class for collectibles
template <typename T>
class Collectible 
{
private:
    vector<pair<int, int>> items;
    char symbol;
    T value;
public:
    Collectible(char s, T v) : symbol(s), value(v) {}

    void addItem(int x, int y)
    {
        items.push_back({x, y});
    }

    // Getter for item positions
    const vector<pair<int, int>>& getItems() const
    {
        return items;
    }

    bool collect(int px, int py)
    {
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            if (it->first == px && it->second == py)
            {
                items.erase(it);
                return true;
            }
        }
        return false;
    }

    void draw(vector<string>& grid) const
    {
        for (const auto& item : items)
        {
            grid[item.second][item.first] = symbol;
        }
    }

    T getValue() const 
    { 
        return value; 
    }

    bool empty() const 
    { 
        return items.empty(); 
    }
};

int main() 
{
    srand((unsigned)time(0));
    const int width = 15, height = 10;

    string name, highScorePlayerName;
    cout << "Welcome to the 2D Game!" << endl;
    cout << "Enter Your Name: ";
    cin >> name;

    Player2D player(0, 0, name);
    Enemy2D enemy(width - 1, height - 1);
    Collectible<int> coins('C', 10);
    const int numCoins = 20;
    Blockades blockades('|');
    const int numBlocks = 15;
    int highestScore = 0;

    // Load highest score from file
    ifstream infile("highest_score.txt");
    if (infile.is_open())
    {
        getline(infile, highScorePlayerName); // Read the full first line (name, including spaces)
        infile >> highestScore;                // Read the score from the second line
        infile.close();
    }

    // Initialize coins with random positions
    // Ensures coins do not overlap with player, enemy or another coin
    for (int i = 0; i < numCoins; ++i) 
    {
        bool copyFound = false;
        int cx[20];
        int cy[20]; 
        cx[i] = rand() % width;
        cy[i] = rand() % height;

        for(int j = 0; j < i; j++)
        {
            if ((cx[i] == cx[j]) && (cy[i] == cy[j]))
            {
                copyFound = true;
                break;
            }
        }

        if ((cx[i] != 0 || cy[i] != 0) && (cx[i] != width - 1 || cy[i] != height - 1) && (copyFound == false))
        {
            coins.addItem(cx[i], cy[i]);
        }
        else
        {
            --i;
        }
    }

    // Initialize blockades with random positions (not on player, enemy, or overlapping)
    for (int i = 0; i < numBlocks; ++i) 
    {
        int bx = rand() % width;
        int by = rand() % height;
        // Don't place on player, enemy, or duplicate
        if ((bx == 0 && by == 0) || (bx == width - 1 && by == height - 1)) 
        {
            --i;
            continue;
        }
        bool duplicate = false;
        // Check for duplicate blockades
        for (const auto& b : blockades.getBlocks()) 
        {
            if (b.first == bx && b.second == by) 
            {
                duplicate = true;
                break;
            }
        }
        // Check for overlap with coins
        for (const auto& item : coins.getItems()) 
        {
            if (item.first == bx && item.second == by) 
            {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) 
        {
            blockades.addBlock(bx, by);
        } 
        else 
        {
            --i;
        }
    }

    bool gameOver = false;

    while (!gameOver) 
    {
        system("cls");
        vector<string> grid(height, string(width, '_'));
        blockades.draw(grid);
        coins.draw(grid);
        player.draw(grid);
        enemy.draw(grid);
        // Add spacing between underscores for better visibility (after drawing collectibles/players/enemy)
        for (auto& row : grid) 
        {
            string spaced;
            for (char c : row) 
            {

                spaced += c;
                spaced += ' ';
            }
            row = spaced;
        }
        cout << "\tGame Grid" << endl;
        cout << endl;
        cout << "Collect As Many Coins As You Can!" << endl;
        cout << endl;

        for (const auto& row : grid) 
        {
            cout << '|' << row << '|' << endl;
            cout << endl;
        }

        cout << "Score: " << player.getScore() << endl;
        cout << "Highest Score: " << highestScore << " By Player: " << highScorePlayerName << endl;
        cout << "Move (W/A/S/D), Q to Quit: ";
        char move;
        cin >> move;

        if (move == 'q') break;

        // Calculate next position for player using new move
        player.move(move, width, height, blockades);

        if (coins.collect(player.getX(), player.getY())) 
        {
            player.addScore(coins.getValue());
            cout << "Collected a Coin!" << endl;
            if (player.getScore() > highestScore)
            {
                highestScore = player.getScore();
                highScorePlayerName = player.getName();
                cout << "Congratulations! New Highest Score: " << highestScore << " By Player: " << highScorePlayerName << endl;
            }
        }

        // Enemy movement using new chase
        enemy.chase(player, width, height, blockades);

        if (player.getX() == enemy.getX() && player.getY() == enemy.getY()) 
        {
            cout << "You Were Caught By The Enemy! Game Over!" << endl;
            gameOver = true;
        }

        if (coins.empty()) 
        {
            cout << "You Collected All The Coins! You Won!" << endl;
            gameOver = true;
        }
    }
    cout << "Final Score: " << player.getScore() << endl;
    cout << "Highest Score: " << highestScore << endl;

    // Save highest score to file if beaten
    ofstream outfile("highest_score.txt");
    if (outfile.is_open())
    {
        outfile << highScorePlayerName << endl;
        outfile << highestScore;
        outfile.close();
    }
    return 0;
}

