#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 3
#define PLAYER_POS 5
#define MAX_OBS 100
#define MAX_BG 50

typedef enum { NORMAL, FAST, BIG } ObstacleType;

typedef struct {
    int line;
    int x;
    ObstacleType type;
    int speed_counter;
} Obstacle;

typedef struct {
    int y;
    int x;
} Background;

Obstacle obstacles[MAX_OBS];
int obs_count = 0;
Background bg[MAX_BG];

int score = 0;
int delay = 100000;
int highscore = 0;

void load_highscore() {
    FILE *f = fopen("highscore.dat", "r");
    if (f) {
        fscanf(f, "%d", &highscore);
        fclose(f);
    }
}

void save_highscore() {
    if (score > highscore) {
        FILE *f = fopen("highscore.dat", "w");
        if (f) {
            fprintf(f, "%d", score);
            fclose(f);
        }
    }
}

void init_colors() {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);   // 플레이어
    init_pair(3, COLOR_WHITE, COLOR_BLACK);   // 배경
    init_pair(4, COLOR_CYAN, COLOR_BLACK);    // 텍스트
    init_pair(10, COLOR_BLACK, COLOR_YELLOW);      // 일반 장애물 (빨강 배경)
    init_pair(11, COLOR_BLACK, COLOR_BLUE);  // 큰 장애물 (마젠타 배경)
    init_pair(12, COLOR_WHITE, COLOR_BLACK);   // 빠른 장애물 (노랑 텍스트)
}

void init_background() {
    for (int i = 0; i < MAX_BG; i++) {
        bg[i].y = (rand() % HEIGHT) * 2;
        bg[i].x = rand() % WIDTH;
    }
}

void update_background() {
    for (int i = 0; i < MAX_BG; i++) {
        bg[i].x--;
        if (bg[i].x < 0) {
            bg[i].x = WIDTH - 1;
            bg[i].y = (rand() % HEIGHT) * 2;
        }
    }
}

void draw_background() {
    attron(COLOR_PAIR(3));
    for (int i = 0; i < MAX_BG; i++) {
        mvaddch(bg[i].y, bg[i].x, '.');
    }
    attroff(COLOR_PAIR(3));
}

void add_obstacle() {
    if (obs_count >= MAX_OBS) return;
    obstacles[obs_count].line = rand() % HEIGHT;
    obstacles[obs_count].x = WIDTH - 1;
    obstacles[obs_count].speed_counter = 0;

    int r = rand() % 100;
    if (r < 60) {
        obstacles[obs_count].type = NORMAL;
    } else if (r < 85) {
        obstacles[obs_count].type = FAST;
    } else {
        obstacles[obs_count].type = BIG;
    }

    obs_count++;
}

void update_obstacles() {
    for (int i = 0; i < obs_count; i++) {
        obstacles[i].speed_counter++;

        int move = 0;
        if (obstacles[i].type == FAST) {
            if (obstacles[i].speed_counter >= 1) {
                move = 2;
                obstacles[i].speed_counter = 0;
            }
        } else {
            if (obstacles[i].speed_counter >= 2) {
                move = 1;
                obstacles[i].speed_counter = 0;
            }
        }

        obstacles[i].x -= move;
    }

    int j = 0;
    for (int i = 0; i < obs_count; i++) {
        if (obstacles[i].x >= -2) {
            obstacles[j++] = obstacles[i];
        } else {
            score++;
        }
    }
    obs_count = j;
}

void draw(int player_line) {
    clear();

    draw_background();

    // Draw tracks
    for (int y = 0; y < HEIGHT; y++) {
        mvhline(y * 2, 0, '-', WIDTH);
    }

    // Draw player
    attron(COLOR_PAIR(1));
    mvaddch(player_line * 2, PLAYER_POS, 'O');
    attroff(COLOR_PAIR(1));

    // Draw obstacles
    for (int i = 0; i < obs_count; i++) {
        int y = obstacles[i].line * 2;
        int x = obstacles[i].x;

        if (obstacles[i].type == NORMAL) {
            attron(COLOR_PAIR(10));
            mvaddch(y, x, ' ');
            attroff(COLOR_PAIR(10));
        } else if (obstacles[i].type == FAST) {
            attron(COLOR_PAIR(12));
            mvaddch(y, x, '<');
            mvaddch(y, x + 1, '<');
            attroff(COLOR_PAIR(12));
        } else if (obstacles[i].type == BIG) {
            attron(COLOR_PAIR(11));
            mvaddch(y, x, ' ');
            mvaddch(y, x - 1, ' ');
            attroff(COLOR_PAIR(11));
        }
    }

    // Draw score
    attron(COLOR_PAIR(4));
    mvprintw(HEIGHT * 2, 0, "Score: %d | High Score: %d | Keyboard Arrow to move | q to quit", score, highscore);
    attroff(COLOR_PAIR(4));

    refresh();
}

int check_collision(int player_line) {
    for (int i = 0; i < obs_count; i++) {
        if (obstacles[i].line != player_line) continue;

        int obs_x = obstacles[i].x;
        if (obstacles[i].type == BIG) {
            if (obs_x == PLAYER_POS || obs_x - 1 == PLAYER_POS) {
                return 1;
            }
        } else {
            if (obs_x == PLAYER_POS) {
                return 1;
            }
        }
    }
    return 0;
}

void reset_game() {
    obs_count = 0;
    score = 0;
    delay = 100000;
    init_background();
}

void start_screen() {
    clear();
    attron(COLOR_PAIR(4));
    mvprintw(5, (WIDTH / 2) - 10, "=== Bash n Dash ===");
    mvprintw(7, (WIDTH / 2) - 18, "Avoid obstacles and survive as long as possible!");
    mvprintw(11, (WIDTH / 2) - 12, "Press SPACE to Start");
    mvprintw(13, (WIDTH / 2) - 12, "Move: Keyboard Arrow Keys   |   Quit: q");
    attroff(COLOR_PAIR(4));
    refresh();

    int ch;
    while ((ch = getch()) != ' ' && ch != 'q') {
        usleep(10000);
    }
    if (ch == 'q') {
        endwin();
        exit(0);
    }
}

void game_over_screen() {
    attron(COLOR_PAIR(4));
    mvprintw(HEIGHT * 2 + 2, (WIDTH / 2) - 10, "GAME OVER! Final Score: %d", score);
    if (score > highscore) {
        mvprintw(HEIGHT * 2 + 4, (WIDTH / 2) - 8, "New High Score!");
    }
    mvprintw(HEIGHT * 2 + 6, (WIDTH / 2) - 14, "Press SPACE to Restart or q to Quit");
    attroff(COLOR_PAIR(4));
    refresh();

    int ch;
    while ((ch = getch()) != ' ' && ch != 'q') {
        usleep(10000);
    }
    if (ch == 'q') {
        endwin();
        exit(0);
    }
}

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    srand(time(NULL));
    init_colors();
    load_highscore();

    while (1) {
        start_screen();
        reset_game();

        int player_line = 1;
        int tick = 0;

        while (1) {
            int ch = getch();
            if (ch == KEY_UP) {
                if (player_line > 0) player_line--;
            }
            if (ch == KEY_DOWN) {
                if (player_line < HEIGHT - 1) player_line++;
            }
            if (ch == 'q') {
                save_highscore();
                endwin();
                exit(0);
            }

            if (tick % 10 == 0) {
                if (rand() % 100 < 70) {
                    add_obstacle();
                }
            }

            update_background();
            update_obstacles();
            draw(player_line);

            if (check_collision(player_line)) {
                beep();
                save_highscore();
                if (score > highscore) highscore = score;
                game_over_screen();
                break;
            }

            if (tick % 50 == 0 && delay > 40000) {
                delay -= 5000;
            }

            usleep(delay);
            tick++;
        }
    }

    endwin();
    return 0;
}
