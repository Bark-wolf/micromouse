#include <iostream>
#include <string>
#include<vector>
#include "vec2.h"
#include "API.h"
#include <queue>
#include <string>
#include <sstream>


int counter = 0;

class cell {
private:
    int distance; unsigned char walls; bool c; bool v;
    //bitwise operations to improve efficiency
public:
    cell() {
        distance = 0;//default to max
        walls = 0b0000;//default to no walls
        c = false;
        v = false;
        
    }
    void set_c(bool f) {
        c = f;
    }
    bool get_c() {
        return c;
    }
    void set_v(bool v) {
        this->v = v;
    }
    bool get_v() {
        return v;
    }
    unsigned char get_walls() {
        return walls;
    }
    void set_walls(unsigned char w) {
        walls = w;
    }
    void set_distance(int d) {
        distance = d;
    }
    int get_distance() {
        return distance;
    }
    std::string cell_to_string() {
        
        std::stringstream ss;
        ss << distance;
        std::string str = ss.str();
        return str;
    }
    

};
/*
North - 0b0001
West - 0b0010
South - 0b0100
East - 0b1000
maze is 16 x 16
goal is 7,7 7,8 8,8 8,7
*/
std::vector<vec2> goal = { {7,7}, {7,8}, {8,7} , {8,8} };
vec2 current;
int maze_width = 16; int maze_height = 16;
class maze {
private:
    std::vector<std::vector<cell>> cell_map;
    std::queue<unsigned char> movement;
public:
    void flood_fill(std::vector<vec2>goal , vec2 start) {
        std::queue<vec2> queue;
        for (int row = 0; row < cell_map.size(); ++row) {
            for (int col = 0; col < cell_map[row].size(); ++col) {
                cell_map[row][col].set_c(false);
                cell_map[row][col].set_v(false);
            }
        }
        for (int i = 0; i < goal.size(); i++) {
            queue.push(goal[i]);
            cell_map[goal[i].y][goal[i].x].set_c(true);
        }
        while (!queue.empty()) {
            std::cout <<"no of loops occuring: "<< counter << '\n';
            counter++;
            int row = queue.front().y;
            int col = queue.front().x;
            queue.pop();
            if (row > -1 && row < 16 && col >-1 && col < 16) {
                if (!cell_map[row][col].get_v()) {
                    //north
                    if (row < 15) {
                        if (!(cell_map[row][col].get_walls() & 0b0001) && !cell_map[row + 1][col].get_c()) {
                            if (!cell_map[row + 1][col].get_c()) {
                                cell_map[row + 1][col].set_c(true);
                                cell_map[row + 1][col].set_distance(cell_map[row][col].get_distance() + 1);
                            }
                            queue.push(vec2(row + 1, col));
                        }
                    }
                    //west
                    if (col > 0) {
                        if (!(cell_map[row][col].get_walls() & 0b0010) && !cell_map[row][col - 1].get_c()) {
                            if (!cell_map[row][col - 1].get_c()) {
                                cell_map[row][col - 1].set_c(true);
                                cell_map[row][col - 1].set_distance(cell_map[row][col].get_distance() + 1);
                            }
                            queue.push(vec2(row, col - 1));
                        }
                    }
                    if (row > 0) {
                        //south
                        if (!(cell_map[row][col].get_walls() & 0b0100) && !cell_map[row - 1][col].get_c()) {
                            if (!cell_map[row - 1][col].get_c()) {
                                cell_map[row - 1][col].set_c(true);
                                cell_map[row - 1][col].set_distance(cell_map[row][col].get_distance() + 1);
                            }
                            queue.push(vec2(row - 1, col));
                        }
                    }
                    //east
                    if (col < 15) {
                        if (!(cell_map[row][col].get_walls() & 0b1000) && !cell_map[row][col + 1].get_c()) {
                            if (!cell_map[row][col + 1].get_c()) {
                                cell_map[row][col + 1].set_c(true);
                                cell_map[row][col + 1].set_distance(cell_map[row][col].get_distance() + 1);
                            }
                            queue.push(vec2(row, col + 1));
                        }
                    }
                    cell_map[row][col].set_v(true);
                }
                

            }
        }
    }
    std::queue<unsigned char> best_route(vec2 current) {
        std::queue<unsigned char> q;
        int row = current.y;
        int col = current.x;
        int min_dist = cell_map[row][col].get_distance();
        while (!min_dist == 0) {
            if (row > -1 && row < 16 && col >-1 && col < 16) {
                //north
                if (row < 15) {
                    if (cell_map[row + 1][col].get_distance() < min_dist) {
                        min_dist = cell_map[row + 1][col].get_distance();
                        q.push(0b0001);
                        row++;
                    }
                }
                //east
                if (col > 0) {
                    if (cell_map[row][col - 1].get_distance() < min_dist) {
                        min_dist = cell_map[row][col - 1].get_distance();
                        q.push(0b0010);
                        col--;
                    }
                }
                //south
                if (row > 0) {
                    if (cell_map[row - 1][col].get_distance() < min_dist) {
                        min_dist = cell_map[row - 1][col].get_distance();
                        q.push(0b0100);
                        row--;
                    }
                }
                //west
                if (row < 15) {
                    if (cell_map[row][col + 1].get_distance() < min_dist) {
                        min_dist = cell_map[row][col + 1].get_distance();
                        q.push(0b1000);
                        col++;
                    }
                }
            }
        }
        return q;
    
    }
    maze(int width, int height) {
        cell_map.resize(height);
        for (auto& a : cell_map) {
            a.resize(width);
        }
    }
    void print_maze() {
        for (auto& b : cell_map) {
            std::cout << '\n';
            for (auto& a : b){
                std::cout << a.get_distance();
            }
        }
    }
    std::vector <std::vector<cell>> return_map() {
        return cell_map;
    }
};

    void log(const std::string& text) {
        std::cerr << text << std::endl;
    }
    int main(int argc, char* argv[]) {
        maze maze(maze_width, maze_height);
        vec2 start(0, maze_height);
        maze.flood_fill(goal, start);
        log("Running...");
        API::setColor(0, 0, 'G');
        API::setText(1, 1, "aa");
        for (int i = 0; i < maze_height; i++) {
            for (int j = 0; j < maze_width; j++) {
                API::setText(i, j, maze.return_map()[i][j].cell_to_string());
            }
        }
        std::queue<unsigned char> instructions = maze.best_route(current);
        while (true) {
            printf("%c", instructions.front());
            if (instructions.front() & 0b0001)
            {
                log("north");
                API::moveForward();
            }
            else if (instructions.front() == 0b0010) {
                std::cout << "east";
                API::turnRight();
                API::moveForward();
            }
            else if (instructions.front() == 0b0100) {
                API::turnRight();
                API::turnRight();
                API::moveForward();
                std::cout << "south";
            }
            else if (instructions.front() == 0b1000) {
                API::turnLeft();
                API::moveForward();
                std::cout << "west";
            }
            instructions.pop();
        }
    }

