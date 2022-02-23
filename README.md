# Maze and enemy: a demo for UE4 using C++

This is a quick and dirty project for me to get familiar with C++ API and data structures used in Unreal Engine 4. In this project, I implemented several classes including a player character with the ability to melee and cast spell, a HUD for a health bar and callable inventory system, pickup items, a particle system for spell effect, and two types of monsters (melee and range).

(Another C++ Unreal project is [here (Overwatch3)](https://github.com/legenary/overwatch3).)

(I also have two C++ projects using [Bullet physics engine](https://pybullet.org/wordpress/) (an open source physics engine both for making games and robotics research) [here (MuscleModel)](https://github.com/legenary/MuscleModel), and [here (WhiskerModel)](https://github.com/legenary/WhiskerModel).)

<img src="Documents/maze.PNG" height="360px" width="615px" >

## 1 Monsters

<img src="Documents/monsters.PNG" height="360px" width="615px" >

In this project, two of the very basic types of monsters are implemented in a quick and dirty way. Two types of monsters share the same sight range to spot the player. Once the player character is spotted, 

(1) the melee monster will move towards the player up until it is in melee range, after which it will initiate melee attack as long as the player stays in its melee range. The melee attack deals damage to all actors it sweeps through if whose root component is overlapped, exluding the monster itself (ofc no self damage). As soon as the player leaves its melee range, the monster will stop melee attack sequence and start chasing, until finally the player escapes its sight.

(2) the range monster will move towards the player up until a point that it can initiate a range attack safely (so it will be farther away than melee monster). If the player character moves toward the range monster as long as not into the melee range, it will keep shooting projectiles and not move. As soon as player are inside the melee range, it will back up until it feels safe and restart attacking. 

Both monsters have a health bar floating above their heads to indicate their HP left. When taking damage, the monster will abort attack and play hit react animation. Once the HP becomes zero, a death animation will be triggered after which they are destroyed.

## 2 Pick up items and inventory system

<img src="Documents/pickup.PNG" height="360px" width="615px" >
<img src="Documents/inventory.PNG" height="360px" width="615px" >

There are several pickup items that the player character can pickup by approaching them, a chair, a rock, and a blue pyramid (don't ask me why). After being picked up, the item will disappear on the map and available in the player characters backpack. The user can check what's in his backpack by calling the inventory system with `I`. When the inventory system is called, the mosue is active and the player lose the ability to move also. The icon, item name, and quantity will be shown in the inventory system. The player can use the monse to drag the icons around to arrange its backpack.

The game currently doesn't provide any functionality for the player to consume or destroy items in the backpack so the chair and the rock will stay there forever until the game is shut down. However, the blue pyramid actually gives the player character 5 casts of blizzard spell, and can be consumed.

## 3 Player character

<img src="Documents/particles.PNG" height="360px" width="615px" >

The player character has the basic ability to pitch and yaw, responding to mouse movement, and move towards different direction responding to `W`, `A`, `S`, `D`.

The player is spawned equipped with a blunt weapon. A melee attack can be initiated by `Left Mouse Click` and will deal one-time damage to monsters. The player can also pick up blizzard spells (blue pyramid) on the map, after which the player will have 5 more casts. The blizzard spell is casted by `Right Mouse Click` and will deal continues damage to monsters if they are in the damage volume. 





