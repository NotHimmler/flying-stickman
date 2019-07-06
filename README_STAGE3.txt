======= CONFIG FILE INSTRUCTIONS FOR STAGE 3 =======
To enable stage 3, the setting "stage: 3" should be in the config file. Otherwise, stage 1 will run.
Set the setting for the appropriate stage.

Level design is achieved by simple adding more obstacle lines as per the stage 2 config specification.
The number of player lives is set by the line:
	lives: x
	Where x is a positive integer. Non-integer or negative integers will give an error message and stop the program.

	
======= Behavioural Design Pattern Used =======
Observer Pattern
- Subject and Observer interfaces
- StickManSubject implements the subject interface and decorates the stickman to allow for notifying observers about changes in the stickman state.
  Namely, whether or not the stickman has collided with an obstacle
- An observer to the stickman subject is the Stage3Dialog and also the Stage3Decorator which shows the level and lives
	Allows the Dialog class to know when a collision has occured so that lives can be subtracted or added, or powerups applied
	
	
======= Efficient Use of Memory =======
Flyweight Pattern
- SpriteFlyweightFactory
  Return/Create a shared pointer for the coin/heart sprite if one doesn't already exist. All coins reference the same QPixmap object
  

======= Score Calculation Algorithm =======
Score for level = num_coins*10 + max((space_sum/10 - time_taken), 0)*level


======= Extensions =======
- Randomly generated coins as a score modifier
	Implemented using Inheritance from Entity class. StickmanSubject notifies Dialog of coin collision
- Randomly generated hearts to get extra lives
	Implemented using Inheritance from Entity class. StickmanSubject notifies Dialog of coin collision
- End of game scoreboard
	Persistent even after program close. Reads and writes highscores file.
	Highlights current score if it makes it onto the leaderboard.
- Restart without reopening the program at the end of the game
	When the game is over from dying or finishing all levels, listens for the R key then resets appropriate
	dialog member varaiables