SuperEasyAwesomeDraw
==
Drawing application for 1-3 year old children who are still learning to use mouse and keyboard.

Kid won't easily get the program into weird state even if they mash all the buttons.

When the program starts, the crayon is already drawing on the canvas, just move the mouse around. When kid gets better with mouse, you can enable "draw only when mouse button is pressed".

![alt text](https://github.com/dusteddk/supereasyawesomedraw/blob/master/screenshot.jpg?raw=true)

Features
==
- No menus
- No dialogs
- No interface elements

Controls (kid)
==
- Move the mouse to draw
- Press space-bar to change color
- Roll wheel to change color
- Press backspace to clear the canvas

Controls (parent)
==
- Ctrl + w - Quits
- Ctrl + s - Saves the drawing (in the current directory)
- Ctrl + f - Toggle fullscreen (borderless)
- Ctrl + Enter - Toggle between "always draw" and "press mouse button to draw"

Building
==
```
cmake .
make
```

Running
==
```
./draw
```

Requires
==
- cmake
- make
- C compiler that supports C11 (gcc for example)
- SDL2 and development headers
- SDL2-image and development headers

License
==
WTFPL (See LICENSE file)
