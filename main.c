// qlearn.c

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "qtypes.h"


const pos_t dir[ MAX_ACTIONS ] =
{
  { -1,  0 },  /* N */
  {  0,  1 },  /* E */
  {  1,  0 },  /* S */
  {  0, -1 }   /* W */
};


char environment[ Y_MAX ][ X_MAX ] =
{
//  0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9
 { '+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+' },
 { '|',' ',' ',' ',' ',' ',' ',' ',' ','#','#','#',' ','#','#','#','#','#',' ','|' },
 { '|',' ',' ',' ','#','#','#',' ',' ','#','#','#',' ','#','#','#','#','#',' ','|' },
 { '|','#',' ',' ',' ','#','#','#',' ',' ',' ',' ',' ','#','#',' ',' ',' ',' ','|' },
 { '|','#',' ',' ',' ',' ','#','#','#','#','#','#','#','#',' ',' ',' ',' ',' ','|' },
 { '|','#',' ',' ',' ',' ','#','#','#','#','#','#','#','#',' ','#','#',' ',' ','|' },
 { '|',' ',' ',' ','#','#','#',' ',' ',' ',' ','#','#',' ',' ',' ','#','#',' ','|' },
 { '|',' ',' ','#','#','#','#','#',' ',' ',' ','#','#',' ',' ','#','#','#',' ','|' },
 { '|',' ',' ','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ','|' },
 { '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#','#',' ',' ','#','#',' ','|' },
 { '|',' ','#',' ',' ',' ','#','#','#',' ',' ','#','#','#',' ','#','#','#','#','|' },
 { '|','#','#','#',' ',' ','#','#','#',' ',' ',' ','#','#',' ','#','$',' ',' ','|' },
 { '|','#','#','#',' ','#','#','#','#',' ',' ',' ',' ',' ',' ','#','#','#',' ','|' },
 { '|','#','#','#',' ',' ','#','#','#',' ','#','#','#','#',' ','#','#','#',' ','|' },
 { '|','#','#','#',' ',' ',' ','#','#',' ','#','#','#',' ',' ',' ','#','#',' ','|' },
 { '|','#','#',' ',' ',' ',' ',' ','#',' ','#',' ',' ',' ','#',' ','#','#',' ','|' },
 { '|','#',' ',' ',' ','#','#',' ',' ',' ',' ',' ','#','#','#',' ','#','#',' ','|' },
 { '|',' ',' ',' ','#','#','#','#',' ',' ',' ',' ','#','#','#',' ',' ',' ',' ','|' },
 { '|',' ',' ',' ','#','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|' },
 { '+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+' }
};


pos_t start = { 1,  1};
pos_t goal =  {11, 16};

stateAction_t stateSpace[ Y_MAX ][ X_MAX ];


int getReward( char input )
{
   switch( input )
   {
      case '-':
      case '|':
      case '#':
         // Obstacle, not a legal move
         return -1;
      case ' ':
      case '.':
         // Open space, legal move, 0 reward
         return 0;
      case '$':
         // Goal, legal move, 1 reward
         return 1;
   }

   return 0;
}


void initStateSpace( void )
{
   for ( int y = 0 ; y < Y_MAX ; y++ )
   {
      for ( int x = 0 ; x < X_MAX ; x++ )
      {
         stateSpace[ y ][ x ].QSum = 0.0;
         stateSpace[ y ][ x ].QMax = 0.0;

         for ( int action = 0 ; action < MAX_ACTIONS ; action++ )
         {
            stateSpace[ y ][ x ].QVal[ action ] = 0.0;
         }
      }
   }

   return;
}


void findMaxQ( int y, int x )
{
  stateSpace[ y ][ x ].QMax = 0.0;

  for ( int i = 0 ; i < MAX_ACTIONS ; i++ )
  {
    if ( stateSpace[ y ][ x ].QVal[ i ] > stateSpace[ y ][ x ].QMax )
    {
      stateSpace[ y ][ x ].QMax = stateSpace[ y ][ x ].QVal[ i ];
    }
  }

  return;
}


int legalMove( int y_state, int x_state, int action )
{
  int y = y_state + dir[ action ].y;
  int x = x_state + dir[ action ].x;

  if ( getReward( environment[ y ][ x ] ) < 0 ) return 0;
  else return 1;
}


int ChooseAgentAction( pos_t *agent, int actionSelection )
{
   int action;

   // Choose the best action
   if ( actionSelection == EXPLOIT )
   {
      findMaxQ( agent->y, agent->x );

      for ( action = 0 ; action < MAX_ACTIONS ; action++ )
      {
         if ( stateSpace[ agent->y ][ agent->x ].QMax == 
              stateSpace[ agent->y ][ agent->x ].QVal[ action ])
         {
            return( action );
         }
      }
   }
   // Choose a random action.
   else if ( actionSelection == EXPLORE )
   {
      do
      {
        action = getRand( MAX_ACTIONS );
      } while ( !legalMove( agent->y, agent->x, action ) );
   }

   return action;
}


void UpdateAgent( pos_t *agent, int action )
{
   int newy = agent->y + dir[ action ].y;
   int newx = agent->x + dir[ action ].x;
   double reward = (double)getReward( environment[ newy ][ newx ] );

   /* Update the QMax value for the state */
   findMaxQ( newy, newx );

   // Evaluate Q value 
   stateSpace[ agent->y ][ agent->x ].QVal[ action ] += 
     LEARNING_RATE * ( reward + ( DISCOUNT_RATE * stateSpace[ newy ][ newx ].QMax) -
                        stateSpace[ agent->y ][ agent->x ].QVal[ action ] );

   // Update the Qsum
   stateSpace[ agent->y ][ agent->x ].QSum = 0.0;

   for ( int i = 0 ; i < MAX_ACTIONS ; i++ )
   {
      stateSpace[ agent->y ][ agent->x ].QSum += stateSpace[ agent->y ][ agent->x ].QVal[ i ];
   }

   // Update the agent's position
   agent->x += dir[ action ].x;
   agent->y += dir[ action ].y;

   // If agent has reached the goal, move it back to the initial state
   if ( ( agent->x == goal.x ) && ( agent->y == goal.y ) )
   {
      agent->x = start.x; agent->y = start.y;
   }

   return;
}


void ExecuteAgent( void )
{
   pos_t agent;
   int action;

   agent = start;

   // Execute the agent with the exploit policy to find the best path.
   while ( !( ( agent.x == goal.x ) && ( agent.y == goal.y ) ) )
   {
      // Drop a breadcrumb in the environment
      environment[ agent.y ][ agent.x ] = '.';

      action = ChooseAgentAction( &agent, EXPLOIT );

      agent.x += dir[ action ].x;
      agent.y += dir[ action ].y;
   }

   // Show the environment with the agent's path.
   printf( "\n" );
   for ( int y = 0 ; y < Y_MAX ; y++ )
   {
      for ( int x = 0 ; x < X_MAX ; x++ )
      {
         printf( "%c ", environment[ y ][ x ] );
      }
      printf( "\n" );
   }
   printf( "\n" );

   return;
}


int main()
{
   pos_t agent = start;

   srand( time( NULL ) );

   initStateSpace( );

   for ( long epochs = 0 ; epochs < MAX_EPOCHS ; epochs++ )
   {
      // Select the action for the agent.
      int action = ChooseAgentAction( &agent, EXPLORE );

      // Update the agent based upon the action.
      UpdateAgent( &agent, action );
   }

   // Show the agent's path
   ExecuteAgent( );

   return 0;
}

