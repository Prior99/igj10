# Innojam 10 (Parkour PaNick)

The goal of the game is to reach as far right as possible, avoiding to fall to death, being smashed to death, shreddered to death or die of insanity.
Collect the orbs to get yourself a littlebit of extra time which you can use to freeze the clock and solve otherwise unsolvable parts of the map.

**Collect the orbs and jump onto the next roof:**

![](screenshots/screenshot-01.png)


**Avoid being stomped to death by flying stompers:**

![](screenshots/screenshot-02.png)


**Lol, you died:**

![](screenshots/screenshot-03.png)

**Use your superpower but risk your life and your sanity:**

![](screenshots/screenshot-04.png)


## Development Setup

### 0. Basics

Use a Linux-based machine. Preferrably Arch Linux. Register at [git.92k.de](https://git.92k.de/). Join the group 'gamejam'. Add your public ssh-key.
If you do not have one, run: ```ssh-keygen``` and see the new key using ```cat ~/.ssh/id_rsa.pub```.

### 1. Tools

You need to have some basic build-tools installed (C Compiler, Make, C-Make, etc...)

On Arch Linux you could run: ```sudo pacman -S base-devel cmake clang git```.


### 2. 3rd party libraries

Some libraries need to be installed using your favorite systems package manager:

- SDL
- SDL_image
- SDL_ttf
- SDL_mixer
- libc++

On Arch Linux you could run: ```sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf libc++```.

### 3. Clone this repository

You need to clone this repository somewhere onto your laptop: ```git clone --recursive gitlab@git.92k.de:gamejam/igj10.git```.
Please note the ```--recursive``` which will also initialize the neccessary git submodules. If you did not use ```---recursive```, please initialize them manually.

### 4. Build the sources

There is a Makefile included for your convenience.

 - Build the sources: ```make```
 - Run the game: ```make run```
 - Clean up: ```make clean```

# Attribution

Sven-Hendrik Haase of which the [boilerplate](https://github.com/svenstaro/innojam9) was used.

Alex Dmitriev for C++ Troubleshooting in an hour of need.

# Contributors

This game was created in 49 hours by:

 - Frederick Gnodtke (Code and Graphics)
 - Maurice Grages (Sounds and Graphics)
 - Andra Rübsteck (Code)
 - Merlin Rübsteck (Code)
 - Julian Tobergte (Code)
