/*
north - 0b0001
East - 0b0010
South - 0b0100
West - 0b1000
maze is 16 x 16
goal is 7,7 7,8 8,8 8,7
*/
#include <iostream>
#include <string>
#include<vector>
#include <queue>
#include <string>
#include <sstream>
#include "API.h"
struct vec2 {
    int x; int y;
    vec2() :x(0.0), y(0.0) {};
    vec2(int x, int y) {
        this->x = x;
        this->y = y;
    }
    vec2(const vec2& vec) {
        this->x = vec.x;
        this->y = vec.y;
    }
    vec2 operator+(vec2 v) {
        return vec2(x + v.x, y + v.y);
    }
    vec2 operator-(vec2 v) {
        return vec2(x - v.x, y - v.y);
    }
};
void log(const std::string& text) {
    std::cerr << text << std::endl;
}
enum direction : unsigned char {
    north = 0b0001,
    east = 0b0010,
    south = 0b0100,
    west = 0b1000,
};
int counter = 0;
int maze_width = 16;
int maze_height = 16;
vec2 start(0, 0);
vec2 current_pos(0, 0);
std::vector<direction> all_dir = { north, east, south, west };
std::vector<vec2> goal = { {8,8}, {7,8}, {8,7} , {7,7} };
direction current_dir = north;

direction get_left_dir(direction dir)
{
    switch (dir) {
    case (north):
        return west;
        break;
    case (west):
        return south;
        break;
    case (south):
        return east;
        break;
    case (east):
        return north;
        break;
    }
}
direction get_right_dir(direction dir) {
    switch (dir) {
    case (north):
        return east;
        break;
    case (east):
        return south;
        break;
    case (south):
        return west;
        break;
    case (west):
        return north;
        break;
    }
}
direction get_opposite_dir(direction dir) {
    switch (dir) {
    case (north):
        return south;
        break;
    case (east):
        return west;
        break;
    case (south):
        return north;
        break;
    case (west):
        return east;
        break;
    }
}
unsigned char dir_to_char(direction dir) {
    switch (dir) {
    case north:
        return 'n';
        break;
    case east:
        return 'e';
        break;
    case south:
        return 's';
        break;
    case west:
        return 'w';
        break;
    }
}
vec2 dir_to_vec(direction dir) {
    switch (dir) {
    case north:
        return vec2(0, 1);
    case east:
        return vec2(1, 0);
    case south:
        return vec2(0, -1);
    case west:
        return vec2(-1, 0);
    }
}
bool in_range(vec2 pos) {
    int x = pos.x;
    int y = pos.y;
    if (x > -1 && x < 16 && y>-1 && y < 16) {
        return true;
    }
    return false;
}
std::string to_string(int a) {
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    return str;
}

class cell {
private:
    int distance; unsigned char walls; bool c; bool v;
public:
    cell() {
        distance = 0;
        walls = 0b0000;
        c = false;
        v = false;
    }
    void set_c(bool f) {
        c = f;
    }
    bool get_c() {
        return c;
    }
    void set_v(bool v_) {
        v = v_;
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
    void add_walls(unsigned char c) {
        walls |= c;
    }
    void add_walls(direction d) {
        walls |= d;
    }
    bool exist_wall(unsigned char c) {
        if ((walls & c) == 0) {
            //there is no wall
            return false;
        }
        //is a wall
        return true;
    }
    bool exist_wall(direction c) {
        if ((walls & c) == 0) {
            //there is no wall
            return false;
        }
        //is a wall
        return true;
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
std::vector<std::vector<cell>> cell_map;
void init_map(int width, int height) {
    cell_map.resize(height);
    for (auto& a : cell_map) {
        a.resize(width);
    }
}
void print_map() {
    for (int i = 0; i < maze_height; i++) {
        for (int j = 0; j < maze_width; j++) {
            API::setText(j, i, cell_map[i][j].cell_to_string());
        }
    }
}
void flood_map(const std::vector<vec2>goal, const vec2 start) {
    std::queue<vec2> q;
    for (int row = 0; row < maze_height; row++) {
        for (int col = 0; col < maze_width; col++) {
            cell_map[row][col].set_c(false);
            cell_map[row][col].set_v(false);
            cell_map[row][col].set_distance(200);
        }
    }
    for (auto& a : goal) {
        cell_map[a.y][a.x].set_c(true);
        cell_map[a.y][a.x].set_distance(0);
        q.push(a);
    }
    while (!q.empty()) {
        int row = q.front().y;
        int col = q.front().x;
        q.pop();
        if (row > -1 && row< maze_height && col >-1 && col < maze_width) {
            if (!cell_map[row][col].get_v()) {
                cell_map[row][col].set_v(true);
                for (const direction& dir : all_dir) {
                    int nrow = row;
                    int ncol = col;

                    switch (dir) {
                    case north:
                        nrow++;
                        break;
                    case east:
                        ncol++;
                        break;
                    case south:
                        nrow--;
                        break;
                    case west:
                        ncol--;
                        break;
                    }
                    if (nrow > -1 && nrow <maze_height && ncol > -1 && ncol < maze_width && !cell_map[row][col].exist_wall(dir)
                        && !cell_map[nrow][ncol].exist_wall(get_opposite_dir(dir)) && !cell_map[nrow][ncol].get_c())
                    {
                        cell_map[nrow][ncol].set_c(true);
                        cell_map[nrow][ncol].set_distance(cell_map[row][col].get_distance() + 1);
                        q.push(vec2(ncol, nrow));
                    }
                }
            }
        }
    }
}
std::queue<direction> best_route() {
    std::queue<direction> q;

    int row = current_pos.y;
    int col = current_pos.x;
    int min_dist = cell_map[row][col].get_distance();

    while (min_dist > 0) {
        if (row > -1 && row < maze_height && col >-1 && col < maze_width) {
            //north
            if (row < 15 && !cell_map[row + 1][col].exist_wall(south) && !cell_map[row][col].exist_wall(north)
                && cell_map[row + 1][col].get_distance() < min_dist)
            {
                min_dist = cell_map[row + 1][col].get_distance();
                q.push(north);
                row++;
            }
            //east
            else if (col < 15 && !cell_map[row][col + 1].exist_wall(west) && !cell_map[row][col].exist_wall(east)
                && cell_map[row][col + 1].get_distance() < min_dist)
            {

                min_dist = cell_map[row][col + 1].get_distance();
                q.push(east);
                col++;
            }
            //south
            else if (row > 0 && !cell_map[row - 1][col].exist_wall(north) && !cell_map[row][col].exist_wall(south)
                && cell_map[row - 1][col].get_distance() < min_dist)
            {
                min_dist = cell_map[row - 1][col].get_distance();
                q.push(south);
                row--;
            }
            //west
            else if (col > 0 && !cell_map[row][col - 1].exist_wall(east) && !cell_map[row][col].exist_wall(west)
                && cell_map[row][col - 1].get_distance() < min_dist)
            {
                min_dist = cell_map[row][col - 1].get_distance();
                q.push(west);
                col--;
            }
        }
    }
    return q;
}

void update_pos(direction dir) {
    if (dir & north) {
        current_pos.y++;
    }
    else if (dir & east) {
        current_pos.x++;
    }
    else if (dir & south) {
        current_pos.y--;
    }
    else if (dir & west) {
        current_pos.x--;
    }
}
void update_wall(vec2 pos) {
    bool front_wall = API::wallFront();
    bool left_wall = API::wallLeft();
    bool right_wall = API::wallRight();

    direction right_dir = get_right_dir(current_dir);
    direction left_dir = get_left_dir(current_dir);

    int y = pos.y;
    int x = pos.x;
    vec2 front = dir_to_vec(current_dir);
    vec2 right = dir_to_vec(right_dir);
    vec2 left = dir_to_vec(left_dir);

    if (front_wall) {
        cell_map[y][x].add_walls(current_dir);
        if (in_range(front + pos)) {
            cell_map[y + front.y][x + front.x].add_walls(get_opposite_dir(current_dir));
        }
        API::setWall(x, y, dir_to_char(current_dir));
    }
    if (left_wall) {
        cell_map[y][x].add_walls(left_dir);
        if (in_range(left + pos)) {
            cell_map[y + left.y][x + left.x].add_walls(get_opposite_dir(left_dir));
        }
        API::setWall(x, y, dir_to_char(left_dir));
    }
    if (right_wall) {
        cell_map[y][x].add_walls(right_dir);
        if (in_range(right + pos)) {
            cell_map[y + right.y][x + right.x].add_walls(get_opposite_dir(right_dir));
        }
        API::setWall(x, y, dir_to_char(right_dir));
    }

}
void update_movement(direction temp) {
    if ((temp & current_dir) != 0)
    {
        log("F");
        API::moveForward(1);
    }
    else if ((temp & get_right_dir(current_dir)) != 0) {
        log("R");
        API::turnRight();
        API::moveForward(1);
        current_dir = temp;
    }
    else if ((temp & get_left_dir(current_dir)) != 0) {
        log("L");
        API::turnLeft();
        API::moveForward(1);
        current_dir = temp;
    }
    else {
        log("U");
        API::turnLeft();
        API::turnLeft();
        API::moveForward(1);
        current_dir = temp;
    }
}

int main(int argc, char* argv[]) {
    init_map(maze_width, maze_height);
    flood_map(goal, start);
    std::queue<direction> instructions = best_route();
    while (!instructions.empty()) {
        update_wall(current_pos);
        flood_map(goal, start);
        print_map();
        instructions = best_route();
        direction temp = instructions.front();
        update_pos(temp);
        update_movement(temp);
        instructions.pop();
    }
    start = current_pos;
    std::vector<vec2> goal_2 = { vec2(0,0) };
    flood_map(goal_2, start);
    instructions = best_route();
    while (!instructions.empty()) {
        update_wall(current_pos);
        flood_map(goal_2, start);
        print_map();
        instructions = best_route();
        direction temp = instructions.front();
        update_pos(temp);
        update_movement(temp);
        instructions.pop();
    }
    start = vec2(0, 0);
    flood_map(goal, start);
    instructions = best_route();
    print_map();
    while (!instructions.empty()) {
        update_wall(current_pos);
        flood_map(goal, start);
        print_map();
        instructions = best_route();
        direction temp = instructions.front();
        update_pos(temp);
        update_movement(temp);
        instructions.pop();
    }
    start = current_pos;
    goal_2 = { vec2(0,0) };
    flood_map(goal_2, start);
    instructions = best_route();
    while (!instructions.empty()) {
        update_wall(current_pos);
        flood_map(goal_2, start);
        print_map();
        instructions = best_route();
        direction temp = instructions.front();
        update_pos(temp);
        update_movement(temp);
        instructions.pop();
    }
    start = vec2(0, 0);
    flood_map(goal, start);
    instructions = best_route();
    print_map();
    while (!instructions.empty()) {
        update_wall(current_pos);
        flood_map(goal, start);
        print_map();
        instructions = best_route();
        direction temp = instructions.front();
        update_pos(temp);
        update_movement(temp);
        instructions.pop();
    }
    return 0;
}