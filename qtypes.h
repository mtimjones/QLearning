
#include <stdlib.h>

#define X_MAX           20      /* Grid World Size */
#define Y_MAX           20

#define MAX_EPOCHS	200000L /* Number of epochs to run */

#define MAX_ACTIONS     4       /* Actions define 4 possible directions */

typedef struct {
   int y;
   int x;
} pos_t;

typedef struct {
   double QVal[ MAX_ACTIONS ];
   double QSum;
   double QMax;
} stateAction_t;

#define LEARNING_RATE	0.3	/* alpha */
#define DISCOUNT_RATE   0.8	/* gamma */

#define EXPLOIT         0   /* Choose best Q */
#define EXPLORE         1   /* Probabilistically choose best Q */

#define getSRand()      ( ( double ) rand( ) / ( double ) RAND_MAX )
#define getRand(x)      ( int )( ( double )( x ) * rand( ) / ( RAND_MAX+1.0 ) )

