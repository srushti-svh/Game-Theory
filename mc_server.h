
#ifndef MC_SERVER_H
#define MC_SERVER_H
#include "pd_server.h"
struct Node
{
  double visitCount;
  double winCount;
  //struct Node** children;
  char choice;
  struct Node* left;
  struct Node* right;
  struct Node* parent;
};
typedef struct Node node_t;

struct Payoff
{
  char a1[2];
  double a2[2];
};
typedef struct Payoff Payoff_t;

/*
struct Tree
{
  node_t root;
};
typedef struct Tree tree_t;
*/
char mcts(node_t*, char*, char*, int);
node_t* initializeRoot();
void initialize(node_t*, node_t*);
void display(node_t*);
void free_node(node_t*);
int predict(node_t*, char, payoff_t*);
int rollout(node_t*, char, payoff_t*);
void backpropogate(node_t*, double);
node_t* expand(node_t*);
node_t* selectNode(node_t*);
node_t* uct(node_t*);
void predict_avg(node_t*, char*, double, Payoff_t*);
double rollout_avg(node_t*, char*, double, Payoff_t*);
#endif
