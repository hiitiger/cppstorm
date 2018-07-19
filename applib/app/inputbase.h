#pragma once

namespace Storm
{
    enum InputKey
    {
        E_KEY_LBUTTON       = 0x01,
        E_KEY_RBUTTON       = 0x02,
        E_KEY_CANCEL        = 0x03,
        E_KEY_MBUTTON       = 0x04,
        E_KEY_XBUTTON1      = 0x05,
        E_KEY_XBUTTON2      = 0x06,
        E_KEY_BACK          = 0x08,
        E_KEY_TAB           = 0x09,
        E_KEY_CLEAR         = 0x0C,
        E_KEY_RETURN        = 0x0D,

        E_KEY_SHIFT         = 0x10,
        E_KEY_CONTROL       = 0x11,
        E_KEY_MENU          = 0x12,
        E_KEY_PAUSE         = 0x13,
        E_KEY_CAPITAL       = 0x14,

        E_KEY_KANA          = 0x15,
        E_KEY_HANGEUL       = 0x15,
        E_KEY_HANGUL        = 0x15,
        E_KEY_JUNJA         = 0x17,
        E_KEY_FINAL         = 0x18,
        E_KEY_HANJA         = 0x19,
        E_KEY_KANJI         = 0x19,
        E_KEY_ESCAPE        = 0x1B,
        E_KEY_CONVERT       = 0x1C,
        E_KEY_NONCONVERT    = 0x1D,
        E_KEY_ACCEPT        = 0x1E,
        E_KEY_MODECHANGE    = 0x1F,
        E_KEY_SPACE         = 0x20,
        E_KEY_PRIOR         = 0x21,
        E_KEY_NEXT          = 0x22,
        E_KEY_END           = 0x23,
        E_KEY_HOME          = 0x24,
        E_KEY_LEFT          = 0x25,
        E_KEY_UP            = 0x26,
        E_KEY_RIGHT         = 0x27,
        E_KEY_DOWN          = 0x28,
        E_KEY_SELECT        = 0x29,
        E_KEY_PRINT         = 0x2A,
        E_KEY_EXECUTE       = 0x2B,
        E_KEY_SNAPSHOT      = 0x2C,
        E_KEY_INSERT        = 0x2D,
        E_KEY_DELETE        = 0x2E,
        E_KEY_HELP          = 0x2F,

        E_KEY_0             = 0x30,
        E_KEY_1             = 0x31,
        E_KEY_2             = 0x32,
        E_KEY_3             = 0x33,
        E_KEY_4             = 0x34,
        E_KEY_5             = 0x35,
        E_KEY_6             = 0x36,
        E_KEY_7             = 0x37,
        E_KEY_8             = 0x38,
        E_KEY_9             = 0x39,

        E_KEY_A             = 0x41,
        E_KEY_B             = 0x42,
        E_KEY_C             = 0x43,
        E_KEY_D             = 0x44,
        E_KEY_E             = 0x45,
        E_KEY_F             = 0x46,
        E_KEY_G             = 0x47,
        E_KEY_H             = 0x48,
        E_KEY_I             = 0x49,
        E_KEY_J             = 0x4A,
        E_KEY_K             = 0x4B,
        E_KEY_L             = 0x4C,
        E_KEY_M             = 0x4D,
        E_KEY_N             = 0x4E,
        E_KEY_O             = 0x4F,
        E_KEY_P             = 0x50,
        E_KEY_Q             = 0x51,
        E_KEY_R             = 0x52,
        E_KEY_S             = 0x53,
        E_KEY_T             = 0x54,
        E_KEY_U             = 0x55,
        E_KEY_V             = 0x56,
        E_KEY_W             = 0x57,
        E_KEY_X             = 0x58,
        E_KEY_Y             = 0x59,
        E_KEY_Z             = 0x5A,

        E_KEY_LWIN          = 0x5B,
        E_KEY_RWIN          = 0x5C,
        E_KEY_APPS          = 0x5D,
        E_KEY_SLEEP         = 0x5F,                              
        E_KEY_NUMPAD0       = 0x60,
        E_KEY_NUMPAD1       = 0x61,
        E_KEY_NUMPAD2       = 0x62,
        E_KEY_NUMPAD3       = 0x63,
        E_KEY_NUMPAD4       = 0x64,
        E_KEY_NUMPAD5       = 0x65,
        E_KEY_NUMPAD6       = 0x66,
        E_KEY_NUMPAD7       = 0x67,
        E_KEY_NUMPAD8       = 0x68,
        E_KEY_NUMPAD9       = 0x69,
        E_KEY_MULTIPLY      = 0x6A,
        E_KEY_ADD           = 0x6B,
        E_KEY_SEPARATOR     = 0x6C,
        E_KEY_SUBTRACT      = 0x6D,
        E_KEY_DECIMAL       = 0x6E,
        E_KEY_DIVIDE        = 0x6F,
        E_KEY_F1            = 0x70,
        E_KEY_F2            = 0x71,
        E_KEY_F3            = 0x72,
        E_KEY_F4            = 0x73,
        E_KEY_F5            = 0x74,
        E_KEY_F6            = 0x75,
        E_KEY_F7            = 0x76,
        E_KEY_F8            = 0x77,
        E_KEY_F9            = 0x78,
        E_KEY_F10           = 0x79,
        E_KEY_F11           = 0x7A,
        E_KEY_F12           = 0x7B,
        E_KEY_F13           = 0x7C,
        E_KEY_F14           = 0x7D,
        E_KEY_F15           = 0x7E,
        E_KEY_F16           = 0x7F,
        E_KEY_F17           = 0x80,
        E_KEY_F18           = 0x81,
        E_KEY_F19           = 0x82,
        E_KEY_F20           = 0x83,
        E_KEY_F21           = 0x84,
        E_KEY_F22           = 0x85,
        E_KEY_F23           = 0x86,
        E_KEY_F24           = 0x87,
        E_KEY_NUMLOCK       = 0x90,
        E_KEY_SCROLL        = 0x91,
        E_KEY_OEM_NEC_EQUAL = 0x92,
        E_KEY_LSHIFT        = 0xA0,
        E_KEY_RSHIFT        = 0xA1,
        E_KEY_LCONTROL      = 0xA2,
        E_KEY_RCONTROL      = 0xA3,
        E_KEY_LMENU         = 0xA4,
        E_KEY_RMENU         = 0xA5,
        E_KEY_OEM_1         = 0xBA,  // ';:' for US
        E_KEY_OEM_PLUS      = 0xBB,  // '+' any country
        E_KEY_OEM_COMMA     = 0xBC,  // ',' any country
        E_KEY_OEM_MINUS     = 0xBD,  // '-' any country
        E_KEY_OEM_PERIOD    = 0xBE,  // '.' any country
        E_KEY_OEM_2         = 0xBF,  // '/?' for US
        E_KEY_OEM_3         = 0xC0,  // '`~' for US
        E_KEY_OEM_4         = 0xDB, //  '[{' for US
        E_KEY_OEM_5         = 0xDC, //  '\|' for US
        E_KEY_OEM_6         = 0xDD, //  ']}' for US
        E_KEY_OEM_7         = 0xDE, //  ''"' for US
        E_KEY_OEM_8         = 0xDF,
    };

    enum ModifierKey
    {
        E_NoModifier = 0x00000000,
        E_ShiftModifier = 0x02000000,
        E_ControlModifier = 0x04000000,
        E_AltModifier = 0x08000000,
    };

    DEFINE_ENUM_FLAG_OPERATORS(ModifierKey);

    enum KeyAction
    {
        E_KeyAction_Down = 1,
        E_KeyAction_Up = 2,
    };


    enum MouseButton
    {
        E_MB_None = 0,
        E_MB_Left = 0x1,
        E_MB_Right = 0x2,
        E_MB_Middle = 0x4,
    };

    enum  MouseAction
    {
        E_MouseAction_Press = 1,
        E_MouseAction_Release = 2,
   //     E_MouseAction_Move = 3,
    };
}
