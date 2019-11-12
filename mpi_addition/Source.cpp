#include "mpi.h"
#include <iostream>
#include <fstream>
#include <vector>

#define MAX_SIZE_TAG 0

int main()
{
	MPI_Init(nullptr, nullptr);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	const auto filename = "numbers.txt";

	if (world_rank == 0)
	{
		auto number_size = 0, max_size = 0;
		char c;

		std::ifstream stream(filename, std::ifstream::binary);

		// read byte by byte and compute max line length
		while (stream.get(c))
		{
			if (c == '\r')
				continue;
			if (c == '\n')
			{
				if (max_size < number_size)
					max_size = number_size;
				number_size = 0;
				continue;
			}
			number_size++;
		}

		stream.close();

		// send max line length (max number)
		for (auto i = 1; i < world_size; i++)
			MPI_Send(&max_size, 1, MPI_INT, i, MAX_SIZE_TAG, MPI_COMM_WORLD);
	}
	else
	{
		int max_size;
		char c;
		std::vector<unsigned char> number1, number2;

		// receive number of max size from root process 
		MPI_Recv(&max_size, 1, MPI_INT, 0, MAX_SIZE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// compute start and finish intervals based on rank and max size
		const auto interval_length = max_size / (world_size - 1);
		const auto remainder = max_size % (world_size - 1);

		const auto start = interval_length * (world_rank - 1)
			+ remainder - (world_rank <= remainder) * (remainder - world_rank + 1);
		const auto finish = interval_length * world_rank
			+ remainder - (world_rank <= remainder) * (remainder - world_rank);

		std::ifstream stream(filename, std::ifstream::binary);

		// seek to start of first line
		stream.seekg(start, std::ifstream::beg);


		// read first number
		for (auto i = start; i < finish; i++)
		{
			stream.get(c);

			if (c == '\r')
			{
				// fill with zeroes if necessary
				for (auto j = i; j < finish; j++)
				{
					number1.push_back(0);
				}
				break;
			}

			number1.push_back(c - '0');
		}

		// skip line and seek to start of next line
		stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		stream.seekg(start, std::ifstream::cur);


		// read second number
		for (auto i = start; i < finish; i++)
		{
			stream.get(c);

			if (c == '\r')
			{
				// fill with zeroes if necessary
				for (auto j = i; j < finish; j++)
					number2.push_back(0);
				break;
			}

			number2.push_back(c - '0');
		}

		stream.close();
	}
	MPI_Finalize();
}
