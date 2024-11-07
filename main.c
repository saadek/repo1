#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    Display *display;
    Window window;
    int screen;
    GC gc;
    XEvent event;
    unsigned long black, white;
    int sr = 100;

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
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    // Utworzenie kontekstu graficznego
    gc = XCreateGC(display, window, 0, NULL);
    XSetForeground(display, gc, black);

    // Główna pętla
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
        
            XDrawLine(display, window, gc, sr + 0, sr + 0, sr + 40, sr + 50);
            XDrawLine(display, window, gc, sr + 0, sr + 0, sr + 20, sr + 0);
            XDrawLine(display, window, gc, sr + 20, sr + 0, sr + 50, sr + 40);
            XDrawLine(display, window, gc, sr + 50, sr + 40, sr + 80, sr + 0);
            XDrawLine(display, window, gc, sr + 80, sr + 0, sr + 100, sr + 0);
            XDrawLine(display, window, gc, sr + 60, sr + 50, sr + 100, sr + 0);
            XDrawLine(display, window, gc, sr + 60, sr + 50, sr + 100, sr + 100);
            XDrawLine(display, window, gc, sr + 100, sr + 100, sr + 80, sr + 100);
            XDrawLine(display, window, gc, sr + 80, sr + 100, sr + 50, sr + 60);
            XDrawLine(display, window, gc, sr + 20, sr + 100, sr + 50, sr + 60);
            XDrawLine(display, window, gc, sr + 0, sr + 100, sr + 20, sr + 100);
            XDrawLine(display, window, gc, sr + 0, sr + 100, sr + 40, sr + 50);

            // Ustawienie koloru wypełnienia półokręgu
            XSetForeground(display, gc, 0xFF5733); // Ustal kolor np. pomarańczowy (RGB: #FF5733)

            // Rysowanie i wypełnianie półokręgu
            XFillArc(display, window, gc, 100, 200, 100, 100, 0, 180 * 64); // 180*64 to półokrąg

            // Przywracanie koloru linii
            XSetForeground(display, gc, black);
            XDrawArc(display, window, gc, 100, 200, 100, 100, 0, 180 * 64); // Obramowanie półokręgu
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

