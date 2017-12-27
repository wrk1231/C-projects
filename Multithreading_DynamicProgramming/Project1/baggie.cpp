
#include "stdafx.h" 
#include <windows.h> 
#include <process.h>
#include "baggie.h"

double mytimecheck(void);

// implementation file for class baggie

baggie::baggie(int Nshares_in, int T_in, double alpha_in, double pi1_in, double pi2_in, double p1_in, double p2_in, double rho_in,
	int name_in)
{
	Nshares = Nshares_in; T = T_in; alpha = alpha_in; pi1 = pi1_in; pi2 = pi2_in; p1 = p1_in; p2 = p2_in; rho = rho_in;
	name = name_in;
	iterationsdone = 0;
	resultpath = (int *)calloc(T_in, sizeof(int));
	result = 0;
	status = WAITING;

}

void baggie::setconsolemutex(HANDLE consolemutexinput)
{
	consolemutex = consolemutexinput;
}
void baggie::setmastermutex(HANDLE mastermutexinput)
{
	mastermutex = mastermutexinput;
}

void baggie::letmein(void)
{
	char icangoin;
	int localinheavysection;

	icangoin = 0;
	while (icangoin == 0) {
		Sleep(1000);
		WaitForSingleObject(heavysectionmutex, INFINITE);

		if ((*address_of_nowinheavysection) < maxworkersinheavysection) {
			/** key logic: it checks to see if the number of workers in the heavy section is less than the
			number we want to allow **/
			icangoin = 1;
			++*address_of_nowinheavysection; //** increase the count
			localinheavysection = *address_of_nowinheavysection;
			// so localinheavysection will have the count of busy workers
		}

		ReleaseMutex(heavysectionmutex);
	}
	WaitForSingleObject(consolemutex, INFINITE);
	cout << "******worker" << name << ": I'm in and there are " << localinheavysection << " total busy workers\n";
	// we can use localinheavysection without protecting it with a mutex, because it is a local variable to this function, i.e.
	// it is not shared with other mutexes
	ReleaseMutex(consolemutex);
}

void baggie::seeya(void)
{

	WaitForSingleObject(heavysectionmutex, INFINITE);
	--*address_of_nowinheavysection;
	ReleaseMutex(heavysectionmutex);

}
void baggie::baggiecomp(void)
{
	int i, j, h, t, iteration;
	double outeriterations;
	int countforone, *positions;
	double *optimal, newprice, candidate, bestone = 0;
	optimal = (double *)calloc((Nshares + 1)*T, sizeof(double));
	positions = (int *)calloc((Nshares + 1)*T, sizeof(int));
	outeriterations = 0;
	status = RUNNING;
	double t1 = mytimecheck();  // see the comments below.  mytimecheck() returns the time of day in milliseconds
								// it is defined in mytimer.cpp
	letmein(); // check to see if we can become busy
	for (t = T - 1; t >= 0; t--) {
		for (j = 0; j <= Nshares; j++) {
			if (t == T - 1) {
				newprice = p1 * (1 - alpha * pow((double)j, pi1)) + p2 * (1 - alpha * pow((double)j, pi2));
				optimal[(T - 1) * (Nshares + 1) + j] = newprice * j * rho;
				if (bestone < optimal[(T - 1) * (Nshares + 1) + j]) {
					bestone = optimal[(T - 1) * (Nshares + 1) + j];
					countforone = j;
				}
				positions[(T - 1) * (Nshares + 1) + j] = j;
			}
			else {
				bestone = 0;
				for (h = 0; h <= j; h++) {
					newprice = p1 * (1 - alpha*pow((double)h, pi1)) + p2 * (1 - alpha*pow((double)h, pi2));
					candidate = rho * newprice * (h + optimal[(t + 1)*(Nshares + 1) + j - h]) + (1 - rho) * newprice * optimal[(t + 1)*(Nshares + 1) + j];
					if (candidate > bestone) {
						bestone = candidate;
						positions[t * (Nshares + 1) + j] = h;
					}
				}
				optimal[t * (Nshares + 1) + j] = bestone;
			}
		}
		++outeriterations;
	}
	iterationsdone = outeriterations;
	double t2 = mytimecheck();  // check out to see how this function works, it's in mytimer.cpp
								// mytimecheck simply returns the time of day in milliseconds
	double tdiff;

	tdiff = t2 - t1;  // t1 was set above 

	WaitForSingleObject(consolemutex, INFINITE);
	printf(" >> worker %d:  I have completed my work in time %g\n", name, tdiff);
	ReleaseMutex(consolemutex);

	seeya();

	WaitForSingleObject(consolemutex, INFINITE);
	printf(" >> worker %d:  I am out\n", name);
	ReleaseMutex(consolemutex);

	if (T == 1) {
		result = optimal[countforone];
		resultpath[0] = countforone;
		printf("worker %d: optimal revenue for trade sequencing = %g\n", name, result);
		printf("worker %d: At stage t = %d we sell %d shares.\n", name, T, resultpath[0]);
	}
	else {
		result = optimal[Nshares];
		printf("worker %d: optimal revenue for trade sequencing = %g\n", name, result);
		int i = 1, tempN = Nshares;
		while (T >= i) {
			resultpath[i - 1] = positions[(i - 1) * (Nshares + 1) + tempN];
			printf("worker %d: At stage t = %d we sell %d shares.\n", name, i, resultpath[i - 1]);
			tempN = tempN - positions[(i - 1) * (Nshares + 1) + tempN];
			i++;
		}
	}
}

