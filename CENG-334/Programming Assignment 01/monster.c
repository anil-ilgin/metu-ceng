#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "logging.h"
#include "message.h"
#include <math.h>

typedef struct monster {
    int health;
    int damage;
    int resist;
    int range;
} monster;

monster new_monster;

int max(int a, int b) {
    if (a > b) return a;
    else return b;
}

void initMonster(char * inputs[]) {
    new_monster.health = atoi(inputs[1]);
    new_monster.damage = atoi(inputs[2]);
    new_monster.resist = atoi(inputs[3]);
    new_monster.range = atoi(inputs[4]);
}

int manhattan_distance(int x,int y,int xbar,int ybar) {
    return abs(x - xbar) + abs(y - ybar);
}

coordinate coord_calc(coordinate player,coordinate monster) {
    coordinate new_loc;
    if (player.y < monster.y) {
        if (player.x < monster.x) {
            new_loc.x = monster.x - 1 ;
            new_loc.y = monster.y -1 ;
        }
        else if (player.x == monster.x) {
            new_loc.x = monster.x;
            new_loc.y = monster.y - 1;
        }
        else {
            new_loc.x = monster.x + 1;
            new_loc.y = monster.y - 1;
        }
    }
    else if (player.y == monster.y) {
        if (player.x < monster.x) {
            new_loc.x = monster.x - 1;
            new_loc.y = monster.y;
        }
        else if (player.x == monster.x) {
            new_loc.x = monster.x;
            new_loc.y = monster.y;
        }
        else {
            new_loc.x = monster.x + 1;
            new_loc.y = monster.y;
        }
    }
    else {
        if (player.x < monster.x) {
            new_loc.x = monster.x - 1;
            new_loc.y = monster.y + 1;
        }
        else if (player.x == monster.x) {
            new_loc.x = monster.x;
            new_loc.y = monster.y + 1;
        }
        else {
            new_loc.x = monster.x + 1;
            new_loc.y = monster.y + 1;
        }
    }
    return new_loc;
}

monster_response respond(monster_message message) {
    monster_response response;
    int isDead = 0;
    new_monster.health = new_monster.health - max(0, message.damage - new_monster.resist);
    if (new_monster.health <= 0) isDead = 1;
    if (isDead) {
        response.mr_type = mr_dead;
        return response;
    }
    else {
        int dist = manhattan_distance(message.new_position.x, message.new_position.y, message.player_coordinate.x, message.player_coordinate.y);
        if (dist <= new_monster.range) {
            response.mr_type = mr_attack;
            response.mr_content.attack = new_monster.damage;
            return response;
        }
        else {
            response.mr_type = mr_move;
            response.mr_content.move_to = coord_calc(message.player_coordinate, message.new_position);
            return response;
        }
    }


}


int main(int argc, char *argv[]){
    monster_message input;
    initMonster(argv);
    monster_response response;
    response.mr_type = mr_ready;
    response.mr_content.attack = 0;
    response.mr_content.move_to.x = 0;
    response.mr_content.move_to.y = 0;
    write(1, &response, sizeof(response));

    while(1) {
        read(0, &input, sizeof(input));
        response = respond(input);
        if (input.game_over) {
            exit(0);
        }
        fflush(stdout);
        write(1, &response, sizeof(response));
    }

    return 0;
}
