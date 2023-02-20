# Arduino-Snake-Game

Snake is a video game genre where the player maneuvers a growing line that becomes a primary obstacle to itself. The player controls a dot, square, or object on a bordered plane. As it moves forward, it leaves a trail behind, resembling a moving snake. In some game versions, the end of the trail is in a fixed position, so the snake continually gets longer as it moves. In another common scheme, the snake has a specific length, so there is a moving tail a fixed number of units away from the head. The player loses when the snake runs into the screen border, other obstacles, or itself.
The solution implemented for this project could be described as follows: a sole player attempts to eat items (also called "apples") by running into them with the head of the snake. Each item eaten makes the snake longer, so avoiding collision with the snake becomes progressively more difficult.
The apples do not disappear after a certain number of seconds and the game is over only when the snake runs into itself.

The screen was divided into 800 segments (an array of 32*25 = 800 elements; 32 segments for X – axis and 25 for Y - axis) in order to draw the apples and the snake as it increases. One of these map grid elements could represent an apple or an aditional segment added to the snake’s length (the sizes of a segment are: the width is 5 pixels and the height is 5 pixels). The game starts when the Down direction is selected, and the game state become set to Game. Before starting the game, a logo screen will appear and the player is informed how to start a new game.

# Software diagram 
![Screenshot 2023-02-20 222850](https://user-images.githubusercontent.com/59019060/220193077-f8bfdfe1-b5f3-4f99-b7fa-697af15a102e.jpg)
