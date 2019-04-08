# 2Dot_Version2.0
A simple two handed racing game I've been working on

Code can be found in SDL_2dot_game/SDL_2dot_game and a UNIX executable is in SDL_2dot_game/Build/Product/Debug

Version 2.0 Updates
-
1) Added speed boost to control scheme (default key = 'Space')
2) Changed dot velocity and deccelration values to make movement more responsive

This was written using the SDL, SDL_IMG, and SDL_TTF libs and compiled using xCode for mac.  The raw C++ files and headers
are also given

This game is a culmination of the excellent LazyFoo Tutorials on SDL.  The game is a racer where each hand controls WASD 
movement of a dot, with goal of getting each dot into the endzone before time expires.

There is leaderboard functionaility along with variable controls set by the user.

Assets Information
Fonts
 - OpenSans-Regular.ttf : Obtained from https://www.fontsquirrel.com/fonts

Audio (Royality Free)
 - Menu theme - "Sky_puzzle" from Soundimage.org
 - Game theme - "Techno_caper" from soundimage.org"
 - Wall bouce - 
 - Button Click - "Click" from SoundBible.com"
 - Victory Que - "Short_triumphal_fanfare" from SoundBible.com
 - Game loss que - "Aww_sympathy" from SoundBible.com

Textures
 - All textures we created in Paintbrush for mac; no expense was spared

4/2/19: There is a stuttering issue if you have USB peripherals plugged in.  It is a bug in SDL caused by USB devices (any device besides mouse and keyboard).  It causes a stutter every 3000ms
