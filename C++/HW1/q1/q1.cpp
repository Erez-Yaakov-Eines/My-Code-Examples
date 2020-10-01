#include<iostream>
#include<ctime>
using namespace std;

/************************************* DECLERATIONS **********************************************/

/**********************************************************************************************//**
 * \fn	int* rand_arr_joker(int& size);
 *
 * \brief	Generates a random array of integers of user selected size, and an additional "joker" 
 * 			random integer.
 * 			Compares random array elements to joker and produces a dynamic array containing indices
 * 			of matching elements.
 *
 * \author	Erez Yaakov Eines
 * \date	12-Aug-18
 *
 * \param [in,out]	size	The size of returned indices array (by reference).
 *
 * \return	Null if it fails, else a pointer to an int.
 **************************************************************************************************/

int* rand_arr_joker(int& size);

/**********************************************************************************************//**
 * \fn	int* destroy_array(int* array, int size);
 *
 * \brief	Destroys the array
 *
 * \author	Erez Yaakov Eines
 * \date	12-Aug-18
 *
 * \param [in,out]	array	If non-null, the array to destroy.
 * \param 		  	size 	array size.
 *
 * \return	Null if it fails, else a pointer to an int.
 **************************************************************************************************/

int* destroy_array(int* array, int size);

/**********************************************************************************************//**
 * \fn	int* arr_alloc_inc1(int* array, int& size);
 *
 * \brief	Array allocate increment 1
 *
 * \author	Erez Yaakov Eines
 * \date	12-Aug-18
 *
 * \param [in,out]	array	If non-null, the array to increment.
 * \param [in,out]	size 	Input array's size.
 *
 * \return	original input array if it fails, else a pointer to an int.
 **************************************************************************************************/

int* arr_alloc_inc1(int* array, int& size);

/**********************************************************************************************//**
 * \fn	void print_arr(int* array, int size);
 *
 * \brief	Print array
 *
 * \author	Erez Yaakov Eines
 * \date	12-Aug-18
 *
 * \param [in,out]	array	If non-null, the array to print.
 * \param 		  	size 	Array size.
 **************************************************************************************************/

void print_arr(int* array, int size);

/**********************************************************************************************//**
 * \fn	void throwErr(const char * error);
 *
 * \brief	Outputs error string to cerr and returns
 *
 * \author	Erez Yaakov Eines
 * \date	12-Aug-18
 *
 * \param	error	Error string const.
 **************************************************************************************************/

void throwErr(const char * error);


/********************************* IMPLEMENTATIONS ************************************************/

int* rand_arr_joker(int& dup_size)
{
	dup_size = 0;
	int rand_arr_size;
	srand(std::time(NULL)); //init random seed using current time
	int lucky = rand() % 10; //draw random joker (0-9 integer) to check against user array
	int* dup_array = 0; //joker duplicate array pointer
	cout << "enter amount of random integers" << endl;
	cin.clear();
	cin >> rand_arr_size;
	if (rand_arr_size <= 0) return (int*)nullptr;// invalid user input.
	int* rand_arr = new int[rand_arr_size]; //allocate user random integer array. does not check input validity
	for (int i = 0; i < rand_arr_size; ++i) //populate new array
	{
		rand_arr[i] = rand() % 10;
		if (rand_arr[i] == lucky) //check if draw integer is the joker
		{
			dup_array = arr_alloc_inc1(dup_array, dup_size);
			dup_array[dup_size - 1] = i;
		}
	}
	cout << "original random array is: " << endl;// print to default stream out
	print_arr(rand_arr, rand_arr_size);
	cout << "joker number is: " << lucky << endl;
	rand_arr = destroy_array(rand_arr,rand_arr_size);
	return dup_array;
}

int* destroy_array(int* array, int size)
{
	if (!array)
		if (size == 0) return NULL;// this situation is valid
		else { throwErr("mismatching array size in destroy_array()"); return NULL; } //this situation is invalid
	delete[] array;
	return NULL;
}

int* arr_alloc_inc1(int* array, int& size)
{
	if (!array)
		if(size!=0) { throwErr("mismatching array size in arr_alloc_inc1"); return array; }
		else
		{
			array = new int[1];
			size = 1;
			return array;
		}

	int* temp_array = new int[size + 1];// allocate new array
	for (int i = 0; i < size; ++i)// elementwise copy old array to new array.
	{
		temp_array[i] = array[i];
	}
	++size;// new array is larger by 1
	delete[] array;// release old array
	return temp_array;
}

void print_arr(int* array, int size)
{
	if (!array) { throwErr("null pointer in print_arr()"); return; }
	if (size < 0) { throwErr("negative size error in print_arr()"); return; }
	if (size == 0) { throwErr("empty array print in print_arr()"); return; }

	for (int i = 0; i < size; ++i)
	{
		cout << array[i] << " ";
	}
	cout << endl;
}

void throwErr(const char * error)
{
	if (!error) return;
	cerr << error << endl;
	return;
}
/*************************** ENTRY POINT ************************************/

int main() {
	int size , *array = NULL;
	array = rand_arr_joker(size);
	cout << "dup array is: " << endl;
	if (size == 0) cout << "empty... No jokers found." << endl;
	else print_arr(array, size);
	array = destroy_array(array,size);
	return EXIT_SUCCESS;
}