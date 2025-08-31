// GAME_H
#ifndef GAME_H
#define GAME_H

typedef struct {
	int x;
	int y;
} vec2ui;

typedef struct {
	int x;
	int y;
} vec2i;

// simple integer rectangle type
typedef struct {
    vec2i offset;
    vec2i bounds;

    int top() { return offset.y; }
    int bot() { return offset.y + bounds.y; }
    int left() { return offset.x; }
    int right() { return offset.x + bounds.x; }

    int width() { return bounds.x; }
    int height() { return bounds.y; }

    bool contains(vec2i a) { return (a.x >= offset.x && a.x < right()) && 
                                    (a.y >= offset.y && a.y < bot()); }
} rect;

int init();
void run();
void close();

#endif 
