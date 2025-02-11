#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

void frequency(int* arr) {

	bool* check = new bool[78];
		for (int i = 1; i <= 74; i++) {
			check[i] = 0;
		}
		for (int i = 1; i <= 74; i++) {
			if (check[i] == 1) {
				continue;
			}
			int count = 1;
			for (int j = i + 1; j < 75; j++) {
				if (arr[i] == arr[j]) {
					check[j] = 1;
					count++;
				}
			}
			cout << "frequency of " << arr[i] << " is: " << count << endl;
		}
}

int main() {

	// Providing a seed value
	srand((unsigned)time(NULL));
	int* arr = new int[76];
	// Loop to get 5 random numbers
	for (int i = 1; i <= 75; i++) {
		int random = 0 + (rand() % 75);
		arr[i] = random;

		// Print the random number
		cout << random << endl;
	}
	cout << "--------------" << endl;
	
	int key = 0;
	int j = 0;
	for (int i = 2; i <75; i++) {
		key = arr[i];
		j = i - 1;
		while (j > 0 && key < arr[j]) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}

	 frequency(arr);


	return 0;
}