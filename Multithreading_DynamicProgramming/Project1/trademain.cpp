#include "stdafx.h"
#include <windows.h> 
#include <process.h>
#include <stdio.h>
#include <stdlib.h> 
#include "baggie.h" 
#include <iostream>
#include <fstream>
#include <sstream>


unsigned _stdcall comp_wrapper(void *foo);

int main(int argc, char *argv[])
{
	HANDLE *pThread;
	unsigned *pthreadID;
	HANDLE consolemutex;
	HANDLE *mastermutexes;
	int retcode = 0;
	int N, W, k;
	FILE *in = NULL;
	char mybuffer[100];
	int *Nshares = NULL, *T = NULL;
	double *alpha = NULL, *pi1 = NULL, *pi2 = NULL, *p1 = NULL, *p2 = NULL, *rho = NULL;
	ofstream outFile;
	baggie **ppbaggies;

	if (argc != 2) {
		printf("usage: trademain.exe datafilename\n");
		retcode = 1; goto BACK;
	}

	in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("could not open %s for reading\n", argv[1]);
		retcode = 1;
		goto BACK;
	}

	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	N = atoi(mybuffer);
	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	W = atoi(mybuffer);
	printf("total threads: %d, workers allowed at most: %d\n", N, W);
	Nshares = (int *)calloc(N, sizeof(int));
	T = (int *)calloc(N, sizeof(int));
	alpha = (double *)calloc(N, sizeof(double));
	pi1 = (double *)calloc(N, sizeof(double));
	pi2 = (double *)calloc(N, sizeof(double));
	p1 = (double *)calloc(N, sizeof(double));
	p2 = (double *)calloc(N, sizeof(double));
	rho = (double *)calloc(N, sizeof(double));
	if (Nshares == NULL || T == NULL || alpha == NULL || pi1 == NULL || pi2 == NULL || p1 == NULL || p2 == NULL || rho == NULL) {
		printf("no memery\n");
		retcode = 1;
		goto BACK;
	}
	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	for (k = 0; k < N; k++) {
		fscanf(in, "%s", mybuffer);
		Nshares[k] = atoi(mybuffer);
		fscanf(in, "%s", mybuffer);
		T[k] = atoi(mybuffer);
		fscanf(in, "%s", mybuffer);
		alpha[k] = atof(mybuffer);
		fscanf(in, "%s", mybuffer);
		pi1[k] = atof(mybuffer);
		fscanf(in, "%s", mybuffer);
		pi2[k] = atof(mybuffer);
		fscanf(in, "%s", mybuffer);
		p1[k] = atof(mybuffer);
		fscanf(in, "%s", mybuffer);
		p2[k] = atof(mybuffer);
		fscanf(in, "%s", mybuffer);
		rho[k] = atof(mybuffer);
	}
	fscanf(in, "%s", mybuffer);
	fclose(in);
	printf("read datafile done!\n");
	//goto BACK;


	ppbaggies = (baggie **)calloc(N, sizeof(baggie *));
	/** ppbaggies is an array, each of whose members is the address of a baggie, and so the type of ppbaggies is baggie ** **/
	if (ppbaggies == NULL) {
		cout << "cannot allocate" << N << "baggies\n";
		retcode = 1; goto BACK;
	}
	pThread = (HANDLE *)calloc(N, sizeof(HANDLE));
	pthreadID = (unsigned *)calloc(N, sizeof(unsigned));
	mastermutexes = (HANDLE *)calloc(N, sizeof(HANDLE));
	if ((pThread == NULL) || (pthreadID == NULL) || (mastermutexes == NULL)) {
		cout << "cannot allocate" << N << "handles and threadids\n";
		retcode = 1; goto BACK;
	}

	for (int j = 0; j < N; j++) {
		ppbaggies[j] = new baggie(Nshares[j], T[j], alpha[j], pi1[j], pi2[j], p1[j], p2[j], rho[j], j);
		// "jobs": normally we would get a list of jobs from e.g. a file
	}


	consolemutex = CreateMutex(NULL, 0, NULL);

	for (int j = 0; j < N; j++) {
		ppbaggies[j]->setconsolemutex(consolemutex); // consolemutex shared across workers plus master
	}

	HANDLE heavymutex;
	heavymutex = CreateMutex(NULL, 0, NULL);

	int nowinheavy = 0;

	for (int j = 0; j < N; j++) {
		mastermutexes[j] = CreateMutex(NULL, 0, NULL);
		ppbaggies[j]->setmastermutex(mastermutexes[j]);

		ppbaggies[j]->setmaxworkersinheavysection(W);
		ppbaggies[j]->setheavysectionmutex(heavymutex);

		ppbaggies[j]->setnowinheavyaddress(&nowinheavy);


	}

	for (int j = 0; j < N; j++) {
		pThread[j] = (HANDLE)_beginthreadex(NULL, 0, &comp_wrapper, (void *)ppbaggies[j],
			0, &pthreadID[j]);
	}

	int numberrunning = N;

	for (; numberrunning > 0;) {
		Sleep(10000);
		printf("master will now check on workers\n"); fflush(stdout);

		for (int j = 0; j < N; j++) {
			double jiterations;
			char jstatus = RUNNING;

			WaitForSingleObject(mastermutexes[j], INFINITE);

			jstatus = ppbaggies[j]->getstatus();

			ReleaseMutex(mastermutexes[j]);



			if (jstatus == RUNNING) {


				WaitForSingleObject(mastermutexes[j], INFINITE);

				jiterations = ppbaggies[j]->getmeits();

				double limit = T[j];
				if (jiterations >= limit) {
					ppbaggies[j]->setstatustofinished();
					--numberrunning;
				}

				ReleaseMutex(mastermutexes[j]);


				WaitForSingleObject(consolemutex, INFINITE);
				printf("master: worker %d has done %g iterations, limit: %g\n", j,
					jiterations, limit);
				ReleaseMutex(consolemutex);

			}

		}
	}

	double *resultarray = NULL;
	resultarray = (double *)calloc(N, sizeof(double));
	if (resultarray == NULL) {
		cout << "cannot allocate" << N << "resultarray\n";
		retcode = 1; goto BACK;
	}
	for (int j = 0; j < N; j++) {
		WaitForSingleObject(pThread[j], INFINITE);
		printf("--> thread %d done\n", j);
		resultarray[j] = ppbaggies[j]->returnresult();
		delete ppbaggies[j]; // calls destructor
	}
	free(ppbaggies); 
	outFile.open("dataresult_rho.csv", ios::out);
	for (int j = 0; j < N; j++) {
		outFile << resultarray[j] << endl;
	}
	outFile.close();
BACK:
	return retcode;
}

unsigned _stdcall comp_wrapper(void *genericaddress)
{
	baggie *pbaggie = (baggie *)genericaddress;
	pbaggie->baggiecomp();
	return 0;
}

