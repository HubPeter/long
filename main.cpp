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
bool CheckNumber( const char* sNumber );

int main(void){
	//1000000 put from low index
	/* Careful we need \0 here  */
	char sNumber1[MAXSZ+1];
	char sNumber2[MAXSZ+1];
	char sResult[MAXSZ+1];

	sNumber1[MAXSZ]='\0';
	sNumber2[MAXSZ]='\0';
	sResult[MAXSZ] = '\0';

	int nLenNum1;//Effective length of Number1:
	int nLenNum2;//Effective Length of Number2:
	
	cout<<"Input number1:"<<endl;
	cin>>sNumber1;
	if( !CheckNumber( sNumber1 ) ){
		cout<<"Numbers only"<<endl;
		exit(1);
	}
	nLenNum1 = strlen( sNumber1 );
	cout<<"Input number2:"<<endl;
	cin>>sNumber2;
	if( !CheckNumber( sNumber2 ) ){
		cout<<"Numbers only"<<endl;
		exit( 1 );
	}
	nLenNum2 = strlen( sNumber2 );

	// After here char arraies are treated as string
	//show before Reverse
	cout<<"=====================================*"<<endl;
	cout<<sNumber2;
	cout<<" * "<<endl;
	cout<<sNumber1;
	cout<<"="<<endl;
	/*set unused bit to zero
	*now is 100382\0xxxxx I want is 0000100382
	*/
	memset( sNumber1+strlen(sNumber1), '0', MAXSZ-strlen(sNumber1) );
	Reverse( sNumber1, nLenNum1 );
	//xxx1382 [0,(MAXSZ)-strlen(sNumber1)-1]
	memset( sNumber2+strlen(sNumber2), '0', MAXSZ-strlen(sNumber2) );
	Reverse( sNumber2, nLenNum2 );
	
	memset( sResult, '0', MAXSZ );
	cout<<"-----------------------------------"<<endl;
	Multiply( sResult, sNumber1, sNumber2, nLenNum1, \
		nLenNum2 );
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
	//*********
	for(int iOfNum1=MAXSZ-1; iOfNum1>=(MAXSZ-nLenNum1); --iOfNum1){
		//Compute sNumber1[iOfNum1] * sNumber2
		/*
		* 285930731
		* x    3
		*/
		//********Here secnod time BUg
		for(int iOfNum2=MAXSZ-1; iOfNum2>=(MAXSZ-nLenNum2); --iOfNum2){
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
			//Here is iOfNum2
			int iForLocalAdd = (iOfNum2+iOfNum1)-MAXSZ+1;
			// Index 0 is lowest data \
					remember, but in highest index
			int nCarry = nResult2D / 10;// To remember carry result \
						from add, like 7*8,carry is 5
			int nReminder = nResult2D % 10;// Hers is for\
					 example 56 - 50 = 6;
			//log begin
			//cout<<"|---->nCarry:"<<nCarry<<endl;;
			cout<<sNumber1[iOfNum1]<<"x"<<sNumber2[iOfNum2];
			cout<<"="<<nResult2D<<endl;
			//log end
			while( iForLocalAdd>=0 ){//this rule is only for boundary
				//计算余数
				cout<<"nReminder"<<sResult[iForLocalAdd]<<"+"<<nReminder<<"+"<<nCarry<<endl;//log
				nReminder = sResult[iForLocalAdd]-'0'+ nReminder \
					+ nCarry;
				nCarry = 0;//clear nCarry
				//如果余数》=10，进位
				if( nReminder>=10 ){
					nCarry = nReminder / 10;//Before here nCarry \
						may bigger than 1, e.g. 7*8=56
					nReminder = nReminder % 10;
				}
				//将计算后的余数赋值到结果中
				//cout<<"iForLocalAdd"<<iForLocalAdd<<"nReminder:"<<sResult[iForLocalAdd]<<endl;//log
				sResult[iForLocalAdd] = '0'+nReminder;
				//cout<<"nReminder:"<<sResult[iForLocalAdd]<<endl;//log
				//cout<<"sResult:"<<sResult<<endl;//log
				//如果此时进位为0，说明已经计算完毕，可以退出循环
				if( nCarry==0 ){// No carry here, you are free
					break;
				}
				//如果没有退出需要继续执行，直到没有进位
				iForLocalAdd--;//for high power's addition
			}
			// Check overflow, sorry This is still Boundary :(
			if( nCarry!=0 ){// U are in trouble !
				cout<<"Result overflow."<<endl;
				exit(1);
			}
			//log
			//cout<<"nCarry"<<nCarry<<"<----|"<<endl;
			//log end
		}
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

bool CheckNumber( const char* sNumber ){
	assert( sNumber!=NULL );
	char * pCur = (char*) sNumber;
	while(  (*pCur!='\0') && (*pCur>='0') && (*pCur<='9') ){
		pCur++;
	}
	if( *pCur=='\0' ){
		return 1;
	}
	else{
		return 0;
	}
}














