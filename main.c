#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main() {
    Display *display;
    Window window;
    int screen;
    GC gc;
    XEvent event;
    unsigned long black, white;
    int sr_x = 100, sr_y = 100; // Początkowy środek ciężkości „X”
    int drag = 0; // Flaga przeciągania
    float scale = 1.0; // Początkowa skala

    // Inicjalizacja wyświetlacza
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Nie można otworzyć wyświetlacza\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);

    // Utworzenie okna
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 400, 300, 1, black, white);
    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XMapWindow(display, window);

    // Utworzenie kontekstu graficznego
    gc = XCreateGC(display, window, 0, NULL);
    XSetForeground(display, gc, black);

    // Funkcja do rysowania skalowanego „X”
    void draw_scaled_x(int centerX, int centerY, float scale) {
        XClearWindow(display, window);

        // Rysowanie skalowanego „X”
        XDrawLine(display, window, gc, centerX + scale * 0, centerY + scale * 0, centerX + scale * 40, centerY + scale * 50);
        XDrawLine(display, window, gc, centerX + scale * 0, centerY + scale * 0, centerX + scale * 20, centerY + scale * 0);
        XDrawLine(display, window, gc, centerX + scale * 20, centerY + scale * 0, centerX + scale * 50, centerY + scale * 40);
        XDrawLine(display, window, gc, centerX + scale * 50, centerY + scale * 40, centerX + scale * 80, centerY + scale * 0);
        XDrawLine(display, window, gc, centerX + scale * 80, centerY + scale * 0, centerX + scale * 100, centerY + scale * 0);
        XDrawLine(display, window, gc, centerX + scale * 60, centerY + scale * 50, centerX + scale * 100, centerY + scale * 0);
        XDrawLine(display, window, gc, centerX + scale * 60, centerY + scale * 50, centerX + scale * 100, centerY + scale * 100);
        XDrawLine(display, window, gc, centerX + scale * 100, centerY + scale * 100, centerX + scale * 80, centerY + scale * 100);
        XDrawLine(display, window, gc, centerX + scale * 80, centerY + scale * 100, centerX + scale * 50, centerY + scale * 60);
        XDrawLine(display, window, gc, centerX + scale * 20, centerY + scale * 100, centerX + scale * 50, centerY + scale * 60);
        XDrawLine(display, window, gc, centerX + scale * 0, centerY + scale * 100, centerX + scale * 20, centerY + scale * 100);
        XDrawLine(display, window, gc, centerX + scale * 0, centerY + scale * 100, centerX + scale * 40, centerY + scale * 50);
    }

    // Główna pętla
    while (1) {
        XNextEvent(display, &event);
        
        if (event.type == Expose) {
            draw_scaled_x(sr_x, sr_y, scale);
        }
        
        // Przycisk myszy wciśnięty - aktywowanie przeciągania
        if (event.type == ButtonPress) {
            drag = 1;
        }

        // Przycisk myszy zwolniony - dezaktywowanie przeciągania
        if (event.type == ButtonRelease) {
            drag = 0;
        }

        // Ruch myszy podczas przeciągania
        if (event.type == MotionNotify && drag) {
            // Oblicz nową skalę na podstawie odległości od początkowego środka ciężkości
            int dx = event.xmotion.x - sr_x;
            int dy = event.xmotion.y - sr_y;
            scale = sqrt(dx * dx + dy * dy) / 100.0; // Skala zależna od odległości

            if (scale < 0.5) scale = 0.5;   // Minimalna skala
            if (scale > 3.0) scale = 3.0;   // Maksymalna skala

            draw_scaled_x(sr_x, sr_y, scale);
        }

        // Zamknięcie okna przy naciśnięciu klawisza
        if (event.type == KeyPress)
            break;
    }

    // Sprzątanie
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}

