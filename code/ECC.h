#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include<time.h>
#define MAX 100 
typedef struct point{	
	int point_x;	
	int point_y;
	}Point;
typedef struct ecc{
	struct point p[MAX];
	int len;}ECCPoint;
typedef struct generator{	
	Point p;
	int p_class;}GENE_SET; 
	
	
void get_all_points();
int int_sqrt(int s);
Point timesPiont(int k,Point p);
Point add_two_points(Point p1,Point p2);
int inverse(int n,int b);
void get_generetor_class();
void encrypt_ecc();
void decrypt_ecc(); 
int mod_p(int s);
int isPrime(int n);
