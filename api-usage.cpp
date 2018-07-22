/**
 * api-usage.cpp
 *
 * Driver class with a simple example of how to instantiate and use the BRKGA API.
 * See SampleDecoder.h for details on the decoder's implementation.
 *
 * Created on : Nov 17, 2011 by rtoso
 * Authors    : Rodrigo Franco Toso <rtoso@cs.rutgers.edu>
 *              Mauricio G.C. Resende <mgcr@research.att.com>
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018
 * Rodrigo Franco Toso (rfrancotoso@gmail.com) and
 * Mauricio G.C. Resende
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <iostream>
#include <algorithm>
#include "BRKGA.h"
#include "MTRand.h"
#include "SampleDecoder.h"
#include <ctime>


int main_de_taubateh(char nameTest[], std::vector<int> A, int V, int expectedSolution) {

	const unsigned n = A.size();		// size of chromosomes
	const unsigned p = 100;		// size of population
	const double pe = 0.10;		// fraction of population to be the elite-set
	const double pm = 0.30;		// fraction of population to be replaced by mutants
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 3;		// number of independent populations
	const unsigned MAXT = 1;	// number of threads for parallel decoding


	SampleDecoder decoder(A, V);				// initialize the decoder

	const long unsigned rngSeed = 0;	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator

	// initialize the BRKGA-based heuristic
	BRKGA< SampleDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

	unsigned generation = 0;		// current generation
	const unsigned X_INTVL = 50;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 1000;	// run for 1000 gens
	std::cout << "Test Case: " << nameTest << " | Expected Solution: " << expectedSolution << std::endl;
	std::cout << "Running for " << MAX_GENS << " generations..." << std::endl;
	clock_t begin = clock();
	do {
		algorithm.evolve();	// evolve the population for one generation

		if((++generation) % X_INTVL == 0) {
			algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
		}

            std::cout << "G: " << generation <<" Current solution = " << algorithm.getBestFitness() << std::endl;
	} while (generation < MAX_GENS);

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	// print the fitness of the top 10 individuals of each population:
	std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
	const unsigned bound = std::min(p, unsigned(10));	// makes sure we have 10 individuals
	/*for(unsigned i = 0; i < K; ++i) {
		std::cout << "Population #" << i << ":" << std::endl;
		for(unsigned j = 0; j < bound; ++j) {
			std::cout << "\t" << j << ") "
					<< algorithm.getPopulation(i).getFitness(j) << std::endl;
		}
	}*/

	std::cout << "Best solution found has objective value = " << algorithm.getBestFitness() << " at " <<  elapsed_secs << " seconds :)"<< std::endl;

	system("pause");

	return 0;
}
int main(int argc, char *argv[]) {
	int qtd, V, N, bestSolution;
	char nameTest[50];
	FILE *arq;

	if (argc > 1) {
		arq = fopen(argv[1], "r");
		if (arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
		else {
			fscanf(arq, "%d\n", &qtd);
			for (int i = 0; i < qtd; i++) {
				fscanf(arq, "%s\n", nameTest);
				fscanf(arq, "%d %d %d", &V, &N, &bestSolution);
				std::vector<int> A(N);
				for (int j = 0; j < N; j++) {
					fscanf(arq, "%d\n", &A[j]);
				}
				main_de_taubateh(nameTest, A, V, bestSolution);
				//system("pause");
			}

			fclose(arq);
		}



		return 0;
	}


}
