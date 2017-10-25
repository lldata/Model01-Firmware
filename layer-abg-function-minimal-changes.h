// Define the ABG Minimal Changes function layer

/* This layer contains the extra keys PgUp, PgDn and Num that are missing from
 * the ABG multilingual layer. It is otherwise identical to the standard 
 * function layer. It is strongly recommended that if the ABG multilingual 
 * layer is enabled, one of the ABG function layers be used in place of the
 * standard layer.
 */

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

KEYMAP_STACKED
  (___,      Key_F1,           Key_F2,      Key_F3,       Key_F4,        Key_F5,           XXX,
   Key_Tab,  ___,              Key_mouseUp, Key_PageUp,   Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE,
   Key_Home, Key_mouseL,       Key_mouseDn, Key_mouseR,   Key_mouseBtnL, Key_mouseWarpNW,
   Key_End,  Key_PrintScreen,  Key_Insert,  Key_PageDown, Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE,
   ___, Key_Delete, ___, ___,
   ___,

   Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                   Key_F9,          Key_F10,          Key_F11,
   Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
                               Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  ___,              Key_KeypadNumLock,
   Key_PcApplication,          Key_Mute,               Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,             Key_Backslash,    Key_Pipe,
   ___, ___, Key_Enter, ___,
   ___)

// *INDENT-ON*
