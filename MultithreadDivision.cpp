#include <iostream>
#include <unistd.h>
#include <string.h>
#include <time.h>		//usleep()
#include <pthread.h>	//pthread.create()
#include <mutex>		//mutex

#define MAXTHREADS 3	//max threads required

using namespace std;
mutex mtx;

typedef int int32_t;
typedef short int16_t;

int64_t cmcnt = 0; //common multiple of 3, 7, and 11 counter

/*
	Core 1 - Prints numbers divisible by 3
*/
void *core01(void *arg)
{
	int64_t i;	//counter for functions
	
	for(i = 0;; i++){
		if(i % 3 == 0 && i % 231 != 0){
			mtx.lock();
			cout << "\033[1;31m [ core 1 ] " << "Integer " << i << " is divisible by 3\033[0m" << endl; 
			mtx.unlock();
			usleep(3.3333e5);	//333ms delay
		} else{
			if(i % 231 == 0){
				cmcnt++;
				do {
					usleep(1e6);
				} while(cmcnt < 3);
				mtx.lock();
				cout << "\033[1;31m [ core 1 ] " << "Integer " << i << " is divisible by 3, 7, and 11\033[0m" << endl; 
				mtx.unlock();
				
				cmcnt = 0;
			}	
		}
		
	}	
}

/*
	Core 2 - Prints numbers divisible by 7
*/
void *core02(void *arg)
{
	int64_t i;	//counter for functions
	
	for(i = 0;; i++){
		if(i % 7 == 0 && i % 231 != 0){
			mtx.lock();
			cout << "\033[1;32m [ core 2 ] " << "Integer " << i << " is divisible by 7\033[0m" << endl; 
			mtx.unlock();
			usleep(6.6666e5);	//666ms delay
		} else{
			if(i % 231 == 0){
				cmcnt++;
				do {
					usleep(1e6);
				} while(cmcnt < 3);
				mtx.lock();
				cout << "\033[1;32m [ core 2 ] " << "Integer " << i << " is divisible by 3, 7, and 11\033[0m" << endl;
				mtx.unlock();
				
				cmcnt = 0;
			}
		}
	}	
}

/*
	Core 3 - Prints numbers divisible by 11
*/
void *core03(void *arg)
{
	int64_t i;	//counter for functions
	
	for(i = 0;; i++){
		if(i % 11 == 0 && i % 231 != 0){
			mtx.lock();
			cout << "\033[1;33m [ core 3 ] " << "Integer " << i << " is divisible by 11\033[0m" << endl;
			mtx.unlock();			
			usleep(1e6);	//1 sec delay
		} else{
			if(i % 231 == 0){
				cmcnt++;
				do {
					usleep(1e6);
				} while(cmcnt < 3);
				mtx.lock();
				cout << "\033[1;33m [ core 3 ] " << "Integer " << i << " is divisible by 3,7, and 11\033[0m" << endl;
				mtx.unlock();
				
				cmcnt = 0;
			}
		}
	}
}

static void* (*pcore[])(void *) = {core01, core02, core03};	//Creating cores

void create_threads()
{
	int64_t i, err;
	cpu_set_t cpuset;
	pthread_t tid[MAXTHREADS];
	
	for(i = 0; i < MAXTHREADS; i++){
		err = pthread_create(&tid[i], NULL, pcore[i], NULL);
		if(err){
			cout << "[ create_threads ] Thread cannot be created, error" << err << endl;
			return;
		} 
	}
}

int32_t main()
{
	cout << " [ main ] " << "Program started" << endl;
	create_threads();
	
	while(1){
		usleep(1e6);
	}	
	return 0;
}	
