# 2Dot
A simple two handed racing game I've been working on

This was written using the SDL, SDL_IMG, and SDL_TTF libs and compiled using xCode for mac.  The raw C++ files and headers
are also given

This game is a culmination of the excellent LazyFoo Tutorials on SDL.  The game is a racer where each hand controls WASD 
movement of a dot, with goal of getting each dot into the endzone before time expires.

There is leaderboard functionaility along with variable controls set by the user.

4/2/19: There is a stuttering issue if you have USB peripherals plugged in.  It is a bug in SDL caused by USB devices (any device besides mouse and keyboard).  It causes a stutter every 3000ms
