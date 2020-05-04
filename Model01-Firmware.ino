
// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

// GOAL - make a great layout for MacOS (I also have a windows version in the master branch)
//
// Features:
// remap arrows to inverted T (IJKL)
// map U/O to ctrl + left/right arrow for easier text selection
// remap mouse to inverted T (ESDF)
// right fn locks to fn layer (ninja text editing mode!)
// enable one shot modifiers (non sticky)
// programming brackets keys:
// esc:       [{
// butterfly: ]}
// led:       =+
// any:       '"
// Danish characters to fn layer:
// fn + p = å
// fn + ; = æ
// fn + ' = ø
// Map fn + ZXCVA to ctrl + undo, cut, copy, paste, select all
// fn + lower upper right thumb keys is left/right click
// Thumb keys moved around (cmd, bksp, shift, ctrl) (ctrl, shift, space, alt)
// prog key is mapped to esc
// heatmap leds on by default
// assumes WinCompose is installed with US language and RightAlt/AltGr modifier selected in windows
// I use the layout with colemak as the OS layout, but the keyboard firmware is still querty

//#define KALEIDOSCOPE_HOSTOS_GUESSER 1
/**
 * These #include directives pull in the Kaleidoscope firmware core,
 * as well as the Kaleidoscope plugins we use in the Model 01's firmware
 */


// The Kaleidoscope core
#include "Kaleidoscope.h"

// Support for storing the keymap in EEPROM
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"

// Support for communicating with the host via a simple Serial protocol
#include "Kaleidoscope-FocusSerial.h"

// Support for keys that move the mouse
#include "Kaleidoscope-MouseKeys.h"

// Support for macros
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"
// https://kaleidoscope.readthedocs.io/en/latest/plugins/LED-ActiveModColor.html
#include <Kaleidoscope-LED-ActiveModColor.h>

// Support for "Numpad" mode, which is mostly just the Numpad specific LED mode
#include "Kaleidoscope-NumPad.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for an LED mode creates a heat map of the most used keys
#include "Kaleidoscope-Heatmap.h"

// highlight active modifiers
#include "Kaleidoscope-LED-ActiveModColor.h"
// keep modifiers active for a moment
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Escape-OneShot.h"
// #include <kaleidoscope/hid.h>
//#include <Kaleidoscope-DualUse.h>
//#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-TopsyTurvy.h"

// Support for shared palettes for other plugins, like Colormap below
#include "Kaleidoscope-LED-Palette-Theme.h"

// Support for an LED mode that lets one configure per-layer color maps
#include "Kaleidoscope-Colormap.h"

// Support for Keyboardio's internal keyboard testing mode
#include "Kaleidoscope-HardwareTestMode.h"

//#include "Kaleidoscope-HostOS.h"
//#include "Kaleidoscope/HostOS-select.h"
//#include "Kaleidoscope-Unicode.h"

// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// Support for magic combos (key chords that trigger an action)
#include "Kaleidoscope-MagicCombo.h"

// Support for USB quirks, like changing the key state report protocol
#include "Kaleidoscope-USB-Quirks.h"

/** This 'enum' is a list of all the macros used by the Model 01's firmware
  * The names aren't particularly important. What is important is that each
  * is unique.
  *
  * These are the names of your macros. They'll be used in two places.
  * The first is in your keymap definitions. There, you'll use the syntax
  * `M(MACRO_NAME)` to mark a specific keymap position as triggering `MACRO_NAME`
  *
  * The second usage is in the 'switch' statement in the `macroAction` function.
  * That switch statement actually runs the code associated with a macro when
  * a macro key is pressed.
  */

enum { 
      L_AE,
      L_OE,
      L_AA,
      L_E_,
      M_WIN,
      M_MAC,
      M_LNX
      };

/** The Model 01's key layouts are defined as 'keymaps'. By default, there are three
  * keymaps: The standard QWERTY keymap, the "Function layer" keymap and the "Numpad"
  * keymap.
  *
  * Each keymap is defined as a list using the 'KEYMAP_STACKED' macro, built
  * of first the left hand's layout, followed by the right hand's layout.
  *
  * Keymaps typically consist mostly of `Key_` definitions. There are many, many keys
  * defined as part of the USB HID Keyboard specification. You can find the names
  * (if not yet the explanations) for all the standard `Key_` defintions offered by
  * Kaleidoscope in these files:
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keyboard.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_consumerctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_sysctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keymaps.h
  *
  * Additional things that should be documented here include
  *   using ___ to let keypresses fall through to the previously active layer
  *   using XXX to mark a keyswitch as 'blocked' on this layer
  *   using ShiftToLayer() and LockLayer() keys to change the active keymap.
  *   keeping NUM and FN consistent and accessible on all layers
  *
  * The PROG key is special, since it is how you indicate to the board that you
  * want to flash the firmware. However, it can be remapped to a regular key.
  * When the keyboard boots, it first looks to see whether the PROG key is held
  * down; if it is, it simply awaits further flashing instructions. If it is
  * not, it continues loading the rest of the firmware and the keyboard
  * functions normally, with whatever binding you have set to PROG. More detail
  * here: https://community.keyboard.io/t/how-the-prog-key-gets-you-into-the-bootloader/506/8
  *
  * The "keymaps" data structure is a list of the keymaps compiled into the firmware.
  * The order of keymaps in the list is important, as the ShiftToLayer(#) and LockLayer(#)
  * macros switch to key layers based on this list.
  *
  *

  * A key defined as 'ShiftToLayer(FUNCTION)' will switch to FUNCTION while held.
  * Similarly, a key defined as 'LockLayer(NUMPAD)' will switch to NUMPAD when tapped.
  */

/**
  * Layers are "0-indexed" -- That is the first one is layer 0. The second one is layer 1.
  * The third one is layer 2.
  * This 'enum' lets us use names like QWERTY, FUNCTION, and NUMPAD in place of
  * the numbers 0, 1 and 2.
  */

enum { QWERTY, NUMPAD, FUNCTION }; // layers

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

KEYMAPS(

  [QWERTY] = KEYMAP_STACKED
  (Key_Escape,   Key_1, Key_2, Key_3, Key_4, Key_5, TOPSY(Quote),
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, TOPSY(LeftBracket),
   Key_LeftControl, Key_Backspace, OSM(LeftShift), OSM(LeftGui),
   ShiftToLayer(FUNCTION),

   Key_Quote,         Key_6, Key_7, Key_8,     Key_9,         Key_0,         LockLayer(NUMPAD),
   Key_Enter,         Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
                      Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
   Key_LeftAlt,  Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
   OSM(LeftGui), OSM(LeftShift),  Key_Spacebar, Key_RightAlt, // Key_RightAlt is not oneshot, since wincompose assumes you tap and release the wincompose key
   LockLayer(FUNCTION)),
   
  [NUMPAD] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, Key_LEDEffectNext,
   M(M_LNX), ___, ___, ___, ___, ___, ___,
   M(M_WIN), ___, ___, ___, ___, ___,
   M(M_MAC), ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   ___, ___,       Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___,
   Key_Enter, ___, Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      Key_PrintScreen,
              ___, Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,         Key_Quote,
   ___, Key_KeypadMultiply, Key_Keypad0, Key_Comma, Key_Period,    Key_KeypadDivide,   Key_Enter,
   ___, ___, ___, ___,
   ___),
    
  [FUNCTION] =  KEYMAP_STACKED
  (Key_Escape,          Key_F1,           Key_F2,          Key_F3,        Key_F4,          Key_F5,           Key_Equals,
   Key_Insert,          ___,              ___,             ___,           ___,             ___,              Key_Tab,
   Key_mouseScrollUp,   LGUI(Key_A),      LALT(Key_Semicolon),             LALT(Key_A),   LALT(Key_Semicolon),     LALT(Key_Quote),
   Key_mouseScrollDn,   LGUI(Key_Z),      LGUI(Key_X),     LGUI(Key_C),   LGUI(Key_V),     Key_Delete,       TOPSY(RightBracket),
   ___, ___, ___, ___,
   ___,

   ___, Key_F6,          Key_F7,           Key_F8,          Key_F9,                   Key_F10,                  Key_F11,
   Key_Enter,                  Key_Home,   LALT(Key_LeftArrow),   Key_UpArrow,        LALT(Key_RightArrow),     LALT(Key_A),         Key_F12,
                               Key_End,         Key_LeftArrow,    Key_DownArrow,            Key_RightArrow,     LALT(Key_O),     LALT(Key_Quote),
   Consumer_PlaySlashPause,    Consumer_ScanNextTrack,Key_Mute,   Consumer_VolumeDecrement, Consumer_VolumeIncrement, Key_Backslash,   Key_Pipe,
   ___, ___, Key_mouseBtnL, Key_mouseBtnR,
   ___)
) // KEYMAPS(
 
/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

/** macroAction dispatches keymap events that are tied to a macro
    to that macro. It takes two uint8_t parameters.

    The first is the macro being called (the entry in the 'enum' earlier in this file).
    The second is the state of the keyswitch. You can use the keyswitch state to figure out
    if the key has just been toggled on, is currently pressed or if it's just been released.

    The 'switch' statement should have a 'case' for each entry of the macro enum.
    Each 'case' statement should call out to a function to handle the macro in question.

 */
 const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {
    case L_AE:
      compose2(Key_A, true, Key_E, true, keyState);
      break;
    case L_OE:
      compose2(Key_Slash, false, Key_O, true, keyState);
      break;
    case L_AA:
      compose2(Key_O, false, Key_A, true, keyState);
      break;
    case L_E_:
      compose2(Key_Quote, false, Key_E, true, keyState);
      break;
    case M_LNX:
      //HostOS.os(kaleidoscope::hostos::LINUX);
      break;
    case M_MAC:
      //HostOS.os(kaleidoscope::hostos::OSX);
      break;
    case M_WIN:
      //HostOS.os(kaleidoscope::hostos::WINDOWS);
      break;
  }
  return MACRO_NONE;
 }

static void compose2(Key key1, bool shift1, Key key2, bool shift2, uint8_t keyState) {
  if (!keyToggledOn(keyState)) {
    return;
  }
    bool shifted = Kaleidoscope.hid().keyboard().wasModifierKeyActive(Key_LeftShift)
  || Kaleidoscope.hid().keyboard().wasModifierKeyActive(Key_RightShift);

  tap(Key_RightAlt);
  if (shifted && shift1) press(Key_LeftShift);
  tap(key1);
  if (shifted && shift1) release(Key_LeftShift);
  if (shifted && shift2) press(Key_LeftShift);
  tap(key2);
  if (shifted && shift2) release(Key_LeftShift);
  //release(Key_RightAlt);
}

static void press(Key key) {
  Kaleidoscope.hid().keyboard().pressKey(key);
  Kaleidoscope.hid().keyboard().sendReport();
}

static void release(Key key) {
  Kaleidoscope.hid().keyboard().releaseKey(key);
  Kaleidoscope.hid().keyboard().sendReport();
}

static void tap(Key key) {
  press(key);
  release(key);
}

/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
 * and turns them back on when it wakes up.
 */
void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event) {
  switch (event) {
  case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.disable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.enable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Sleep:
    break;
  }
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
 * resume, and sleep) to other functions that perform action based on these
 * events.
 */
void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

/** This 'enum' is a list of all the magic combos used by the Model 01's
 * firmware The names aren't particularly important. What is important is that
 * each is unique.
 *
 * These are the names of your magic combos. They will be used by the
 * `USE_MAGIC_COMBOS` call below.
 */
enum {
  // Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
  // mode.
  COMBO_TOGGLE_NKRO_MODE,
  // Enter test mode
  COMBO_ENTER_TEST_MODE
};

/** Wrappers, to be used by MagicCombo. **/

/**
 * This simply toggles the keyboard protocol via USBQuirks, and wraps it within
 * a function with an unused argument, to match what MagicCombo expects.
 */
static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

/**
 *  This enters the hardware test mode
 */
static void enterHardwareTestMode(uint8_t combo_index) {
  HardwareTestMode.runTests();
}


/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS({.action = toggleKeyboardProtocol,
                  // Left Fn + Esc + Shift
                  .keys = { R3C6, R2C6, R3C7 }
}, {
  .action = enterHardwareTestMode,
  // Left Fn + Prog + LED
  .keys = { R3C6, R0C0, R0C6 }
});

// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(
  // The EEPROMSettings & EEPROMKeymap plugins make it possible to have an
  // editable keymap in EEPROM.
  EEPROMSettings,
  EEPROMKeymap,

  // Focus allows bi-directional communication with the host, and is the
  // interface through which the keymap in EEPROM can be edited.
  Focus,

  // FocusSettingsCommand adds a few Focus commands, intended to aid in
  // changing some settings of the keyboard, such as the default layer (via the
  // `settings.defaultLayer` command)
  FocusSettingsCommand,

  // FocusEEPROMCommand adds a set of Focus commands, which are very helpful in
  // both debugging, and in backing up one's EEPROM contents.
  FocusEEPROMCommand,

  // The boot greeting effect pulses the LED button for 10 seconds after the
  // keyboard is first connected
  BootGreetingEffect,

  // The hardware test mode, which can be invoked by tapping Prog, LED and the
  // left Fn button at the same time.
  HardwareTestMode,

  // LEDControl provides support for other LED modes
  LEDControl,
  // color active mods
  ActiveModColorEffect,
  // We start with the LED effect that turns off all the LEDs.
  LEDOff,

    // make a heapmap of mostly used keys
    HeatmapEffect,

  // disable other LED modes
  // // The rainbow effect changes the color of all of the keyboard's keys at the same time
  // // running through all the colors of the rainbow.
  // LEDRainbowEffect,

  // // The rainbow wave effect lights up your keyboard with all the colors of a rainbow
  // // and slowly moves the rainbow across your keyboard
  // LEDRainbowWaveEffect,

  // // The chase effect follows the adventure of a blue pixel which chases a red pixel across
  // // your keyboard. Spoiler: the blue pixel never catches the red pixel
  // LEDChaseEffect,

  // // These static effects turn your keyboard's LEDs a variety of colors
  // solidRed, solidOrange, solidYellow, solidGreen, solidBlue, solidIndigo, solidViolet,

  // // The breathe effect slowly pulses all of the LEDs on your keyboard
  // LEDBreatheEffect,

  // // The AlphaSquare effect prints each character you type, using your
  // // keyboard's LEDs as a display
  // AlphaSquareEffect,

  // // The stalker effect lights up the keys you've pressed recently
  // StalkerEffect,

  // The LED Palette Theme plugin provides a shared palette for other plugins,
  // like Colormap below
  LEDPaletteTheme,

  // The Colormap effect makes it possible to set up per-layer colormaps
  ColormapEffect,

  // The numpad plugin is responsible for lighting up the 'numpad' mode
  // with a custom LED effect
  NumPad,

  // The macros plugin adds support for macros
  Macros,

  // The MouseKeys plugin lets you add keys to your keymap which move the mouse.
  MouseKeys,

  // The HostPowerManagement plugin allows us to turn LEDs off when then host
  // goes to sleep, and resume them when it wakes up.
  HostPowerManagement,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at the
  // same time.
  MagicCombo,

  // enable oneshot and dualuse plugins
  OneShot,
  EscapeOneShot,
  //DualUse,
  //Qukeys,
  TopsyTurvy,

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  USBQuirks
);

/** The 'setup' function is one of the two standard Arduino sketch functions.
  * It's called when your keyboard first powers up. This is where you set up
  * Kaleidoscope and any plugins.
  */
void setup() {
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  // While we hope to improve this in the future, the NumPad plugin
  // needs to be explicitly told which keymap layer is your numpad layer
  NumPad.numPadLayer = NUMPAD;
  // We configure the AlphaSquare effect to use RED letters
//  AlphaSquare.color = CRGB(255, 0, 0);
  // We set the brightness of the rainbow effects to 150 (on a scale of 0-255)
  // This draws more than 500mA, but looks much nicer than a dimmer effect
//  LEDRainbowEffect.brightness(150);
//  LEDRainbowWaveEffect.brightness(150);

  // Set the action key the test mode should listen for to Left Fn
  HardwareTestMode.setActionKey(R3C6);

  // The LED Stalker mode has a few effects. The one we like is called
  // 'BlazingTrail'. For details on other options, see
  // https://github.com/keyboardio/Kaleidoscope/blob/master/docs/plugins/LED-Stalker.md
//  StalkerEffect.variant = STALKER(BlazingTrail);

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
//  LEDOff.activate();

  // activate heatmap at startup instead
  HeatmapEffect.activate();

    // To make the keymap editable without flashing new firmware, we store
  // additional layers in EEPROM. For now, we reserve space for five layers. If
  // one wants to use these layers, just set the default layer to one in EEPROM,
  // by using the `settings.defaultLayer` Focus command, or by using the
  // `keymap.onlyCustom` command to use EEPROM layers only.
  EEPROMKeymap.setup(5);

  // We need to tell the Colormap plugin how many layers we want to have custom
  // maps for. To make things simple, we set it to five layers, which is how
  // many editable layers we have (see above).
  ColormapEffect.max_layers(5);

  // configure one-shot:
  // disable sticky feature
  //OneShot.double_tap_sticky = false;
  OneShot.disableStickabilityForModifiers();
  // shorter timer
  OneShot.time_out = 1500;

  ActiveModColorEffect.highlight_color = CRGB(0x00, 0x00, 0xff);
}

/** loop is the second of the standard Arduino sketch functions.
  * As you might expect, it runs in a loop, never exiting.
  *
  * For Kaleidoscope-based keyboard firmware, you usually just want to
  * call Kaleidoscope.loop(); and not do anything custom here.
  */

void loop() {
  Kaleidoscope.loop();
}
