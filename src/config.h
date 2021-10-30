#ifndef CONFIG_H_
#define CONFIG_H_

/** Triangular Mesh Settings **/

/* Shading Method */
#define PHONG_SHADING true
#define FLAT_SHADING false


/** Antialiasing Setting **/

/* Mode */
#define JITTERING true
#define RANDOM false
#define UNIFORM false

/* Number of Samples */
#define H_SAMPLES 2
#define V_SAMPLES 2


/** Raytracer Setting  **/

/* Maximum number of secondary rays */
#define DEPTH 3

/* Maximum Viewing Distance */
#define MAX_DIST 1000

/* Background Colour */
#define BACKGROUND_COLOUR black()

/* Reflection Model */
#define PHONG true
#define BILNN_PHONG false

#endif
