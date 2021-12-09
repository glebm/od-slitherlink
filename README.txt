Slitherlink v1.2
---------------------------

Introduction
------------

Slitherlink is a logic puzzle game created in 1989 by Japanese publisher Nikoli,
famous for creating Sudoku, among many other logic puzzles.
I discovered the game a few years ago on the Nintendo DS with 'Puzzle Series
Vol. 5: Slitherlink', which was never released outside of Japan. It's such a
good game I became addicted to it, and now it's time to spread the virus :)

This is my implementation of the game for Dingux (Dingoo Linux).
Its main features are:

o 456 levels
o Four grid sizes: 5x5 (easy), 7x7, 10x10, 20x20 (hard)
o Undo button
o Progress saved automatically
o Save state option


Installation
------------

As usual:

o Extract the archive keeping the directory structure
o Copy the directory 'Slitherlink' and its contents somewhere on your SD card
  under 'local'
o Create a shortcut in your favorite menu.


Rules
-----

In Slitherlink, you draw lines on a grid to create a meandering path,
following the rules:

o The path must form a single loop, without crossing itself or branching.
o The numbers indicate how many lines surround each cell. Empty cells may be
  surrounded by any number of lines.

There is one unique solution for each level.
You can draw small x's on segments that cannot be connected.
You should be able to find the solution by logical deduction only, without guessing.

To Slitherlink beginners, I strongly advise to consult the Wikipedia page,
particularly the chapter 'Solution methods', which is a big help at the
beginning: http://en.wikipedia.org/wiki/Slitherlink#Solution_methods


Controls
--------

o D-pad        Move the cursor
o A/B/X/Y      Draw a line in the corresponding direction, then an 'x', then nothing
o L + A/B/X/Y  Draw an 'x' in the corresponding direction, then a line, then nothing
o R            Undo last action
o SELECT       Quit
o START        Open in-game menu, allowing to:
                 - Retry
                 - Save state
                 - Enable/disable sound
                 - Display help
                 - Quit game


Credits
-------

Homepage:      http://beyondds.free.fr/
Development:   Mia (http://beyondds.free.fr/)
Original game: Nikoli (http://www.nikoli.com/)
Puzzles:       Krazydad (http://www.krazydad.com/slitherlink/)
Fonts:         Karmatic Arcade by Vic Fieger (http://www.vicfieger.com/)
               Hardpixel by Jovanny Lemonad (http://jovanny.ru/)
Sounds:        Junggle
Background:    ??? (if you know the author, please e-mail me)
