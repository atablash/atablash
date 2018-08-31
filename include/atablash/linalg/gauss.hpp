//////////////////////////// GAUSS ELIMINATION ////////////////////////////////////////
//
// Implementation: Adam Blaszkiewicz (atablash.pl), 2011-2014
//
/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




/*


#pragma once
namespace ab{


enum class PIVOT{
	NO,
	ROWS,
	FULL
};

// forward declarations
template<class T> class Matrix;
template<class T> class Vector;

// TODO: check after modifications
// solves linear equations system using Gauss elimination with full pivoting
// cols/rows permutations are not performed implicitly in memory - using wPerm and kPerm arrays
//
// IN 'matrix' - square matrix to solve
// equations are of form: matrix[ith row] = result[i]
// result overwrites 'wynik' array
// RETURNS: solution dimension or -1 if no solution
// 2011-12-07
template<PIVOT pivot = PIVOT::ROWS, class FLOAT = double, class MAT = Matrix<FLOAT>, class VEC = Vector<FLOAT>>
int LinearSolveGaussFullPivoting(const MAT& matrix, VEC& result, const FLOAT& epsilon = 1.0e-10)
{
	A(matrix.height() == matrix.width() && matrix.width() == result.height());
	int n = result.height();

	int *data, *wPerm, *kPerm;
	if(pivot == PIVOT::ROWS){
		data = new int[2*n];
		wPerm = data + n;
	}
	if(pivot == PIVOT::FULL){
		data = new int[3*n];
		wPerm = data + n;
		kPerm = data + 2*n;
	}

	for(int i=0; i<n; ++i){
		if(pivot == PIVOT::ROWS || pivot == PIVOT::FULL) wPerm[i] = i;
		if(pivot == PIVOT::FULL) kPerm[i] = i;
	}

	for(int krok=0; krok<n-1; ++krok){
		FLOAT vmax = -1.0;
		int mi,mj;

		// search for maximum element
		for(int i=krok; i<n; ++i){
			if(pivot == PIVOT::NO)break;
			for(int j=krok; j<n; ++j){
				FLOAT nval = abs(matrix[n*wPerm[i] + kPerm[j]]);
				if(nval > vmax){
					vmax = nval;
					mi = i;
					if(pivot == PIVOT::FULL) mj = j;
				}
				if(pivot == PIVOT::FULL) break;
			}
		}

		int ki=krok, kj=krok;
		if(pivot == PIVOT::FULL || pivot == PIVOT::ROWS) ki = wPerm[mi];
		if(pivot == PIVOT::FULL) kj = kPerm[mj];

		// rows and cols swap
		if(pivot == PIVOT::FULL || pivot == PIVOT::ROWS){
			wPerm[mi] = wPerm[krok];
			wPerm[krok] = ki;
		}
		if(pivot == PIVOT::FULL){
			kPerm[mj] = kPerm[krok];
			kPerm[krok] = kj;
		}

		FLOAT *wierszKrok = &matrix(ki);
		for(int i=krok+1; i<n; ++i){
			int pi=i, pj=j;
			if(pivot == PIVOT::FULL || PIVOT == PIVOT::ROWS) pi = wPerm[i];
			if(pivot == PIVOT::FULL) pj = kPerm[j];

			VEC& wiersz = &matrix(pi);
			const FLOAT& pierwszy = wiersz(kj);
			FLOAT r = -pierwszy/wierszKrok(kj);

			for(int j=krok+1; j<n; ++j){
				wiersz(pj) += r*wierszKrok(pj);
			}
			wynik[pi] += r*wynik(ki);
		}
	}

	int wymiar = 0;

	// upper triangular matrix by now
	for(int krok = n-1; krok>0; --krok){
		int ki=krok, kj=krok;
		if(pivot == FULL || PIVOT == ROWS) ki = wPerm[krok];
		if(pivot == FULL) kj = kPerm[krok];

		FLOAT& wk = wynik(ki);
		FLOAT glowny = matrix(ki, kj);
		if(abs(glowny) < epsilon){
			// the system has no solution
			if(abs(wk) > epsilon){
				delete[] data;
				return -1;
			}

			// we choose one of the solutions by setting one of the coordinates to 1.0
			glowny = 1.0;
			wk = 1.0;
			++wymiar;
		}
		//cout<<"wk: "<<wk<<"   glowny: "<<glowny<<endl;
		for(int i=0; i<krok; ++i){
			int pi=i;
			if(pivot == FULL || PIVOT == ROWS) pi = wPerm[i];

			wynik[pi] -= wk * macierz(pi, kj) / glowny;
		}
		wk /= glowny;
	}
	int ii=0, jj=0;
	if(pivot == FULL || PIVOT == ROWS) ii = wPerm[0];
	if(pivot == FULL) jj = kPerm[0];
	wynik(ii) /= matrix(ii, jj);

	// undo rows and cols permutation
	if(pivot == FULL || pivot == ROWS){
		FLOAT *wynik2 = data;
		for(int i=0; i<n; ++i){
			int ii=i, jj=i;
			if(pivot == FULL || PIVOT == ROWS) ii = kPerm[i];
			if(pivot == FULL) jj = wPerm[j];

			wynik2[ii] = wynik[jj];
		}
		memcpy(wynik,wynik2,sizeof(FLOAT)*n);
	}

	delete[] data;

	return wymiar;
}




// solves linear equations system using Gauss elimination with rows pivoting
// row permutations are not performed implicite in memory - we use wPerm array
// RETURNS: solution dimension
// nxn matrix 'macierz' is stored row-by-row
// equations are of form: macierz[ith row] = wynik[i]
// result overwrites 'wynik' array
// 2011-12-07
int LinearSolveGaussPivoting(FLOAT *macierz, FLOAT *wynik, int n)
{
	int *wPerm = new int[n];

	for(int i=0; i<n; ++i)
	{
		wPerm[i] = i;
	}

	for(int krok=0; krok<n-1; ++krok)
	{
		FLOAT vmax = -1.0;
		int mi;

		// looking for maximum element
		for(int i=krok; i<n; ++i)
		{
			FLOAT nval = abs(macierz[n*wPerm[i]]);
			if(nval > vmax)
			{
				vmax = nval;
				mi = i;
			}
		}


		int ki = wPerm[mi];

		// rows swap
		wPerm[mi] = wPerm[krok];
		wPerm[krok] = ki;


		FLOAT *wierszKrok = &macierz[n*ki];

		for(int i=krok+1; i<n; ++i)
		{
			FLOAT *wiersz = &macierz[n*wPerm[i]];
			const FLOAT& pierwszy = wiersz[krok];
			FLOAT r = -pierwszy/wierszKrok[krok];

			for(int j=krok+1; j<n; ++j)
			{
				wiersz[j] += r*wierszKrok[j];
			}
			wynik[wPerm[i]] += r*wynik[ki];
		}
	}

	int wymiar = 0;

	// by now we have an upper triangular matrix
	for(int krok = n-1; krok>0; --krok)
	{
		FLOAT& wk = wynik[wPerm[krok]];
		FLOAT glowny = macierz[n*wPerm[krok] + krok];
		if(abs(glowny) < 1.0e-10)
		{
			// the system has no solutions
			if(abs(wk) > 1.0e-10)
			{
				delete[] wPerm;
				return -1;
			}

			// choose one of the solutions by setting one of the coordinates to 1.0
			glowny = 1.0;
			wk = 1.0;
			++wymiar;
		}

		for(int i=0; i<krok; ++i)
		{
			wynik[wPerm[i]] -= wk * macierz[n*wPerm[i] + krok]/glowny;
		}
		wk /= glowny;
	}
	wynik[wPerm[0]] /= macierz[n*wPerm[0] + 0];

	// undo cols permutation
	FLOAT *wynik2 = new FLOAT[n];
	for(int i=0; i<n; ++i)
	{
		wynik2[i] = wynik[wPerm[i]];
	}

	memcpy(wynik,wynik2,sizeof(FLOAT)*n);

	delete[] wPerm;
	delete[] wynik2;

	return wymiar;
}





}

*/


