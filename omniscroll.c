#include <ApplicationServices/ApplicationServices.h>

// Adjust the scrolling speed relative to mouse move speed.
// 1 matches mouse movement exactly. This felt too sluggish.
// 1.5 works for me.
// 2 is a bit fast for me.
#define MULTIPLIER 1

// This app does not respect the "Scroll Direction: Natural" flag in System Preferences > Mouse.
// Set NATURAL_FACTOR to -1 if the checkbox is disabled to match your scrolling inversion preference.
#define NATURAL_FACTOR 1

// Comment following line to disable horizontal scrolling
#define HORIZONTAL_SCROLL_ENABLED

// Store the scroll starting point which will be included on each scroll event.
// This allows the cursor to move outside of the window and the scroll events will keep going to the original window.
// Without this, dragging the cursor outside of the window will start sending the scroll events to the window under 
// the new cursor position.
static CGPoint scrollOrigin;

// For computing the delta of each event
static float lastX = 0, lastY = 0;

    static CGEventRef eventOccurred(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon)
    {
        CGPoint location = CGEventGetLocation(event);
        CGMouseButton button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);

        switch(type) 
        {
            case kCGEventOtherMouseDown:
            {
                if(button == kCGMouseButtonCenter)
                {
                    scrollOrigin = location;
                    lastX = location.x;
                    lastY = location.y;
                }
                break;
            }
            case kCGEventOtherMouseDragged:
            {
                // When the mouse center button is held down and the mouse is moved, translate movement into scroll wheel.
                if(button == kCGMouseButtonCenter)
                {
                    float deltaX = location.x - lastX;
                    float deltaY = location.y - lastY;
                    lastX = location.x;
                    lastY = location.y;

                    // Vertical scroll
                    int dy = (int)round(deltaY * MULTIPLIER * NATURAL_FACTOR);
                    CGEventRef scrollEvent = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, dy);
                    if(scrollEvent != NULL)
                    {
                        CGEventSetLocation(event, scrollOrigin);
                        CGEventPost(kCGHIDEventTap, scrollEvent);
                        CFRelease(scrollEvent);
                    }

    #ifdef HORIZONTAL_SCROLL_ENABLED
                    // Some apps can't scroll diagonally so simulate this with two separate events.
                    // Ex: https://bugs.chromium.org/p/chromium/issues/detail?id=114956
                    int dx = (int)round(deltaX * MULTIPLIER * NATURAL_FACTOR);
                    scrollEvent = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 2, 0, dx);
                    if(scrollEvent != NULL)
                    {
                        CGEventSetLocation(event, scrollOrigin);
                        CGEventPost(kCGHIDEventTap, scrollEvent);
                        CFRelease(scrollEvent);
                    }
    #endif
                }
                break;
            }
        }

        return event;
    }

CFMachPortRef createEventTap()
{  
    CGEventMask eventMask = CGEventMaskBit(kCGEventOtherMouseDown)
                            | CGEventMaskBit(kCGEventOtherMouseDragged);

    CFStringRef keys[] = { kAXTrustedCheckOptionPrompt };
    CFBooleanRef values[] = { kCFBooleanTrue };
    CFDictionaryRef dictionary = CFDictionaryCreate(NULL,
                                                    (const void **)&keys,
                                                    (const void **)&values,
                                                    1,
                                                    &kCFTypeDictionaryKeyCallBacks,
                                                    &kCFTypeDictionaryValueCallBacks);

    int isTrusted = AXIsProcessTrustedWithOptions(dictionary);
    if(!isTrusted)
    {
        printf("Application is not trusted in System Preferences > Security > Accessibility\n");
        printf("Rurun after trusting application.\n");
        return NULL;
    }

    CFRelease(dictionary);

    return CGEventTapCreate(kCGHIDEventTap,
                            kCGHeadInsertEventTap, 
                            kCGEventTapOptionListenOnly,
                            eventMask, 
                            eventOccurred, 
                            NULL); 
}

int main (void)
{
    CFMachPortRef tap = createEventTap();

    if (tap) 
    {
        printf("Omni scrolling active. Press and hold mouse center button to scroll. Press Control+C to exit.\n");
        CFRunLoopSourceRef rl = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, tap, 0);
        CFRunLoopAddSource(CFRunLoopGetMain(), rl, kCFRunLoopCommonModes);
        CGEventTapEnable(tap, true);
        CFRunLoopRun();
        CFRelease(tap);
    }

    return 0;
}
