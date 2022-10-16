#include<stdio.h>
#include "pd_server.h"
#include<time.h>
#include<stdlib.h>
#include "pbPlots.h"
#include "supportLib.h"
#include "mc_server.h"
#include <string.h>

#define MAX 20

node_t* choose_func(int choice, node_t* root1, char (**decision)(char*,char*,int), char* str)
{
  switch(choice)
    {
    case 1:
      *decision = &nasty;
      strcpy(str, "Nasty");
      break;
    case 2:
      *decision = &pasty;
      strcpy(str, "Pasty");
      break;
    case 3:
      *decision = &eye_for_eye;
      strcpy(str, "Eye for Eye");
      break;
    case 4:
      *decision = &egalitarian;
      strcpy(str, "Egalitarian");
      break;
    case 5:
      root1 = initializeRoot();
      strcpy(str, "Monte Carlo Tree Search");
      //decision = &mcts;
      break;
    }
  return root1;
}



int main(int argc, char* argv[]) {
  char strategy[3];
  double xs[100];
  double ys1[100];
  double ys2[100];
  srand(time(NULL)); // set the seed to generate random numbers 
  int n;
  // Initialize scores, decision history and pointers to strategy functions
  int score1 = 0, score2 = 0, score3 = 0;
  int x, y, z;
  int count = 0;
  node_t* root1 = NULL;
  node_t* root2 = NULL;
  char history0[100];
  char history1[100];
  char history2[100];
  int choice, choice2;
  char (*decision)(char*, char*, int);
  char (*decision2)(char*, char*, int);
  char (*decision3)(char*, char*, char*, int);
  char str1[30];
  char str2[30];

  // Make the payoff list with each entry as a structure of strategies and payoffs
  payoff_t list_for_2[4] =
  {
    {"cc", {3, 3}},
    {"cd", {0, 5}},
    {"dc", {5, 0}},
    {"dd", {1, 1}}
  };

  payoff_t list[] =
  {
    {"ccc", {4, 4, 4}},
    {"ccd", {2, 2, 5}},
    {"cdc", {2, 5, 2}},
    {"dcc", {5, 2, 2}},
    {"cdd", {0, 3, 3}},
    {"dcd", {3, 0, 3}},
    {"ddc", {3, 3, 0}},
    {"ddd", {1, 1, 1}}
  };

  FILE* storage1;
  
  storage1 = fopen("file4.csv", "w+");
  if(storage1 == NULL)
    {
      printf("Error in opening the file/s\n");
      return 1;
    }
    

  printf("How many players(2 or 3)?");
  scanf("%d", &n);

  if(n == 2)
    {
      printf("Strategies: \n");
      printf("1. Nasty\n");
      printf("2. Pasty\n");
      
      printf("3. Eye For Eye\n");
      printf("4. Egalitarian\n");
      printf("5. Monte Carlo Tree Search\n");
      
      do
	{
	  printf("Enter your choice: (1/2/3/4/5)?");
	  scanf("%d", &choice);
	 
	  root1 = choose_func(choice, root1, &decision, str1);
	}
      while(choice>5);

      
      do
	{
	  printf("Against: (1/2/3/4/5)?");
	  scanf("%d", &choice2);
	 
	  root2 = choose_func(choice2, root2, &decision2, str2);
	}
      while(choice2>5);
    
      fprintf(storage1, "%d,%d,%d\n", 0, choice, choice2);

      printf("%s:%s\n", str1, str2);
      for(int i=0; i<MAX; i++) {
        
	// get first strategy in a random play
	//strategy[0] = genRandom();
	//strategy[1] = (*decision2)(history0, history1, i);

	if(choice==5)
	  {
	    strategy[0] = mcts(root1, history1, history0, 0);
	  }
	else
	  strategy[0] = (*decision)(history1, history0, i);
	if(choice2==5)
	  {
	    strategy[1] = mcts(root2, history0, history1, 1);
	  }
	else
	  strategy[1] = (*decision2)(history0, history1, i);
	
   
	history0[i] = strategy[0];
	history1[i] = strategy[1];
   
	calc_score2(strategy, &x, &y, list_for_2);
    
	score1 += x;
	score2 += y;
	fprintf(storage1, "%d,%d,%d\n", i, score1, score2);
	//fprintf(storage2, "%d,%d\n", i, score2);


	printf("%d: %d\n", x, y);
      }
      printf("Total scores are: \n");
      printf("%d: %d\n", score1, score2);
      display(root1);
      free_node(root1);
      free_node(root2);
    }
  else if(n==3)
    {
      printf("Strategies: \n");
      printf("1. Tough Eye for Eye\n");
      printf("2. Soft Eye for Eye\n");
      printf("Enter your choice: (1/2)?");
      scanf("%d", &choice);
      switch(choice)
	{
	case 1:
	  decision3 = &tough_efore;
	  break;
	case 2:
	  decision3 = &soft_efore;
	  break;
	default:
	  exit(0);
	}
      for(int i=0; i<MAX; i++)
	{
	  strategy[0] = genRandom();
	  strategy[1] = genRandom();
	  strategy[2] = (*decision3)(history0, history1, history2, i);
	  history0[i] = strategy[0];
	  history1[i] = strategy[1];
	  history2[i] = strategy[2];
   
	  calc_score3(strategy, &x, &y, &z, list);
    
	  score1 += x;
	  score2 += y;
	  score3 += z;
	  printf("%d: %d: %d, \n", x, y, z);
	}
      printf("Total scores are: \n");
      printf("%d: %d: %d\n", score1, score2, score3);
      exit(0);
    }
    
  char data[20];
  int k = 0;
  int m=0;
  char* ptr;
  fseek(storage1, 0, SEEK_SET); // set the file pointer to the beginning of the file
  
  RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
  RGBABitmapImageReference *canvasReference2 =  CreateRGBABitmapImageReference();
  fgets(data, 20, storage1);
  while(fgets(data, 20, storage1) != NULL)
    {
      char* col1 = strtok(data, ",");
     
      xs[k] = strtod(col1, &ptr);
      col1 = strtok(NULL, ",");
      ys1[k] = strtod(col1, &ptr);
      col1 = strtok(NULL, ",");
      ys2[k] = strtod(col1, &ptr);
      k++;
    }
  
  DrawScatterPlot(canvasReference, 600, 400, xs, MAX, ys1, MAX);
 
  DrawScatterPlot(canvasReference2, 600, 400, xs, MAX, ys2, MAX);
  
  size_t length;
  double *pngdata = ConvertToPNG(&length, canvasReference->image);
  WriteToFile(pngdata, length, "trial2.png");
  DeleteImage(canvasReference->image);

  pngdata = ConvertToPNG(&length, canvasReference2->image);
  WriteToFile(pngdata, length, "trial3.png");
  DeleteImage(canvasReference2->image);
  
  fclose(storage1);
  root1 = NULL;
  root2 = NULL;
  /*
  for(int i=0; i<MAX; i++)
    printf("%f %f %f\n", xs[i], ys1[i], ys2[i]);
  */
  return 0;
}
