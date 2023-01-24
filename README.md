# x11

Repository name is pretty cocky, it is just a set of two programs
using [Xlib](https://www.x.org/releases/X11R7.5/doc/libX11/libX11.html).

## Command reader

Reads commands in a file and executes them.
Options are really limited, more in a section **Grammer**.

Examples are in `capture_and_clicker.txt`, `2d.txt`, and `commands.txt`.

All of them are matching my OS (logo, keyboard shortcut) and used software.

### Grammer

Format is really weak. It does not support comments, fails with the first error.

It does not support anything complicated, it is just LL(0). One of the commands silently
_remembers_ its state (`FIND`).

- `PRESS SEQUENCE <text>`
  - Writes down `<text>`. Focus on window is ignored, so the outcome is unknown.
- `PRESS COMMAND <text>`
  - Parses `<text>`, but instead writing everything down, it tries to convert hyphen-separated text to phrases, where some of them have special meaning.
  - CTR -> press left control
  - ALT -> press left alt
  - unknown -> just press it
  - For instance, `CTR-ALT-T` opens terminal.
- `FIND <path_to_bitmap>`
  - Finds first occurrence of image, stores its coordinates.
- `MOVE MOUSE`
  - Moves mouse to a position stored after the last `FIND` call.
  - Position is forgotten.
- `CLICK`
  - Left click of the mouse.

## SGT – Puzzles Collection: Light up

One of the games which comes with the Xubuntu is Light up, which is fairly simple.
It is like placing rooks on the board with some extra rules.

`light_up` solves this 7x7 puzzle on 1920x1080 resolution (fullscreen).

It shows some capabilities of _xlib_, like reading a gaming board, moving a mouse,
pressing left/right button. Game evaluation is done in code, board is read just once,
outcome of the clicks is not evaluated, everything is just a representation of the
inner state.

Game is solved the same way as a human would do it. No bruteforce, guessing, nor CSP
(Constraint satisfaction problem).
