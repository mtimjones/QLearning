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

//
// Return the reward value for the state
//
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

//
// Initialize the Q data for the state/action space.
//
void initStateSpace( void )
{
   for ( int y = 0 ; y < Y_MAX ; y++ )
   {
      for ( int x = 0 ; x < X_MAX ; x++ )
      {
         stateSpace[ y ][ x ].QMax = 0.0;

         for ( int action = 0 ; action < MAX_ACTIONS ; action++ )
         {
            stateSpace[ y ][ x ].QVal[ action ] = 0.0;
         }
      }
   }

   return;
}

//
// Identify whether the desired move is legal.
//
int legalMove( int y_state, int x_state, int action )
{
  int y = y_state + dir[ action ].y;
  int x = x_state + dir[ action ].x;

  if ( getReward( environment[ y ][ x ] ) < 0 ) return 0;
  else return 1;
}

//
// Choose an action based upon the selection policy.
//
int ChooseAgentAction( pos_t *agent, int actionSelection )
{
   int action;

   // Choose the best action (largest Q-value)
   if ( actionSelection == EXPLOIT )
   {
      action = 0;

      for ( int i = 1 ; i < MAX_ACTIONS ; i++ )
      {
         if ( stateSpace[ agent->y ][ agent->x ].QVal[ i ] >
              stateSpace[ agent->y ][ agent->x ].QVal[ action ] )
         {
            action = i;
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

//
// Update the agent using the Q-value function.
//
void UpdateAgent( pos_t *agent, int action )
{
   int newy = agent->y + dir[ action ].y;
   int newx = agent->x + dir[ action ].x;
   double reward = (double)getReward( environment[ newy ][ newx ] );

   // Evaluate Q value 
   stateSpace[ agent->y ][ agent->x ].QVal[ action ] += 
     LEARNING_RATE * ( reward + ( DISCOUNT_RATE * stateSpace[ newy ][ newx ].QMax) -
                        stateSpace[ agent->y ][ agent->x ].QVal[ action ] );

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

//
// Execute the agent using the exploit policy to show its path.
//
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

//
// Main function for q-learning.
//
int main()
{
   pos_t agent = start;

   srand( time( NULL ) );

   // Init the state/action Q data
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

