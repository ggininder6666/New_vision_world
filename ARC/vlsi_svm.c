#include "embARC.h"
#include "embARC_debug.h"
#include "stdlib.h"
#include "embARC_error.h"
#include "board.h"

// parameters depend on SVM
const float support_vector[19][20] = { { 1.05,  1.31,  1.71,  1.55,  1.76,  1.49,  1.63,  1.0 ,  1.66,  1.0 , -1.68,  1.3 , -1.73,  1.3 , -1.7 ,  1.29, -0.96,  1.11, -1.21,  1.25},\
									   { 1.53,  0.89,  1.47,  0.93,  1.41,  1.06,  1.43,  1.05,  1.43,  1.05,  0.1 ,  1.2 , -0.13,  1.45, -0.32,  1.64, -0.55,  1.61, -0.88,  1.42},\
									   {-0.44,  1.34, -0.31,  1.62, -0.12,  1.54,  0.05,  1.46,  0.33,  1.35, -1.29,  0.86, -1.23,  1.15, -1.24,  1.16, -1.23,  1.16, -1.2 ,  1.12},\
									   { 0.44,  1.09,  0.63,  1.3 ,  0.85,  1.34,  1.06,  1.3 ,  1.29,  1.22, -0.08,  1.06, -0.26,  1.31, -0.46,  1.28, -0.67,  1.29, -1.05,  1.21},\
									   {-0.34,  1.28, -0.21,  1.6 ,  0.0 ,  1.62,  0.18,  1.53,  0.46,  1.41, -1.64,  1.33, -0.91,  1.29, -1.06,  1.04, -1.05,  1.2 , -1.05,  1.2 },\
									   { 1.44,  0.82,  1.43,  0.91,  1.29,  0.93,  1.21,  0.88,  1.25,  1.04, -1.32,  1.28, -1.34,  1.31, -1.2 ,  1.06, -1.18,  1.06, -1.1 ,  1.24},\
									   { 1.74,  1.48,  2.02,  1.68,  1.97,  1.64,  2.29,  1.58,  2.36,  1.44,  1.31,  1.52,  1.45,  1.64,  1.19,  1.72,  0.97,  1.71,  0.72,  1.75},\
									   { 1.48,  0.84,  1.48,  0.83,  1.5 ,  0.84,  1.44,  1.03,  1.44,  1.03, -0.39,  1.13, -0.71,  1.45, -0.93,  1.66, -1.13,  1.61, -1.38,  1.46},\
									   {-0.39,  1.21, -0.3 ,  1.55,  0.01,  1.75,  0.22,  1.71,  0.45,  1.54, -1.22,  1.24, -1.3 ,  0.94, -1.23,  0.98, -1.21,  0.86, -1.17,  1.0 },\
									   { 1.1 ,  0.85,  1.07,  0.94,  0.97,  1.06,  0.98,  1.06,  0.85,  1.06, -0.77,  1.17, -0.81,  1.04, -0.82,  1.12, -0.87,  1.14, -0.91,  1.16},\
									   { 0.7 ,  1.02,  0.71,  1.03,  0.7 ,  1.03,  0.79,  1.09,  1.08,  1.0 , -1.23,  1.07, -1.23,  1.08, -1.04,  1.05, -0.99,  1.06, -0.94,  1.1 },\
	                                   { 0.73,  1.22,  0.75,  1.07,  0.93,  1.06,  0.93,  1.19,  0.98,  0.93, -1.07,  1.26, -0.97,  1.23, -0.91,  1.2 , -0.86,  1.21, -0.8 ,  1.24},\
									   { 0.51,  0.95,  0.45,  1.07,  0.42,  1.07,  0.76,  1.1 ,  0.78,  1.12, -0.7 ,  0.43, -0.72,  0.22, -0.81,  0.15, -0.82,  0.13, -1.01,  0.29},\
									   { 0.52,  0.96,  0.49,  1.14,  0.67,  1.11,  0.77,  1.23,  0.78,  1.23, -0.6 ,  1.05, -0.6 ,  1.04, -0.56,  1.06, -0.56,  1.08, -0.63,  1.21},\
									   { 0.44,  0.76,  0.32,  0.76,  0.31,  0.76,  0.31,  0.77,  0.31,  0.78, -0.47,  0.5 , -0.61,  0.62, -0.62,  0.6 , -0.65,  0.61, -0.67,  0.6 },\
									   { 1.09,  0.79,  1.09,  0.79,  1.04,  0.92,  0.94,  1.06,  0.94,  1.06, -1.0 ,  1.25, -0.98,  1.28, -0.93,  1.25, -0.87,  1.25, -0.82,  1.25},\
									   { 0.85,  1.17,  0.85,  1.22,  0.82,  1.13,  0.82,  1.13,  0.82,  1.13, -1.2 ,  0.95, -1.21,  0.99, -1.1 ,  1.02, -1.02,  1.02, -1.0 ,  1.05},\
                                       {-1.3 , -0.47, -1.52, -0.56, -1.59, -0.42, -1.62, -0.33, -1.61, -0.25, -1.52, -0.57, -1.51, -0.55, -1.27, -0.19, -1.44, -0.21, -1.44, -0.18},\
                                       { 1.23, -0.47,  1.03, -0.26,  1.0 , -0.14,  1.0 , -0.01,  1.0 ,  0.0 ,  0.2 , -0.45,  0.2 , -0.43,  0.13, -0.59,  0.13, -0.63, -0.23, -0.71} };	

// nv
const int n_support[3] = {9, 8, 2};
// a
const float dual_coef[2][19] = { { 0.33955646, 1.0, 0.70531901, 0.0, 0.45868112, 1.0, 0.04339473, 0.56828131, 0.718098, -0.58952839, -1.0, -1.0, -0.0, -0.63851771, -0.0, -0.60528451, -1.0, -0.01189492, -0.06164192},\
						         { 0.0, 0.0, 0.0, 0.04550335, 0.0, 0.02803348, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00418591, 0.0, 0.17133139, 0.0, 0.0, -0.05695206, -0.11856524} };

// b
const float intercept[3] = {-8.95019776, -0.67424865, -0.3820745};
// cs
const float classes[3] = { 0.0, 1.0, 2.0};



int vlsi_svm_predict(float svm_test_data[20])
{
	// First: kernel => sv dot X_test
	float kvalue[19];
	int i = 0;
	int j = 0;
	float sum = 0;

	for (i = 0; i < 19; i++) {
		for (j = 0; j < 20; j++) {
			sum = sum + support_vector[i][j] * svm_test_data[j];
		}
		kvalue[i] = sum;
		sum = 0;
	}
    

	// Define the start and end index for support vectors for each class
	int n_support_start[3] = { 0, 9, 17 };
	int n_support_end[3] = { 9, 17, 19 };

	// Calculate: sum(a_p * k(x_p, x)) between every 2 classes8
	int p = 0;
	int n = 0;
	int vote[3] = { 0,0,0 };
	float decision_value[3];
	for (i = 0; i < 3; i++) {
		for (j = i + 1; j < 3; j++) {
			for (p = n_support_start[j]; p < n_support_end[j]; p++) {
				sum = sum + dual_coef[i][p] * kvalue[p];
			}
			for (p = n_support_start[i]; p < n_support_end[i]; p++) {
				sum = sum + dual_coef[j-1][p] * kvalue[p];
			}
			// Add the intercept
			sum = sum + intercept[n];
			decision_value[n] = sum;
			sum = 0;
			// Voting
			if (decision_value[n] > 0)
				vote[i] = vote[i] + 1;
			else
				vote[j] = vote[j] + 1;
			n = n+1;
		}
	}
    
	volatile int vote_max_index = 0;    //Bug with optimize
    
	for (i = 1; i < 3; i++)
    {
		if (vote[i] > vote[vote_max_index])
        {
			vote_max_index = i;
        }
    }
	
    return vote_max_index;
}

