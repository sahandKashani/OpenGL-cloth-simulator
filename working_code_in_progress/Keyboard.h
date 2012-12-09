#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard
{
private:
    static Keyboard* instance;

    // array which holds pressed status values of all keyboard keys other than the
    // special ones (arrows + F1..F12 keys + home + end ...)
    char keyboardStatus[256];
    float angleIncrement;
    float translationIncrement;

protected:
    Keyboard();

public:
    static Keyboard* getInstance();

    void resetKeyboardStatus();
    void handleNormalKeyboardInput(unsigned char key, int x, int y);
    void handleNormalKeyboardRelease(unsigned char key, int x, int y);
    void applyNormalKeyboardActions();

    void handleSpecialKeyboardInput(int key, int x, int y);
};

#endif