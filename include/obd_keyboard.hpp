#ifndef OBD_KEYBOARD_HPP
#define OBD_KEYBOARD_HPP

char waitForKey(); // lit une touche sans bloquer plus que nécessaire
bool isEscape(char key);
bool isEnter(char key);

#endif
