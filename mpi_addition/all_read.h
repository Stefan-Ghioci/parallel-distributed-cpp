#pragma once
#include "mpi.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>

#define MAX_SIZE_TAG 0
#define PASS_CARRY_TAG 1

static void all_read()
{
	MPI_Init(nullptr, nullptr);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	const auto filename_number1 = "number1.txt";
	const auto filename_number2 = "number2.txt";

	std::ifstream stream;
	unsigned char* sum = nullptr;
	int* counts = nullptr;
	unsigned char* partial_sum = nullptr;
	auto partial_sum_size = 0;
	auto max_size = 0;
	double start_time = 0;

	if (world_rank == 0)
	{
		// start measuring elapsed time

		start_time = MPI_Wtime();


		// compare number sizes to get sum max_size 

		stream.open(filename_number1, std::ios::binary);
		stream.seekg(0, std::ifstream::end);
		const int size1 = stream.tellg();
		stream.close();

		stream.open(filename_number2, std::ios::binary);
		stream.seekg(0, std::ifstream::end);
		const int size2 = stream.tellg();
		stream.close();

		max_size = std::max(size1, size2);

		// send max_size to the other processes 

		for (auto i = 1; i < world_size; i++)
			MPI_Send(&max_size, 1, MPI_INT, i, MAX_SIZE_TAG, MPI_COMM_WORLD);
	}
	else
	{
		// receive max_size from root 

		MPI_Recv(&max_size, 1, MPI_INT, 0, MAX_SIZE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		// calculate interval bounds (balanced loads)

		const auto interval_length = max_size / (world_size - 1);
		const auto remainder = max_size % (world_size - 1);

		const auto start = interval_length * (world_rank - 1)
			+ remainder - (world_rank <= remainder) * (remainder - world_rank + 1);
		const auto finish = interval_length * world_rank
			+ remainder - (world_rank <= remainder) * (remainder - world_rank);


		// read interval from first number

		auto size = finish - start;
		auto* number1 = new unsigned char[size]();
		stream.open(filename_number1, std::ios::binary | std::ios::ate);

		stream.seekg(-finish, std::ifstream::cur);

		char c;
		auto i = size;

		if (stream.peek() == EOF)
		{
			stream.clear();
			stream.seekg(-start - 1, std::ifstream::end);
			if (stream.peek() == EOF)
				i = 0;
			else
			{
				i = stream.tellg();
				i++;
				stream.seekg(0, std::ifstream::beg);
			}
		}

		while (stream.peek() != EOF && i != 0)
		{
			stream.get(c);
			number1[--i] = c - '0';
		}
		stream.close();


		// read interval from second number

		auto* number2 = new unsigned char[size]();
		stream.open(filename_number2, std::ios::binary | std::ios::ate);

		stream.seekg(-finish, std::ifstream::cur);

		i = size;

		if (stream.peek() == EOF)
		{
			stream.clear();
			stream.seekg(-start - 1, std::ifstream::end);
			if (stream.peek() == EOF)
				i = 0;
			else
			{
				i = stream.tellg();
				i++;
				stream.seekg(0, std::ifstream::beg);
			}
		}

		while (stream.peek() != EOF && i != 0)
		{
			stream.get(c);
			number2[--i] = c - '0';
		}
		stream.close();


		// compute partial sum from the 2 intervals

		partial_sum_size = size + (world_rank == world_size - 1);
		partial_sum = new unsigned char[partial_sum_size]();

		auto carry = 0, digit_sum = 0;

		for (i = 0; i < size; i++)
		{
			digit_sum = number1[i] + number2[i] + carry;
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
				for (i = 0; i < partial_sum_size; i++)
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

	// gather all partial sum sizes into the root process, then gather the partial sums 

	counts = new int[world_size];

	MPI_Gather(&partial_sum_size, 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	auto* disps = new int[world_size];

	for (auto i = 0; i < world_size; i++)
		disps[i] = i > 0 ? disps[i - 1] + counts[i - 1] : 0;

	if (world_rank == 0)
		sum = new unsigned char[disps[world_size - 1] + counts[world_size - 1]];

	MPI_Gatherv(partial_sum, partial_sum_size, MPI_UNSIGNED_CHAR, sum, counts, disps, MPI_UNSIGNED_CHAR, 0,
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
		ofstream << "all read," << max_size << "," << elapsed_time << "," << world_size << std::endl;
		ofstream.close();
	}


	// end current process

	MPI_Finalize();
}
