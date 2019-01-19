# 2D-Collisions
Simple 2D Collision Software with Circles/Convex Polygons/Lines

Unfortunately this is not the latest version which was lost to the ether. I had added in multi-threading to further speed things up and had implemented a 3D version. The problem of randomly generating 3D convex polygons was a really fun mini challenge!
This piece of software lets you place, drag, throw and launch balls and convex polygons as well as draw lines and make some obstacles. There is also some experimental stuff that breaks the collisions into zones to test the differences between different approaches to the exponential problem of checking all objects for collisions!
Also! Check out some of the videos I have added that demo some of the features (Apologies for the horrendous quality)

Controls

Note: Several things must be #defined to be enabled. These can be found in "sphere_JK_OO_particle.h"

EDGE_PREDICTION	    (Only with circles) Calculates collisions ahead of time instead of moving then checking for overlap
THREAD_USAGE        Not currently used in this version
MOVE_RESIST 0.996;  Global friction on moving pieces. 1 = no friction, 0 = infinite friction.
GRAVITY             Yup
DRAW_POLY_DEBUG     This will show the bounding information on the convex polygons, with extra information if they are almost touching
GRID                Regular grid that can be scaled into smaller or larger sections
HGRID	              (Only with circles) Hierarchical grid that dynamically splits up depending on the number of circles in each segment

Not all of these are functioning and try not to use things that would obviously clash such as GRID and HGRID at the same time!	

Input	|	Output
------------------------------------------------------------------------------------------------------
L-Mouse	| (HOLD) Picks up a particle which can be dragged and thrown 
R-Mouse	| (HOLD) Hold and drag mouse to give a particle an push along that vector
l	| Creates a line: Hold down, click down L-Mouse for start, drag and release for end
n	| Creates a new random circle at the mouse location
b	| Creates a new random convex polygon at the mouse location
= (+)	| Makes target larger
-	| Makes target smaller
.	| Makes target (polygon) spin clockwise
,	| Makes target (polygon) spin anti-clockwise
]	| Increases the static grid size by 1
[	| Decreases the static grid size by 1
t	| Decreases the number of times the hierarchical grid can be divided
y	| Increases the number of times the hierarchical grid can be divided
g	| Decreases the number of circles required to warrant a hierarchical grid space to be split
h	| Increases the number of circles required to warrant a hierarchical grid space to be split
	
