# 2Dot_Version2.0.2
A simple two handed racing game I've been working on

Code can be found in SDL_2dot_game/SDL_2dot_game and a UNIX executable is in SDL_2dot_game/Build/Product either Debug or Release should work

User Note: If you are using a mac keyboard dont use "J", "H", and "Space" simultaneously for  keybinds, there is a weird keyboard blocking issue

## Version 2.0.2 Updates (4/11/2019)
 
 1.  New larger map; original map still included in assets under "level_0.map" change the value of levelMapFile in globals
 2.  Fixed dot  (-)y axis collision bug that would sometimes cause the dot to get stuck in the wall
 3.  Fixed a bug that would cause the boost function to accelerate the dot without user input
        3a. Currently you can only choose "Space" of a normal key as the boost key, working on adding support for shift etc.
 4.  Touched up the interface for added a new name to the leaderboard
 5.  Added sound effects to mouse selection events in Settings and Menu so they didn't feel left out compared to key events
        5a.  Added sound effects to the growth effect as well
 6.  Corrected a documentation issue in the SDL lib (no more documentation warnings!)
 7.  Corrected error where y-axis velocity would not normalize correctly
 8.  Added capability to handle different endzone sizes (they still have to be square)
 9.  You can now generate your own maps in the repo wcole/2Dot_map_gen (see readme there)




## Version 2.0.1 Updates (4/9/2019)
-
1) Added speed boost to control scheme (default key = 'Space')
2) Changed dot velocity and decel values to make movement more responsive
3) Added SOUND!! 
4) Changed timing system from countdown to countup, now every run will finish, but not everyone gets on the leaderboard!
5) Some texture updates
6) Red checkerboard tile now causes the dots to swell in size making navigation much more difficult

This was written using the SDL, SDL_IMG, and SDL_TTF libs and compiled using xCode for mac.  The raw C++ files and headers
are also given

This game is a culmination of the excellent LazyFoo Tutorials on SDL.  The game is a racer where each hand controls WASD 
movement of a dot, with goal of getting each dot into the endzone before time expires.

There is leaderboard functionaility along with variable controls set by the user.

##Assets Information

##Fonts
 - OpenSans-Regular.ttf : Obtained from https://www.fontsquirrel.com/fonts

##Audio (Royalty Free)
 - Menu theme - "Sky_puzzle" from Soundimage.org
 - Game theme - "Techno_caper" from soundimage.org
 - Wall bounce - "pling.wav" from SoundBible.com
 - Button Click - "Click" from SoundBible.com
 - Victory Que - "Short_triumphal_fanfare" from SoundBible.com
 - Game loss que - "Aww_sympathy" from SoundBible.com

##Textures
 - All textures created in Paintbrush for mac; no expense was spared

4/2/19: There is a stuttering issue if you have USB peripherals plugged in.  It is a bug in SDL caused by USB devices (any device besides mouse and keyboard).  It causes a stutter every 3000ms
