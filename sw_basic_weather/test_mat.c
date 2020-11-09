#include<stdio.h>
#include<engine.h>
#include<matrix.h>

int main (void)
{
	Engine *pEngine;
	int i,j;
	
	int a[10]={5,3,1,6,9,3,5,6,8,2};

	mxArray *mxMatrix, *mxDet;
	double *pmxMatrix, *pDet;

	if(!(pEngine=engOpen(NULL)))
	{
		printf("Can not open the MATLAB engine.\n");
	}

	mxMatrix=mxCreateDoubleMatrix(10,1,mxREAL);
	pmxMatrix=mxGetPr(mxMatrix);
	
	for(i=0;i<10;i++) 
	{
		*((pmxMatrix)+i)=a[i];
	}
	engPutVariable(pEngine,"M",mxMatrix);
	engEvalString(pEngine,"figure(1)");
	engEvalString(pEngine,"bar(M)");
}
