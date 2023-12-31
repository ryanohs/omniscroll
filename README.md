# omniscroll

This utility allows you to scroll any window in Mac OS by holding the middle mouse button and moving the mouse. It supports both vertical and horiztonal scrolling.

To invert vertical scrolling change NATURAL_FACTOR to -1.

Horizontal scrolling can be disabled by commenting the HORIZONTAL_SCROLL_ENABLED line.

The sensitivity can be adjusted by changing the MULTIPLIER value. I prefer the value 1.5.

You must build the code yourself using `make`. If you do not have the Apple developer tools installed you will be prompted when running Make.

To run, use the command `./omniscroll` in Terminal.

This utility runs from Terminal. You must grant Terminal access to the Accessibility APIs in System Preferences > Security > Accessibility.

![System Preferences dialog](Permissions.png)

# Known issues

In Terminal, middle click pastes the clipboard.

In Visual Studio Code, middle click creates additional cursors. To workaround this issue, set Multi Cursor Limit to 1 in settings.

In Rider, middle click creates additional cursors and the scroll functionality does not work on any scrollable region in Rider. No known workaround.

# Future enhancements

I would like to be able to run this as a menu bar app.

I do not have an Apple developer account to sign and distribute a binary version.

Add keyboard shortcut to enable or disable.

Add momentum scrolling like on mobile.
