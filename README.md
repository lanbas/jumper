# jumper
A jumper game built with SDL2 and C++. 

![Image showing still frame of shrinky game](img/demo.png)

## Prerequisites
Development was tested on Ubuntu22.04 with g++ 11.4.0 and SDL 2.0. Instructions for installing SDL2 and its related necessary dependencies can be found here. For debian users, `sudo apt-get install libsdl2-dev` should suffice, but you can refer to the [installation documentation](https://wiki.libsdl.org/SDL2/Installation#linuxunix) for official instructions.

## Setup and Installation 
```bash
git clone https://github.com/lanbas/jumper.git
cd jumper
make
```
## Controls
 * Jump - `SPACE`
 * Duck - `S` or `DOWN`

## Play 
```bash
./jumper
```

Obstacles spawn in intermittently. Jump or duck to avoid them as necessary. Obstacle width and jumper speed increase over time. 
