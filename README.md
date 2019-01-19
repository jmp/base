# Base

This is base code for simple, 2-dimensional games written in C.
It is something I've developed for my own use over several years.

Features:

* Sprite/texture loading from BMP files.
* Sound effect loading from WAV files.
* Bitmap font system.
* Configuration saving/loading from text files.
* Debugging facilities with logging to file.

## Compiling

Use GCC to compile.

The following libraries are required for compilation:

* SDL (https://libsdl.org/)
* SDL_mixer (https://libsdl.org/projects/SDL_mixer/)
* PhysicsFS (https://icculus.org/physfs/)

Run `make`, and the binary should be created under `bin/`.

## License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

See the `LICENSE` file for the full license text.

## Third party licenses

The SDL parts under `ext/` are under the zlib license:
https://www.libsdl.org/license.php

The PhysicsFS parts under `ext/` are under the zlib license:
https://hg.icculus.org/icculus/physfs/raw-file/tip/LICENSE.txt
