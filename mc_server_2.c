#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
#include<string.h>
#include "mc_server.h"
#include "pd_server.h"
#include<time.h>


payoff_t list[] =
  {
    {"cc", {3, 3}},
    {"cd", {0, 5}},
    {"dc", {5, 0}},
    {"dd", {1, 1}}
  };
char mcts(node_t* root, char* h0, char* h1, int n)
{
  char c;
  c = (n==0)?'0':'1';
  node_t* selectedNode = selectNode(root);
  int result = rollout(selectedNode, c, list);
  backpropogate(selectedNode, result);
  return selectedNode->choice;
}

node_t* initializeRoot()
{
  node_t* root = (node_t*)malloc(sizeof(node_t));
  if(root == NULL)
    {
      printf("Memory allocation failed\n");
      exit(-1);
    }
  root->left = (node_t*)malloc(sizeof(node_t));
  root->right = (node_t*)malloc(sizeof(node_t));
  if(root->left == NULL || root->right == NULL)
    {
      printf("Memory allocation failed\n");
      exit(-1);
    }
  
  root->left->choice = 'c';
  root->right->choice = 'd';
  initialize(root->left, root);
  initialize(root->right, root);
  root->parent = NULL;
  root->choice = '\0';
  root->visitCount = 0;
  root->winCount = 0;
  return root;
}

void initialize(node_t* node, node_t* parentNode)
{
  node->left = NULL;
  node->right = NULL;
  node->parent = parentNode;
  node->visitCount = 0;
  node->winCount = 0;

}
void display(node_t* root)
{
  node_t* temp = root;
  if(temp!=NULL)
    {
      display(temp->left);
      printf("%.1f/%.1f\n",temp->visitCount, temp->winCount);
      display(temp->right);
      
    }
  return;
}

void free_node(node_t* root)
{
  if(root == NULL)
    return;

  free_node(root->left);
  free_node(root->right);

  printf("Deleted node: %.1f/%.1f\n", root->visitCount, root->winCount);
  free(root);
}
      
int predict(node_t* root, char c, payoff_t* list)
{
  node_t* selectedNode = selectNode(root);
  int result = rollout(selectedNode, c, list);
  backpropogate(selectedNode, result);
  return result;
}
int rollout(node_t* node, char c, payoff_t* list)
{
  //finish rollout and get the final score
  char mc, opponent;
  char template[3];
  int n;
  int x, y;
  
  
  opponent = genRandom();
  mc = node->choice;
  if(c == '0')
    {template[0] = mc; template[1] = opponent;}
  else
    {template[1] = mc; template[0] = opponent;}
  calc_score2(template, &x, &y, list);
  
  return (c=='0')?x:y;
  
}

void backpropogate(node_t* node, double result)
{
  node_t* temp  = node;
  while(temp != NULL)
    {
      temp->visitCount += 1;
      temp->winCount += result;
      temp = temp->parent;
    }
}

node_t* expand(node_t* node)
{
  
  if(node->left == NULL && node->right == NULL)
    {
      node->left = (node_t*)malloc(sizeof(node_t));
      node->right = (node_t*)malloc(sizeof(node_t));
      node->left->choice = 'c';
      node->right->choice = 'd';
      initialize(node->left, node);
      initialize(node->right, node);
    }

    return node->left;

  
}
node_t* selectNode(node_t* root)
{
  node_t* node = root;
  while(node->left != NULL && node->right != NULL)
    {
      if(node->left->visitCount == 0 && node->right->visitCount==0)
	 return node->left;
      node = uct(node);
    }
    if(node->visitCount != 0)
        node = expand(node);
  return node;
}

node_t* uct(node_t* node)
{
  node_t* n1 = node->left;
  node_t* n2 = node->right;
  double uct1, uct2;
  //printf("%d\n", n2->visitCount);
  if(fpclassify(n1->winCount/n1->visitCount) == FP_NAN)
    return n1;
  uct1 = (n1->winCount)/(n1->visitCount) + 2* sqrt(log(node->visitCount)/(n1->visitCount));
  
  if(fpclassify(n2->winCount/n2->visitCount) == FP_NAN)
    return n2;
  uct2 = (n2->winCount)/(n2->visitCount) + 2*sqrt(log(node->visitCount)/n2->visitCount);

  return (uct1 > uct2)?n1:n2;
}

void predict_avg(node_t* head, char* data1, double stats, Payoff_t* list)
{
   node_t* selectedNode = selectNode(head);
   double result = rollout_avg(selectedNode, data1, stats, list);
   backpropogate(selectedNode, result);
}

double rollout_avg(node_t* node, char* data1, double stats, Payoff_t* list)
{
  char pepsi, cola;
  double n;
  double score = 0;
  double x;
  

  n = (rand() % 2);
  pepsi = (n >= 0.5)?'y': 'n';

  cola = (node->choice == 'd')?'y':'n';

  for(int i=0; i<4; i++)
    {
      if((pepsi == list[i].a1[0]) && (cola == list[i].a1[1]))
	{
	  x = list[i].a2[0];
	}
    }
  return (stats - x);
  /*
  cola = (node->choice == 'd')?'y':'n';
  for(int j = 0; j<5; j++)
  {
        n = (rand() % 2);
        pepsi = (n == 1)?'y': 'n';
        for(int i=0; i<4; i++)
        {
            if(pepsi == list[j].a1[0]) && (cola == list[j].a1[1]))
	        {
                // dec corresponds to coco cola and data to pepsi
	            x = list[j].a2[0];
		    printf("%f\n", x);
	        }
        }
        score += (stats - x);
        printf("%f\n", stats - x);
    }
  return score;
   */
}


