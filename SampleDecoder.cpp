/*
 * SampleDecoder.cpp
 *
 * For more information, see SampleDecoder.h
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

#include "SampleDecoder.h"
#include <iostream>
#include <math.h>
#include<limits>

std::vector<int> matMul(std::vector<int> A, int** X) {
	std::vector<int> Out(A.size());
	for (int j = 0; j < A.size(); j++) {
		Out[j] = 0;
		for (int i = 0; i < A.size(); i++) {
			Out[j] += A[i] * X[i][j];
		}
	}

	return Out;
}

SampleDecoder::SampleDecoder(std::vector<int> A, int V) {
	SampleDecoder::A = A;
	SampleDecoder::V = V;
}

SampleDecoder::~SampleDecoder() { }

// Runs in O(n \log n):
double SampleDecoder::decode(const std::vector< double >& chromosome) const {
	double myFitness = 0.0;
	int lenA = A.size();

	/*for (int i = 0; i < chromosome.size(); i++) std::cout << chromosome[i] << " ";
	std::cout << std::endl;

	system("pause");
	*/
	myFitness = (double)lenA;

	int **X = (int **)malloc(sizeof(int*) * lenA);
	if (X != NULL) {
		for (int i = 0; i < lenA; i++) {
			X[i] = (int *)malloc(sizeof(int) * lenA);
			for (int j = 0; j < lenA; j++) X[i][j] = 0;
		}

		for (int i = 0; i < lenA; i++) {
			int indice = (int)round((lenA - 1) * chromosome[i]);
			X[i][indice] = 1;
		}
	}

	/*
	for (int i = 0; i < lenA; i++) {
		for (int j = 0; j < lenA; j++) {
			std::cout << X[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/

	/*
	for (int i = 0; i < A.size(); i++) {
		std::cout << A[i] << " ";
	}
	std::cout << std::endl;*/



	//system("pause");

	std::vector<int> AX = matMul(A, X);

	/*
	for (int i = 0; i < AX.size(); i++) {
		std::cout << AX[i] << " ";
	}
	std::cout << std::endl;*/

	bool tudoCabe = true;
	for (int i = 0; i < AX.size(); i++) {
		if (AX[i] > V) {
			tudoCabe = false;
			break;
		}
	}

	if (tudoCabe) {
		myFitness = 0;
		for (int i = 0; i < lenA; i++) {
			bool has1 = false;
			for (int j = 0; j < lenA; j++) {
				if (X[j][i] == 1) {
					has1 = true;
					break;
				}
			}
			if (has1) myFitness += 1.0;
		}
	}
	else {
		myFitness = std::numeric_limits<double>::max();
	}

	//std::cout << myFitness << std::endl;
	//system("pause");

	for (int i = 0; i < lenA; i++) free(X[i]);
	free(X);

	// Return the fitness:
	return myFitness;
}


