#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ---------- Global Variables ---------- */
int maxX, maxY;

/* ---------- Forward Declarations ---------- */
void dda_line(int x1, int y1, int x2, int y2, int color);
void bresenham_line(int x1, int y1, int x2, int y2, int color);
void midpoint_circle(int xc, int yc, int r, int color);
void draw_ellipse(int xc, int yc, int rx, int ry, int color);

void translate(int &x, int &y, int tx, int ty);
void rotate_around(int &x, int &y, int cx, int cy, float angle_deg);
void scale_around(int &x, int &y, int cx, int cy, float sx, float sy);

int get_number(char* prompt, int min, int max, int def);
int get_transformation_choice();

void display_menu();
void display_line_menu();

void line_demo();
void circle_demo();
void ellipse_demo();
void rectangle_demo();
void triangle_demo();

/* ---------- Drawing Functions ---------- */
void dda_line(int x1, int y1, int x2, int y2, int color) {
    int dx = x2 - x1, dy = y2 - y1;
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
        if (e2 < dx)  { err += dx; y1 += sy; }
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
        if (d1 < 0) { x++; dx += 2*ry*ry; d1 += dx + ry*ry; }
        else { x++; y--; dx += 2*ry*ry; dy -= 2*rx*rx; d1 += dx - dy + ry*ry; }
    }

    d2 = ry*ry*(x+0.5f)*(x+0.5f) + rx*rx*(y-1)*(y-1) - rx*rx*ry*ry;
    while (y >= 0) {
        putpixel(xc+x,yc+y,color); putpixel(xc-x,yc+y,color);
        putpixel(xc+x,yc-y,color); putpixel(xc-x,yc-y,color);
        if (d2 > 0) { y--; dy -= 2*rx*rx; d2 += rx*rx - dy; }
        else { y--; x++; dx += 2*ry*ry; dy -= 2*rx*rx; d2 += dx - dy + rx*rx; }
    }
}

/* ---------- Transformation Helpers ---------- */
void translate(int &x, int &y, int tx, int ty) {
    x += tx; y += ty;
}

void rotate_around(int &x, int &y, int cx, int cy, float angle_deg) {
    float rad = angle_deg * M_PI / 180.0f;
    float c = cos(rad), s = sin(rad);
    int tx = x - cx, ty = y - cy;
    x = cx + (int)(tx * c - ty * s + 0.5f);
    y = cy + (int)(tx * s + ty * c + 0.5f);
}

void scale_around(int &x, int &y, int cx, int cy, float sx, float sy) {
    x = cx + (int)((x - cx) * sx + 0.5f);
    y = cy + (int)((y - cy) * sy + 0.5f);
}

/* ---------- Input & Choice Functions ---------- */
int get_number(char* prompt, int min, int max, int def) {
    char buf[32] = "";
    int i = 0;
    int px = maxX/2, py = maxY/2;

    cleardevice();
    setcolor(LIGHTCYAN);
    rectangle(px-160, py-80, px+160, py+80);
    setfillstyle(SOLID_FILL, BLUE);
    bar(px-159, py-79, px+159, py+79);

    setcolor(YELLOW);
    outtextxy(px-140, py-60, prompt);

    setcolor(WHITE);
    rectangle(px-80, py-20, px+80, py+30);
    setfillstyle(SOLID_FILL, BLACK);
    bar(px-79, py-19, px+79, py+29);

    while (1) {
        char ch = getch();
        if (ch == 13) { // Enter
            if (i == 0) return def;
            buf[i] = '\0';
            int val = atoi(buf);
            if (val >= min && val <= max) return val;
            return def;
        }
        if (ch == 8 && i > 0) { // Backspace
            i--;
            buf[i] = '\0';
            bar(px-79, py-19, px+79, py+29);
            outtextxy(px-75, py-12, buf);
        }
        if ((ch >= '0' && ch <= '9') && i < 10) {
            buf[i++] = ch;
            buf[i] = '\0';
            bar(px-79, py-19, px+79, py+29);
            outtextxy(px-75, py-12, buf);
        }
        if (ch == '-' && i == 0) { // Allow negative at start
            buf[i++] = '-';
            buf[i] = '\0';
            bar(px-79, py-19, px+79, py+29);
            outtextxy(px-75, py-12, buf);
        }
    }
}

int get_transformation_choice() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3, HORIZ_DIR, 2);
    outtextxy(maxX/2-220, 80, "Select Transformation to Display");
    settextstyle(0, HORIZ_DIR, 1);

    setcolor(YELLOW);
    outtextxy(maxX/2-160, 140, "1. Translation only");
    outtextxy(maxX/2-160, 170, "2. Rotation only");
    outtextxy(maxX/2-160, 200, "3. Scaling only");
    outtextxy(maxX/2-160, 230, "4. Show ALL transformations");
    outtextxy(maxX/2-160, 260, "5. Skip / Return to menu");

    setcolor(LIGHTGREEN);
    outtextxy(maxX/2-160, 310, "Your choice (1-5): ");

    while (1) {
        char ch = getch();
        if (ch >= '1' && ch <= '5') return ch - '0';
    }
}

/* ---------- Menu Display ---------- */
void display_menu() {
    cleardevice();
    setcolor(LIGHTMAGENTA);
    settextstyle(3, HORIZ_DIR, 4);
    outtextxy(maxX/2 - 180, 20, "GRAPHICS DEMO");
    settextstyle(0, HORIZ_DIR, 1);

    setcolor(LIGHTCYAN);
    rectangle(maxX/2-150, 80, maxX/2+150, 380);
    setfillstyle(SOLID_FILL, BLUE);
    bar(maxX/2-149,81,maxX/2+149,379);

    setcolor(YELLOW);
    outtextxy(maxX/2-130, 110, "1. Line Algorithms");
    outtextxy(maxX/2-130, 150, "2. Circle");
    outtextxy(maxX/2-130, 190, "3. Ellipse");
    outtextxy(maxX/2-130, 230, "4. Rectangle");
    outtextxy(maxX/2-130, 270, "5. Triangle");
    outtextxy(maxX/2-130, 310, "6. Exit");

    setcolor(LIGHTGREEN);
    outtextxy(maxX/2-130, 350, "Enter choice (1-6): ");
}

void display_line_menu() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(maxX/2-160, 50, "LINE DRAWING");
    settextstyle(0, HORIZ_DIR, 1);

    setcolor(YELLOW);
    outtextxy(maxX/2-120, 140, "1. DDA Algorithm");
    outtextxy(maxX/2-120, 180, "2. Bresenham Algorithm");
    outtextxy(maxX/2-120, 220, "3. Back");

    setcolor(LIGHTGREEN);
    outtextxy(maxX/2-120, 280, "Choice (1-3): ");
}

/* ---------- Shape Demos ---------- */

void line_demo() {
    display_line_menu();
    char ch = getch();
    if (ch == '3') return;
    int alg = ch - '0';
    if (alg < 1 || alg > 2) return;

    cleardevice();

    int x1 = get_number("x1: ", 0, 639, 100);
    int y1 = get_number("y1: ", 0, 479, 120);
    int x2 = get_number("x2: ", 0, 639, 540);
    int y2 = get_number("y2: ", 0, 479, 360);

    int choice = get_transformation_choice();
    if (choice == 5) return;

    cleardevice();
    setcolor(DARKGRAY);
    line(0, maxY/2, maxX, maxY/2);
    line(maxX/2, 0, maxX/2, maxY);

    int cx = (x1 + x2) / 2;
    int cy = (y1 + y2) / 2;

    setcolor(WHITE);
    if (alg == 1) dda_line(x1,y1,x2,y2,WHITE);
    else          bresenham_line(x1,y1,x2,y2,WHITE);
    outtextxy(10, 10, "ORIGINAL");

    int ypos = 35;

    if (choice == 1 || choice == 4) {
        int tx = get_number("Translate X: ", -300, 300, 120);
        int ty = get_number("Translate Y: ", -200, 200, 80);
        int tx1 = x1, ty1 = y1, tx2 = x2, ty2 = y2;
        translate(tx1, ty1, tx, ty);
        translate(tx2, ty2, tx, ty);
        setcolor(LIGHTRED);
        if (alg == 1) dda_line(tx1,ty1,tx2,ty2,LIGHTRED);
        else          bresenham_line(tx1,ty1,tx2,ty2,LIGHTRED);
        char buf[80]; sprintf(buf, "Translated (%d, %d)", tx, ty);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    if (choice == 2 || choice == 4) {
        float ang = get_number("Angle (deg): ", -180, 180, 45);
        int rx1 = x1, ry1 = y1, rx2 = x2, ry2 = y2;
        rotate_around(rx1, ry1, cx, cy, ang);
        rotate_around(rx2, ry2, cx, cy, ang);
        setcolor(LIGHTGREEN);
        if (alg == 1) dda_line(rx1,ry1,rx2,ry2,LIGHTGREEN);
        else          bresenham_line(rx1,ry1,rx2,ry2,LIGHTGREEN);
        char buf[80]; sprintf(buf, "Rotated %.0f deg", ang);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    if (choice == 3 || choice == 4) {
        float sx = get_number("Scale X (5..30 = ×0.5..×3): ", 5, 30, 15) / 10.0f;
        float sy = get_number("Scale Y (5..30 = ×0.5..×3): ", 5, 30, 15) / 10.0f;
        int sx1 = x1, sy1 = y1, sx2 = x2, sy2 = y2;
        scale_around(sx1, sy1, cx, cy, sx, sy);
        scale_around(sx2, sy2, cx, cy, sx, sy);
        setcolor(YELLOW);
        if (alg == 1) dda_line(sx1,sy1,sx2,sy2,YELLOW);
        else          bresenham_line(sx1,sy1,sx2,sy2,YELLOW);
        char buf[80]; sprintf(buf, "Scaled (%.1f × %.1f)", sx, sy);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    setcolor(LIGHTCYAN);
    outtextxy(10, maxY-40, "Press any key to continue...");
    getch();
}

void circle_demo() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3,HORIZ_DIR,3);
    outtextxy(maxX/2-140,40,"CIRCLE");

    int xc = get_number("Center X: ", 0, 639, maxX/2);
    int yc = get_number("Center Y: ", 0, 479, maxY/2);
    int r  = get_number("Radius:   ", 10, 240, 90);

    int choice = get_transformation_choice();
    if (choice == 5) return;

    cleardevice();
    setcolor(DARKGRAY);
    line(0, maxY/2, maxX, maxY/2);
    line(maxX/2, 0, maxX/2, maxY);

    setcolor(LIGHTMAGENTA);
    midpoint_circle(xc, yc, r, LIGHTMAGENTA);
    outtextxy(10, 10, "ORIGINAL");

    int ypos = 35;

    if (choice == 1 || choice == 4) {
        int tx = get_number("Translate X: ", -300, 300, 150);
        int ty = get_number("Translate Y: ", -200, 200, 80);
        setcolor(CYAN);
        midpoint_circle(xc + tx, yc + ty, r, CYAN);
        char buf[80]; sprintf(buf, "Translated (%d, %d)", tx, ty);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    if (choice == 2 || choice == 4) {
        outtextxy(10, ypos, "Note: Circle rotation around center is invisible"); ypos += 25;
    }

    if (choice == 3 || choice == 4) {
        float s = get_number("Scale (5..30 = ×0.5..×3): ", 5, 30, 14) / 10.0f;
        setcolor(YELLOW);
        midpoint_circle(xc, yc, (int)(r * s + 0.5f), YELLOW);
        char buf[80]; sprintf(buf, "Scaled ×%.1f", s);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    setcolor(LIGHTCYAN);
    outtextxy(10, maxY-40, "Press any key...");
    getch();
}

void ellipse_demo() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3,HORIZ_DIR,3);
    outtextxy(maxX/2-120,40,"ELLIPSE");

    int xc = get_number("Center X: ", 0, 639, maxX/2);
    int yc = get_number("Center Y: ", 0, 479, maxY/2);
    int rx = get_number("Radius X: ", 20, 300, 140);
    int ry = get_number("Radius Y: ", 20, 200, 80);

    int choice = get_transformation_choice();
    if (choice == 5) return;

    cleardevice();
    setcolor(DARKGRAY);
    line(0, maxY/2, maxX, maxY/2);
    line(maxX/2, 0, maxX/2, maxY);

    setcolor(LIGHTBLUE);
    draw_ellipse(xc, yc, rx, ry, LIGHTBLUE);
    outtextxy(10, 10, "ORIGINAL");

    int ypos = 35;

    if (choice == 1 || choice == 4) {
        int tx = get_number("Translate X: ", -300, 300, 140);
        int ty = get_number("Translate Y: ", -200, 200, 70);
        setcolor(LIGHTRED);
        draw_ellipse(xc + tx, yc + ty, rx, ry, LIGHTRED);
        char buf[80]; sprintf(buf, "Translated (%d, %d)", tx, ty);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    if (choice == 2 || choice == 4) {
        outtextxy(10, ypos, "Ellipse rotation not implemented (needs full point transform)"); ypos += 25;
    }

    if (choice == 3 || choice == 4) {
        float s = get_number("Scale (5..30 = ×0.5..×3): ", 5, 30, 14) / 10.0f;
        setcolor(YELLOW);
        draw_ellipse(xc, yc, (int)(rx * s + 0.5f), (int)(ry * s + 0.5f), YELLOW);
        char buf[80]; sprintf(buf, "Scaled ×%.1f", s);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    setcolor(LIGHTCYAN);
    outtextxy(10, maxY-40, "Press any key...");
    getch();
}

void rectangle_demo() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3,HORIZ_DIR,3);
    outtextxy(maxX/2-160,40,"RECTANGLE");

    int x1 = get_number("Left   X: ", 0, 639, maxX/2 - 120);
    int y1 = get_number("Top    Y: ", 0, 479, maxY/2 - 80);
    int x2 = get_number("Right  X: ", 0, 639, maxX/2 + 120);
    int y2 = get_number("Bottom Y: ", 0, 479, maxY/2 + 80);

    int choice = get_transformation_choice();
    if (choice == 5) return;

    cleardevice();
    setcolor(DARKGRAY);
    line(0, maxY/2, maxX, maxY/2);
    line(maxX/2, 0, maxX/2, maxY);

    int cx = (x1 + x2) / 2;
    int cy = (y1 + y2) / 2;

    setcolor(LIGHTGREEN);
    rectangle(x1, y1, x2, y2);
    outtextxy(10, 10, "ORIGINAL");

    int ypos = 35;

    if (choice == 1 || choice == 4) {
        int tx = get_number("Translate X: ", -300, 300, 140);
        int ty = get_number("Translate Y: ", -200, 200, 70);
        setcolor(LIGHTRED);
        rectangle(x1+tx, y1+ty, x2+tx, y2+ty);
        char buf[80]; sprintf(buf, "Translated (%d, %d)", tx, ty);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    if (choice == 2 || choice == 4) {
        float ang = get_number("Angle (deg): ", -180, 180, 30);
        int ax = x1, ay = y1;
        int bx = x2, by = y1;
        int cx_ = x2, cy_ = y2;
        int dx = x1, dy = y2;

        rotate_around(ax, ay, cx, cy, ang);
        rotate_around(bx, by, cx, cy, ang);
        rotate_around(cx_, cy_, cx, cy, ang);
        rotate_around(dx, dy, cx, cy, ang);

        setcolor(LIGHTMAGENTA);
        line(ax, ay, bx, by);
        line(bx, by, cx_, cy_);
        line(cx_, cy_, dx, dy);
        line(dx, dy, ax, ay);
        char buf[80]; sprintf(buf, "Rotated %.0f deg", ang);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    if (choice == 3 || choice == 4) {
        float s = get_number("Scale (5..30 = ×0.5..×3): ", 5, 30, 15) / 10.0f;
        setcolor(YELLOW);
        rectangle((int)(cx + (x1-cx)*s + 0.5f),
                  (int)(cy + (y1-cy)*s + 0.5f),
                  (int)(cx + (x2-cx)*s + 0.5f),
                  (int)(cy + (y2-cy)*s + 0.5f));
        char buf[80]; sprintf(buf, "Scaled ×%.1f", s);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    setcolor(LIGHTCYAN);
    outtextxy(10, maxY-40, "Press any key...");
    getch();
}

void triangle_demo() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3,HORIZ_DIR,3);
    outtextxy(maxX/2-140,40,"TRIANGLE");

    int x1 = get_number("Vertex 1 X: ", 0, 639, maxX/2);
    int y1 = get_number("Vertex 1 Y: ", 0, 479, maxY/2 - 140);
    int x2 = get_number("Vertex 2 X: ", 0, 639, maxX/2 - 160);
    int y2 = get_number("Vertex 2 Y: ", 0, 479, maxY/2 + 100);
    int x3 = get_number("Vertex 3 X: ", 0, 639, maxX/2 + 160);
    int y3 = get_number("Vertex 3 Y: ", 0, 479, maxY/2 + 100);

    int choice = get_transformation_choice();
    if (choice == 5) return;

    cleardevice();
    setcolor(DARKGRAY);
    line(0, maxY/2, maxX, maxY/2);
    line(maxX/2, 0, maxX/2, maxY);

    int cx = (x1 + x2 + x3) / 3;
    int cy = (y1 + y2 + y3) / 3;

    setcolor(LIGHTRED);
    line(x1,y1,x2,y2); line(x2,y2,x3,y3); line(x3,y3,x1,y1);
    circle(x1,y1,5); circle(x2,y2,5); circle(x3,y3,5);
    outtextxy(10, 10, "ORIGINAL");

    int ypos = 35;

    if (choice == 1 || choice == 4) {
        int tx = get_number("Translate X: ", -300, 300, 140);
        int ty = get_number("Translate Y: ", -200, 200, 60);
        setcolor(LIGHTGREEN);
        line(x1+tx,y1+ty,x2+tx,y2+ty);
        line(x2+tx,y2+ty,x3+tx,y3+ty);
        line(x3+tx,y3+ty,x1+tx,y1+ty);
        char buf[80]; sprintf(buf, "Translated (%d, %d)", tx, ty);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    if (choice == 2 || choice == 4) {
        float ang = get_number("Angle (deg): ", -180, 180, 45);
        int ax = x1, ay = y1;
        int bx = x2, by = y2;
        int cx_ = x3, cy_ = y3;
        rotate_around(ax, ay, cx, cy, ang);
        rotate_around(bx, by, cx, cy, ang);
        rotate_around(cx_, cy_, cx, cy, ang);
        setcolor(LIGHTMAGENTA);
        line(ax,ay,bx,by);
        line(bx,by,cx_,cy_);
        line(cx_,cy_,ax,ay);
        char buf[80]; sprintf(buf, "Rotated %.0f deg", ang);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    if (choice == 3 || choice == 4) {
        float s = get_number("Scale (5..30 = ×0.5..×3): ", 5, 30, 14) / 10.0f;
        setcolor(YELLOW);
        int ax = (int)(cx + (x1-cx)*s + 0.5f);
        int ay = (int)(cy + (y1-cy)*s + 0.5f);
        int bx = (int)(cx + (x2-cx)*s + 0.5f);
        int by = (int)(cy + (y2-cy)*s + 0.5f);
        int cx_ = (int)(cx + (x3-cx)*s + 0.5f);
        int cy_ = (int)(cy + (y3-cy)*s + 0.5f);
        line(ax,ay,bx,by);
        line(bx,by,cx_,cy_);
        line(cx_,cy_,ax,ay);
        char buf[80]; sprintf(buf, "Scaled ×%.1f", s);
        outtextxy(10, ypos, buf); ypos += 25;
    }

    setcolor(LIGHTCYAN);
    outtextxy(10, maxY-40, "Press any key...");
    getch();
}

/* ---------- Main Program ---------- */
int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");  // ← adjust path if needed

    maxX = getmaxx();
    maxY = getmaxy();

    while (1) {
        display_menu();
        char ch = getch();
        if (ch < '1' || ch > '6') {
            setcolor(LIGHTRED);
            outtextxy(maxX/2-180, maxY-60, "Invalid choice! Press any key...");
            getch();
            continue;
        }

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
                settextstyle(3, HORIZ_DIR, 4);
                outtextxy(maxX/2-220, maxY/2-60, "THANK YOU!");
                settextstyle(0, HORIZ_DIR, 1);
                outtextxy(maxX/2-140, maxY/2+20, "Press any key to exit...");
                getch();
                closegraph();
                return 0;
        }
    }
}