# Shooter Demo

<img src="https://github.com/katerinavlasova/Shooter_Demo/blob/master/Content for README/view.png" width="550" height="300">

## Description
***Kill all enemies on the level!***

Each level has a certain amount of enemies. The goal is to kill all enemies before they kill you.
Every enemy spawns at some initial location. They patrol surroundings in the next order:
1. go to some random location within a certain range; 
2. wait there for 5 seconds; 
3. go to their initial location; 
4. repeat in 5 seconds. 
 
If the player is seen by enemies, they follow the player and try to shoot this player. If the player is lost out of sight, enemies go to the location, where the player was seen last time, in order to investigate ( = wait there for 5 seconds) and after that, they return to their initial location.

During the game, the player can pick up guns with different qualities and it'll help to pass the level faster and easier.

<img src="https://github.com/katerinavlasova/Shooter_Demo/blob/master/Content for README/shooter_git_gif.gif">

## About the project

The project was implemented by scratch on UE4. 

Character and gun classes are implemented on C++. Character class handles movement input, line tracing for guns to pick up, swapping guns while gun class handles dealing damage from shooting.

Besides the UI (main menu, win/death screen), Blueprints also handle health system, conditions for winning/losing.
Behavior Trees was used for AI. 

## Plans for development

* Add more levels! (currently, there are 2 levels)
* Add more unique enemies! (currently, there are 1 type of regular enemy and 1 boss enemy)


Credits to Epic Games for these great assets! You can get them [here](https://www.unrealengine.com/marketplace/en-US/assets?keywords=Paragon%3A).


