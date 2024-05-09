#include <iostream>
#include <mpi.h>
#include <vector>
#include <ctime>

// Metoda pătratului mijlociu
unsigned long long midSquareRandom(unsigned long long seed, int iterations) {
    unsigned long long num = seed;
    unsigned long long result = 0;
    for (int i = 0; i < iterations; ++i) {
        num *= num;
        num /= 100; // extrage cifrele din mijloc
        result = result * 10 + (num % 10);
    }
    return result;
}

// Generator liniar congruențial
unsigned long long linearCongruentialRandom(unsigned long long seed, unsigned long long a, unsigned long long c, unsigned long long m, int iterations) {
    unsigned long long num = seed;
    unsigned long long result = 0;
    for (int i = 0; i < iterations; ++i) {
        num = (a * num + c) % m;
        result = result * 10 + (num % 10);
    }
    return result;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned long long seed = std::time(0) + rank; // seed-ul pentru fiecare proces MPI
    int iterations = 5; // numărul de iterații pentru fiecare proces

    if (rank == 0) {
        std::cout << "Metoda pătratului mijlociu:\n";
    }

    unsigned long long midSquareResult = midSquareRandom(seed, iterations);
    std::vector<unsigned long long> results(size);
    MPI_Gather(&midSquareResult, 1, MPI_UNSIGNED_LONG_LONG, &results[0], 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; ++i) {
            std::cout << "Proces " << i << ": " << results[i] << std::endl;
        }
    }

    if (rank == 0) {
        std::cout << "\nGeneratorul liniar congruențial:\n";
    }

    unsigned long long a = 1664525;
    unsigned long long c = 1013904223;
    unsigned long long m = 4294967296; // 2^32
    unsigned long long linearCongruentialResult = linearCongruentialRandom(seed, a, c, m, iterations);
    MPI_Gather(&linearCongruentialResult, 1, MPI_UNSIGNED_LONG_LONG, &results[0], 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; ++i) {
            std::cout << "Proces " << i << ": " << results[i] << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}