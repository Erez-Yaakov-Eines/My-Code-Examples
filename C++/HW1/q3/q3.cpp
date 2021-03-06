#include<iostream>
#include<string>
using namespace std;
/******************************** DECLERATIONS ************************************/

/**********************************************************************************************//**
 * \fn	char ** GuessWord(const char * wordToGuess, int &size, bool& success);
 *
 * \brief	starts a small word guessing game in command.
 * 			user tries to guess "wordToGuess" and is given new successive letters each turn.
 * 			game ends after user successfully guesses wordToGuess or attempts = length(wordToGuess)-1
 *
 * \author	Erez Yaakov Eines
 * \date	13-Aug-18
 *
 * \param 		  	wordToGuess	The word to guess.
 * \param [in,out]	size	   	returned guesses array size.
 * \param [in,out]	success	   	True if the user was successful, false otherwise.
 *
 * \return	nullptr if fails, else a pointer to array of guesses.
 **************************************************************************************************/

char ** GuessWord(const char * wordToGuess, int &size, bool& success);

/**********************************************************************************************//**
 * \fn	void destroyGuesses(char** guesses, int& size);
 *
 * \brief	Destroys the array of guesses
 *
 * \author	Erez Yaakov Eines
 * \date	13-Aug-18
 *
 * \param [in]	guesses	If non-null, array of char*.
 * \param [in]	size   	The size of array.
 **************************************************************************************************/

void destroyGuesses(char** guesses, int size);
char** IncAndCpyStrArr(char** source, int& size); // destroys source, // ran out of "ko'ach" to document
void printStrArr(const char** source, int size);// ran out of "ko'ach" to document
void* throwErr(const char* error);// ran out of "ko'ach" to document
char** getGuess(char(&userBuffer)[20], char** guesses, int& size);// ran out of "ko'ach" to document
/******************************** IMPLEMENTATIONS ************************************/
char ** GuessWord(const char * wordToGuess, int & size, bool & success)
{
	if (!wordToGuess) return (char**)throwErr("error - nullptr sent to GuessWord");

	size = 0; success = false;
	int wordToGuessLen = strlen(wordToGuess);
	char userBuffer[20], **guesses = nullptr;

	for (int i = 0; i < wordToGuessLen; ++i)
	{
		guesses = getGuess(userBuffer, guesses, size);
		if (strcmp(wordToGuess, userBuffer) == 0) // strings are equal
		{
			cout << "that's correct" << endl;
			success = true;
			return guesses;
		}
		cout << "next character is: " << wordToGuess[i] << endl;
	}
	return guesses;
}

void destroyGuesses(char ** guesses, int size)
{
	if (!guesses)
	{
		if (size != 0) { cerr << "mismatch arr size in destroyGuesses()" << endl; return; }
		return;
	}
	for (int i = 0; i < size; ++i)
	{
		if (!guesses[i]) { cerr << "unexpected nullptr in destroyGuesses()" << endl; continue; }
		delete[] guesses[i];
	}
	delete[] guesses;
	return;
}

char ** IncAndCpyStrArr(char ** source, int& size)
{
	if (!source) return (char**)throwErr("nullptr sent to copyStringArray");

	char** newStringArray = new char*[size + 1];
	for (int i = 0; i < size; ++i)
	{
		newStringArray[i] = source[i];
	}
	++size;
	delete[] source;
	return newStringArray;
}

void printStrArr(const char ** source, int size)
{
	if (!source) return;

	cout << "you tried the following words: " << endl << endl;
	for (int i = 0; i < size; ++i)
	{
		cout << "guess #" << i + 1 << " was: " << source[i] << endl;
	}
	cout << endl;
	return;
}

void* throwErr(const char * error)
{
	if (!error) return nullptr;
	cerr << error << endl;
	return nullptr;
}

char** getGuess(char(&userBuffer)[20], char** guesses, int& size)
{
	cout << "please guess a word: ";
	cin.clear();
	cin.getline(userBuffer, 20); cout << endl;
	if (!size)
	{
		if (guesses) return (char**)throwErr("array size mismatch in getGuess()");
		guesses = new char*[1];
		guesses[0] = new char[strlen(userBuffer) + 1];
		strcpy_s(guesses[0], strlen(userBuffer) + 1, userBuffer);
		++size;
		return guesses;
	}
	guesses = IncAndCpyStrArr(guesses, size);
	guesses[size - 1] = new char[strlen(userBuffer) + 1];
	strcpy_s(guesses[size - 1], strlen(userBuffer) + 1, userBuffer);
	return guesses;
}
/**************************** ENTRY POINT *****************************************/
int main()
{
	char** guesses = NULL, word[10] = "custodian";
	int size = 0;
	bool success = false;
	guesses = GuessWord(word, size, success);

	cout << "*******************************************************" << endl;
	if (success) cout << "success!!! mother of god you nailed it!!!" << endl;
	else cout << "Game Over!" << endl << "The correct word is: " << word << endl;
	printStrArr((const char**)guesses, size);
	cout << "*******************************************************" << endl;

	destroyGuesses(guesses, size);
    return 0;
}

