#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include<time.h>
#define MAX 100 
void encrypt_ecc() {}
void decrypt_ecc() {}

int isPrime(int n){	
	int i,k;    
	k = sqrt(n);
    for (i = 2; i <= k;i++)    
	{
    	if (n%i == 0)
			break;	
	}          
		if (i <=k)
			return -1;	
		}     
		else {    	 
			return 0;	
		} 
}
 
int mod_p(int s){	
	int i;		
	int result;		
	i = s / p;	
	result = s - i * p;	
	if (result >= 0)	
	{		
		return result;	
	}	
	else	
	{		
		return result + p;	
	}
}
