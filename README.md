# Introduction

I am a university student from UPC studying Videogame Design and Development and here is my research in how to move units in group in a RTS game. Hope you enjoy it :)

I'll show you how to implement coordinated unit movement in a RTS game. There isn't only one way to do it, depending on the game you want to create, there are different options you must choose, from decisions that are visually more attractive and realist to options that are not that realistic but consume less CPU.

Before we dive into coordinated unit movement, let's take a look at some of the movement issues facing game developers today. Most of these have to do with minimizing CPU load versus maximizing the accuracy and intelligence of the movement.

## Moving one unit versus moving multiple units.

Moving one unit is quite simple and it doesn't consume much CPU, however, if you want to move a hundred of units at the same time, your system must be very conservative in its CPU use.

## Some movement features are CPU intensive.

Very few games that move hundreds of units support advanced behaviour such as modelling the acceleration and deceleration of these units. Especially the movement of large ships and heavily armored units has a lot more realism with acceleration and deceleration, but this comes at a high cost of CPU usage.

This movement calculation becomes more complicated as we have to apply the time differential to the acceleration to obtain the velocity, and if we want to predict the movement as we'll later, those becomes even more complicated.

Another feature could be that units turn around before moving, or will moving. This is also difficult, first of all because most pathfinding algorithms are not able to take a turn radius into account. Moreover, that could cause problems such as the unit not being able to follow the path because of turn radius restrictions, depending on how the code is made. In my opinion the best solution is to slow down the unit to make the turn and then return its normal speed, but this involves a set of extra calculations.

## Different lengths for the main game update loop

What could happen if we do not use a consistent simulation interval? If this happens, the distance a unit will travel between each update may vary as shown in the image below:

![Constant vs Varied Update Img](https://s9.postimg.org/jclf22jbj/Constant_vs_Varied_update.png)

A good smoothing update system can solve this.

## Unit collisions

How do you split units once they collide? One solution to solve this may seem to not let them collide at all, however, this gets more problems show up than solved. First of all, as said by Dave Pottinger, game programmer and designer who worked on game engines and AI in Age of Empires series, "No matter how much code you write, your units will always find a way to overlap". But, besides that, this isn't the most important problem; the thing is sometimes you need to let them overlap a little (for example in combat). If you try not to let them, units often will walk of their way to fight other units and that will make them take more damage. Once more, depending on your game you will have to decide which overlap is acceptable.

## Map complexity

The more complex the map is, the more complicated and difficult good movement will be to create. Not only the map size matters (this is more a pathfinding optimization need), but if the quality of the map increases, the player will expect the same from the units movement.

## Random maps or controlled scenarios?

If it is a random map, you can't hard-code some paths, which increases the CPU use of the pathfinding.

---

In games where their objects hardly ever came into contact, you can get away with a relatively simple movement system. However if you have hundreds of units like in any RTS that need to have collision, this is not going to be so easy and the quality and accuracy requirements of your movement system need to be dramatically raised.

Now, let’s see what we need to implement a group movement in our game. We'll consider that we already have those units in our map, and our objective now will be to move them as a group.

## Group Selection

The most common way to select our troops is to do a rectangle around them and select the ones that are inside, so we'll do that.

We save a rectangle with the point where we click the mouse, we wait until the player drags the mouse to select the area, and finish the width and height of it when the player releases the mouse. We also need to draw this rectangle while the player does it, or it will confuse him.

Just after creating the rectangle, we'll check if any unit is in its inside, if there is any, we will select them.

### Selection Limit

A good question we should make to ourselves is, should I do a selection limit?

Nowadays, in RTS games there usually is a limit in our selection. The reason is more for game design than technical, having 10k individuals on screen and being able to command each one will become unfeasible from a gameplay perspective long before the hardware gives out on you.

The selection limit depends on your game, of course, but that's just a number, how do we do it?

The easiest and less costly way is just capping them, but in this case, you can't really decide which ones will select first and which ones will last. If you want to select them, let’s say in order in reference as where do you first clicked to draw the rectangle and want the units that are closest to this point I recommend to do a Map and add all the units in the map with a priority order (depending on how you want to do it), with it you can arrange them and the selection would be this order.

Before passing to the next step, we should create some visual feedback on which units are selected; this could be a circle (that we can use after with the collisions).

## Simple Movement Algorithm

Now that we have our units selected we will start moving them. This part will be only to move a unit, after we have done this, we could move a group of units, but they will end up overlapping in the same point. We will solve this later, by now, let’s move them.

Let’s create a pseudocode, but before, some assumptions:

- We'll be given a path to follow, that stores in each unit structure and that this path is viable (has no collisions) at the time it is given to us.
- Our unit will have 3 states: Moving, ActualObjectiveReached and FinalObjectiveReached.
- The map can change so if we are moving and we build something in our path, it is possible we have to adjust the movement.

So, let's start, as soon as we're given the path

  Put unit state to Moving.
  
  Search for the next tile we are moving and choose it as our objective.
  
  Then we enter to the next loop:
  
{

  If there isn't any:
  
  Put unit state to FinalObjectiveReached,
  
  Change Walk animation,
  
  Exit Function.
  
  Else If our objective isn't walkable:
  
  Repath.
  
  Else:
  
  We calculate the distance and the angle to the point and move the unit.
  
  If ActualObjectiveReached is true: (which shall be tested with the distance between our ActualObjective and our unit position)
  
  If it's true:
  
  Pop the path we just walked in from the list/vector.
  
  Change the ActualObjective to the next tile.
  
}

## Collision Determination

As I said before, now we have to split those units. In order to do that, first we need to check if they have collided. If we notice a collision between teo units, we'll split them.

### How do we represent the collider of a unit?

In most strategy games, this collider consists in a circle or sphere if the game is in 3D. Doing a sphere instead of things like oriented bounding boxes or low-level polygon to polygon intersections, is a quicker collision detection that will usually improve performance.

### Single Unit, Group of Units and Formation

For a single unit, the circle collider is enough, however in a group of units and in a formation it is a bit more complicated.

For a group of units, the acceptable minimum is to check each unit in the group for a collision.

In the case of a formation, it requires the same checks as a group, but we'll also need to check internal collisions within the formation.

### Unit overlapping

As we saw in the introduction, units must overlap a bit in order to improve the gameplay. There are many ways to do this; one is that each unit has a soft radius and a hard radius.

![Soft and Hard Radius Img](https://s23.postimg.org/jcd3vemkb/01mov02.png)

When the unit is stopped or in a formation, we shall check the soft radius to maintain it in a good position, however, when we are moving it, or it is attacking something as we saw, we must let them overlap a bit, which means we can take in consideration the hard radius instead.

In our project we will simply ignore all the collisions while moving, this way is a bit less realistic, but it is easy to change and, of course, less CPU consuming.

### Collision timing

There can be two types of collision as well that you may consider in your game. Immediate collisions, which are currently existing collisions between two objects, and future collisions, which will happen at a specified point in the future.

We will only consider immediate collisions in our project.

## Splitting Units

If we detected a collision with two units, we need to move one of them. The first question you must ask is, which one? This could not be really important depending on the game, however if you want this to be logical (i.e.: an elephant that moves because a soldier is coming his way isn't), you can do it with a priority id in each unit.

What we'll do to split them is call a function that recives the unit you want to move and calculate a new position among the ones it already has, to improve this we will check for a walkable and unoccupied tile.


## Discrete vs. Continuous Simulation

Most movement algorithms are discrete. A discrete simulation is if a unit moves from point A to point B without considering what is between them. A continuous one would consider the volume between them as well.

If we have a continuous simulation there shall be no problem, however it is better to have a discrete one as it is less expensive.

The problem comes when we have a large update, for example if in an online game, someone is lagged, the unit can move a lot in this update, if we do not control this, a unit can, for example, walk through a wall.

I'll expose two possible solutions:

The first one could be to sub-sample each move into a series of smaller moves. In base of the size of the unit, we make an interval small enough to guarantee that no other unit can fit between two of the same points, and then we check those points with the collision system.

The other one, which it's a bit easier, is to create a move line. A move line is a line that represents the movement of the unit, if one unit goes from point A to point B, we create a line that goes from A to B, and check if this line has any collision.


## Predicted Positions

Predicted positions are simply a set of positions that indicate where a unit will be in the future. Those positions can be calculated using the same movement algorithm that is used at moving the unit.

If we do this, we shall be careful of not recalculating all our predicted positions at every frame.

The next optimization is to create a prediction system that handles both points and lines. If a unit is traveling in a straight line, we can designate an enclosed volume by using the current position, a future position, and the unit's soft movement 

## For Further Information

This is mainly the most common issues you will face if you decide to build a group movement system, however here I'll leave you some extra information:

- Group Movement:

[Coordinated Unit Movement](http://www.gamasutra.com/view/feature/131720/coordinated_unit_movement.php?page=1), by Dave Pottinger (Programmer and Designer in Age of Empires series)

[Implementing Coordinated Movement](http://www.gamasutra.com/view/feature/131721/implementing_coordinated_movement.php?page=3), a continuation of the previous link.

 - Pathfinding: 
 
 I haven't talked about it really much because I have a friend who did a similar research to this one with [Pathfinding Optimization.](https://danny0ner.github.io/Pathfinding-Optimization/) And [here](https://github.com/Danny0ner/Pathfinding-Optimization) you have his repository.
 
 And [here](https://www.hindawi.com/journals/ijcgt/2008/873913/) a one a bit more complicated.
 
 - Collisions:
 
 A really simple [collision detector](http://lazyfoo.net/tutorials/SDL/29_circular_collision_detection/index.php) to start with if you have trouble.

- Formations:

If you want to extend this and do formations instead of group movement, I think it can be interesting to know when to use each one, which is well explained [here.](http://blog.ocad.ca/wordpress/vism2b15-fw2011-01/2011/10/%E2%80%9Cage-of-empires-ii-age-of-kings%E2%80%9D-grouping-and-formations/?doing_wp_cron=1489083347.2696259021759033203125)
