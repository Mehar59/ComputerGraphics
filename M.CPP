#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ---------- Global Variables ---------- */
int maxX, maxY;  // Store screen dimensions

/* ---------- Drawing Functions ---------- */
void dda_line(int x1, int y1, int x2, int y2, int color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    if (steps == 0) {
        putpixel(x1, y1, color);
        return;
    }
    
    float xinc = dx / (float)steps;
    float yinc = dy / (float)steps;
    float x = x1, y = y1;
    
    for(int i = 0; i <= steps; i++) {
        putpixel((int)(x + 0.5), (int)(y + 0.5), color);
        x += xinc;
        y += yinc;
    }
}

void bresenham_line(int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 > x1) ? 1 : -1;
    int sy = (y2 > y1) ? 1 : -1;
    int err = dx - dy;
    
    while(1) {
        putpixel(x1, y1, color);
        if(x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if(e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if(e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void midpoint_circle(int xc, int yc, int r, int color) {
    int x = 0, y = r;
    int p = 1 - r;
    
    while(x <= y) {
        putpixel(xc + x, yc + y, color);
        putpixel(xc - x, yc + y, color);
        putpixel(xc + x, yc - y, color);
        putpixel(xc - x, yc - y, color);
        putpixel(xc + y, yc + x, color);
        putpixel(xc - y, yc + x, color);
        putpixel(xc + y, yc - x, color);
        putpixel(xc - y, yc - x, color);
        
        x++;
        if(p < 0) {
            p += 2 * x + 1;
        }
        else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

void bresenham_circle(int xc, int yc, int r, int color) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    
    while(x <= y) {
        putpixel(xc + x, yc + y, color);
        putpixel(xc - x, yc + y, color);
        putpixel(xc + x, yc - y, color);
        putpixel(xc - x, yc - y, color);
        putpixel(xc + y, yc + x, color);
        putpixel(xc - y, yc + x, color);
        putpixel(xc + y, yc - x, color);
        putpixel(xc - y, yc - x, color);
        
        if(d < 0) {
            d += 4 * x + 6;
        }
        else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void draw_ellipse(int xc, int yc, int rx, int ry, int color) {
    float d1, d2;
    int x = 0, y = ry;
    d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    float dx = 2 * ry * ry * x;
    float dy = 2 * rx * rx * y;
    
    // Region 1
    while(dx < dy) {
        putpixel(xc + x, yc + y, color);
        putpixel(xc - x, yc + y, color);
        putpixel(xc + x, yc - y, color);
        putpixel(xc - x, yc - y, color);
        
        if(d1 < 0) {
            x++;
            dx += 2 * ry * ry;
            d1 += dx + (ry * ry);
        }
        else {
            x++;
            y--;
            dx += 2 * ry * ry;
            dy -= 2 * rx * rx;
            d1 += dx - dy + (ry * ry);
        }
    }
    
    // Region 2
    d2 = (ry * ry * (x + 0.5) * (x + 0.5)) + 
         (rx * rx * (y - 1) * (y - 1)) - 
         (rx * rx * ry * ry);
         
    while(y >= 0) {
        putpixel(xc + x, yc + y, color);
        putpixel(xc - x, yc + y, color);
        putpixel(xc + x, yc - y, color);
        putpixel(xc - x, yc - y, color);
        
        if(d2 > 0) {
            y--;
            dy -= 2 * rx * rx;
            d2 += (rx * rx) - dy;
        }
        else {
            y--;
            x++;
            dx += 2 * ry * ry;
            dy -= 2 * rx * rx;
            d2 += dx - dy + (rx * rx);
        }
    }
}

/* ---------- Input Functions ---------- */
int get_number(char* prompt, int min, int max, int default_val) {
    char input[10] = "";
    char ch;
    int i = 0, num;
    int x = maxX/2, y = maxY/2;
    
    cleardevice();
    
    // Draw input box
    setcolor(LIGHTCYAN);
    rectangle(x-100, y-50, x+100, y+50);
    setfillstyle(SOLID_FILL, BLUE);
    bar(x-99, y-49, x+99, y+49);
    
    setcolor(YELLOW);
    outtextxy(x-90, y-30, prompt);
    
    setcolor(WHITE);
    rectangle(x-50, y, x+50, y+20);
    setfillstyle(SOLID_FILL, BLACK);
    bar(x-49, y+1, x+49, y+19);
    
    outtextxy(x-45, y+5, "      ");
    
    while(1) {
        ch = getch();
        
        if(ch == 13) {  // Enter key
            if(i > 0) {
                input[i] = '\0';
                num = atoi(input);
                if(num >= min && num <= max) return num;
            }
            return default_val;
        }
        else if(ch == 8) {  // Backspace
            if(i > 0) {
                i--;
                input[i] = '\0';
                setfillstyle(SOLID_FILL, BLACK);
                bar(x-49, y+1, x+49, y+19);
                outtextxy(x-45, y+5, input);
            }
        }
        else if(ch >= '0' && ch <= '9') {
            if(i < 4) {
                input[i] = ch;
                input[i+1] = '\0';
                i++;
                setfillstyle(SOLID_FILL, BLACK);
                bar(x-49, y+1, x+49, y+19);
                outtextxy(x-45, y+5, input);
            }
        }
    }
}

/* ---------- Display Functions ---------- */
void draw_box(int x1, int y1, int x2, int y2, int bg_color, int border_color) {
    setcolor(border_color);
    rectangle(x1, y1, x2, y2);
    setfillstyle(SOLID_FILL, bg_color);
    bar(x1+1, y1+1, x2-1, y2-1);
}

void display_menu() {
    cleardevice();
    
    // Title box
    setcolor(LIGHTMAGENTA);
    settextstyle(3, HORIZ_DIR, 4);
    outtextxy(maxX/2 - 150, 30, "GRAPHICS DEMO");
    settextstyle(0, HORIZ_DIR, 1);
    
    // Main menu box
    draw_box(maxX/2 - 120, 100, maxX/2 + 120, 350, BLUE, LIGHTCYAN);
    
    setcolor(YELLOW);
    outtextxy(maxX/2 - 100, 120, "1. Line Drawing Algorithms");
    outtextxy(maxX/2 - 100, 150, "2. Circle Drawing Algorithms");
    outtextxy(maxX/2 - 100, 180, "3. Ellipse Drawing");
    outtextxy(maxX/2 - 100, 210, "4. Rectangle Drawing");
    outtextxy(maxX/2 - 100, 240, "5. Triangle Drawing");
    outtextxy(maxX/2 - 100, 270, "6. Exit Program");
    
    setcolor(LIGHTGREEN);
    outtextxy(maxX/2 - 100, 310, "Select option (1-6): ");
    rectangle(maxX/2 + 40, 305, maxX/2 + 70, 325);
    
    // Footer
    setcolor(LIGHTGRAY);
    outtextxy(10, maxY - 20, "Use number keys 1-6 to select");
}

void display_line_menu() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(maxX/2 - 100, 50, "LINE DRAWING");
    settextstyle(0, HORIZ_DIR, 1);
    
    draw_box(maxX/2 - 120, 100, maxX/2 + 120, 250, BLUE, LIGHTCYAN);
    
    setcolor(YELLOW);
    outtextxy(maxX/2 - 100, 120, "1. DDA Algorithm");
    outtextxy(maxX/2 - 100, 150, "2. Bresenham Algorithm");
    outtextxy(maxX/2 - 100, 180, "3. Back to Main Menu");
    
    setcolor(LIGHTGREEN);
    outtextxy(maxX/2 - 100, 210, "Select algorithm (1-3): ");
}

void display_circle_menu() {
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(maxX/2 - 100, 50, "CIRCLE DRAWING");
    settextstyle(0, HORIZ_DIR, 1);
    
    draw_box(maxX/2 - 120, 100, maxX/2 + 120, 250, BLUE, LIGHTCYAN);
    
    setcolor(YELLOW);
    outtextxy(maxX/2 - 100, 120, "1. Midpoint Algorithm");
    outtextxy(maxX/2 - 100, 150, "2. Bresenham Algorithm");
    outtextxy(maxX/2 - 100, 180, "3. Back to Main Menu");
    
    setcolor(LIGHTGREEN);
    outtextxy(maxX/2 - 100, 210, "Select algorithm (1-3): ");
}

/* ---------- Demo Functions ---------- */
void line_demo() {
    int x1, y1, x2, y2;
    int choice;
    
    display_line_menu();
    choice = getch() - '0';
    if(choice == 3) return;
    
    cleardevice();
    
    // Get coordinates
    x1 = get_number("Enter x1 (0-639): ", 0, 639, 100);
    y1 = get_number("Enter y1 (0-479): ", 0, 479, 100);
    x2 = get_number("Enter x2 (0-639): ", 0, 639, 300);
    y2 = get_number("Enter y2 (0-479): ", 0, 479, 300);
    
    cleardevice();
    
    // Draw coordinate axes
    setcolor(DARKGRAY);
    line(maxX/2, 0, maxX/2, maxY);  // Y-axis
    line(0, maxY/2, maxX, maxY/2);  // X-axis
    
    // Draw line
    setcolor(choice == 1 ? LIGHTRED : LIGHTGREEN);
    if(choice == 1) {
        dda_line(x1, y1, x2, y2, LIGHTRED);
        outtextxy(10, 10, "DDA Line Algorithm");
    } else {
        bresenham_line(x1, y1, x2, y2, LIGHTGREEN);
        outtextxy(10, 10, "Bresenham Line Algorithm");
    }
    
    // Draw endpoints
    setcolor(YELLOW);
    circle(x1, y1, 3);
    circle(x2, y2, 3);
    
    // Display info
    char info[80];
    sprintf(info, "From (%d,%d) to (%d,%d)", x1, y1, x2, y2);
    outtextxy(10, maxY - 60, info);
    outtextxy(10, maxY - 40, "Press any key to continue...");
    
    getch();
}

void circle_demo() {
    int xc, yc, r;
    int choice;
    
    display_circle_menu();
    choice = getch() - '0';
    if(choice == 3) return;
    
    cleardevice();
    
    // Get parameters
    xc = get_number("Enter center X (0-639): ", 0, 639, maxX/2);
    yc = get_number("Enter center Y (0-479): ", 0, 479, maxY/2);
    r = get_number("Enter radius (10-200): ", 10, 200, 100);
    
    cleardevice();
    
    // Draw coordinate axes
    setcolor(DARKGRAY);
    line(maxX/2, 0, maxX/2, maxY);
    line(0, maxY/2, maxX, maxY/2);
    
    // Draw circle
    setcolor(choice == 1 ? LIGHTMAGENTA : CYAN);
    if(choice == 1) {
        midpoint_circle(xc, yc, r, LIGHTMAGENTA);
        outtextxy(10, 10, "Midpoint Circle Algorithm");
    } else {
        bresenham_circle(xc, yc, r, CYAN);
        outtextxy(10, 10, "Bresenham Circle Algorithm");
    }
    
    // Draw center
    setcolor(YELLOW);
    circle(xc, yc, 3);
    
    // Display info
    char info[80];
    sprintf(info, "Center: (%d,%d) Radius: %d", xc, yc, r);
    outtextxy(10, maxY - 60, info);
    outtextxy(10, maxY - 40, "Press any key to continue...");
    
    getch();
}

void ellipse_demo() {
    int xc, yc, rx, ry;
    
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(maxX/2 - 80, 50, "ELLIPSE");
    settextstyle(0, HORIZ_DIR, 1);
    
    // Get parameters
    xc = get_number("Enter center X (0-639): ", 0, 639, maxX/2);
    yc = get_number("Enter center Y (0-479): ", 0, 479, maxY/2);
    rx = get_number("Enter X-radius (10-200): ", 10, 200, 150);
    ry = get_number("Enter Y-radius (10-200): ", 10, 200, 100);
    
    cleardevice();
    
    // Draw coordinate axes
    setcolor(DARKGRAY);
    line(maxX/2, 0, maxX/2, maxY);
    line(0, maxY/2, maxX, maxY/2);
    
    // Draw ellipse
    setcolor(LIGHTBLUE);
    draw_ellipse(xc, yc, rx, ry, LIGHTBLUE);
    outtextxy(10, 10, "Ellipse Drawing Algorithm");
    
    // Draw center
    setcolor(YELLOW);
    circle(xc, yc, 3);
    
    // Display info
    char info[80];
    sprintf(info, "Center: (%d,%d) Rx: %d Ry: %d", xc, yc, rx, ry);
    outtextxy(10, maxY - 60, info);
    outtextxy(10, maxY - 40, "Press any key to continue...");
    
    getch();
}

void rectangle_demo() {
    int x1, y1, x2, y2;
    
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(maxX/2 - 100, 50, "RECTANGLE");
    settextstyle(0, HORIZ_DIR, 1);
    
    // Get coordinates
    x1 = get_number("Enter x1 (0-639): ", 0, 639, maxX/2 - 100);
    y1 = get_number("Enter y1 (0-479): ", 0, 479, maxY/2 - 80);
    x2 = get_number("Enter x2 (0-639): ", 0, 639, maxX/2 + 100);
    y2 = get_number("Enter y2 (0-479): ", 0, 479, maxY/2 + 80);
    
    cleardevice();
    
    // Draw coordinate axes
    setcolor(DARKGRAY);
    line(maxX/2, 0, maxX/2, maxY);
    line(0, maxY/2, maxX, maxY/2);
    
    // Draw rectangle
    setcolor(LIGHTGREEN);
    rectangle(x1, y1, x2, y2);
    outtextxy(10, 10, "Rectangle Drawing");
    
    // Display info
    char info[100];
    sprintf(info, "Top-left: (%d,%d) Bottom-right: (%d,%d)", x1, y1, x2, y2);
    outtextxy(10, maxY - 60, info);
    outtextxy(10, maxY - 40, "Press any key to continue...");
    
    getch();
}

void triangle_demo() {
    int x1, y1, x2, y2, x3, y3;
    
    cleardevice();
    setcolor(LIGHTCYAN);
    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(maxX/2 - 80, 50, "TRIANGLE");
    settextstyle(0, HORIZ_DIR, 1);
    
    // Get coordinates
    x1 = get_number("Enter x1 (0-639): ", 0, 639, maxX/2);
    y1 = get_number("Enter y1 (0-479): ", 0, 479, maxY/2 - 100);
    x2 = get_number("Enter x2 (0-639): ", 0, 639, maxX/2 - 100);
    y2 = get_number("Enter y2 (0-479): ", 0, 479, maxY/2 + 100);
    x3 = get_number("Enter x3 (0-639): ", 0, 639, maxX/2 + 100);
    y3 = get_number("Enter y3 (0-479): ", 0, 479, maxY/2 + 100);
    
    cleardevice();
    
    // Draw coordinate axes
    setcolor(DARKGRAY);
    line(maxX/2, 0, maxX/2, maxY);
    line(0, maxY/2, maxX, maxY/2);
    
    // Draw triangle
    setcolor(LIGHTRED);
    line(x1, y1, x2, y2);
    line(x2, y2, x3, y3);
    line(x3, y3, x1, y1);
    outtextxy(10, 10, "Triangle Drawing");
    
    // Draw vertices
    setcolor(YELLOW);
    circle(x1, y1, 3);
    circle(x2, y2, 3);
    circle(x3, y3, 3);
    
    // Display info
    char info[120];
    sprintf(info, "Vertices: (%d,%d), (%d,%d), (%d,%d)", x1, y1, x2, y2, x3, y3);
    outtextxy(10, maxY - 60, info);
    outtextxy(10, maxY - 40, "Press any key to continue...");
    
    getch();
}

/* ---------- Main Function ---------- */
void main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    
    maxX = getmaxx();
    maxY = getmaxy();
    
    int choice;
    char ch;
    
    while(1) {
        display_menu();
        
        ch = getch();
        choice = ch - '0';
        
        switch(choice) {
            case 1: line_demo(); break;
            case 2: circle_demo(); break;
            case 3: ellipse_demo(); break;
            case 4: rectangle_demo(); break;
            case 5: triangle_demo(); break;
            case 6:
                cleardevice();
                setcolor(LIGHTCYAN);
                settextstyle(3, HORIZ_DIR, 4);
                outtextxy(maxX/2 - 150, maxY/2 - 50, "THANK YOU!");
                settextstyle(0, HORIZ_DIR, 1);
                setcolor(YELLOW);
                outtextxy(maxX/2 - 100, maxY/2 + 20, "Graphics Demo Program");
                outtextxy(maxX/2 - 80, maxY/2 + 60, "Press any key to exit");
                getch();
                closegraph();
                return;
            default:
                // Show error message
                setcolor(LIGHTRED);
                outtextxy(maxX/2 - 80, maxY - 50, "Invalid choice! Press any key...");
                getch();
                break;
        }
    }
}