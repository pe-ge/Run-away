#ifndef CONSTANTS_H
#define CONSTANTS_H

#define AUTOSCROLL_3 1 //rychlost pohybu mrakov

#define BAR2_GFX_LIMIT 5

#define BULLET_HP_DRAIN 12 //kolko zivota uberie zasah gulky

#define VERTICAL_SCROLL_TOP 100
#define VERTICAL_SCROLL_BOTTOM 150
#define VERTICAL_SCROLL_MAXSPEED 15
#define VERTICAL_SCROLL_MAXSPEED_DOWN 8

#define IRONSTICK_ATTACK_REPEATS 0
#define IRONSTICK_ATTACK_DELAY   10
#define IRONSTICK_PLAYER_DAMAGE  10

#define SUPER_SPEED_DELAY_F_SEC 1.5

#define BULLET_PPF  10  //rychlost nabojov, pixely za frame

#define BPS         2    // rychlost palby, pocet nabojov za sekundu

#define PLATFORMS_MOVESPEED 2 // rychlost pohybu platform

#define MAX_ATTACKS 2    // maximalny pocet nabojov od jedneho nepriatela na obrazovke

#define FPS 27     // maximalne fps

#define FLIP_DELAY 2    // cas po ktorom padajuca platforma spadne
#define FLIP_ANIM 2    // cas ako dlho bude padat
#define FLIP_STAY 10    // cas po ktorom sa vrati

#define BOWLINGBALL_SPEED   7
#define BOWLINGBALL_VSPEED  14

#define JETPACK_JETS_SPEED  10
#define JETPACK_FALL_SPEED  5

#define TRAMPOLINE_POWER 20

#define DEF_JPOWER 16           // sila skoku

#define DEF_LFORCE 10           // maximalna rychlost pohybu

#define RightScrollPoint 350 //pozicia hraca pri ktorej kamera zacne scrollovat doprava
#define LeftScrollPoint 200   // pozicia hraca pri ktorej kamera zacne scrollovat dolava

#define PLANE1_FACTOR 2     
#define PLANE2_FACTOR 6    
#define PLANE3_FACTOR 18   

#define SEEKER_CAM_SPD 13 // rychlost hladania hraca kamerou, ak sa nahle teleportol

#define LEFT 3
#define RIGHT 4

//ulozene v typedefoch
#define TYPE_PLATFORM   1
#define TYPE_HOLLOW     2
#define TYPE_SOLID      3
#define TYPE_ENEMY 		4
#define TYPE_LEAN	  	5

#define CM_FOLLOW_PLAYER   1001
#define CM_STATIC          1002
#define CM_TASK            1003
#define CM_SEEK_DOOR       1004

#define TOTALCYCLES 15000.00

#endif