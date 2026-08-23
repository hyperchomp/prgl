#ifndef PRGL_INPUT_H
#define PRGL_INPUT_H

#include <stdbool.h>

enum PRGLKeyboardKey
{
    /* Printable keys (Standard 7-bit ASCII Values) */
    PRGL_KEY_SPACE             = 32,
    PRGL_KEY_APOSTROPHE        = 39,  /* ' */
    PRGL_KEY_COMMA             = 44,  /* , */
    PRGL_KEY_MINUS             = 45,  /* - */
    PRGL_KEY_PERIOD            = 46,  /* . */
    PRGL_KEY_SLASH             = 47,  /* / */
    PRGL_KEY_0                 = 48,
    PRGL_KEY_1                 = 49,
    PRGL_KEY_2                 = 50,
    PRGL_KEY_3                 = 51,
    PRGL_KEY_4                 = 52,
    PRGL_KEY_5                 = 53,
    PRGL_KEY_6                 = 54,
    PRGL_KEY_7                 = 55,
    PRGL_KEY_8                 = 56,
    PRGL_KEY_9                 = 57,
    PRGL_KEY_SEMICOLON         = 59,  /* ; */
    PRGL_KEY_EQUAL             = 61,  /* = */
    PRGL_KEY_A                 = 65,
    PRGL_KEY_B                 = 66,
    PRGL_KEY_C                 = 67,
    PRGL_KEY_D                 = 68,
    PRGL_KEY_E                 = 69,
    PRGL_KEY_F                 = 70,
    PRGL_KEY_G                 = 71,
    PRGL_KEY_H                 = 72,
    PRGL_KEY_I                 = 73,
    PRGL_KEY_J                 = 74,
    PRGL_KEY_K                 = 75,
    PRGL_KEY_L                 = 76,
    PRGL_KEY_M                 = 77,
    PRGL_KEY_N                 = 78,
    PRGL_KEY_O                 = 79,
    PRGL_KEY_P                 = 80,
    PRGL_KEY_Q                 = 81,
    PRGL_KEY_R                 = 82,
    PRGL_KEY_S                 = 83,
    PRGL_KEY_T                 = 84,
    PRGL_KEY_U                 = 85,
    PRGL_KEY_V                 = 86,
    PRGL_KEY_W                 = 87,
    PRGL_KEY_X                 = 88,
    PRGL_KEY_Y                 = 89,
    PRGL_KEY_Z                 = 90,
    PRGL_KEY_LEFT_BRACKET      = 91,  /* [ */
    PRGL_KEY_BACKSLASH         = 92,  /* \ */
    PRGL_KEY_RIGHT_BRACKET     = 93,  /* ] */
    PRGL_KEY_GRAVE_ACCENT      = 96,  /* ` */
    PRGL_KEY_WORLD_1           = 161, /* non-US #1 */
    PRGL_KEY_WORLD_2           = 162, /* non-US #2 */

    /* Function keys */
    PRGL_KEY_ESCAPE            = 256,
    PRGL_KEY_ENTER             = 257,
    PRGL_KEY_TAB               = 258,
    PRGL_KEY_BACKSPACE         = 259,
    PRGL_KEY_INSERT            = 260,
    PRGL_KEY_DELETE            = 261,
    PRGL_KEY_RIGHT             = 262,
    PRGL_KEY_LEFT              = 263,
    PRGL_KEY_DOWN              = 264,
    PRGL_KEY_UP                = 265,
    PRGL_KEY_PAGE_UP           = 266,
    PRGL_KEY_PAGE_DOWN         = 267,
    PRGL_KEY_HOME              = 268,
    PRGL_KEY_END               = 269,
    PRGL_KEY_CAPS_LOCK         = 280,
    PRGL_KEY_SCROLL_LOCK       = 281,
    PRGL_KEY_NUM_LOCK          = 282,
    PRGL_KEY_PRINT_SCREEN      = 283,
    PRGL_KEY_PAUSE             = 284,
    PRGL_KEY_F1                = 290,
    PRGL_KEY_F2                = 291,
    PRGL_KEY_F3                = 292,
    PRGL_KEY_F4                = 293,
    PRGL_KEY_F5                = 294,
    PRGL_KEY_F6                = 295,
    PRGL_KEY_F7                = 296,
    PRGL_KEY_F8                = 297,
    PRGL_KEY_F9                = 298,
    PRGL_KEY_F10               = 299,
    PRGL_KEY_F11               = 300,
    PRGL_KEY_F12               = 301,
    PRGL_KEY_F13               = 302,
    PRGL_KEY_F14               = 303,
    PRGL_KEY_F15               = 304,
    PRGL_KEY_F16               = 305,
    PRGL_KEY_F17               = 306,
    PRGL_KEY_F18               = 307,
    PRGL_KEY_F19               = 308,
    PRGL_KEY_F20               = 309,
    PRGL_KEY_F21               = 310,
    PRGL_KEY_F22               = 311,
    PRGL_KEY_F23               = 312,
    PRGL_KEY_F24               = 313,
    PRGL_KEY_F25               = 314,
    
    /* Keypad keys */
    PRGL_KEY_KP_0              = 320,
    PRGL_KEY_KP_1              = 321,
    PRGL_KEY_KP_2              = 322,
    PRGL_KEY_KP_3              = 323,
    PRGL_KEY_KP_4              = 324,
    PRGL_KEY_KP_5              = 325,
    PRGL_KEY_KP_6              = 326,
    PRGL_KEY_KP_7              = 327,
    PRGL_KEY_KP_8              = 328,
    PRGL_KEY_KP_9              = 329,
    PRGL_KEY_KP_DECIMAL        = 330,
    PRGL_KEY_KP_DIVIDE         = 331,
    PRGL_KEY_KP_MULTIPLY       = 332,
    PRGL_KEY_KP_SUBTRACT       = 333,
    PRGL_KEY_KP_ADD            = 334,
    PRGL_KEY_KP_ENTER          = 335,
    PRGL_KEY_KP_EQUAL          = 336,
    
    /* Modifier keys */
    PRGL_KEY_LEFT_SHIFT        = 340,
    PRGL_KEY_LEFT_CONTROL      = 341,
    PRGL_KEY_LEFT_ALT          = 342,
    PRGL_KEY_LEFT_SUPER        = 343,
    PRGL_KEY_RIGHT_SHIFT       = 344,
    PRGL_KEY_RIGHT_CONTROL     = 345,
    PRGL_KEY_RIGHT_ALT         = 346,
    PRGL_KEY_RIGHT_SUPER       = 347,
    PRGL_KEY_MENU              = 348
};

bool prgl_key_pressed(int key);
void prgl_mouse_position(double *x_pos, double *y_pos);

#endif
