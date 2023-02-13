#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "logging.h"
#include "message.h"
#include <limits.h>
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

map_info mapInfo;
char exec_player [255];
int p_arg [3];
int m_arg [MONSTER_LIMIT][6];
char exec_mon [MONSTER_LIMIT][255];
int monster_num;
char mon_type;
pid_t monster_pid [MONSTER_LIMIT];
int monster_pipes[MONSTER_LIMIT][2];
int player_pipe[2];
pid_t player_pid;
game_over_status status;
bool is_game_over ;
int player_attack[MONSTER_LIMIT];
int ready_signals[MONSTER_LIMIT + 1] = {0};
bool ready = false;


void init() {
    is_game_over = false;
    scanf("%d %d",&mapInfo.map_width, &mapInfo.map_height);
    scanf("%d %d", &mapInfo.door.x, &mapInfo.door.y);
    scanf("%d %d", &mapInfo.player.x, &mapInfo.player.y);
    scanf("%s %d %d %d", exec_player, &p_arg[0], &p_arg[1], &p_arg[2]);
    scanf("%d", &monster_num);
    mapInfo.alive_monster_count = monster_num;
    for(int i=0; i < monster_num; i++) {
        scanf("%s %c %d %d %d %d %d %d", exec_mon[i], &mon_type, &m_arg[i][0], &m_arg[i][1], &m_arg[i][2], &m_arg[i][3], &m_arg[i][4], &m_arg[i][5]  );
        mapInfo.monster_types[i] = mon_type;
        mapInfo.monster_coordinates[i].x = m_arg[i][0];
        mapInfo.monster_coordinates[i].y = m_arg[i][1];
    }
    for(int i=monster_num; i<MONSTER_LIMIT; i++) {
        mapInfo.monster_coordinates[i].x = INT_MAX;
        mapInfo.monster_coordinates[i].y = INT_MAX;
        mapInfo.monster_types[i] = '-';
    }

}

void pipe_setup() {
    PIPE(player_pipe);
    player_pid = fork();
    if (player_pid == 0) {
        dup2(player_pipe[0],0);
        dup2(player_pipe[0],1);
        close(player_pipe[1]);
        char parg1[4], parg2[4], parg3[4], parg4[4], parg5[4];
        sprintf(parg1, "%d", mapInfo.door.x);
        sprintf(parg2, "%d", mapInfo.door.y);
        sprintf(parg3, "%d", p_arg[0]);
        sprintf(parg4, "%d", p_arg[1]);
        sprintf(parg5, "%d", p_arg[2]);
        char* pargv[] = {"./player",parg1,parg2,parg3, parg4, parg5, NULL};
        execv("./player", pargv);
    }
    else {
        close(player_pipe[0]);
    }

    for(int i=0; i < monster_num; i++) {
        PIPE(monster_pipes[i]);
        monster_pid[i] = fork();

        if (monster_pid[i] == 0) {
            dup2(monster_pipes[i][0],0);
            dup2(monster_pipes[i][0],1);
            close(monster_pipes[i][1]);
            char arg1[4], arg2[4], arg3[4], arg4[4];
            sprintf(arg1,"%d",m_arg[i][2]); sprintf(arg2,"%d",m_arg[i][3]);
            sprintf(arg3,"%d",m_arg[i][4]); sprintf(arg4,"%d",m_arg[i][5]);
            char* argv[] = {"./monster",arg1,arg2,arg3,arg4,NULL};
            execv("./monster", argv);
        }
        else {
            close(monster_pipes[i][0]);
        }


    }
}


void KillMonster(int i) {
    close(monster_pipes[i][0]);
    kill(monster_pid[i], SIGTERM);
    waitpid(monster_pid[i], NULL, 0);
}

void KillPlayer() {
    close(player_pipe[1]);
    kill(player_pid, SIGTERM);
    waitpid(player_pid, NULL, 0);
}

void swap(coordinate *a, coordinate *b) {
    coordinate  temp = *a;
    a->x = b->x;
    a->y = b->y;
    b->x = temp.x;
    b->y = temp.y;
}
void swapchar(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}
void swappipes(int a, int b) {
    int temp [2] ; temp[0] = monster_pipes[a][0]; temp[1] = monster_pipes[a][1];
    int temp_pid = monster_pid[a];
    monster_pipes[a][0] = monster_pipes[b][0]; monster_pipes[a][1] = monster_pipes[b][1];
    monster_pipes[b][0] = temp[0]; monster_pipes[b][1] = temp[1];
    monster_pid[a] = monster_pid[b];
    monster_pid[b] = temp_pid;
}

void sort() {
    int a,b;
    for(int i=0; i < MONSTER_LIMIT - 1; i++) {
        for(int j=0; j < MONSTER_LIMIT - 1 - i; j++) {
            if (mapInfo.monster_coordinates[j].x == INT_MAX) {
                a = INT_MAX;
            }
            else {
                a = mapInfo.monster_coordinates[j].x*10000 + mapInfo.monster_coordinates[j].y ;
            }
            if (mapInfo.monster_coordinates[j+1].x == INT_MAX) {
                b = INT_MAX;
            }
            else {
                b = mapInfo.monster_coordinates[j+1].x*10000 + mapInfo.monster_coordinates[j+1].y ;
            }
            if(a > b) {
                swap(&mapInfo.monster_coordinates[j], &mapInfo.monster_coordinates[j+1]) ;
                swapchar(&mapInfo.monster_types[j],&mapInfo.monster_types[j+1]);
                swappipes(j,j+1);
            }
        }
    }

}

int sum() {
    int ret_val = 0;
    for(int i=0; i < MONSTER_LIMIT + 1; i++) {
        ret_val += ready_signals[i];
    }
    for(int i=0; i < MONSTER_LIMIT + 1; i++) {
        ready_signals[i] = 0;
    }
    return ret_val;
}

void send_monster(int i,coordinate monster, coordinate player, int damage, bool game_over ) {
    monster_message m_message = {0};
    m_message.damage = damage;
    m_message.game_over = game_over;
    m_message.new_position.x = monster.x;
    m_message.new_position.y = monster.y;
    m_message.player_coordinate.x = player.x;
    m_message.player_coordinate.y = player.y;

    write(monster_pipes[i][1], &m_message, sizeof(m_message));
    fflush(stdout);
}

int receive_monster() {

    int damage_player = 0;
    monster_response response = {0};
    for(int i=0; i < monster_num; i++ ) {
        read(monster_pipes[i][1], &response, sizeof(response));
        if (ready) {
            if (response.mr_type == mr_dead) {
                mapInfo.monster_coordinates[i].x = INT_MAX;
                mapInfo.monster_coordinates[i].y = INT_MAX;
                mapInfo.monster_types[i] = '-';
                KillMonster(i);
                mapInfo.alive_monster_count--;
                if(mapInfo.alive_monster_count == 0) {
                    is_game_over = true;
                    monster_num = 0;
                    status = go_survived;
                    break;
                }
            }
            else if (response.mr_type == mr_move) {

                int target_x = response.mr_content.move_to.x;
                int target_y = response.mr_content.move_to.y;
                if (mapInfo.monster_types[i] == 'r') {

                }
                if (mapInfo.monster_types[i] == 'e') {

                }
                if (target_x >= mapInfo.map_width - 1 || target_y >= mapInfo.map_height - 1) {

                }
                else if (target_x <= 0 || target_y <= 0) {

                }
                else if (target_x == mapInfo.door.x && target_y == mapInfo.door.y) {

                }
                else {
                    bool space_empty = true;
                    if (target_x == mapInfo.player.x && target_y == mapInfo.player.y) space_empty = false;
                    for(int j =0; j < monster_num; j++) {
                        if (target_x == mapInfo.monster_coordinates[j].x && target_y == mapInfo.monster_coordinates[j].y) space_empty = false;
                    }
                    if (space_empty) {
                        mapInfo.monster_coordinates[i].x = target_x;
                        mapInfo.monster_coordinates[i].y = target_y;
                    }
                    else {

                    }
                }
            }
            else if (response.mr_type == mr_attack) {
                damage_player += response.mr_content.attack;
            }
            else if (response.mr_type == mr_ready) {
                ready_signals[i] = 1;
            }
        }
        else {
            if (response.mr_type == mr_ready) {
                ready_signals[i] = 1;
                
            }
        }

    }
    sort();
    monster_num = mapInfo.alive_monster_count;
    return damage_player;
}

void send_player(int total_damage) {
    player_message message = {0};
    for(int i = 0; i < mapInfo.alive_monster_count; i++) {
        message.monster_coordinates[i].x = mapInfo.monster_coordinates[i].x;
        message.monster_coordinates[i].y = mapInfo.monster_coordinates[i].y;
    }
    message.alive_monster_count = mapInfo.alive_monster_count;
    message.new_position.x = mapInfo.player.x;
    message.new_position.y = mapInfo.player.y;
    message.game_over = is_game_over;
    message.total_damage = total_damage;

    write(player_pipe[1], &message, sizeof(player_message));
    fflush(stdout);
}

void receive_player() {
    player_response response = {0};
    read(player_pipe[1], &response, sizeof (player_response));
    for(int i=0; i < MONSTER_LIMIT; i++) {
        player_attack[i] = 0;
    }
    if (ready) {
        if (response.pr_type == pr_dead ) {
            status = go_died;
            is_game_over = true;
        }
        else if (response.pr_type == pr_attack) {
            for(int i = 0; i < mapInfo.alive_monster_count; i++) {
                player_attack[i] = response.pr_content.attacked[i];
            }
        }
        else if (response.pr_type == pr_move) {
            int target_x = response.pr_content.move_to.x;
            int target_y = response.pr_content.move_to.y;
            if (target_x == mapInfo.door.x && target_y == mapInfo.door.y ) {
                mapInfo.player.x = target_x;
                mapInfo.player.y = target_y;
                is_game_over = 1;
                status = go_reached;
            }
            else if (target_x >= mapInfo.map_width - 1 || target_y >= mapInfo.map_height - 1) {

            }
            else if (target_x <= 0 || target_y <= 0 ) {

            }
            else {
                bool space_free = true;
                for(int i=0; i < monster_num; i++) {
                    if (target_x == mapInfo.monster_coordinates[i].x && target_y == mapInfo.monster_coordinates[i].y) space_free = false;
                }
                if (space_free) {
                    mapInfo.player.x = target_x;
                    mapInfo.player.y = target_y;
                }
            }
        }
        else if(response.pr_type == pr_ready) {
            ready_signals[MONSTER_LIMIT] = 1;
        }
    }
    else {
        if(response.pr_type == pr_ready) {
            ready_signals[MONSTER_LIMIT] = 1;
        }
    }


}


void game() {
    int sumx;
    do {
    	receive_monster();
        receive_player();
        sumx = sum();
    }
    while( sumx < mapInfo.alive_monster_count + 1);
    ready = true;
    int total_damage = 0;
    while(!is_game_over) {
        if ( waitpid(player_pid, NULL, WNOHANG ) ) {
            status = go_left;
            is_game_over = 1;
            break;
        }
        else {
            print_map(&mapInfo);
            send_player(total_damage);
            receive_player();

            if (is_game_over) break;
            for(int i=0; i < mapInfo.alive_monster_count; i++) {
                send_monster(i,mapInfo.monster_coordinates[i],mapInfo.player, player_attack[i], is_game_over);
            }
            total_damage = receive_monster();

        }

    }
    for(int i=0; i < mapInfo.alive_monster_count; i++) {
        send_monster(i,mapInfo.monster_coordinates[i], mapInfo.player, player_attack[i], is_game_over);
    }

}



int main() {
    init();
    pipe_setup();
    game();
    print_map(&mapInfo);
    print_game_over(status);
    for(int i=0; i < mapInfo.alive_monster_count; i++) {
        KillMonster(i);
    }
    KillPlayer();
    return 0;
}
