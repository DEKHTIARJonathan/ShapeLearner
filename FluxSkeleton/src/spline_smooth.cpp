#include "spline_smooth.h"
#include <math.h>
#include <stdio.h>
#include <malloc.h>
/*************************************************************************************************************	      
 The following is function solves a linear system of equations having a symmetric, cyclically five-diagonal
 (pentadiagonal) coefficient matrix. The function is given the size of the square matrix, given by n, where n
 in our current application denotes the number of nodes of the filament we want to smooth. 
 Array A contains the diagonal entries of the five-diagonal matrix. Array B contains the first subdiagonal
 (i.e. the diagonal directly under the main diagonal) of the matrix and array C contains the entries on the 
 second subdiagonal. Initially, array F contains the right hand side values of the linear equations.
 The solution vector is stored in array F by the time the function terminates. In addition, in the course
 of the function, arrays A, B and C are overwritten with useful information to used in solving the system.
 The approach taken to solve the system is by decomposing our five-diagonal matrix, M, into 2 matrices, L and
 U where L is a lower triangular matrix and U is an upper triangular matrix, such that M=LU.
 This function is a direct implementaion of the Fortran program PENPES found in the book "One Dimensional
 Spline Interpolation Algorithms" by Helmuth Spath. 
****************************************************************************************************************/
void Solve_Linear_System(int n, 
			 float *A, 
			 float *B, 
			 float *C, 
			 float *F)
{

  //working space arrays
  float *D, *E, *H;

  float z, h1, h3, h2, h4, h5, hh1 = 0, hh3, hhh1, sum;
  int nm1, nm2, nm3, nm4, nm5, k, km1, km2, kp1, i;

  int nn = 2*n;
  // first, create work arrays
  D = (float *)malloc(sizeof(float)*(nn+1));
  E = (float *)malloc(sizeof(float)*(nn+1));
  H = (float *)malloc(sizeof(float)*(nn+1));

  n=n-1;
  nm1=n-1;
  nm2=n-2;
  nm3=n-3;
  nm4=n-4;

  z=A[1];


  A[1]=F[1]/z;
  D[1]=C[nm1]/z;
  E[1]=B[n]/z;
  F[1]=C[nm1];
  H[1]=B[n];
  h3=B[1];
  h1=C[1];
  h4=B[1]/z;
  B[1]=h4;
  C[1]=C[1]/z;

  z=A[2] - (h3*h4);



  A[2]=(F[2]-h3*A[1])/z;
  D[2]=-(h3*D[1])/z;
  E[2]=(C[n]- h3*E[1])/z;
  F[2]=-F[1]*h4;
  H[2]=C[n]-H[1]*h4;
  hh3=h3;
  h3=B[2]-h1*h4;
  h4=(B[2]-hh3*C[1])/z;
  B[2]=h4;

  if(n > 5)
    {
      nm5=n-5;

      for(k=3;k<=nm3;k++)
	{
	  km1=k-1;
	  km2=k-2;
	  kp1=k+1;
	  hh1=h1;
	  h1=C[km1];
	  C[km1]=C[km1]/z;
	  h2=C[km2];
	  z=A[k] - hh1*h2 - h3*h4;



	  A[k]=(F[k]-hh1*A[km2]-h3*A[km1])/z;
	  D[k]=-(hh1*D[km2] + h3*D[km1])/z;
	  E[k]=-(hh1*E[km2] + h3*E[km1])/z;
	  F[k]=-F[km2]*h2-F[km1]*h4;
	  H[k]=-H[km2]*h2-H[km1]*h4;
	  hh3=h3;
	  h3=B[k]- h1*h4;
	  h4=(B[k]-hh3*C[km1])/z;
	  B[k]=h4;
	}

      hhh1=hh1;
      hh1=h1;
      h1=C[nm3]-F[nm5]*C[nm5]-F[nm4]*B[nm4];
      C[nm3]=(C[nm3]-hhh1*D[nm5]-hh3*D[nm4])/z;
    }//end if

  else
    {
      hh1=h1;
      h1=C[nm3]-F[nm4]*B[nm4];
      C[nm3]=(C[nm3]-hh3*D[nm4])/z;
    }

  h2=C[nm4];
  z=A[nm2]-hh1*h2 - h3*h4;



  A[nm2]=(F[nm2]-hh1*A[nm4]-h3*A[nm3])/z;
  hh3=h3;
  h3=B[nm2]-F[nm4]*h2-h1*h4;
  h4=(B[nm2]-D[nm4]*hh1-hh3*C[nm3])/z;
  B[nm2]=h4;
  hhh1=hh1;
  hh1=h1;
  h1=C[nm2]-H[nm4]*h2-H[nm3]*B[nm3];
  C[nm2]=(C[nm2]-hhh1*E[nm4]-hh3*E[nm3])/z;

  sum=0.0;
  
  for(i=1;i<=nm4;i++)
    sum=sum+F[i]*D[i];

  z=A[nm1]-hh1*C[nm3]-h3*h4-sum;



  sum=0.0;

  for(i=1;i<=nm4;i++)
    sum=sum+F[i]*A[i];


  A[nm1]=(F[nm1]-sum-hh1*A[nm3]-h3*A[nm2])/z;

  sum=0.0;

  for(i=1;i<=nm4;i++)
    sum=sum + F[i]*E[i];


  h5=B[nm1];
  h4=(h5-h3*C[nm2]-sum-hh1*E[nm3])/z;
  B[nm1]=h4;

  sum=0.0;

  for(i=1;i<=nm4;i++)
      sum=sum+H[i]*D[i];

  hh3=h3;
  h3=h5-h1*B[nm2]-sum-H[nm3]*C[nm3];

  sum=0.0;

  for(i=1;i<=nm3;i++)
    sum=sum+H[i]*E[i];



  z=A[n]-h1*C[nm2]-h3*h4-sum;


  sum=0.0;
  for(i=1;i<=nm3;i++)
    sum=sum+H[i]*A[i];


  A[n]=(F[n]-sum-h1*A[nm2]-h3*A[nm1])/z;
  F[n]=A[n];
  F[nm1]=A[nm1]-B[nm1]*F[n];
  F[nm2]=A[nm2]-B[nm2]*F[nm1]-C[nm2]*F[n];
  F[nm3]=A[nm3]-B[nm3]*F[nm2]-C[nm3]*F[nm1]-E[nm3]*F[n];

 for(i=nm4;i>=1;i=i-1)
   F[i]=A[i]-B[i]*F[i+1]-C[i]*F[i+2]-D[i]*F[nm1]-E[i]*F[n];


 // finally, free working arrays
 free(D);
 free(E);
 free(H);

}





/*************************************************************************************************************	      
 The following function smooths a number of filaments using a given wieght, weight. The filaments are
 assumed to be 3-dimensional and the filament node locations are stored in the 2D array Position (the standard 
 structure used in this application for storing filament nodes). The function returns the smoothed out nodes
 in the given array Position (and therefore updating it).
 The smoothing is done by using smoothing cubic splines where, for each closed filament, the original nodes 
 are used as "knots" for the splining. Since we assume all filaments are CLOSED, periodic smoothing is used where
 the last node and first node are the same. 
 Since our filaments are 3dimensional, we need to perform the smoothing splines in each coordinate. Therefore, we
 need to parametrize our filaments using a parameter, S, that runs over the length of each filament.
 Since smooothing splines smooth out a 1dimensional function (x,f(x)), we have to perform smoothing splines
 3 times for each filament such that each coordinate is smoothed out i.e. (s,X(s)), (s,Y(s)) and (s,Z(s)).
 The parameter values are stored in array S. The wieght determines how close our fit is. As wieght->0, maximum
 smoothing is reached and our filaments will become 1 point. As weight->infinitiy, our splines become
 INTERPOLATING splines and therefore going through the original points. The heart of this algorithm is
 an implementation of the Fortran program CUBSM2 in the book "One dimensional interpolating algorithms" by
 Helmuth Spath.  
****************************************************************************************************************/
void Periodic_Smoothing_Spline(Coordinates *Points,
			       int n, // the number of nodes
			       float weight)
{

  //the following is the data structure needed as workspace for the algorithm
  //float Filament[NODES][4];
  float **Filament;
  float *A, *B, *C, *D, *Y2, *S;

  //  int Fil_Num, 
  int i, nodes, n1, n2, j, w, k, k1;//, ns ;

  float seg_length, ds, h, h1, h2, h3, h4, r1, r2, p, s;//, dsavg, snew;

  int kn=2*n+5; 
  // now, create work arrays
  A = (float *)malloc(sizeof(float)*(kn+1));
  B = (float *)malloc(sizeof(float)*(kn+1));
  C = (float *)malloc(sizeof(float)*(kn+1));
  D = (float *)malloc(sizeof(float)*(kn+1));
  Y2= (float *)malloc(sizeof(float)*(kn+1));
  S = (float *)malloc(sizeof(float)*(kn+1)); 

  Filament = (float**)malloc(sizeof(float*)*(kn+1));
  for(i=0; i<n+1; i++){
    Filament[i] = (float *)malloc(sizeof(float)*10);
  }
  

  for(i=0; i<n;i++){
    //    fprintf(stderr, "%f %f\n", Points[i].xcord, Points[i].ycord);
/*      Points[i].xcord += 0.1; */
/*      Points[i].ycord += 0.1; */
  }
  

  //set the weights; these are the same value for all nodes i.e. polynomials
  p=(float)1.0/weight;  

  //get the total number of filaments
  //Fil_Num=(int)Points[0].xcord;



  //smooth each filament (we assume each filament is closed 3D curve)
  //for(i=1;i<=Fil_Num;i++)
  {

      //get the number of nodes in the current filament such that the last node is equal to the first node i.e. periodic 
      nodes= n; //(int)Points[0].xcord + 1;

      n1=nodes-1;
      n2=nodes-2;


      /*transfer the nodes of the current filament to a temporary
	storage 2d-array of floats This is done in order to make the
	subsequent calculations easier since the x, y and z coordinates of
	node j are accessed by Filament[j][1], Filament[j][2] and
	Filament[j][3].*/  
      for(j=1;j<=n1;j++)
	{
	  Filament[j][1]=Points[j].xcord;
	  Filament[j][2]=Points[j].ycord;
	  Filament[j][3]=Points[j].zcord;
	}

      //let the last element in array Filament be identical to the first one (this structure is needed for algorithm)
      Filament[nodes][1]=Points[1].xcord;
      Filament[nodes][2]=Points[1].ycord;
      Filament[nodes][3]=Points[1].zcord;
     

      //we need to reparametrize our 3d-curve (filament) by S 
      //we store the cumulative length of our filament in array S as such
      S[1]=0.0;

      for(j=2;j<=n1;j++)
	{
	  seg_length = SQR(Filament[j][1]-Filament[j-1][1])
	    +  SQR(Filament[j][2]-Filament[j-1][2])
	    +  SQR(Filament[j][3]-Filament[j-1][3]);

	  ds=sqrt(seg_length);

	  //add the length of this node to the total length of the filament
	  S[j]= S[j-1] + ds;
	}


      //fill-up the last entry of array S (it's more efficient this way)
      seg_length = SQR(Filament[1][1]-Filament[n1][1])
	+  SQR(Filament[1][2]-Filament[n1][2])
	+  SQR(Filament[1][3]-Filament[n1][3]);
      
      ds=sqrt(seg_length);
      
      //add the length of this node to the total length of the filament
      S[nodes]= S[n1] + ds;   

   
     //smooth each coordinate (x,y,z) using splines
      for(w=1;w<=3;w++)
	{

	  h1 = (float)1.0/(S[nodes]-S[n1]);
	  
	  h2 = (float)1.0/(S[2]-S[1]);


	  r1=(Filament[1][w] - Filament[n1][w])*h1;

	  //compute preparations 
	  for(k=1;k<=n1;k++)
	    {
	      k1=k+1;
	      s=h1+h2;
  
	      if(k < n1)
		{
		  h3=(float)1.0/(S[k+2]-S[k1]);
		  r2=(Filament[k1][w] - Filament[k][w])*h2;
		}
	      else
		{
		  h3=(float)1.0/(S[2]-S[1]);
		  r2=(Filament[1][w] - Filament[n1][w])*h2;
		}


	      A[k]=(float)((2.0/h1) + (2.0/h2) + 6.0*(h1*h1*p + s*s*p + h2*h2*p));

	      if(k < n1)
		B[k]=(float)((1.0/h2) - 6.0*h2*(p*s + p*(h2+h3)));

	      if(k < n2)
		C[k]=(float)(6.0*p*h2*h3);
	      
	      //array Y2 contains the values of the second derivatives
	      Y2[k]=(float)(6.0*(r2-r1));

	      if(k==1)
		{
		  h4=(float)(1.0/(S[n1]-S[n2]));
		  B[n1]=(float)(1.0/h1 - 6.0*h1*(p*(h1+h4) + p*(h1+h2)));
		  C[n2]=(float)(6.0*p*h4*h1);
		}
	      
	      if(k==2)
		  C[n1]=(float)(6.0*p*h1/(S[nodes]-S[n1]));

		  h1=h2;
		  h2=h3;
		  r1=r2;
	       
	    }//end for loop over k

	  /*now that we have the values of the second derivatives stored in Y2, we proceed
	    to solve the system of linear equations. The solution will be outputed in array Y2 too*/
	  Solve_Linear_System(n1, A, B, C, Y2);

	  Y2[nodes]=Y2[1];

	  h1=(Y2[1]-Y2[n1])/(S[nodes]-S[n1]);

	  //now that we've solved th equations, we proceed to calculate the polynomial coefficients

	  for(k=1;k<=n1;k++)
	    {
	      k1=k+1;
	      h=(float)(1.0/(S[k1]-S[k]));
	      B[k]=h;
	      h2=h*(Y2[k1]-Y2[k]);
	      D[k]=(float)(h2/6.0);
	      A[k]=Filament[k][w]-(h2-h1)*p;
	      C[k]=(float)(Y2[k]/2.0);
	      h1=h2;
	    }

	  A[nodes]=A[1];

	  for(k=1;k<=n1;k++)
	    {
	      k1=k+1;
	      h=B[k];
	      B[k]=(float)((A[k1]-A[k])*h - (Y2[k1] + 2.0*Y2[k])/(6.0*h));
	    }

	  /*we now have the coefficient of the polynomials for the given filament and given coordinate. We can update our nodes and 
	    adjust the coordinate value of all nodes in order to smooth them. Array A, B, C and D contain the coefficients. We'll update the 
	    original array Points to impose lasting changes.*/

       

	  //let's try to place nodes ON the produced smoothing spline
	  if(w==1)
	    {

	      //smooth-out the xcord. of the current filament's nodes
	      for(j=1;j<=n1-1;j++)
		{

		  //place a node on the cubic polynomial, halfway between each pair of original knots
		  ds=(float)((S[j+1]-S[j])/2.0);

		  Points[j].xcord= A[j] + ds*B[j] + ds*ds*C[j] + ds*ds*ds*D[j];
		  
/*  		  fprintf(stderr, "%f %f %f %f %f \n", Points[j].xcord, */
/*  			  A[j], B[j], C[j], D[j]); */
		}	  
	      
	    }

	  else if(w==2)
	    {	      

	      //smooth-out the ycord. of the current filament's nodes
	      for(j=1;j<=n1-1;j++)
		{
		  //place a node on the cubic polynomial, halfway between each pair of original knots
		  ds=(float)((S[j+1]-S[j])/2.0);
		  
		  Points[j].ycord= A[j] + ds*B[j] + ds*ds*C[j] + ds*ds*ds*D[j];

		  //fprintf(stderr, "%f %f %f %f %f \n", Points[j].ycord,
		  // A[j], B[j], C[j], D[j]);
		  //fprintf(stderr, "%f Points[j].ycord\n", 
		  //	  A[j] + ds*B[j] + ds*ds*C[j] + ds*ds*ds*D[j]);
		}	  
	      
	    }
	  
	  else if(w==3)
	    {	     
	      //smooth-out the zcord. of the current filament's nodes
	      for(j=1;j<=n1;j++)
		{

		  //place a node on the cubic polynomial, halfway between each pair of original knots
		  //		  if (j==n1) ds = 
		  ds=(float)((S[j+1]-S[j])/2.0);
		  
		  Points[j].zcord= A[j] + ds*B[j] + ds*ds*C[j] + ds*ds*ds*D[j];		
		}	  	      
	      
	    }	  
	  
      
	}//finish smoothing over all coordinates for the given filament

    }//end smoothing all filaments

  

  // time to kill the work arrays
  free(A);
  free(B);
  free(C);
  free(S);
  free(Y2);
  free(S);

  for(i=0; i<n+1; i++){
    free(Filament[i]);
  }
  free(Filament);
}

