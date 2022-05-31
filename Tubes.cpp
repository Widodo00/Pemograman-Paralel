/*
	Compile: mpic++ Tubes.cpp -o Tubes
	Run    : 1. mpiexec -n 4 ./Tubes
			 2. mpiexec -np 4 ./Tubes
			 3. mpirun --n 4 ./Tubes
			 4. mpirun --np 4 ./Tubes
*/

#include <iostream>
#include <math.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>

using namespace std;

int main()
{
	int ID, tahun, P = 4, i;
	float kupon, diskonto, hkupon, nominal, sum = 0, duration;
	struct timeval t0, t1;

	MPI_Status status;

	gettimeofday(&t0, NULL);

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &ID);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	if (ID == 0)
	{
		kupon = 0.08;
		diskonto = 0.07;
		nominal = 1000;
		tahun = 8;
		hkupon = kupon * nominal;
	}
	MPI_Bcast(&hkupon, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&diskonto, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&tahun, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&nominal, 1, MPI_INT, 0, MPI_COMM_WORLD);

	float local[tahun / P], data[tahun], pv[tahun / P], nilai[tahun / P];

	if (ID == 0)
	{
		for (i = 0; i < tahun; i++)
		{
			data[i] = i + 1;
		}
	}

	MPI_Scatter(&data, tahun / P, MPI_INT, &local, tahun / P, MPI_INT, 0, MPI_COMM_WORLD);

	for (i = 0; i < tahun / P; i++)
	{
		pv[i] = 1 / pow((diskonto + 1), local[i]);
		if (ID == 3 && i == (tahun / P) - 1)
		{
			nilai[i] = (hkupon + nominal) * pv[i];
		}
		else
		{
			nilai[i] = hkupon * pv[i];
		}
		local[i] = nilai[i];
	}

	MPI_Gather(&local, tahun / P, MPI_INT, &data, tahun / P, MPI_INT, 0, MPI_COMM_WORLD);

	if (ID == 0)
	{
		cout << "Nilai Obigasi: ";

		for (i = 0; i < tahun; i++)
			sum += data[i];
		cout << sum << endl;
	}

	MPI_Finalize();

	gettimeofday(&t1, NULL);

	if (ID == 0)
	{
		duration = ((float)(t1.tv_usec - t0.tv_usec) / 100000000) + (float)(t1.tv_sec - t0.tv_sec);

		cout << "Duration: " << duration << " s" << endl;
	}

	return 0;
}
