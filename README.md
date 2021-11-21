<!-- Output copied to clipboard! -->

<!-----
NEW: Check the "Suppress top comment" option to remove this info from the output.

Conversion time: 3.449 seconds.


Using this Markdown file:

1. Paste this output into your source file.
2. See the notes and action items below regarding this conversion run.
3. Check the rendered output (headings, lists, code blocks, tables) for proper
   formatting and use a linkchecker before you publish this page.

Conversion notes:

* Docs to Markdown version 1.0β31
* Sun Nov 21 2021 14:19:33 GMT-0800 (PST)
* Source doc: RayRPGExample
* This document has images: check for >>>>>  gd2md-html alert:  inline image link in generated source and store images to your server. NOTE: Images in exported zip file from Google Docs may not appear in  the same order as they do in your doc. Please check the images!

----->


<p style="color: red; font-weight: bold">>>>>>  gd2md-html alert:  ERRORs: 0; WARNINGs: 0; ALERTS: 35.</p>
<ul style="color: red; font-weight: bold"><li>See top comment block for details on ERRORs and WARNINGs. <li>In the converted Markdown or HTML, search for inline alerts that start with >>>>>  gd2md-html alert:  for specific instances that need correction.</ul>

<p style="color: red; font-weight: bold">Links to alert messages:</p><a href="#gdcalert1">alert1</a>
<a href="#gdcalert2">alert2</a>
<a href="#gdcalert3">alert3</a>
<a href="#gdcalert4">alert4</a>
<a href="#gdcalert5">alert5</a>
<a href="#gdcalert6">alert6</a>
<a href="#gdcalert7">alert7</a>
<a href="#gdcalert8">alert8</a>
<a href="#gdcalert9">alert9</a>
<a href="#gdcalert10">alert10</a>
<a href="#gdcalert11">alert11</a>
<a href="#gdcalert12">alert12</a>
<a href="#gdcalert13">alert13</a>
<a href="#gdcalert14">alert14</a>
<a href="#gdcalert15">alert15</a>
<a href="#gdcalert16">alert16</a>
<a href="#gdcalert17">alert17</a>
<a href="#gdcalert18">alert18</a>
<a href="#gdcalert19">alert19</a>
<a href="#gdcalert20">alert20</a>
<a href="#gdcalert21">alert21</a>
<a href="#gdcalert22">alert22</a>
<a href="#gdcalert23">alert23</a>
<a href="#gdcalert24">alert24</a>
<a href="#gdcalert25">alert25</a>
<a href="#gdcalert26">alert26</a>
<a href="#gdcalert27">alert27</a>
<a href="#gdcalert28">alert28</a>
<a href="#gdcalert29">alert29</a>
<a href="#gdcalert30">alert30</a>
<a href="#gdcalert31">alert31</a>
<a href="#gdcalert32">alert32</a>
<a href="#gdcalert33">alert33</a>
<a href="#gdcalert34">alert34</a>
<a href="#gdcalert35">alert35</a>

<p style="color: red; font-weight: bold">>>>>> PLEASE check and correct alert issues and delete this message and the inline alerts.<hr></p>



## Raylib RPG Example


## Concept

A single player dungeon crawl type game intended to be a basic example of the genre to people who are less familiar with game programming and design. Will use raylib with simple 2d top down sprites and sprite sheets to create a graphical rogue-like display.



<p id="gdcalert1" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image1.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert2">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image1.png "image_tooltip")


_Possible assets to use._


## Scope

The scope of this project must be very limited by it’s simple nature. The main goal is for easy to understand code, not efficiency. The project must be completable in roughly a week’s time.

Things to **_avoid _**for the initial example.



* Procedural Level Generation
* Complex Character Creation
* Complex Combat Systems
* Multiplayer
* Economy (vendors)
* Character Stats
* ECS


## Requirements



* Basic C++
    * No Templates, simple containers (array, list, etc..)
    * Single Threaded
* Use Tiled for maps if needed
    * Wall collision
    * Doors
    * Triggers
* Use simple sprite sheets
* Single player
    * One base player type, equipment says what you can do
* Simple health system
* Simple combat system
    * Melee
    * Ranged
        * No Ammo
    * Scrolls
* Multiple monster types
    * Different health and attacks
* Simple inventory
    * Paperdoll for equipment
        * Armor
        * Weapon
    * Backpack slots
* Lootable items
    * Containers (chest and barrels)
    * Monsters
    * Coin for score
* Activatable Items
    * Heal (Potion)
    * Protection (Scroll)
    * Attack (Potion)
    * Direct Damage (Fireball Wand)
* Level Transitions
* Win and Lose States
    * Win when you loot final level chest
    * Loose when you die, YOLO
* Simple sound effects


## 


## Open Questions

None at this time.


## GamePlay


#### Basic UI

The basic UI will have the player in the level as the focus of the camera in the main content area of the window. A player health bar, gold(score), and action buttons are along the bottom. Menus are shown over the main view with a transparent background. Pressing I or the Inventory Action button on the UI will bring up the inventory window.



<p id="gdcalert2" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image2.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert3">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image2.png "image_tooltip")



#### 


##### Inventory Window 

<p id="gdcalert3" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image3.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert4">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image3.png "image_tooltip")


The Inventory window will have a grid of item slots (20), and two equippable slots for weapon and armor. The user can drag items from inventory into the equippable slots. Items dragged out of the window will be dropped. When selected or hovered a tooltip will show the item's name and bonuses. Any item can be put in any item slot. Only weapons can be put in the weapon slot, and only armor can be put in the armor slot.



<p id="gdcalert4" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image4.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert5">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image4.png "image_tooltip")



#### Controls

The player moves around the world by clicking on areas on the map (Click to move). Pressing the left mouse button activates an attack for the primary weapon. If the primary weapon is Melee it attacks all enemies in a radius around the player. If the primary weapon is ranged, the cursor will be an attack cursor when not over a UI element. When a ranged weapon is used then the enemy under the cursor will be attacked. Primary attacks have a cooldown based on a speed stored with the weapon. Changing weapons does not reset the cooldown time from a previous attack. The primary attack action button in the UI will gray out during the cooldown.


#### Movement 

<p id="gdcalert5" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image5.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert6">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image5.png "image_tooltip")


When the player moves around the map, they are constrained to an inset rectangle around the screen, similar to the “Player push camera on getting too close to screen edge” mode from the [core_2d_camera_platformer example](https://www.raylib.com/examples/core/loader.html?name=core_2d_camera_platformer). Some levels will be larger than the screen and will scroll using this method.


#### Gameplay

The game starts with the player equipped with no armor and no weapon, in a simple room with a stairwell down. This first room has a lootable chest with a health potion in it and a sign saying how to loot it (click on it when near). While in this first room the UI will have tooltips showing what each button does.

When the player enters the stairwell they go down a level to the combat training room.

This room has a basic melee combat encounter against a simple enemy, like a rat. Their is a chest with a dagger in it at the start of the room, with a text that describes how to equip it. Opening the chest with the dagger triggers a door to open to the next room. The rat will begin attacking when the player enters that room. Killing the rat opens a door to a third room with a chest and basic armor and stairs down.

 

Third room is similar to the second room but trains ranged combat with throwing daggers. This room has no chest at the end.

The fourth through Nth room are gameplay rooms, with more monsters and loot. In the last room is a boss monster and a chest that ends the game. When the game is over you are shown a win screen and clicking will take you back to the main menu.

If you die, you are shown a defeat screen and clicking will take you back to the menu.


##### Combat 

<p id="gdcalert6" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image6.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert7">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image6.png "image_tooltip")


Combat is a simple process. The attack damage of the weapon + A random number from -3 to 6 is the attack roll. The target’s defense is subtracted from this. Any positive remainder is removed from the health of the target. When the target’s health is 0 it dies. Dead monsters leave a corpse behind that may have loot from a loot table.


#### Monsters 

<p id="gdcalert7" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image7.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert8">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image7.png "image_tooltip")


Monsters are given an attack skill using an internal weapon. This can be ranged or melee. Monster AI will be very simple, move toward the player when in view and attack when in range. When a monster is killed it may drop lootable items and/or coins near it. When dropped these items appear near the monster to be looted.


#### Items 

<p id="gdcalert8" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image8.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert9">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image8.png "image_tooltip")


Items are shown in the world as icons on the map. Items can be picked up by clicking on them. If the player has room in the inventory the item is put in the next available slot. If there is no room in the inventory the item is dropped. Dropped items are placed near the player within the map. Items can overlap.

Some items are activatable, like a health potion, or protection scroll. When an activatable item is picked up, it’s icon is shown on the action bar on the hud. Clicking the icon in the hud will activate the item. Activated items are destroyed after a single use and are removed from inventory.

 \
Coin “items” have a value and are added to the coin score when picked up. Coins do not go into inventory.


## Application Design

The game application will be made up of multiple source and header files that handle the following items.



* Main (application startup)
* Resources
* Menus
* Map
* Player
* Monster
* Items
* Gameplay
* Drawing/UI


### Resource System

A resource management system will handle loading and storing all used textures and sounds. Textures and sounds will be given an ID using an enumeration and code will request a reference to the resource by this ID.


### Menu System

A menu system will reference the current menu by an ID. The menu system will maintain a stack of menu IDs that can be pushed and popped as needed. 


### Map System

The map system will handle loading the level from the resource system and providing storage for items and monsters that are created by the gameplay system. The map system will not draw the level or any of its contents, it is simply data and state storage.


### Player, Monsters, and Items

Players, Monsters and Items will be simple classes that define the data for each instance of the object. Functions will exist to create and fill out the data for each type of monster or item. These objects will then be stored by the map system for use by gameplay.


### Gameplay System

The Gameplay system will handle all game logic. It will take input from the player and apply it to the state stored in the map system. The gameplay system is responsible for all player actions, monster AI, and combat resolution. 


### Drawing/UI System

The drawing/UI system is responsible for drawing the map and handling events from the HUD. It uses data from the map to draw and passes events into the gameplay system as needed. The UI system is responsible for handling events in the inventory window.


## Content

This section describes the specific content used in the game.


###    Menus

The game will start on the main menu. During gameplay pressing escape will pause the game and show the main menu.


#### Main Menu

The main menu has the following items.



* Start Game
    * Will change to Resume Game when invoked while the game is playing.
* Options
* Quit


#### Options Menu

The options menu has three items.



* Fullscreen
* Volume
* Back

Player Data 

<p id="gdcalert9" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image9.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert10">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image9.png "image_tooltip")


The player starts with 20 health and 0 defense. Items can increase these values.


### 


### Enemy Types

Describes the types of enemies, stats


#### Rat 

<p id="gdcalert10" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image10.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert11">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image10.png "image_tooltip")




* 1 Health
* 0 Defense
* Claw Attack
    * Melee
    * 1 Damage
    * 1 second cooldown


#### Snek 

<p id="gdcalert11" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image11.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert12">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image11.png "image_tooltip")




* 3 Health
* 10 Defense
* Bite Attack
    * Melee
    * 2 Damage
    * 1 second cooldown


#### Ghust 

<p id="gdcalert12" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image12.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert13">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image12.png "image_tooltip")




* 2 Health
* 15 Defense
* Scare Attack
    * Ranged
    * 10 Damage
    * 5 second cooldown


#### Troll 

<p id="gdcalert13" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image13.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert14">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image13.png "image_tooltip")




* 30 Health
* 10 Defense
* Punch Attack
    * Melee
    * 10 Damage
    * 1 second cooldown


#### Tortile 

<p id="gdcalert14" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image14.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert15">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image14.png "image_tooltip")




* 5 Health
* 18 Defense
* Headbut Attack
    * Melee
    * 1 Damage
    * 15 second cooldown


#### Blorb 

<p id="gdcalert15" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image15.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert16">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image15.png "image_tooltip")




* 20 Health
* 10 Defense
* Eww Attack
    * Melee
    * 15 Damage
    * 2 second cooldown


####  \
DudeBro 

<p id="gdcalert16" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image16.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert17">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image16.png "image_tooltip")




* 20 Health
* 10 Defense
* Rust Attack
    * Melee
    * 20 Damage
    * 2 second cooldown


#### Munk 

<p id="gdcalert17" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image17.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert18">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image17.png "image_tooltip")




* 15 Health
* 14 Defense
* Chant Attack
    * Ranged
    * 25 Damage
    * 3 second cooldown


#### Moderator 

<p id="gdcalert18" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image18.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert19">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image18.png "image_tooltip")




* Boss Mob \
50 Health
* 10 Defense
* Ray Attack
    * Ranged
    * 25 Damage
    * 3 second cooldown


### Items

These items can be picked up from monsters or chests.


#### WeaponsI

Sword 

<p id="gdcalert19" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image19.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert20">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image19.png "image_tooltip")


A basic sword.



* Melee
* Attack Power 0 -2
* Speed 1s

Really Cool Sword 

<p id="gdcalert20" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image20.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert21">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image20.png "image_tooltip")


A fancy sword.



* Melee
* Attack Power 2 -4
* Speed 1s

Awesome Sword

<p id="gdcalert21" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image21.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert22">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image21.png "image_tooltip")


This is a good sword



* Melee
* Attack Power 4 -6
* Speed 1s

Axe 

<p id="gdcalert22" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image22.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert23">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image22.png "image_tooltip")


Better than a sword 



* Melee
* Attack Power 1 -4
* Speed 1.5s

Mighty Axe 

<p id="gdcalert23" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image23.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert24">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image23.png "image_tooltip")


Better than an axe



* Melee
* Attack Power 2 -6
* Speed 1.5s

Battle Fork 

<p id="gdcalert24" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image24.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert25">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image24.png "image_tooltip")


Stabbing for Justice



* Melee
* Attack Power 0-2
* Speed 0.5s

Bow 

<p id="gdcalert25" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image25.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert26">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image25.png "image_tooltip")


Pew-Pew-Pew



* Ranged
* Attack Power 0-2
* Speed 0.25s

Bonkmaster 5000 

<p id="gdcalert26" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image26.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert27">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image26.png "image_tooltip")


Bonk



* Melee
* Attack Power 5-10
* Speed 3s


#### Armor

Pleather Armor 

<p id="gdcalert27" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image27.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert28">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image27.png "image_tooltip")


Better than nothing



* 2 Defense

Chain Shirt 

<p id="gdcalert28" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image28.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert29">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image28.png "image_tooltip")


Jingle Nangle



* 4 Defense

Full Plate 

<p id="gdcalert29" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image29.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert30">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image29.png "image_tooltip")


You are the tank



* 10 Defense


#### Activatable Items

Fud 

<p id="gdcalert30" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image30.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert31">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image30.png "image_tooltip")


A snack



* + 5Health

Potion 

<p id="gdcalert31" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image31.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert32">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image31.png "image_tooltip")


Smells funny, but works



* + 20 health

Shield 

<p id="gdcalert32" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image32.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert33">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image32.png "image_tooltip")




* + 10 defense for 30 seconds

Fireballs 

<p id="gdcalert33" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image33.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert34">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image33.png "image_tooltip")


Boom



* 10 damage to nearest enemy


#### Other Lootables

Monies 

<p id="gdcalert34" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image34.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert35">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image34.png "image_tooltip")


A bag of coins



* 1-10 coins


### Levels


#### Level 1 \


<p id="gdcalert35" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image35.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert36">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image35.png "image_tooltip")

