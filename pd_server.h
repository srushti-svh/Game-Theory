
#ifndef PD_SERVER_H
#define PD_SERVER_H
struct payoff
{
  char a1[3];
  int a2[3];
};
typedef struct payoff payoff_t;
 
char genRandom();

char pasty(char*, char*, int);
char nasty(char*, char*, int);
char eye_for_eye(char*, char*, int);
char egalitarian(char*, char*, int);
void calc_score2(char*, int*, int*, payoff_t*);
	   
void calc_score3(char*, int*, int*, int*, payoff_t*);

char tough_efore(char*, char*, char*, int);
char soft_efore(char*, char*, char*, int);
#endif
