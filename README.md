# Debug Mode

## Introduction :

**Debug Mode** is a mod based on the SADX one (by PKR) with a similar goal: display useful and
various debug information. Here’s the current features :

```
 Display Debug Text so you can see your current speed, action, position, but also
game information like Camera position and extra stuff.
```

 **"Free movements",** a feature that allows you to move everywhere in the stage,
when this is on, physics and gravity no longer apply so you can clip everything. Also
death zones can no longer kill you. Another thing is it can teleport yourself to the last
checkpoint you grabbed. As a small bonus : it gives you 99 lives. This feature was
possible in the vanilla game by setting your action to 58, however, it wasn’t working
on the Hunters characters, but this mod fixes that, so you can use it with every
character.

**Draw Object Collision,** this display collisions in real time including non solid collision (like
triggers) and also the scores, such as when you can make a trick or get points (rail.)


```
There is some stuff that you need to know about collisions though. First, if they don’t
draw on a specific object, that means this object is using « Dynamic Collision ».
```
```
In other words, the collision has pretty much the same form as the object model and
cannot be displayed. Second, there are some collisions where you can stand on them
(such as the cube or rectangle ones) and some of them where you can’t, such as cylinder
model.
```
```
 The saves states feature with multiple slots. While the saves aren’t totally real (we
don’t copy and restore the ram like on Emulator), this one in particular still restores
many stuff, not only position and speed, but also action, animation, camera position,
objects, etc. Enemies, capsule and boxes are also restored, although, note that I do
not have the control of what is restored at the moment, therefore, enemies will be
restored regardless if you saved before or after their death. Loading a save states
only work if you are on the original stage where you saved. If you saved on slot 1 in
Green Forest and try to load it when you are on Metal harbor, you will get an error.
Save states are deleted when you close the game.
```
## Controls :

**_Note : The controls are likely to change with the next updates, since there is no input mod
for SA2, there isn’t that much buttons that I can use (no LB and RB for example.) therefore,
some choices have been made even though they aren’t ideal._**

```
Debug text info :
 To display debug text information, hold Y and down (d-pad). This can only work
during gameplay. Do it again to scroll down the different pages. When you reach the
ending, the text will vanish.
```
```
Collisions :
 Hold Y and UP (d-pad) to draw collision, this also draws non solid collision such as
triggers. Do it again to turn that off.
```
```
Free movements :
 Hold LT + RT + Y to enable « Free movements. »
 Hold X to gain or lose altitude (depending on your stick position.)
 If you want to teleport yourself to the last checkpoint, press B. If you didn’t grab any
Checkpoint, you will go back at the beginning of the stage.
 Press A to cancel « Free Movements. »
```

```
Save States :
```
```
Note : Please don’t hold Y when you are trying to save or load otherwise it won’t
work. This is to prevent accidental save/load while you are trying to simply draw
collision/display information.)
```
```
 Press Left with the d-pad to save
 Press Right with the d-pad to load.
 Press Up to change slot (increase)
 Press Down to change slot (decrease).
```
Thanks for reading, more features will eventually come, until then, feel free to share me
your feedback and report bugs.


