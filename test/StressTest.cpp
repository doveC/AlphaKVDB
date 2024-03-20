#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "../SkipList.h"

#define NUM_THREADS 2
#define TEST_COUNT 100000

using namespace std;

Util::SkipList<int, std::string> skipList(18);

void* insertElement(void* threadId) 
{
    (void)threadId;
    int insertTimes = TEST_COUNT / NUM_THREADS; 

	for (int count = 0; count < insertTimes; count++)
    {
        int num = rand() % TEST_COUNT;
		skipList.insertElement(num, "a"); 
	}
    pthread_exit(nullptr);
}

void* getElement(void* threadId) 
{
    (void)threadId;
    int searchTimes = TEST_COUNT / NUM_THREADS; 

	for (int count = 0; count < searchTimes; count++) 
    {
        int num = rand() % TEST_COUNT;
		skipList.searchElement(num); 
	}
    pthread_exit(nullptr);
}

int main() 
{
    srand(time(nullptr));  

    // Test insert element
    std::chrono::duration<double> elapsed1;
    {
        pthread_t threads[NUM_THREADS];

        auto start = std::chrono::high_resolution_clock::now();

        for(uint64_t i = 0; i < NUM_THREADS; i++) 
        {
            std::cout << "main(): creating thread, " << i << std::endl;
            int rc = pthread_create(&threads[i], nullptr, insertElement, (void*)i);

            if (rc) 
            {
                std::cout << "Error: unable to create thread, " << rc << std::endl;
                exit(-1);
            }
        }

        void* ret;
        for(int i = 0; i < NUM_THREADS; i++) 
        {
            if (pthread_join(threads[i], &ret) != 0)  {
                perror("pthread_create() error"); 
                exit(-1);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); 
        elapsed1 = finish - start;
    }

    // Test get element
    std::chrono::duration<double> elapsed2;
    {
        pthread_t threads[NUM_THREADS];
        auto start = std::chrono::high_resolution_clock::now();

        for(uint64_t i = 0; i < NUM_THREADS; i++ ) 
        {
            std::cout << "main() : creating thread, " << i << std::endl;
            int rc = pthread_create(&threads[i], nullptr, getElement, (void *)i);

            if (rc) 
            {
                std::cout << "Error: unable to create thread," << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for(int i = 0; i < NUM_THREADS; i++ ) 
        {
            if (pthread_join(threads[i], &ret) != 0)  {
                perror("pthread_create() error"); 
                exit(-1);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); 
        elapsed2 = finish - start;
    }

    std::cout << "Insert " << TEST_COUNT << " elements done. Elapsed: " << elapsed1.count() << std::endl;
    std::cout << "Search " << TEST_COUNT << " elements done. Elapsed: " << elapsed2.count() << std::endl;

	pthread_exit(nullptr);
    return 0;
}