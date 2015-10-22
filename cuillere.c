#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include <ncurses.h>

#define CAT_SIZE 5
#define NB_CATS 100
#define TIME_TO_SLEEP 100000

typedef struct {
	int tail;
	int dir;
	int x[CAT_SIZE];
	int y[CAT_SIZE];
} Cat;

void cat_init(Cat *c, int width, int height) {
	int i,
		x = width / 2,//rand() % width,
		y = height / 2;//rand() % height;

	c->tail = 0;
	c->dir = rand() % 8;

	for (i = 0; i < CAT_SIZE; i++) {
		c->x[i] = x;
		c->y[i] = y;
	}
}

void cat_move(Cat *c) {
	int i;

	mvprintw(c->y[c->tail], c->x[c->tail], " ");

	c->dir = (c->dir + 7 + rand() % 3) % 8; // Turn left, right or don't turn.
	switch (c->dir) {
		case 0:
			c->x[c->tail] = c->x[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] + 1; // TODO Replace all lines with dependancies from old head.
			c->y[c->tail] = c->y[(c->tail - 1 + CAT_SIZE) % CAT_SIZE];
			break;
		case 1:
			c->x[c->tail] = c->x[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] + 1;
			c->y[c->tail] = c->y[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] - 1;
			break;
		case 2:
			c->x[c->tail] = c->x[(c->tail - 1 + CAT_SIZE) % CAT_SIZE];
			c->y[c->tail] = c->y[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] - 1;
			break;
		case 3:
			c->x[c->tail] = c->x[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] - 1;
			c->y[c->tail] = c->y[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] - 1;
			break;
		case 4:
			c->x[c->tail] = c->x[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] - 1;
			c->y[c->tail] = c->y[(c->tail - 1 + CAT_SIZE) % CAT_SIZE];
			break;
		case 5:
			c->x[c->tail] = c->x[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] - 1;
			c->y[c->tail] = c->y[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] + 1;
			break;
		case 6:
			c->x[c->tail] = c->x[(c->tail - 1 + CAT_SIZE) % CAT_SIZE];
			c->y[c->tail] = c->y[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] + 1;
			break;
		case 7:
			c->x[c->tail] = c->x[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] + 1;
			c->y[c->tail] = c->y[(c->tail - 1 + CAT_SIZE) % CAT_SIZE] + 1;
			break;
	}

	c->tail = (c->tail + 1) % CAT_SIZE;

	for (i = 0; i < CAT_SIZE - 1; i++) {
		mvprintw(c->y[(i + c->tail) % CAT_SIZE], c->x[(i + c->tail) % CAT_SIZE], i == CAT_SIZE - 2 ? "o" : ".");
	}
	mvprintw(c->y[(i + c->tail) % CAT_SIZE], c->x[(i + c->tail) % CAT_SIZE], "O");
}

void cat_debug(Cat *c) {
	int i;

	printf("Cat:\n");
	printf(" dir: %d\n", c->dir);
	printf(" tail: %d\n", c->tail);
	printf(" x,y :");
	for (i = 0; i < CAT_SIZE; i++) {
		printf(" (%d,%d)", c->x[i], c->y[i]);
	}
	printf("\n");
}

int main(void) {
	Cat cats[NB_CATS];
	time_t t;

	int i;

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int height = w.ws_row,
		width = w.ws_col;

	srand(time(&t));
	
	initscr();
	curs_set(0);

	for (i = 0; i < NB_CATS; i++) {
		cat_init(&cats[i], width, height);
	}

	while (1) {
		for (i = 0; i < NB_CATS; i++) {
			cat_move(&cats[i]);
			//cat_debug(&cats[i]);
		}
		refresh();
		usleep(TIME_TO_SLEEP);
	}

	endwin();

	return 0;
}
