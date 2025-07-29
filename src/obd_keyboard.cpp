#include "../include/obd_keyboard.hpp"
#include <conio.h> // pour _getch

char waitForKey() {
    return _getch(); // attend une touche et la retourne
}

bool isEscape(char key) {
    return key == 27; // 27 = code ASCII de Échap
}

bool isEnter(char key) {
    return key == '\r' || key == '\n'; // selon le terminal
}
