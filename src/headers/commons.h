#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "utils.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define PI 3.14159265358979323846f
#define SIND(d) (sin((d) / 180.0 * PI))
#define COSD(d) (cos((d) / 180.0 * PI))
