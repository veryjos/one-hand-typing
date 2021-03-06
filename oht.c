/**
 * Compile using the following command line:
 * gcc -Wall -o oht oht.c -framework ApplicationServices
 *
 * The source code used as the base for this application can be found at
 * http://osxbook.com/book/bonus/chapter2/alterkeys/
 */

#include "oht.h"

int main(void) {
  CFMachPortRef eventTap;
  CGEventMask eventMask;
  CFRunLoopSourceRef runLoopSource;

  // Create an event tap. We are interested in key presses.
  eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp));
  eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, myCGEventCallback, NULL);
  if (!eventTap) {
    fprintf(stderr, "failed to create event tap\n");
    exit(1);
  }

  // Create a run loop source.
  runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);

  // Add to the current run loop.
  CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

  // Enable the event tap.
  CGEventTapEnable(eventTap, true);

  // Set it all running.
  CFRunLoopRun();

  exit(0);
}

CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
  // The incoming keycode.
  CGKeyCode keycode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

  // Caps lock is mapped to f19 which has the key code 80
  if (keycode == 80) {
    isCapsPressed = (type == kCGEventKeyDown);

    return 0;
  }

  if (isCapsPressed) {
    keycode = swapKeys(keycode);
  }

  // Set the modified keycode field in the event.
  CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t) keycode);

  return event;
}

CGKeyCode swapKeys(CGKeyCode keycode) {
  if (isCapsPressed) {
    switch(keycode) {
      // asdfg row
      case _A:      keycode = _H;       break;
      case _S:      keycode = _J;       break;
      case _D:      keycode = _K;       break;
      case _F:      keycode = _L;       break;
      case _G:      keycode = _COLON;   break;
      case _H:      keycode = _SINGLE_QUOTE; break;
      // qwerty row
      case _Q:      keycode = _Y;       break;
      case _W:      keycode = _U;       break;
      case _E:      keycode = _I;       break;
      case _R:      keycode = _O;       break;
      case _T:      keycode = _P;       break;
      // zxcvb row
      case _Z:      keycode = _N;       break;
      case _X:      keycode = _M;       break;
      case _C:      keycode = _COMMA;   break;
      case _V:      keycode = _PERIOD;  break;
      case _B:      keycode = _FORWARD_SLASH; break;
      // 12345 row
      case _1:      keycode = _0;       break;
      case _2:      keycode = _9;       break;
      case _3:      keycode = _8;       break;
      case _4:      keycode = _7;       break;
      case _5:      keycode = _6;       break;
      // Special keys
      case _SPACE: keycode = _RETURN; break;
      case _TAB: keycode = _DELETE; break;
      case _GRAVE_ACCENT: keycode = _DASH; break;
    }
  }

  return keycode;
}
