#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <array>

namespace Fusion::Keyboard
{

    static constexpr unsigned int keyCount{512}; // Maximum number of supported keys

    enum Key
    {
        SPACE = 32,         /* espaço */
        APOSTROPHE = 39,    /* ' */
        COMMA = 44,         /* , */
        MINUS = 45,         /* - */
        PERIOD = 46,        /* . */
        SLASH = 47,         /* / */
        N_0 = 48,           /* 0 */
        N_1 = 49,           /* 1 */
        N_2 = 50,           /* 2 */
        N_3 = 51,           /* 3 */
        N_4 = 52,           /* 4 */
        N_5 = 53,           /* 5 */
        N_6 = 54,           /* 6 */
        N_7 = 55,           /* 7 */
        N_8 = 56,           /* 8 */
        N_9 = 57,           /* 9 */
        SEMICOLON = 59,     /* ; */
        EQUAL = 61,         /* = */
        A = 65,             /* A */
        B = 66,             /* B */
        C = 67,             /* C */
        D = 68,             /* D */
        E = 69,             /* E */
        F = 70,             /* F */
        G = 71,             /* G */
        H = 72,             /* H */
        I = 73,             /* I */
        J = 74,             /* J */
        K = 75,             /* K */
        L = 76,             /* L */
        M = 77,             /* M */
        N = 78,             /* N */
        O = 79,             /* O */
        P = 80,             /* P */
        Q = 81,             /* Q */
        R = 82,             /* R */
        S = 83,             /* S */
        T = 84,             /* T */
        U = 85,             /* U */
        V = 86,             /* V */
        W = 87,             /* W */
        X = 88,             /* X */
        Y = 89,             /* Y */
        Z = 90,             /* Z */
        LEFT_BRACKET = 91,  /* [ */
        BACKSLASH = 92,     /* \ */
        RIGHT_BRACKET = 93, /* ] */
        GRAVE_ACCENT = 96,  /* ` */
        WORLD_1 = 161,      /* non-US #1 */
        WORLD_2 = 162,      /* non-US #2 */

        /* Function keys */
        ESCAPE = 256,       /* Esc */
        ENTER = 257,        /* Enter */
        TAB = 258,          /* Tab */
        BACKSPACE = 259,    /* Backspace */
        INSERT = 260,       /* Insert */
        DELETE = 261,       /* Delete */
        RIGHT = 262,        /* Seta → */
        LEFT = 263,         /* Seta ← */
        DOWN = 264,         /* Seta ↓ */
        UP = 265,           /* Seta ↑ */
        PAGE_UP = 266,      /* Page Up */
        PAGE_DOWN = 267,    /* Page Down */
        HOME = 268,         /* Home */
        END = 269,          /* End */
        CAPS_LOCK = 280,    /* Caps Lock */
        SCROLL_LOCK = 281,  /* Scroll Lock */
        NUM_LOCK = 282,     /* Num Lock */
        PRINT_SCREEN = 283, /* Print Screen */
        PAUSE = 284,        /* Pause */
        F1 = 290,           /* F1 */
        F2 = 291,           /* F2 */
        F3 = 292,           /* F3 */
        F4 = 293,           /* F4 */
        F5 = 294,           /* F5 */
        F6 = 295,           /* F6 */
        F7 = 296,           /* F7 */
        F8 = 297,           /* F8 */
        F9 = 298,           /* F9 */
        F10 = 299,          /* F10 */
        F11 = 300,          /* F11 */
        F12 = 301,          /* F12 */
        F13 = 302,          /* F13 */
        F14 = 303,          /* F14 */
        F15 = 304,          /* F15 */
        F16 = 305,          /* F16 */
        F17 = 306,          /* F17 */
        F18 = 307,          /* F18 */
        F19 = 308,          /* F19 */
        F20 = 309,          /* F20 */
        F21 = 310,          /* F21 */
        F22 = 311,          /* F22 */
        F23 = 312,          /* F23 */
        F24 = 313,          /* F24 */
        F25 = 314,          /* F25 */

        /* Keypad */
        KP_0 = 320,        /* Keypad 0 */
        KP_1 = 321,        /* Keypad 1 */
        KP_2 = 322,        /* Keypad 2 */
        KP_3 = 323,        /* Keypad 3 */
        KP_4 = 324,        /* Keypad 4 */
        KP_5 = 325,        /* Keypad 5 */
        KP_6 = 326,        /* Keypad 6 */
        KP_7 = 327,        /* Keypad 7 */
        KP_8 = 328,        /* Keypad 8 */
        KP_9 = 329,        /* Keypad 9 */
        KP_DECIMAL = 330,  /* Keypad . */
        KP_DIVIDE = 331,   /* Keypad / */
        KP_MULTIPLY = 332, /* Keypad * */
        KP_SUBTRACT = 333, /* Keypad - */
        KP_ADD = 334,      /* Keypad + */
        KP_ENTER = 335,    /* Keypad Enter */
        KP_EQUAL = 336,    /* Keypad = */

        /* Modificadores */
        LEFT_SHIFT = 340,    /* Shift esquerdo */
        LEFT_CONTROL = 341,  /* Ctrl esquerdo */
        LEFT_ALT = 342,      /* Alt esquerdo */
        LEFT_SUPER = 343,    /* Super (Windows/Command) esquerdo */
        RIGHT_SHIFT = 344,   /* Shift direito */
        RIGHT_CONTROL = 345, /* Ctrl direito */
        RIGHT_ALT = 346,     /* Alt direito */
        RIGHT_SUPER = 347,   /* Super (Windows/Command) direito */
        MENU = 348           /* Menu */
    };


    struct KeyboardData
    {
        std::array<bool, keyCount> currentState{};
        std::array<bool, keyCount> previousState{};
    };

    bool IsKeyPressed(int key);
    bool IsKeyDown(int key);
    bool IsKeyReleased(int key);

} // namespace Fusion::Keyboard

#endif
