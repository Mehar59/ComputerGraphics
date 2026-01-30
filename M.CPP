#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ---------- Global Variables ---------- */
int maxX, maxY;

/* ---------- Basic Drawing Functions (unchanged) ---------- */
void dda_line(int x1, int y1, int x2, int y2, int color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    if (steps == 0) { putpixel(x1, y1, color); return; }
    float xinc = dx / (float)steps;
    float yinc = dy / (float)steps;
    float x = x1, y = y1;
    for(int i = 0; i <= steps; i++) {
        putpixel((int)(x + 0.5), (int)(y + 0.5), color);
        x += xinc; y += yinc;
    }
}

void bresenham_line(int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x2 > x1) ? 1 : -1;
    int sy = (y2 > y1) ? 1 : -1;
    int err = dx - dy;
    while (1) {
        putpixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

void midpoint_circle(int xc, int yc, int r, int color) {
    int x = 0, y = r;
    int p = 1 - r;
    while (x <= y) {
        putpixel(xc + x, yc + y, color); putpixel(xc - x, yc + y, color);
        putpixel(xc + x, yc - y, color); putpixel(xc - x, yc - y, color);
        putpixel(xc + y, yc + x, color); putpixel(xc - y, yc + x, color);
        putpixel(xc + y, yc - x, color); putpixel(xc - y, yc - x, color);
        x++;
        if (p < 0) p += 2 * x + 1;
        else { y--; p += 2 * (x - y) + 1; }
    }
}

void draw_ellipse(int xc, int yc, int rx, int ry, int color) {
    float d1, d2;
    int x = 0, y = ry;
    d1 = ry*ry - rx*rx*ry + 0.25*rx*rx;
    float dx = 2*ry*ry*x, dy = 2*rx*rx*y;

    while (dx < dy) {
        putpixel(xc+x,yc+y,color); putpixel(xc-x,yc+y,color);
        putpixel(xc+x,yc-y,color); putpixel(xc-x,yc-y,color);
        if (d1 < 0) {
            x++; dx += 2*ry*ry;
            d1 += dx + ry*ry;
        } else {
            x++; y--; dx += 2*ry*ry; dy -= 2*rx*rx;
            d1 += dx - dy + ry*ry;
        }
    }

    d2 = ry*ry*(x+0.5)*(x+0.5) + rx*rx*(y-1)*(y-1) - rx*rx*ry*ry;
    while (y >= 0) {
        putpixel(xc+x,yc+y,color); putpixel(xc-x,yc+y,color);
        putpixel(xc+x,yc-y,color); putpixel(xc-x,yc-y,color);
        if (d2 > 0) {
            y--; dy -= 2*rx*rx;
            d2 += rx*rx - dy;
        } else {
            y--; x++; dx += 2*ry*ry; dy -= 2*rx*rx;
            d2 += dx - dy + rx*rx;
        }
    }
}

/* ---------- Transformation Helper Functions ---------- */
void translate(int &x, int &y, int tx, int ty) {
    x += tx;
    y += ty;
}

void rotate_around(int &x, int &y, int cx, int cy, float angle_deg) {
    float rad = angle_deg * M_PI / 180.0;
    float c = cos(rad);
    float s = sin(rad);
    int tx = x - cx;
    int ty = y - cy;
    x = cx + (int)(tx * c - ty * s + 0.5);
    y = cy + (int)(tx * s + ty * c + 0.5);
}

void scale_around(int &x, int &y, int cx, int cy, float sx, float sy) {
    x = cx + (int)((x - cx) * sx + 0.5);
    y = cy + (int)((y - cy) * sy + 0.5);
}

/* ---------- Input Function ---------- */
int get_number(char* prompt, int min, int max, int def) {
    char buf[16] = "";
    int i = 0;
    int x = maxX/2, y = maxY/2;

    cleardevice();
    setcolor(LIGHTCYAN); rectangle(x-120, y-60, x+120, y+60);
    setfillstyle(SOLID_FILL, BLUE); bar(x-119,y-59,x+119,y+59);

    setcolor(YELLOW);
    outtextxy(x-100, y-40, prompt);

    setcolor(WHITE);
    rectangle(x-60, y, x+60, y+25);
    setfillstyle(SOLID_FILL, BLACK); bar(x-59,y+1,x+59,y+24);

    while (1) {
        char ch = getch();
        if (ch == 13) { // Enter
            if (i == 0) return def;
            buf[i] = 0;
            int val = atoi(buf);
            if (val >= min && val <= max) return val;
            return def;
        }
        if (ch == 8 && i > 0) {
            i--; buf[i] = 0;
            bar(x-59,y+1,x+59,y+24);
            outtextxy(x-55, y+8, buf);
        }
        if (ch >= '0' && ch <= '9' && i < 5) {
            buf[i++] = ch;
            buf[i] = 0;
            bar(x-59,y+1,x+59,y+24);
            outtextxy(x-55, y+8, buf);
        }
    }
}

/* ---------- Menu Functions ---------- */
void display_menu() {
    cleardevice();
    setcolor(LIGHTMAGENTA);
    settextstyle(3, HORIZ_DIR, 4);
    outtextxy(maxX/2 - 180, 20, "GRAPHICS DEMO");
    settextstyle(0, HORIZ_DIR, 1);

    setcolor(LIGHTCYAN);
    rectangle(maxX/2-140, 80, maxX/2+140, 380);
    setfillstyle(SOLID_FILL, BLUE);
    bar(maxX/2-139,81,maxX/2+139,379);

    setcolor(YELLOW);
    outtextxy(maxX/2-120, 100, "1. Line (DDA / Bresenham)");
    outtextxy(maxX/2-120, 140, "2. Circle (Midpoint / Bresenham)");
    outtextxy(maxX/2-120, 180, "3. Ellipse");
    outtextxy(maxX/2-120, 220, "4. Rectangle");
    outtextxy(maxX/2-120, 260, "5. Triangle");
    outtextxy(maxX/2-120, 300, "6. Exit");

    setcolor(LIGHTGREEN);
    outtextxy(maxX/2-120, 340, "Enter choice (1-6) : ");
}

void display_line_menu() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(maxX/2-140, 40, "LINE ALGORITHMS");
    settextstyle(0, HORIZ_DIR, 1);

    setcolor(YELLOW);
    outtextxy(maxX/2-100, 120, "1. DDA Line");
    outtextxy(maxX/2-100, 160, "2. Bresenham Line");
    outtextxy(maxX/2-100, 200, "3. Back");

    setcolor(LIGHTGREEN);
    outtextxy(maxX/2-100, 260, "Choice (1-3): ");
}

/* ---------- Demo Functions with Transformations ---------- */

void line_demo() {
    int alg = 0;
    display_line_menu();
    char ch = getch();
    if (ch == '3') return;
    alg = ch - '0';
    if (alg < 1 || alg > 2) return;

    cleardevice();

    int x1 = get_number("x1 (0-639): ", 0, 639, 120);
    int y1 = get_number("y1 (0-479): ", 0, 479, 140);
    int x2 = get_number("x2 (0-639): ", 0, 639, 500);
    int y2 = get_number("y2 (0-479): ", 0, 479, 340);

    cleardevice();

    // Axes
    setcolor(DARKGRAY);
    line(0, maxY/2, maxX, maxY/2);
    line(maxX/2, 0, maxX/2, maxY);

    int cx = (x1 + x2)/2;
    int cy = (y1 + y2)/2;

    // ORIGINAL
    setcolor(WHITE);
    if (alg == 1) dda_line(x1,y1,x2,y2,WHITE);
    else          bresenham_line(x1,y1,x2,y2,WHITE);
    outtextxy(10, 10, "ORIGINAL");

    // TRANSLATED (+140, +80)
    int tx1=x1, ty1=y1, tx2=x2, ty2=y2;
    translate(tx1,ty1,140,80);
    translate(tx2,ty2,140,80);
    setcolor(LIGHTRED);
    if (alg == 1) dda_line(tx1,ty1,tx2,ty2,LIGHTRED);
    else          bresenham_line(tx1,ty1,tx2,ty2,LIGHTRED);
    outtextxy(10, 30, "Translated (+140,+80)");

    // ROTATED 45° around center
    int rx1=x1, ry1=y1, rx2=x2, ry2=y2;
    rotate_around(rx1,ry1,cx,cy,45);
    rotate_around(rx2,ry2,cx,cy,45);
    setcolor(LIGHTGREEN);
    if (alg == 1) dda_line(rx1,ry1,rx2,ry2,LIGHTGREEN);
    else          bresenham_line(rx1,ry1,rx2,ry2,LIGHTGREEN);
    outtextxy(10, 50, "Rotated 45 deg around center");

    // SCALED 1.5x around center
    int sx1=x1, sy1=y1, sx2=x2, sy2=y2;
    scale_around(sx1,sy1,cx,cy,1.5,1.5);
    scale_around(sx2,sy2,cx,cy,1.5,1.5);
    setcolor(YELLOW);
    if (alg == 1) dda_line(sx1,sy1,sx2,sy2,YELLOW);
    else          bresenham_line(sx1,sy1,sx2,sy2,YELLOW);
    outtextxy(10, 70, "Scaled 1.5x around center");

    setcolor(LIGHTCYAN);
    outtextxy(10, maxY-30, "Press any key to return...");
    getch();
}

void circle_demo() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3,HORIZ_DIR,3);
    outtextxy(maxX/2-140,40,"CIRCLE DEMO");
    settextstyle(0,HORIZ_DIR,1);

    int xc = get_number("Center X: ",0,639,maxX/2);
    int yc = get_number("Center Y: ",0,479,maxY/2);
    int r  = get_number("Radius  : ",10,200,90);

    cleardevice();

    // Axes
    setcolor(DARKGRAY);
    line(0, maxY/2, maxX, maxY/2);
    line(maxX/2, 0, maxX/2, maxY);

    // ORIGINAL
    setcolor(LIGHTMAGENTA);
    midpoint_circle(xc,yc,r,LIGHTMAGENTA);
    outtextxy(10,10,"ORIGINAL");

    // TRANSLATED
    setcolor(CYAN);
    midpoint_circle(xc+160, yc+70, r, CYAN);
    outtextxy(10,30,"Translated (+160,+70)");

    // SCALED 1.6x (center remains same)
    setcolor(YELLOW);
    midpoint_circle(xc, yc, (int)(r*1.6+0.5), YELLOW);
    outtextxy(10,50,"Scaled 1.6x");

    // Note: Rotation of circle around its center does not change appearance

    setcolor(LIGHTCYAN);
    outtextxy(10,maxY-30,"Press any key...");
    getch();
}

void ellipse_demo() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3,HORIZ_DIR,3);
    outtextxy(maxX/2-100,40,"ELLIPSE DEMO");

    int xc = get_number("Center X: ",0,639,maxX/2);
    int yc = get_number("Center Y: ",0,479,maxY/2);
    int rx = get_number("Radius X: ",20,200,140);
    int ry = get_number("Radius Y: ",20,200,80);

    cleardevice();

    line(0, maxY/2, maxX, maxY/2);
    line(maxX/2, 0, maxX/2, maxY);

    setcolor(LIGHTBLUE);
    draw_ellipse(xc,yc,rx,ry,LIGHTBLUE);
    outtextxy(10,10,"ORIGINAL");

    // Translated
    setcolor(LIGHTRED);
    draw_ellipse(xc+140,yc+60,rx,ry,LIGHTRED);
    outtextxy(10,30,"Translated");

    // Scaled
    setcolor(YELLOW);
    draw_ellipse(xc,yc,(int)(rx*1.4+0.5),(int)(ry*1.4+0.5),YELLOW);
    outtextxy(10,50,"Scaled 1.4x");

    setcolor(LIGHTCYAN);
    outtextxy(10,maxY-30,"Press any key...");
    getch();
}

void rectangle_demo() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3,HORIZ_DIR,3);
    outtextxy(maxX/2-140,40,"RECTANGLE DEMO");

    int x1 = get_number("x1: ",0,639,maxX/2-120);
    int y1 = get_number("y1: ",0,479,maxY/2-80);
    int x2 = get_number("x2: ",0,639,maxX/2+120);
    int y2 = get_number("y2: ",0,479,maxY/2+80);

    cleardevice();

    line(0,maxY/2,maxX,maxY/2);
    line(maxX/2,0,maxX/2,maxY);

    int cx = (x1+x2)/2;
    int cy = (y1+y2)/2;

    // ORIGINAL
    setcolor(LIGHTGREEN);
    rectangle(x1,y1,x2,y2);
    outtextxy(10,10,"ORIGINAL");

    // TRANSLATED
    setcolor(LIGHTRED);
    rectangle(x1+140,y1+60,x2+140,y2+60);
    outtextxy(10,30,"Translated (+140,+60)");

    // ROTATED 30° around center
    int rx1=x1, ry1=y1, rx2=x2, ry2=y2;
    int rx3=x2, ry3=y1, rx4=x1, ry4=y2;

    rotate_around(rx1,ry1,cx,cy,30);
    rotate_around(rx2,ry2,cx,cy,30);
    rotate_around(rx3,ry3,cx,cy,30);
    rotate_around(rx4,ry4,cx,cy,30);

    setcolor(LIGHTMAGENTA);
    line(rx1,ry1,rx3,ry3);
    line(rx3,ry3,rx2,ry2);
    line(rx2,ry2,rx4,ry4);
    line(rx4,ry4,rx1,ry1);
    outtextxy(10,50,"Rotated 30 deg");

    // SCALED
    int sx1=x1, sy1=y1, sx2=x2, sy2=y2;
    scale_around(sx1,sy1,cx,cy,1.5,1.5);
    scale_around(sx2,sy2,cx,cy,1.5,1.5);

    setcolor(YELLOW);
    rectangle(sx1,sy1,sx2,sy2);
    outtextxy(10,70,"Scaled 1.5x");

    setcolor(LIGHTCYAN);
    outtextxy(10,maxY-30,"Press any key...");
    getch();
}

void triangle_demo() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3,HORIZ_DIR,3);
    outtextxy(maxX/2-120,40,"TRIANGLE DEMO");

    int x1 = get_number("x1: ",0,639,maxX/2);
    int y1 = get_number("y1: ",0,479,maxY/2-140);
    int x2 = get_number("x2: ",0,639,maxX/2-160);
    int y2 = get_number("y2: ",0,479,maxY/2+100);
    int x3 = get_number("x3: ",0,639,maxX/2+160);
    int y3 = get_number("y3: ",0,479,maxY/2+100);

    cleardevice();

    line(0,maxY/2,maxX,maxY/2);
    line(maxX/2,0,maxX/2,maxY);

    int cx = (x1 + x2 + x3)/3;
    int cy = (y1 + y2 + y3)/3;

    // ORIGINAL
    setcolor(LIGHTRED);
    line(x1,y1,x2,y2);
    line(x2,y2,x3,y3);
    line(x3,y3,x1,y1);
    circle(x1,y1,4); circle(x2,y2,4); circle(x3,y3,4);
    outtextxy(10,10,"ORIGINAL");

    // TRANSLATED
    int tx1=x1,ty1=y1, tx2=x2,ty2=y2, tx3=x3,ty3=y3;
    translate(tx1,ty1,140,60);
    translate(tx2,ty2,140,60);
    translate(tx3,ty3,140,60);

    setcolor(LIGHTGREEN);
    line(tx1,ty1,tx2,ty2);
    line(tx2,ty2,tx3,ty3);
    line(tx3,ty3,tx1,ty1);
    outtextxy(10,30,"Translated (+140,+60)");

    // ROTATED 45°
    int rx1=x1,ry1=y1, rx2=x2,ry2=y2, rx3=x3,ry3=y3;
    rotate_around(rx1,ry1,cx,cy,45);
    rotate_around(rx2,ry2,cx,cy,45);
    rotate_around(rx3,ry3,cx,cy,45);

    setcolor(LIGHTMAGENTA);
    line(rx1,ry1,rx2,ry2);
    line(rx2,ry2,rx3,ry3);
    line(rx3,ry3,rx1,ry1);
    outtextxy(10,50,"Rotated 45 deg around centroid");

    // SCALED 1.4x
    int sx1=x1,sy1=y1, sx2=x2,sy2=y2, sx3=x3,sy3=y3;
    scale_around(sx1,sy1,cx,cy,1.4,1.4);
    scale_around(sx2,sy2,cx,cy,1.4,1.4);
    scale_around(sx3,sy3,cx,cy,1.4,1.4);

    setcolor(YELLOW);
    line(sx1,sy1,sx2,sy2);
    line(sx2,sy2,sx3,sy3);
    line(sx3,sy3,sx1,sy1);
    outtextxy(10,70,"Scaled 1.4x around centroid");

    setcolor(LIGHTCYAN);
    outtextxy(10,maxY-30,"Press any key...");
    getch();
}

/* ---------- Main ---------- */
int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");   // adjust path if needed

    maxX = getmaxx();
    maxY = getmaxy();

    while (1) {
        display_menu();
        char ch = getch();
        int opt = ch - '0';

        switch (opt) {
            case 1: line_demo();      break;
            case 2: circle_demo();    break;
            case 3: ellipse_demo();   break;
            case 4: rectangle_demo(); break;
            case 5: triangle_demo();  break;
            case 6:
                cleardevice();
                setcolor(LIGHTCYAN);
                settextstyle(3,HORIZ_DIR,4);
                outtextxy(maxX/2-180, maxY/2-40, "THANK YOU!");
                getch();
                closegraph();
                return 0;
            default:
                setcolor(LIGHTRED);
                outtextxy(maxX/2-140, maxY-50, "Invalid choice! Press any key...");
                getch();
        }
    }
}