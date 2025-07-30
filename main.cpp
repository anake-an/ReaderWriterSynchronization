#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
using namespace std;

// Synchronization variables
pthread_mutex_t mutex;       // Protects read_count
pthread_mutex_t printLock;   // Protects cout
sem_t wrt;
int read_count = 0;
int CYCLES = 3;
bool delayReaders = false; //Optional delay for reader (for TC3)

void* reader(void* arg) {
    int id = *((int*)arg);

    if (delayReaders) sleep(1); // Optional delay to let writer go first

    for (int i = 0; i < CYCLES; ++i) {
        // Entry section
        pthread_mutex_lock(&mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&wrt); // First reader blocks writers
        pthread_mutex_unlock(&mutex);

        // Reading section
        pthread_mutex_lock(&printLock);
        cout << "Reader " << id << " starts reading." << endl;
        pthread_mutex_unlock(&printLock);

        ifstream inFile("io_shared_file.txt"); // Read from the shared file
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                pthread_mutex_lock(&printLock);
                cout << "Reader " << id << " reads: " << line << endl;
                pthread_mutex_unlock(&printLock);
            }
            inFile.close();
        } else { // Error if file can't be opened
            pthread_mutex_lock(&printLock);
            cout << "Reader " << id << " could not open file." << endl;
            pthread_mutex_unlock(&printLock);
        }

        pthread_mutex_lock(&printLock); // Finish reading
        cout << "Reader " << id << " finishes reading." << endl;
        pthread_mutex_unlock(&printLock);

        // Exit section
        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&wrt); // Last reader unblocks writers
        pthread_mutex_unlock(&mutex);

        sleep(1); // Simulate delay between read cycles
    }
    return nullptr;
}

void* writer(void* arg) {
    int id = *((int*)arg); // Get the writer's ID from the argument
    for (int i = 0; i < CYCLES; ++i) {
        pthread_mutex_lock(&printLock); // Show that the writer is waiting to get access
        cout << "Writer " << id << " is waiting to write..." << endl;
        pthread_mutex_unlock(&printLock);

        sem_wait(&wrt); // Only one writer allowed at a time

        pthread_mutex_lock(&printLock);
        cout << "Writer " << id << " starts writing." << endl; // Critical section: start writing
        pthread_mutex_unlock(&printLock);
        // Open the file in append mode and write a line
        ofstream outFile("io_shared_file.txt", ios::app); 
        if (outFile.is_open()) {
            outFile << ", Writer " << id << " wrote at cycle " << (i + 1) << endl;
            outFile.close();
        } else {
            pthread_mutex_lock(&printLock); // Done writing
            cout << "Writer " << id << " could not write to file." << endl;
            pthread_mutex_unlock(&printLock);
        }

        pthread_mutex_lock(&printLock);
        cout << "Writer " << id << " finishes writing." << endl;
        pthread_mutex_unlock(&printLock);

        sem_post(&wrt); // Release the semaphore to let readers/writers continue
        sleep(2); // Simulate time delay between writing cycles
    }
    return nullptr;
}

int main() {
    int numReaders, numWriters;
    // Init sync mechanisms
    pthread_mutex_init(&mutex, nullptr); // For protecting read_count
    pthread_mutex_init(&printLock, nullptr); // For clean printing
    sem_init(&wrt, 0, 1); // Writer semaphore
    // Display banner
    pthread_mutex_lock(&printLock);
    cout << "==================================================================\n";
    cout << " Reader-Writer Synchronization\n";
    cout << "==================================================================\n";
    pthread_mutex_unlock(&printLock);
    // User input
    cout << "Enter number of reader threads: ";
    cin >> numReaders;
    cout << "Enter number of writer threads: ";
    cin >> numWriters;
    cout << "Enter number of cycles per thread: ";
    cin >> CYCLES;
    cout << "Delay readers? (1 = Yes, 0 = No): ";
    int delayInput;
    cin >> delayInput;
    delayReaders = (delayInput == 1);
    cout << "==================================================================\n";
    // Thread creation
    pthread_t readers[numReaders], writers[numWriters];
    int readerIDs[numReaders], writerIDs[numWriters];

    for (int i = 0; i < numReaders; ++i) { // Create reader threads
        readerIDs[i] = i + 1;
        pthread_create(&readers[i], nullptr, reader, &readerIDs[i]);
    }
    for (int i = 0; i < numWriters; ++i) {
        writerIDs[i] = i + 1;
        pthread_create(&writers[i], nullptr, writer, &writerIDs[i]);
    }
    // Thread join
    for (int i = 0; i < numReaders; ++i)
        pthread_join(readers[i], nullptr);
    for (int i = 0; i < numWriters; ++i)
        pthread_join(writers[i], nullptr);

    pthread_mutex_lock(&printLock); // Final output
    cout << "==================================================================\n";
    cout << "Simulation complete. All reader and writer threads have finished\n";
    cout << "==================================================================\n";
    pthread_mutex_unlock(&printLock);
    // Clean-up
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&printLock);
    sem_destroy(&wrt);
    return 0;
}

