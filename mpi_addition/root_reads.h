#pragma once
#include "mpi.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>

#define MAX_SIZE_TAG 0
#define PASS_CARRY_TAG 1

static void root_reads() // NOLINT(bugprone-exception-escape)
{
	MPI_Init(nullptr, nullptr);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	const auto filename_number1 = "number1.txt";
	const auto filename_number2 = "number2.txt";

	std::ifstream stream;
	auto max_size = 0;
	double start_time = 0;
	unsigned char* number1 = nullptr;
	unsigned char* number2 = nullptr;
	unsigned char* partial_sum = nullptr;
	auto partial_sum_size = 0;
	unsigned char* sum = nullptr;

	if (world_rank == 0)
	{
		// start measuring elapsed time

		start_time = MPI_Wtime();

		// compare number sizes to get max size

		stream.open(filename_number1, std::ios::binary);
		stream.seekg(0, std::ifstream::end);
		const int size1 = stream.tellg();
		stream.close();

		stream.open(filename_number2, std::ios::binary);
		stream.seekg(0, std::ifstream::end);
		const int size2 = stream.tellg();
		stream.close();

		max_size = std::max(size1, size2);


		// send max size to the other processes

		for (auto i = 1; i < world_size; i++)
			MPI_Send(&max_size, 1, MPI_INT, i, MAX_SIZE_TAG, MPI_COMM_WORLD);

		// read first number

		stream.open(filename_number1, std::ios::binary);

		number1 = new unsigned char[max_size]();
		auto i = size1;

		char c;
		while (stream.peek() != EOF)
		{
			stream.get(c);
			number1[--i] = c - '0';
		}
		stream.close();


		// read second number

		stream.open(filename_number2, std::ios::binary);

		number2 = new unsigned char[max_size]();
		i = size2;

		while (stream.peek() != EOF)
		{
			stream.get(c);
			number2[--i] = c - '0';
		}
		stream.close();
	}
	else
	{
		// receive max_size from root 

		MPI_Recv(&max_size, 1, MPI_INT, 0, MAX_SIZE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// compute balanced loads

	const auto remainder = max_size % (world_size - 1);
	auto sc_size = world_rank == 0 ? 0 : max_size / (world_size - 1) + (remainder >= world_rank);

	auto* sc_number1 = new unsigned char[sc_size];
	auto* sc_number2 = new unsigned char[sc_size];

	auto* counts = new int [world_size];
	auto* displs = new int[world_size];

	MPI_Gather(&sc_size, 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	for (auto i = 0; i < world_size; i++)
		displs[i] = i > 0 ? displs[i - 1] + counts[i - 1] : 0;


	// scatter the numbers

	MPI_Scatterv(number1, counts, displs, MPI_UNSIGNED_CHAR, sc_number1, sc_size, MPI_UNSIGNED_CHAR, 0,
	             MPI_COMM_WORLD);
	MPI_Scatterv(number2, counts, displs, MPI_UNSIGNED_CHAR, sc_number2, sc_size, MPI_UNSIGNED_CHAR, 0,
	             MPI_COMM_WORLD);


	if (world_rank != 0)
	{
		// compute partial sum from the 2 intervals

		partial_sum_size = sc_size + (world_rank == world_size - 1);
		partial_sum = new unsigned char[partial_sum_size]();

		auto carry = 0, digit_sum = 0;

		for (auto i = 0; i < sc_size; i++)
		{
			digit_sum = sc_number1[i] + sc_number2[i] + carry;
			partial_sum[i] = digit_sum % 10;
			carry = digit_sum / 10;
		}


		// get carry from previous process and add it to the sum (except the first one)

		if (world_rank != 1)
		{
			auto recv_carry = 0;
			MPI_Recv(&recv_carry, 1, MPI_INT, world_rank - 1, PASS_CARRY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			if (recv_carry != 0)
			{
				for (auto i = 0; i < partial_sum_size; i++)
				{
					digit_sum = partial_sum[i] + recv_carry;
					partial_sum[i] = digit_sum % 10;
					recv_carry = digit_sum / 10;
					if (recv_carry == 0)
						break;
				}

				carry += recv_carry;
			}
		}

		// send left-over carry to next process (last process just adds it to the sum)

		if (world_rank != world_size - 1)
			MPI_Send(&carry, 1, MPI_INT, world_rank + 1, PASS_CARRY_TAG, MPI_COMM_WORLD);
	}

	// gather all partial sums into the root process

	counts[world_size - 1]++;

	if (world_rank == 0)
		sum = new unsigned char[displs[world_size - 1] + counts[world_size - 1]];

	MPI_Gatherv(partial_sum, partial_sum_size, MPI_UNSIGNED_CHAR, sum, counts, displs, MPI_UNSIGNED_CHAR, 0,
	            MPI_COMM_WORLD);


	// get results in root process

	if (world_rank == 0)
	{
		// measure and show elapsed time

		auto end_time = MPI_Wtime();
		auto elapsed_time = end_time - start_time;

		printf("%f", elapsed_time);


		// save sum to output file

		std::string result;
		for (auto i = max_size; i >= 0; i--)
		{
			if (i == max_size && sum[i] == 0) continue;
			result += std::to_string(sum[i]);
		}

		std::ofstream ofstream;
		ofstream.open("result.txt", std::ios::trunc);
		ofstream << result;
		ofstream.close();


		// save time to output file

		ofstream.open("stats.csv", std::ios::app);
		ofstream << "root reads," << max_size << "," << elapsed_time << "," << world_size << std::endl;
		ofstream.close();
	}


	// end current process

	MPI_Finalize();
}
