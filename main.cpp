#include<iostream>
#include<assert.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define MAXSZ 100

void Multiply( char sResult[MAXSZ], char sNumber1[MAXSZ], char \
	sNumber2[MAXSZ], int nLenNum1, int nLenNum2 );
void Reverse( char *sNumber, int nLenPre );
void LolcaReverse( char* pBegin, char* pEnd );

int main(void){
	//1000000 put from low index
	/* Careful we need \0 here  */
	char sNumber1[MAXSZ+1];
	char sNumber2[MAXSZ+1];
	char sResult[MAXSZ+1];

	sNumber1[MAXSZ]='\0';
	sNumber2[MAXSZ]='\0';
	sResult[MAXSZ] = '\0';

	cout<<"Input number1:"<<endl;
	cin>>sNumber1;
	cout<<"Input number2:"<<endl;
	cin>>sNumber2;

	// After here char arraies are treated as string
	//show before Reverse
	cout<<sNumber1<<endl;
	cout<<"*"<<endl;
	cout<<sNumber2<<endl;
	cout<<"="<<endl;
	/*set unused bit to zero
	*now is 100382\0xxxxx I want is 0000100382
	*/
	memset( sNumber1+strlen(sNumber1), '0', MAXSZ-strlen(sNumber1) );
	Reverse( sNumber1, strlen(sNumber1) );
	//xxx1382 [0,(MAXSZ)-strlen(sNumber1)-1]
	memset( sNumber2+strlen(sNumber2), '0', MAXSZ-strlen(sNumber2) );
	Reverse( sNumber2, strlen(sNumber2) );

	memset( sResult, '0', MAXSZ );
	Multiply( sResult, sNumber1, sNumber2, strlen(sNumber1), \
		strlen(sNumber2) );
	//000284-->284000
	//Means put 0 strings to the end of sResult
	Reverse( sResult, -1 );
	cout<<sResult<<endl;
}
void Multiply( char *sResult, char *sNumber1, char *sNumber2, \
		int nLenNum1, int nLenNum2 ){
	/*12385937 sNumber2    A little obscure, outer loop is slow
	*x  285693 sNumber1    we use sNumber1's every bit to multiply
	*					   sNumber2
	*/
	//compute from low index 
	//Use sNumber1's each bit to multiply sNumber2
	for(int iOfNum1=0; iOfNum1<nLenNum1; ++iOfNum1){
		//Compute sNumber1[iOfNum1] * sNumber2
		/*
		* 285930731
		* x    3
		*/
		for(int iOfNum2=0; iOfNum2<nLenNum2; ++iOfNum2){
			/* :)! order is human, but I compute in machine way, \
				no difference :D
			*  28593 7 37
			*  x---  8 --
			* =    56
			*/
			/*
			* here '7' x '8' : still in ASCII style
			*/
			int nResult2D = (sNumber1[iOfNum1]-'0') *\
				 (sNumber2[iOfNum2]-'0');
			/*
			* I get 56, now I add it to sResult
			* Assume sResult is:
			*   19209987
			*   +     56 So,Carry here,
			*          3  carry:1
			*        ...  Loop until carry is 0
			*/
			int iForLocalAdd = iOfNum1;// Index 0 is lowest data \
					remember
			int nCarry = nResult2D % 10;// To remember carry result \
						from add, like 7*8,carry is 5
			int nReminder =nResult2D - nCarry*10;// Hers is for\
					 example 56 - 50 = 6;
			int nResultLocalMultiply;
			while( iForLocalAdd<MAXSZ ){// Take care of your National \
					Boundary, Notice is following
				nReminder = sResult[iForLocalAdd]-'0'+ nReminder;
				if( nReminder>=0 ){
					nReminder -= 10;
					nCarry++;
				}
				sResult[iForLocalAdd] += nReminder;// char+int:\
						'4'+4 = 8 :D
				if( nCarry==0 ){// No carry here, you are free
					break;
				}
			}
			// Check overflow, sorry This is still Boundary :(
			if( iForLocalAdd>=MAXSZ ){
				perror("Result overflow.");
				exit(1);
			}
		}
		
	}
	//Reverse Result <-- to show Result in human way :D
	//I love U Douban
	for(int iResultReverse=0; iResultReverse<MAXSZ/2; ++iResultReverse){
		/* 00001 shoule be
		*  10000
		*/
		sResult[iResultReverse]	= sResult[(MAXSZ-1)-iResultReverse];
	}
	
}
/*
Func: Change position of the previous string and the behind string
	e.g. 000989 --> 989000
*sNumber like 0001238
*nLenPre = 3 '000'
*cPreCh = '0'
return : NULL
*/
void Reverse( char *sNumber, int nLenPre ){
	assert( sNumber!=NULL );
	//sResult request recompute length of previous string
	bool bIsResult = 0;
	if( nLenPre==-1 ){
		bIsResult = 1;
		nLenPre=1;
		char * pCur = sNumber;
		while( *pCur=='0' ){
			pCur++;
		}
		nLenPre = pCur-sNumber;
	}
	/*Principle:
	* (X^Y^)^ = YX
	*/
	LolcaReverse( sNumber, sNumber+nLenPre-1 );
	// LocalReverse the behind string 
	// [begin, end] end = MAXSZ-1 ( MAXSZ is length of string
	LolcaReverse( sNumber+nLenPre, sNumber+MAXSZ-1 );
	LolcaReverse( sNumber, sNumber+MAXSZ-1 );
	
	if( bIsResult==1 ){
		//Now sReuslt = 128 000
		//But should be 128\0\0\0;
		memset( sNumber+( MAXSZ-nLenPre ), '\0', nLenPre );
	}
}
/*
 1234 --> 4321
*/
void LolcaReverse( char* pBegin, char* pEnd ){
	assert( (pBegin!=NULL)&&(pEnd!=NULL) );
	char cTemp;
	while( pBegin <pEnd ){
		cTemp = *pBegin;
		*pBegin = *pEnd;
		*pEnd = cTemp;
		--pEnd;
		++pBegin;
	}
}





