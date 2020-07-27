#include <stdlib.h>
#include <stdio.h>

void transpose(double **matrix, double **newMatrix, int numRows, int numColumns)
{

	//transpose values of the matrix
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			newMatrix[j][i] = matrix[i][j];
		}
	}

}

void inverse(double **inputMatrix, double **outputMatrix, int n)
{

	//first create augmented matrix
	double augmentedMatrix[n][2 * n];

	//initialize all of the values of the augmented matrix to 0
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 2 * n; j++)
		{
			augmentedMatrix[i][j] = 0;
		}
	}

	//copy the input matrix into the left side of the augmented matrix
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			augmentedMatrix[i][j] = inputMatrix[i][j];
		}
	}

	//put the identity matrix on the right side of the augmented matrix
	for (int i = 0; i < n; i++)
	{
		for (int j = n; j < 2 * n; j++)
		{

			//if on the diagonal of the right side of the augmented matrix
			if ((j - n) == i)
			{

				augmentedMatrix[i][j] = 1;
			}
			else
			{

				augmentedMatrix[i][j] = 0;
			}
		}
	}


	double multiple;
	double constant;
	//PERFORM INVERSION PROCESS
	//rows in the augmented matrix
	for (int i = 0; i < n; i++)
	{
		//columns in the augmented matrix
		for (int j = 0; j < 2 * n; j++)
		{

			//if at the diagonal of the matrix on the lefthand side
			if (i == j)
			{
				//find the coefficient 
				multiple = augmentedMatrix[i][j];

				if (multiple == 0) {
					printf("\nDivide by 0 ERROR\n");
				}

				//divide the lefthand and righthand side by this coefficient and the coefficient also (to get 1)
				for (int x = 0; x < 2 * n; x++)
				{
					
					augmentedMatrix[i][x] /= multiple;

				}

				//now subtract rows above and below by their coefficient
				for (int y = 0; y < n; y++)
				{
					//no need to subtract a row from itself
					if (y == i)
					{
						continue;
					}
					else
					{
						constant = augmentedMatrix[y][j];

						if (constant == 0) {
							continue;
						}
						else {
						
						//can start at j since we already did all the values to the left
						for (int z = j; z < 2 * n; z++)
						{
							augmentedMatrix[y][z] -= constant * augmentedMatrix[i][z];

						}

						}
					
					}
				}
			}
		}
	}


	//set the output matrix equal to the right hand side
	for (int i = 0; i < n; i++)
	{
		for (int j = n; j < 2 * n; j++)
		{
			outputMatrix[i][j - n] = augmentedMatrix[i][j];
		}
	}

}

void multiply(double **matrix1, int rows1, int col1, double **matrix2, int rows2, int col2, double **result)
{

	//if the num of colunmns of the first matrix are not equal to the num of rows in second matrix,
	//matrix multiplication is not possible
	if (col1 != rows2)
	{
		printf("Invalid Matrix Multiplication Input\n");
		return;
	}

	//first initialize result matrix to have all 0 values (remembering common error from class)
	for (int i = 0; i < rows1; i++)
	{
		for (int j = 0; j < col2; j++)
		{
			result[i][j] = 0;
		}
	}

	//rows of output matrix
	for (int i = 0; i < rows1; i++)
	{
		//columns of output matrix
		for (int j = 0; j < col2; j++)
		{
			//columns of matrix1 and rows of matrix2 (iterating through the part that is being added and multiplied)
			int k = 0;
			while (k < col1)
			{
				result[i][j] = result[i][j] + (matrix1[i][k] * matrix2[k][j]);
				k++;
			}
		}
	}
	
}

int main(int argc, char **argv)
{
	//global values
	int attributes;
	int trainingNum;
	int testNum;
	//get necessary information
	char *trainingData = argv[1];
	char *testData = argv[2];
	FILE *trainingFile = fopen(trainingData, "r");

	//get the number of attributes from the training file
	fscanf(trainingFile, "%d\n", &attributes);
	//get the number of attributes from the training file
	fscanf(trainingFile, "%d\n", &trainingNum);

	//create the 2d array on the heap that will hold the training data

	double **trainData = (double **)malloc((sizeof(double *)) * trainingNum);

	for (int i = 0; i < trainingNum; i++)
	{
		trainData[i] = (double *)malloc(sizeof(double) * (attributes + 1));
	}

	//create the array that will store the prices
	double **prices = (double **)malloc(trainingNum * sizeof(double *));

	for (int i = 0; i < trainingNum; i++)
	{
		prices[i] = (double *)malloc(sizeof(double));
	}

	//2d array to scan the necessary information
	for (int i = 0; i < trainingNum; i++)
	{
		//copy the prices into the prices array
		fscanf(trainingFile, "%lf ,", &prices[i][0]);
		//first column is all 1s
		trainData[i][0] = 1;

		for (int j = 1; j < attributes; j++)
		{

			fscanf(trainingFile, "%lf ,", &trainData[i][j]);
			//printf("%lf\t", trainData[i][j]);
		}
		//make sure to get the last value
		fscanf(trainingFile, "%lf\n", &trainData[i][attributes]);
		//printf("%lf\n", trainData[i][attributes-1]);
	}


	//close one file stream
	fclose(trainingFile);

	//create the transposed Matrix
	double **transposeMatrix = (double **)malloc((sizeof(double *)) * (attributes + 1));

	for (int i = 0; i < attributes + 1; i++)
	{
		transposeMatrix[i] = (double *)malloc(sizeof(double) * (trainingNum));
	}

	//create the transpose of the matrix
	transpose(trainData, transposeMatrix, trainingNum, attributes + 1);

	//create the matrix  equivalent to the regular times the transposed (N x N matrix)
	double **matrixTimesTranspose = (double **)malloc((sizeof(double *)) * attributes + 1);

	for (int i = 0; i < attributes + 1; i++)
	{
		matrixTimesTranspose[i] = (double *)malloc(sizeof(double) * (attributes + 1));
	}

	//multiply the transposedMatrix by the original matrix and set it equal to matrixTimesTranspose
	multiply(transposeMatrix, attributes + 1, trainingNum, trainData, trainingNum, attributes + 1, matrixTimesTranspose);

	//create the inverse of the matrix times tranposed
	double **inverseMatrix = (double **)malloc((sizeof(double *)) * attributes + 1);

	for (int i = 0; i < attributes + 1; i++)
	{
		inverseMatrix[i] = (double *)malloc(sizeof(double) * (attributes + 1));
	}

	//find the inverse of the Transposed Matrix times the original
	inverse(matrixTimesTranspose, inverseMatrix, attributes + 1);

	//create the matrix that is equivalent to the inverse times the transposed
	double **inverseTimesTransposed = (double **)malloc((sizeof(double *)) * attributes + 1);

	for (int i = 0; i < attributes + 1; i++)
	{
		inverseTimesTransposed[i] = (double *)malloc(sizeof(double) * (trainingNum));
	}

	multiply(inverseMatrix, attributes + 1, attributes + 1, transposeMatrix, attributes + 1, trainingNum, inverseTimesTransposed);

	//create the array that will store the final weights
	double **finalWeights = (double **)malloc((attributes + 1) * sizeof(double *));

	for (int i = 0; i < attributes + 1; i++)
	{
		finalWeights[i] = (double *)malloc(sizeof(double));
	}

	//calculate the final weights
	multiply(inverseTimesTransposed, attributes + 1, trainingNum, prices, trainingNum, 1, finalWeights);

	FILE *testFile = fopen(testData, "r");

	//get the number of test data points
	fscanf(testFile, "%d\n", &testNum);

	//allocate space on the heap for the testing data
	double **test = (double **)malloc((sizeof(double *)) * testNum);

	for (int i = 0; i < testNum; i++)
	{
		test[i] = (double *)malloc(sizeof(double) * (attributes + 1));
	}

	//2d array to scan the necessary information
	for (int i = 0; i < testNum; i++)
	{
		//first column of the test data is all 1
		test[i][0] = 1;

		for (int j = 1; j < attributes; j++)
		{
			fscanf(testFile, "%lf ,", &test[i][j]);
			//printf("%lf\t", test[i][j]);
		}
		//make sure to get the last value
		fscanf(testFile, "%lf\n", &test[i][attributes]);
		//printf("%lf\n", test[i][attributes - 1]);
	}

	//close the file streams
	fclose(testFile);

	//create the array that will store the prices
	double **output = (double **)malloc(testNum * sizeof(double *));

	for (int i = 0; i < testNum; i++)
	{
		output[i] = (double *)malloc(sizeof(double));
	}

	multiply(test, testNum, attributes + 1, finalWeights, attributes + 1, 1, output);

	for (int i = 0; i < testNum; i++)
	{
		printf("%0.0lf\n", output[i][0]);
	}

	//freeing data from heap
	for (int i = 0; i < trainingNum; i++)
	{
		free(trainData[i]);
	}
	free(trainData);
	for (int i = 0; i < testNum; i++)
	{
		free(test[i]);
	}
	free(test);
	for (int i = 0; i < attributes + 1; i++)
	{
		free(transposeMatrix[i]);
	}
	free(transposeMatrix);
	for (int i = 0; i < attributes + 1; i++)
	{
		free(matrixTimesTranspose[i]);
	}
	free(matrixTimesTranspose);
	for (int i = 0; i < attributes + 1; i++)
	{
		free(inverseMatrix[i]);
	}
	free(inverseMatrix);
	for (int i = 0; i < attributes + 1; i++)
	{
		free(inverseTimesTransposed[i]);
	}
	free(inverseTimesTransposed);

	for (int i = 0; i < trainingNum; i++)
	{
		free(prices[i]);
	}
	free(prices);
	for (int i = 0; i < attributes + 1; i++)
	{
		free(finalWeights[i]);
	}
	free(finalWeights);
	for (int i = 0; i < testNum; i++)
	{
		free(output[i]);
	}
	free(output);

	//if everything went well
	return 0;
}
