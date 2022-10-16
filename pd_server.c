#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include "pd_server.h"
#include "mc_server.h"
#include<string.h>
char genRandom()
{
  // generate a random number from 0 to 1
  float n;
  
  n = (rand() % 2);
  if(n <= 0.5)
    return 'c';
  else
    return 'd';
}

char pasty(char* history0, char* history1, int n){
  return 'c';
}

char nasty(char* history0, char* history1, int n){
  return 'd';
}


char eye_for_eye(char* h0, char* h1, int n)
{
  if(n>0)
    {
      if(h0[n-1] == 'd')
	return 'd';
    }
  return 'c';
}

char egalitarian(char* h0, char* h1, int n)
{
  int h0_c = 0;
  int h0_d = 0;
  for(int i=0; i<n; i++)
    {
      if(h0[i] == 'c')
	h0_c++;
      
      else if(h0[i] == 'd')
	h0_d++;
    }
  
  if(n > 0)
    {
      if(h0_d > h0_c)
	return 'd';
    }
  return 'c';
} 
  
void calc_score2(char* strategy, int* x, int* y, payoff_t* list)
{
  // compare each of the 2 players' strategies with the character array of the structure
  // and assign the corresponding payoffs
  for(int i=0; i<4; i++)
    {
      if((strategy[0] == list[i].a1[0]) && (strategy[1] == list[i].a1[1]) )
	{
	  *x = list[i].a2[0];
	  *y = list[i].a2[1];
	}
    }
}

void calc_score3(char* strategy, int* x, int* y, int* z, payoff_t* list)
{
  // compare each of the 3 players' strategies with the character array of the structure
  // and assign the corresponding payoffs
  for(int i=0; i<8; i++)
    {
      if((strategy[0] == list[i].a1[0]) && (strategy[1] == list[i].a1[1]) && (strategy[2] == list[i].a1[2]))
	{
	  *x = list[i].a2[0];
	  *y = list[i].a2[1];
	  *z = list[i].a2[2];
	}
    }
}

// defect if either player defected in previous round
char tough_efore(char* h0, char* h1, char* h2, int n)
{
  if(n>0)
    {
  //printf("%c %c\n", h0[n-1], h1[n-1]);
      if((h0[n-1] == 'd') || (h1[n-1] == 'd'))
	return 'd';
    }
  return 'c';
}
// defect if both the players defected in previous round
char soft_efore(char* h0, char* h1, char* h2, int n)
{
  if(n>0)
    {
  //printf("%c %c\n", h0[n-1], h1[n-1]);
      if((h0[n-1] == 'd') && (h1[n-1] == 'd'))
	return 'd';
    }
  return 'c';
}


