#include <iostream>
#include <opencv2\opencv.hpp>
#include<algorithm>
#include <vector>
using namespace cv;
using namespace std;

int** Kmean(Mat&);

void mouse(int event, int x, int y, int flags, void* params) {
	Mat* image = reinterpret_cast<Mat*>(params);

	switch (event) {
	case EVENT_LBUTTONDOWN: {
		int pixelvalue = static_cast<int>(image->at<uchar>(Point(x, y)));
		cout << "Pixel value at(" << x << "," << y << "):" << pixelvalue << endl;
	}
						  break;
	}

}
int retSmall(int a, int b, int c, int d) {
	if (a > 0 || b > 0 || c > 0 || d > 0) {
		if ((b >= a || b == 0) && (c >= a || c == 0) && (d >= a || d == 0) && (a > 0))
			return a;
		else if ((c >= b || c == 0) && (d >= b || d == 0) && (b != 0))
			return b;
		else if ((d == 0) && (c != 0))
			return c;
		else
			return d;
	}

}
int retSmall(int a, int b, int c)
{
	if (a > 0 || b > 0 || c > 0) {
		if ((b >= a || b == 0) && (c >= a || c == 0) && (a > 0))
			return a;
		else if ((c >= b || c == 0) && (b != 0))
			return b;
		else
			return c;
	}
	else return -1;
}
int retSmall(int a, int b) {
	if (a > b) {
		if (b == 0) {
			return a;
		}
		return b;
	}
	else
		return a;
}
int retSmall(int arr[], int a, int n) {
	int v = arr[0];
	for (int i = 0; i < a; i++) {
		if ((arr[i] < v && arr[i] != 0) || v == 0) v = arr[i];
	}
	if (v != 0)
		return v;
	return n;
}
int** labelling(Mat& image) {
	Mat cop;
	
	image.convertTo(cop, CV_32S, 1, 0);
	int row = image.rows;
	int col = image.cols;

	if (image.empty()) {
		cout << "Image not found." << endl;
	}
	//cout << image.rows << "*" << image.cols << endl;
	//setMouseCallback("0000", mouse, reinterpret_cast<void*>(& image));
   // imshow("OOOO",image);
   // waitKey(0);
	//cout << "bla: " << image.rows << "*" << image.cols << endl;

	int** arr = new int* [row];
	for (int i = 0; i < row; i++)
	{
		arr[i] = cop.ptr<int>(i);
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (image.at<uchar>(i, j) == 255)
				arr[i][j] = 1;
			else
				arr[i][j] = 0;
		}
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//cout << arr[i][j];
		}
		//cout << endl;
	}
	int** label = new int* [row];
	for (int i = 0; i < row; i++)
	{
		label[i] = new int[col] {0};
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//cout << label[i][j];
		}
		//cout << endl;
	}


	int check1[3] = { 0 };
	int check2[5] = { 0 };
	int check3[8] = { 0 };
	int check4[24] = { 0 };

	int current_label = 1;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (!label[i][j] && arr[i][j]) {

				//cout << "maincheck" << endl;

				if (i < 0 || i == row) { cout << "check1" << endl; break; } // out of bounds
				if (j < 0 || j == col) { cout << "check2" << endl; break; } // out of bounds
				if (label[i][j] || !arr[i][j]) { cout << "check3" << endl; break; } // already labeled or not marked with 1 
				if (i == 0 && j == 0) {
					//cout << "check4" << endl;							//First element
					label[i][j] = current_label;
					current_label++;
					//cout << i << "," << j << endl;
				}
				else if (j == 0 && i > 0) {									//left column
					if (arr[i - 1][j] == 0 && arr[i - 1][j + 1] == 0) {
						label[i][j] = current_label;
						cout << label[i][j] << endl;
						current_label++;
						//cout << i << "," << j << endl;
					}
					else if (arr[i - 1][j] != 0 || arr[i - 1][j + 1] != 0) {
						label[i][j] = retSmall(label[i - 1][j], label[i - 1][j + 1]);
						//cout << retSmall(arr[i - 1][j], arr[i - 1][j + 1]) << "hmmmmmmmmmmmmmmmmmmmmmm" << endl;
					}

					//cout << "hmm1" << endl;

				}
				//if (1) { cout << j << endl; }
				else if (i == 0 && j > 0) {								//first row
					//cout << "hmm2" << endl;
					if (arr[i][j - 1] == 0) {
						label[i][j] = current_label;
						current_label++;
						//cout << i << "," << j << endl;
					}

					else if (arr[i][j - 1] != 0) {
						label[i][j] = label[i][j - 1];
					}


				}
				else if (j == col - 1 && i > 0) {									//right column			
					if (arr[i][j - 1] == 0 && arr[i - 1][j - 1] == 0 && arr[i - 1][j] == 0) {
						label[i][j] = current_label;
						current_label++;
						//cout << i << "," << j << endl;
					}
					else if (arr[i][j - 1] != 0 || arr[i - 1][j - 1] != 0 || arr[i - 1][j] != 0) {
						//cout << retSmall(label[i][j - 1], label[i - 1][j - 1], label[i - 1][j]) << "lul" << endl;

						label[i][j] = retSmall(label[i][j - 1], label[i - 1][j - 1], label[i - 1][j]);
					}
					//cout << "hmm3" << endl;

				}
				else {			//others

					if (arr[i][j - 1] == 0 && arr[i - 1][j - 1] == 0 && arr[i - 1][j] == 0 && arr[i - 1][j + 1] == 0) {
						label[i][j] = current_label;
						current_label++;
						//cout << i << "," << j << endl;
					}
					else if (arr[i][j - 1] != 0 || arr[i - 1][j - 1] != 0 || arr[i - 1][j] != 0 || arr[i - 1][j + 1] != 0)
					{

						label[i][j] = retSmall(label[i][j - 1], label[i - 1][j - 1], label[i - 1][j], label[i - 1][j + 1]);
						//cout << "check11" << endl;
					}
					//cout << "hmm4" << endl;

				}
				//cout << retSmall(0, 0, 8, 0) << "check1" << endl;

			}

		}
	}

	for (int i = 0; i < row; i++) {
		for (int j = col - 1; j > 0; j--) {
			if (label[i][j]) {
				if (i == 0 && j == 0) {			//left top corner
					check1[0] = label[i][j + 1];
					check1[1] = label[i + 1][j + 1];
					check1[2] = label[i + 1][j];
					label[i][j] = retSmall(check1, 3, label[i][j]);

				}
				else if (i == 0 && j == col - 1) {			//right top corner
					check1[0] = label[i][j - 1];
					check1[1] = label[i + 1][j];
					check1[2] = label[i + 1][j - 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}
				else if (i == row - 1 && j == 0) {			//bot left corner
					check1[0] = label[i][j + 1];
					check1[1] = label[i - 1][j];
					check1[2] = label[i - 1][j + 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}
				else if (i == row - 1 && j == col - 1) {			//right bot corner
					check1[0] = label[i][j - 1];
					check1[1] = label[i - 1][j];
					check1[2] = label[i - 1][j - 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}

				else if (j == 0 && i > 0) {			//left column
					check2[0] = label[i - 1][j];
					check2[1] = label[i - 1][j + 1];
					check2[2] = label[i][j + 1];
					check2[3] = label[i + 1][j + 1];
					check2[4] = label[i + 1][j];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (i == 0 && j > 0) {				//first row
					check2[0] = label[i][j - 1];
					check2[1] = label[i][j + 1];
					check2[2] = label[i + 1][j];
					check2[3] = label[i + 1][j - 1];
					check2[4] = label[i + 1][j + 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (j == col - 1 && i > 0) {			//right column
					check2[0] = label[i + 1][j];
					check2[1] = label[i][j - 1];
					check2[2] = label[i - 1][j];
					check2[3] = label[i - 1][j - 1];
					check2[4] = label[i + 1][j - 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (i == row - 1) {				//last row
					check2[0] = label[i][j - 1];
					check2[1] = label[i - 1][j - 1];
					check2[2] = label[i - 1][j];
					check2[3] = label[i - 1][j + 1];
					check2[4] = label[i][j + 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if ((i>1||i<row-2)|| (j > 1 || j < col - 2)) {
					check3[0] = label[i][j - 1];
					check3[1] = label[i - 1][j - 1];
					check3[2] = label[i - 1][j];
					check3[3] = label[i - 1][j + 1];
					check3[4] = label[i][j + 1];
					check3[5] = label[i + 1][j + 1];
					check3[6] = label[i + 1][j];
					check3[7] = label[i + 1][j - 1];
					label[i][j] = retSmall(check3, 8, label[i][j]);
				}
				else {
					check4[0] = label[i][j - 1];
					check4[1] = label[i - 1][j - 1];
					check4[2] = label[i - 1][j];
					check4[3] = label[i - 1][j + 1];
					check4[4] = label[i][j + 1];
					check4[5] = label[i + 1][j + 1];
					check4[6] = label[i + 1][j];
					check4[7] = label[i + 1][j - 1];

					check4[8] = label[i][j - 2];
					check4[9] = label[i - 1][j - 2];
					check4[10] = label[i - 2][j];
					check4[11] = label[i - 1][j + 2];
					check4[12] = label[i][j + 2];
					check4[13] = label[i + 1][j + 2];
					check4[14] = label[i + 2][j+2];
					check4[15] = label[i + 1][j - 2];

					check4[16] = label[i-2][j - 2];
					check4[17] = label[i - 2][j - 1];
					check4[18] = label[i - 2][j+1];
					check4[19] = label[i -2][j + 2];
					check4[20] = label[i+2][j ];
					check4[21] = label[i + 2][j - 2];
					check4[22] = label[i + 2][j + 1];
					check4[23] = label[i + 2][j - 1];
					label[i][j] = retSmall(check4, 24, label[i][j]);

				}
			}
		}
	}

	for (int i = row - 1; i > 0; i--) {
		for (int j = col - 1; j > 0; j--) {
			if (label[i][j]) {
				if (i == 0 && j == 0) {			//left top corner
					check1[0] = label[i][j + 1];
					check1[1] = label[i + 1][j + 1];
					check1[2] = label[i + 1][j];
					label[i][j] = retSmall(check1, 3, label[i][j]);

				}
				else if (i == 0 && j == col - 1) {			//right top corner
					check1[0] = label[i][j - 1];
					check1[1] = label[i + 1][j];
					check1[2] = label[i + 1][j - 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}
				else if (i == row - 1 && j == 0) {			//bot left corner
					check1[0] = label[i][j + 1];
					check1[1] = label[i - 1][j];
					check1[2] = label[i - 1][j + 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}
				else if (i == row - 1 && j == col - 1) {			//right bot corner
					check1[0] = label[i][j - 1];
					check1[1] = label[i - 1][j];
					check1[2] = label[i - 1][j - 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}

				else if (j == 0 && i > 0) {			//left column
					check2[0] = label[i - 1][j];
					check2[1] = label[i - 1][j + 1];
					check2[2] = label[i][j + 1];
					check2[3] = label[i + 1][j + 1];
					check2[4] = label[i + 1][j];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (i == 0 && j > 0) {				//first row
					check2[0] = label[i][j - 1];
					check2[1] = label[i][j + 1];
					check2[2] = label[i + 1][j];
					check2[3] = label[i + 1][j - 1];
					check2[4] = label[i + 1][j + 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (j == col - 1 && i > 0) {			//right column
					check2[0] = label[i + 1][j];
					check2[1] = label[i][j - 1];
					check2[2] = label[i - 1][j];
					check2[3] = label[i - 1][j - 1];
					check2[4] = label[i + 1][j - 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (i == row - 1) {				//last row
					check2[0] = label[i][j - 1];
					check2[1] = label[i - 1][j - 1];
					check2[2] = label[i - 1][j];
					check2[3] = label[i - 1][j + 1];
					check2[4] = label[i][j + 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if ((i > 1 || i < row - 2) || (j > 1 || j < col - 2)) {
					check3[0] = label[i][j - 1];
					check3[1] = label[i - 1][j - 1];
					check3[2] = label[i - 1][j];
					check3[3] = label[i - 1][j + 1];
					check3[4] = label[i][j + 1];
					check3[5] = label[i + 1][j + 1];
					check3[6] = label[i + 1][j];
					check3[7] = label[i + 1][j - 1];
					label[i][j] = retSmall(check3, 8, label[i][j]);
				}
				else {
					check4[0] = label[i][j - 1];
					check4[1] = label[i - 1][j - 1];
					check4[2] = label[i - 1][j];
					check4[3] = label[i - 1][j + 1];
					check4[4] = label[i][j + 1];
					check4[5] = label[i + 1][j + 1];
					check4[6] = label[i + 1][j];
					check4[7] = label[i + 1][j - 1];

					check4[8] = label[i][j - 2];
					check4[9] = label[i - 1][j - 2];
					check4[10] = label[i - 2][j];
					check4[11] = label[i - 1][j + 2];
					check4[12] = label[i][j + 2];
					check4[13] = label[i + 1][j + 2];
					check4[14] = label[i + 2][j + 2];
					check4[15] = label[i + 1][j - 2];

					check4[16] = label[i - 2][j - 2];
					check4[17] = label[i - 2][j - 1];
					check4[18] = label[i - 2][j + 1];
					check4[19] = label[i - 2][j + 2];
					check4[20] = label[i + 2][j];
					check4[21] = label[i + 2][j - 2];
					check4[22] = label[i + 2][j + 1];
					check4[23] = label[i + 2][j - 1];
					label[i][j] = retSmall(check4, 24, label[i][j]);

				}
			}
		}
	}

	for (int i = 0; i <row; i++) {
		for (int j = col - 1; j > 0; j--) {
			if (label[i][j]) {
				if (i == 0 && j == 0) {			//left top corner
					check1[0] = label[i][j + 1];
					check1[1] = label[i + 1][j + 1];
					check1[2] = label[i + 1][j];
					label[i][j] = retSmall(check1, 3, label[i][j]);

				}
				else if (i == 0 && j == col - 1) {			//right top corner
					check1[0] = label[i][j - 1];
					check1[1] = label[i + 1][j];
					check1[2] = label[i + 1][j - 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}
				else if (i == row - 1 && j == 0) {			//bot left corner
					check1[0] = label[i][j + 1];
					check1[1] = label[i - 1][j];
					check1[2] = label[i - 1][j + 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}
				else if (i == row - 1 && j == col - 1) {			//right bot corner
					check1[0] = label[i][j - 1];
					check1[1] = label[i - 1][j];
					check1[2] = label[i - 1][j - 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}

				else if (j == 0 && i > 0) {			//left column
					check2[0] = label[i - 1][j];
					check2[1] = label[i - 1][j + 1];
					check2[2] = label[i][j + 1];
					check2[3] = label[i + 1][j + 1];
					check2[4] = label[i + 1][j];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (i == 0 && j > 0) {				//first row
					check2[0] = label[i][j - 1];
					check2[1] = label[i][j + 1];
					check2[2] = label[i + 1][j];
					check2[3] = label[i + 1][j - 1];
					check2[4] = label[i + 1][j + 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (j == col - 1 && i > 0) {			//right column
					check2[0] = label[i + 1][j];
					check2[1] = label[i][j - 1];
					check2[2] = label[i - 1][j];
					check2[3] = label[i - 1][j - 1];
					check2[4] = label[i + 1][j - 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (i == row - 1) {				//last row
					check2[0] = label[i][j - 1];
					check2[1] = label[i - 1][j - 1];
					check2[2] = label[i - 1][j];
					check2[3] = label[i - 1][j + 1];
					check2[4] = label[i][j + 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if ((i > 1 || i < row - 2) || (j > 1 || j < col - 2)) {
					check3[0] = label[i][j - 1];
					check3[1] = label[i - 1][j - 1];
					check3[2] = label[i - 1][j];
					check3[3] = label[i - 1][j + 1];
					check3[4] = label[i][j + 1];
					check3[5] = label[i + 1][j + 1];
					check3[6] = label[i + 1][j];
					check3[7] = label[i + 1][j - 1];
					label[i][j] = retSmall(check3, 8, label[i][j]);
				}
				else {
					check4[0] = label[i][j - 1];
					check4[1] = label[i - 1][j - 1];
					check4[2] = label[i - 1][j];
					check4[3] = label[i - 1][j + 1];
					check4[4] = label[i][j + 1];
					check4[5] = label[i + 1][j + 1];
					check4[6] = label[i + 1][j];
					check4[7] = label[i + 1][j - 1];

					check4[8] = label[i][j - 2];
					check4[9] = label[i - 1][j - 2];
					check4[10] = label[i - 2][j];
					check4[11] = label[i - 1][j + 2];
					check4[12] = label[i][j + 2];
					check4[13] = label[i + 1][j + 2];
					check4[14] = label[i + 2][j + 2];
					check4[15] = label[i + 1][j - 2];

					check4[16] = label[i - 2][j - 2];
					check4[17] = label[i - 2][j - 1];
					check4[18] = label[i - 2][j + 1];
					check4[19] = label[i - 2][j + 2];
					check4[20] = label[i + 2][j];
					check4[21] = label[i + 2][j - 2];
					check4[22] = label[i + 2][j + 1];
					check4[23] = label[i + 2][j - 1];
					label[i][j] = retSmall(check4, 24, label[i][j]);

				}
			}
		}
	}

	for (int i = row - 1; i > 0; i--) {
		for (int j = 0; j <col; j++) {
			if (label[i][j]) {
				if (i == 0 && j == 0) {			//left top corner
					check1[0] = label[i][j + 1];
					check1[1] = label[i + 1][j + 1];
					check1[2] = label[i + 1][j];
					label[i][j] = retSmall(check1, 3, label[i][j]);

				}
				else if (i == 0 && j == col - 1) {			//right top corner
					check1[0] = label[i][j - 1];
					check1[1] = label[i + 1][j];
					check1[2] = label[i + 1][j - 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}
				else if (i == row - 1 && j == 0) {			//bot left corner
					check1[0] = label[i][j + 1];
					check1[1] = label[i - 1][j];
					check1[2] = label[i - 1][j + 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}
				else if (i == row - 1 && j == col - 1) {			//right bot corner
					check1[0] = label[i][j - 1];
					check1[1] = label[i - 1][j];
					check1[2] = label[i - 1][j - 1];
					label[i][j] = retSmall(check1, 3, label[i][j]);
				}

				else if (j == 0 && i > 0) {			//left column
					check2[0] = label[i - 1][j];
					check2[1] = label[i - 1][j + 1];
					check2[2] = label[i][j + 1];
					check2[3] = label[i + 1][j + 1];
					check2[4] = label[i + 1][j];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (i == 0 && j > 0) {				//first row
					check2[0] = label[i][j - 1];
					check2[1] = label[i][j + 1];
					check2[2] = label[i + 1][j];
					check2[3] = label[i + 1][j - 1];
					check2[4] = label[i + 1][j + 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (j == col - 1 && i > 0) {			//right column
					check2[0] = label[i + 1][j];
					check2[1] = label[i][j - 1];
					check2[2] = label[i - 1][j];
					check2[3] = label[i - 1][j - 1];
					check2[4] = label[i + 1][j - 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if (i == row - 1) {				//last row
					check2[0] = label[i][j - 1];
					check2[1] = label[i - 1][j - 1];
					check2[2] = label[i - 1][j];
					check2[3] = label[i - 1][j + 1];
					check2[4] = label[i][j + 1];
					label[i][j] = retSmall(check2, 5, label[i][j]);
				}
				else if ((i > 1 || i < row - 2) || (j > 1 || j < col - 2)) {
					check3[0] = label[i][j - 1];
					check3[1] = label[i - 1][j - 1];
					check3[2] = label[i - 1][j];
					check3[3] = label[i - 1][j + 1];
					check3[4] = label[i][j + 1];
					check3[5] = label[i + 1][j + 1];
					check3[6] = label[i + 1][j];
					check3[7] = label[i + 1][j - 1];
					label[i][j] = retSmall(check3, 8, label[i][j]);
				}
				else {
					check4[0] = label[i][j - 1];
					check4[1] = label[i - 1][j - 1];
					check4[2] = label[i - 1][j];
					check4[3] = label[i - 1][j + 1];
					check4[4] = label[i][j + 1];
					check4[5] = label[i + 1][j + 1];
					check4[6] = label[i + 1][j];
					check4[7] = label[i + 1][j - 1];

					check4[8] = label[i][j - 2];
					check4[9] = label[i - 1][j - 2];
					check4[10] = label[i - 2][j];
					check4[11] = label[i - 1][j + 2];
					check4[12] = label[i][j + 2];
					check4[13] = label[i + 1][j + 2];
					check4[14] = label[i + 2][j + 2];
					check4[15] = label[i + 1][j - 2];

					check4[16] = label[i - 2][j - 2];
					check4[17] = label[i - 2][j - 1];
					check4[18] = label[i - 2][j + 1];
					check4[19] = label[i - 2][j + 2];
					check4[20] = label[i + 2][j];
					check4[21] = label[i + 2][j - 2];
					check4[22] = label[i + 2][j + 1];
					check4[23] = label[i + 2][j - 1];
					label[i][j] = retSmall(check4, 24, label[i][j]);

				}
			}
		}
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//cout << label[i][j];
		}
		//cout << endl;
	}
	cout << current_label << endl;
	int* freq = new int[current_label];
	int* freq1 = new int[current_label];
	int count = 1;
	for (int i = 0; i < current_label; i++) {
		freq[i] = count;
		count++;
	}


	for (int i = 0; i < current_label; i++) {
		int count1 = 0;
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				if (freq[i] == label[j][k]) {
					count1++;

				}
			}
		}
		freq1[i] = count1;
		//cout << freq[i] << "     " << freq1[i] << endl;
	}

	int max = INT_MIN;
	int index = -1;
	// Iterate the array
	for (int i = 0; i < current_label; i++)
	{
		if (freq1[i] > max)
		{
			// If current value is greater than max
			// value then replace it with max value
			max = freq1[i];
			index = i;
		}
	}

	int greatest = index + 1;
	//cout << greatest;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (label[i][j] == greatest) {
				image.at<uchar>(i, j) = 255;
				label[i][j] = 255;
			}

			else {
				image.at<uchar>(i, j) = 0;
				label[i][j] = 0;
			}

		}
		
		// cout << x;
	}

	/*imwrite("output.bmp", image);
	system("output.bmp");
	waitKey(0);*/
	namedWindow("Window1");
	imshow("Window1", image);
	waitKey(0);
	return label;
}
int** Kmean(Mat& image) {
	Mat cop;

	image.convertTo(cop, CV_32S, 1, 0);
	int row = image.rows;
	int col = image.cols;

	if (image.empty()) {
		cout << "Image not found." << endl;
	}
	//cout << image.rows << "*" << image.cols << endl;
	//setMouseCallback("0000", mouse, reinterpret_cast<void*>(& image));
   // imshow("OOOO",image);
   // waitKey(0);
	//cout << "bla: " << image.rows << "*" << image.cols << endl;

	int** arr = new int* [row];
	for (int i = 0; i < row; i++)
	{
		arr[i] = cop.ptr<int>(i);
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			
				arr[i][j] = image.at<uchar>(i, j);
			
		}
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//cout << arr[i][j];
		}
		//cout << endl;
	}
	int** label = new int* [row];
	for (int i = 0; i < row; i++)
	{
		label[i] = new int[col] {0};
	}
	int a = 0, b = 0, c1 = 20, c2 = 180,x1=0,x2=0,s1,s2,ans1,ans2;


	while (a != c1 && b != c2) {
		a = c1;
		b = c2;
		x1 = 0;
		x2 = 0;
		s1 = 0;
		s2 = 0;
		int i = 0;
		while (i < row) {
			int j = 0;
			while (j < col) {
				
					if (arr[i][j] != 0) {
						ans1 = abs(arr[i][j] - a);
						ans2 = abs(arr[i][j] - b);
						if (ans1 < ans2) {
							label[i][j] = 0;
						}
						else {
							label[i][j] = 1;
						}
					}
				
				j++;
			}
			
			i++;
		}
		

		i = 0;
		while (i < row) {
			int j = 0;
			while (j < col) {

				if (arr[i][j] != 0) {
					if (label[i][j] == 0) {
						s1 += arr[i][j];
						x1++;
					}
					else if (label[i][j] == 1) {
						//cout << "hmm";
						s2 += arr[i][j];
						x2++;
					}
				}

				j++;
			}

			i++;
		}


		//cout << x1 << " hmm " << x2 << endl;
		c1 = s1 / x1;
		c2 = s2 / x2;
	}
	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (label[i][j] == 0) {
				image.at<uchar>(i, j) = 255;
				label[i][j] = 255;
			}
			else if (label[i][j] == 1) {
				image.at<uchar>(i, j) = 0;
				label[i][j] = 0;
			}
		}
	}
	
	namedWindow("Window1");
	imshow("Window1", image);
	waitKey(0);
	label = labelling(image);
	return label;
}
void DiceCof(Mat& image, Mat& image1, Mat& image2) {
	int** Larr = labelling(image);
	int** Karr = Kmean(image1);
	

	Mat cop;
	//image2 = imread("C:\\Users\\J&W Computers\\Downloads\\Assignment\\Ground Truths\\IMD427_lesion.bmp", 0);
	image2.convertTo(cop, CV_32S, 1, 0);
	int row = image2.rows;
	int col = image2.cols;
	/*test.rows = row;
	test.cols = col;*/

	/*for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			test.at<uchar>(i, j) = Karr[i][j];

		}
	}
	Larr = labelling(test);*/
	if (image.empty()) {
		cout << "Image not found." << endl;
	}
	//cout << image.rows << "*" << image.cols << endl;
	//setMouseCallback("0000", mouse, reinterpret_cast<void*>(& image));
   // imshow("OOOO",image);
   // waitKey(0);
	//cout << "bla: " << image.rows << "*" << image.cols << endl;

	int** Darr = new int* [row];
	for (int i = 0; i < row; i++)
	{
		Darr[i] = cop.ptr<int>(i);
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			Darr[i][j] = image2.at<uchar>(i, j);

		}
	}
	

	double TrueP = 0, FalseP = 0, FalseN = 0, DiceC1=0, DiceC2=0;
	


	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if ((Larr[i][j] == 255 && Darr[i][j] == 255)) { TrueP++; }
			else if (Larr[i][j] == 255 && Darr[i][j] == 0) { FalseP++; }
			else if (Larr[i][j] == 0 && Darr[i][j] == 255) { FalseN++; }
		}
	}
	DiceC1 = (2 * TrueP * 1.0) / ((FalseN + (2 * TrueP) + FalseP) * 1.0);
	cout << TrueP << " " << FalseN << " " << FalseP << endl;

	TrueP = 0, FalseP = 0, FalseN = 0;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if ((Karr[i][j] == 255 && Darr[i][j] == 255)) { TrueP++; }
			else if (Karr[i][j] == 255 && Darr[i][j] == 0) { FalseP++; }
			else if (Karr[i][j] == 0 && Darr[i][j] == 255) { FalseN++; }
		}
	}
	cout << TrueP << " " << FalseN << " " << FalseP << endl;
	DiceC2 = (2 * TrueP * 1.0) / ((FalseN + (2 * TrueP) + FalseP) * 1.0);
	cout << (DiceC1 + DiceC2) / 2 << endl;;
	cout << "Dice Cof of Task1= " << DiceC1 << endl;
	cout << "Dice Cof of Task2= " << DiceC2 << endl;
}
struct nde {
	int data;
	nde* right, * down;
};
nde* construct(int** arr, int i, int j,int m, int n, vector<vector<nde*>>& visited)
{

	if (i > m - 1 || j > n - 1)
		return NULL;

	if (visited[i][j]) {
		return visited[i][j];
	}

	nde* temp = new nde();
	visited[i][j] = temp;
	temp->data = arr[i][j];
	temp->right = construct(arr, i, j + 1, m, n, visited);
	temp->down = construct(arr, i + 1, j, m, n, visited);
	return temp;
}
void display(nde* head)
{

	nde* Rp;

	nde* Dp = head;

	while (Dp) {
		Rp = Dp;

		while (Rp) {
			std::cout << Rp->data << " ";
			Rp = Rp->right;
		}
		std::cout << "\n";
		Dp = Dp->down;
	}
}
void RLC(nde* head, int m) {
	int i = 0;
	int j = 0;
	nde* Rp;

	nde* Dp = head;

	while (Dp) {
		i++;
		j = 0;
		Rp = Dp;

		while (Rp) {
			Rp = Rp->right;
			j++;
		}
		Dp = Dp->down;
	}
	//cout << i << " " << j << endl;;
	// bool flag = false;
	nde* rptr = NULL;
	nde* dptr = head;
	while (dptr) {
		rptr = dptr;
		int check = rptr->data;
		int i = 1;
		while (rptr->right != NULL) {

			if (rptr->data == 1) {
				if ((rptr->data != check || rptr->data != rptr->right->data))
					cout << i << " ";
				if (i == m - 1)
					cout << i;
				i++;
			}
			else
				i++;

			check = rptr->data;
			rptr = rptr->right;

		}
		cout << -1 << endl;;
		dptr = dptr->down;
	}

}
void makeList(cv::Mat& image) {
	cv::Mat cop;
	image.convertTo(cop, CV_32S, 1, 0);
	int m = image.rows;
	int n = image.cols;

	if (image.empty()) {
		cout << "Image not found." << endl;
	}
	cout << image.rows << "*" << image.cols << endl;

	int** arr = new int* [m];
	for (int i = 0; i < m; i++)
	{
		arr[i] = cop.ptr<int>(i);
	}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (image.at<uchar>(i, j) == 255)
				arr[i][j] = 1;
			else
				arr[i][j] = 0;
		}
	}
	vector<vector<nde*>> visited(m, vector<nde*>(n));
	nde* head = construct(arr, 0, 0, m, n, visited);
	//display(head);
	cout << endl << endl;
	RLC(head, m);

}
int main()
{

	Mat image,image1,image2;
	image = imread("C:\\Users\\J&W Computers\\Downloads\\Assignment\\Segmented Outputs\\mIMD004.bmp", 0);
	image1 = imread("C:\\Users\\J&W Computers\\Downloads\\Assignment\\Original Images\\IMD004.bmp", 0);
	image2 = imread("C:\\Users\\J&W Computers\\Downloads\\Assignment\\Ground Truths\\IMD004_lesion.bmp", 0);
	//labelling(image);
	//RunLengthCode(image);
	//Kmean(image1);
	
	DiceCof(image,image1,image2);

	makeList(image);
	return 0;
}
