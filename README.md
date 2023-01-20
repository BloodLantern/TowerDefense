<img style="float: left" alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986305793728542/logo.png width=61>

# &emsp; Fourmi Defense

### *A game about ants attacking a non-existent exit*

![thumbnail](https://cdn.discordapp.com/attachments/1027856325633396798/1065985399719202816/thumbnail.png "Fourmi Defense")

<br/>

## **Summary**
- [How to play](#how-to-play)
    - [Project dependencies](#project-dependencies)
    - [Launching the game](#launching-the-game)
- [Gameplay](#gameplay)
    - [Towers](#towers)
    - [Tower list](#tower-list)
    - [Tower upgrades and stats](#tower-upgrades)
    - [Enemies](#enemies)
    - [Glossary](#glossary)

<br/>

## How to play
### Project dependencies
This project depends on many libraries like [OpenGL](https://www.opengl.org/) and [ImGui](https://www.dearimgui.org/) which are already included in the repository.  
**However, libraries like [Asio](https://sourceforge.net/projects/asio/files/asio/1.24.0%20%28Stable%29/) (asio-1.24.0) and the [networking framework](https://gitlabstudents.isartintra.com/y.demaisonregne/sudonetwork) aren't included so you need to set up these by yourself.**
To do so, the project hierarchy must be as follows:
```
Folder
└───TowerDefense
|       TowerDefense.sln
└───asio
└───sudonetwork
```
*The folder names are case sensitive.*

The TowerDefense folder can be obtained by cloning the repository.

### Launching the game
The only thing left is to open the solution file (TowerDefense.sln) in Visual Studio 2022. Compile it using the big green play button on top of the window, and you're done!

[Back to summary](#summary)

## Gameplay
### **Towers**

Towers have 4 generic stats that can be upgraded through different upgrades. These are [damage](#damage), [attack speed](#attack-speed), [range](#range) and [pierce](#pierce). While the damage and pierce are linked to the projectile and therefore hidden to the player, the attack speed and range can be seen and upgraded at any time in the tower panel by clicking on the tower.  
The tower panel is composed of the tower's name, its texture, its upgrades, its stats like the number of enemies it killed, the damage it dealt ang the money it generated. As said before, the player can also see the precise amount of attack speed and range. These two stats can be upgraded up to 5 times using the button in front of them.  
On top of that, every tower has unique upgrades that greatly affect the stats and in general the power of the tower. These are described in the [tower upgrades](#tower-upgrades) section.

### **Tower list**
1. ### Minigun
<img alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986215075139594/minigun_icon.png width=128>

- Fires at tremendous speeds
- Deals low damage but is very cheap
- Perfect for the early and mid-game

2. ### Cannon
<img alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986215851085884/cannon_icon.png width=128>

- Low attack speed but AOE (Area Of Effect) damage
- Very useful for grouped enemies
- High damage with later upgrades

3. ### Beenest
<img alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986215565864960/beehive.png width=128>

- Medium attack speed
- Spawns bees to attack ants
- The bees choose a target and then hit it until it dies or goes out of range
- Can slow enemies with later upgrades

4. ### Ant spray
<img alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986215347757156/spray.png width=128>

- Deals low damage but has a very high pierce and AOE damage
- Later upgrades help not to miss the target when shooting
- Can slow enemies with later upgrades

5. ### Beehive
<img alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986214831861811/hive.png width=128>

- Doesn't attack enemies. Its only use is support and utility.
- Generates money each round
- Can generate live as well with later upgrades
- Can buff Beenests with later upgrades

### **Tower upgrades**

As said before, towers have custom upgrades that can either change their stats or their way of attacking/working. These upgrades are described below, as well as the base and maximum (including the generic and custom upgrades) stats of each tower.

1. ### Minigun
    - Cost: $100
    - Base stats:
        - Damage: 2
        - Attack speed: 8.0
        - Range: 4.0
        - Pierce: 0
    - Maximum stats:
        - Damage: 3
        - Attack speed: 24.75
        - Range: 6.5
        - Pierce: 0
    - Custom upgrades:
        - Longer cannon: gains 1.0 range.
        - Stronger bullets: gains 1 damage.
        - Ludicrous firing rate: gains 3x base attack speed for a total of 24.0.

2. ### Cannon
    - Cost: $350
    - Base stats:
        - Damage: 5
        - Attack speed: 0.4
        - Range: 5.5
        - Pierce: 40
    - Maximum stats:
        - Damage: In theory 10 + 7 * 8 + 5 * 64 = 386
        - Attack speed: 0.85
        - Range: 6.5
        - Pierce: 500
    - Custom upgrades:
        - Faster bombs: bomb projectile speed is increased to 30 (base: 15)
        - Damage increase: increases the damage to 7
        - Cluster bombs: shoots cluster bombs instead of normal ones. The cluster bombs explode in 8 normal bombs on top of dealing its damage. Because this upgrade is too powerful, the tower also looses 0.1 attack speed.
        - Recursive cluster: cluster bombs now also release 8 more cluster bombs. Because this upgrade is completely overpowered, the tower also looses 0.2 attack speed.

3. ### Beenest
    - Cost: $500
    - Base stats:
        - Damage: 8
        - Attack speed: 0.33 for bee generation and around 1.0 for bee attacks
        - Range: 7.0
        - Pierce: 0
    - Maximum stats:
        - Damage: 11
        - Attack speed: 1.08 for bee generation and around 1.0 for bee attacks
        - Range: 8.0
        - Pierce: 0
    - Custom upgrades:
        - More bees: increases the amount of bees the hive can spawn up to 8. (base: 5)
        - Darts: bees will now shoot darts at their enemies. The darts deal 1 damage and have 5 pierce.
        - Sticky honey: enemies attacked by the bees will be 2 times slower.
        - Angry bees: gains 1 damage.

4. ### Ant spray
    - Cost: $400
    - Base stats:
        - Damage: 5
        - Attack speed: 0.5
        - Range: 6.0
        - Pierce: 500
    -Maximum stats:
        - Damage: 5
        - Attack speed: 1.25
        - Range: 7.0
        - Pierce: 500
    - Custom upgrades:
        - Faster spray: increases projectile speed.
        - Intoxicating spray: adds a slow down to hit enemies.
        - Bigger spray: increases the spray hitbox.
        - More sprays: shoots 3 sprays instead of 1.

5. ### Beehive
    As said before, the beehive is used to generate money or to buff Beenests and therefore doesn't have any damage or pierce. Its attack speed is the speed at which it generates money.  
    - Cost: $800
    - Base stats:
        - Attack speed: 2.0
        - Range: 8.0
        - Money generation: 25
    - Maximum stats:
        - Attack speed: 2.75
        - Range: 9.0
        - Money generation: 50
    - Custom upgrades:
        - Faster production: gains 0.5 attack speed.
        - High quality honey: increases money generation to 50.
        - Healthy honey: gains a 10% chance to heal a random amount of lives in range 1-3 on each money generation.
        - Friendly bees: increases in-range Beenests' bees damage by 1.

Keep in mind that most of the above information can be seen in the tower panel either directly or by hovering over a text, image of value.

### **Enemies**
All the information about enemies can be found in the bestiary in the main menu.

### **Glossary**
#### Damage:
<img alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986437457125516/generic_upgrade_attack_damage_icon.png width=128>

    Damage is the amount of damage a projectile or tower will deal to its target.  
    Even though you can't see this icon in game, it was originally an upgradable stat like the attack speed and range.
#### Attack speed:
<img alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986437809442826/generic_upgrade_attack_speed_icon.png width=128>

    Attack speed is the rate at which a tower shoots. It represents a number of attacks per seconds.
#### Range:
<img alt=BeehiveIcon src=https://cdn.discordapp.com/attachments/1027856325633396798/1065986438107234415/generic_upgrade_range_icon.png width=128>

    Range is the distance at which a tower can attack. It represents a circular range in tiles.
#### Pierce:
    Pierce is the number of enemies the projectile can hit and go through before the it dies.  
    It doesn't have an icon because it is a completely hidden stat of the projectiles. The player can only guess its value.

[Back to summary](#summary)
