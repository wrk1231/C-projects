#include <iostream> 

#define RUNNING 1
#define WAITING 0
#define FINISHED 2 


class baggie {
public:
	baggie(int Nsharesinput, int Tinput, double alphainput, double pi1input, double pi2input, double p1input, double p2input, double rhoinput, int name_in);
	~baggie() { printf("worker %d says goodbye\n", name); }
	void setconsolemutex(HANDLE consolemutexinput);
	void setmastermutex(HANDLE consolemutexinput);
	void baggiecomp();
	double getmeits(void) { return iterationsdone; }
	void setstatustofinished(void) { status = FINISHED; }
	int getstatus(void) { return status; }
	void setheavysectionmutex(HANDLE heavysectioninput) { heavysectionmutex = heavysectioninput; }
	void setmaxworkersinheavysection(int maxheavy) {
		maxworkersinheavysection = maxheavy;
	}
	void setnowinheavyaddress(int *paddress) { address_of_nowinheavysection = paddress; }
	double returnresult(void) { return result; }
private:
	int Nshares;
	int T;
	int name;
	double alpha;
	double pi1;
	double pi2;
	double p1;
	double p2;
	double rho;
	double iterationsdone;
	int status;
	int maxworkersinheavysection;
	int *address_of_nowinheavysection;  /** this is the address of the integer keeping track of how many workers are busy **/
	HANDLE heavysectionmutex;
	HANDLE consolemutex;
	HANDLE mastermutex;
	double result;
	int *resultpath;
	void letmein(void);
	void seeya(void);
};

using namespace std;

#pragma once
