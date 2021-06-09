#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define _OPEN_THREADS
#define NUM_OF_THREADES 4

void* xorFunc(void* string){//xor all char of string
	int* result = (int*)malloc(sizeof(int));
	char* str = (char*) string;
	for(int i = 0 ; i < strlen(str) ; i++)
		*result ^= str[i];
	return (void*)result;
}
char** wordsToEvenNumOfCharsPerWord(char** strArray,int numOfStr,int numOfStrToReturn)
{
	int totalNumOfChars = 0;
	int charPerNewStr = 0;
	for(int i = 0 ; i < numOfStr ; i++)//sum the length of all strings in strArray
		totalNumOfChars += strlen(strArray[i]);
		
	charPerNewStr = totalNumOfChars/numOfStrToReturn;//dividing the number of all chars that is in all of the strArray in the number of str we want to give back
	if(totalNumOfChars%numOfStrToReturn != 0)//if reminder isnt 0 then we need 1 extra space in each str
		charPerNewStr++;
	
	char** newStrArray = (char**)malloc(numOfStrToReturn * sizeof(char*));//creat new array in the size of number of string we want to return
	if(!newStrArray)
		return NULL;
	for(int i = 0 ; i < numOfStrToReturn ; i++)//make new string in a way that all new str has even num of chars
	{
		newStrArray[i] = (char*)malloc((charPerNewStr + 1) * sizeof(char));
		if(!newStrArray[i])
			return NULL;
	}
	int charNum = 0;//curent str char counter
	int currentStr = 0;//curent str counter
	
	for(int i = 0 ; i < numOfStr ; i++)//insert all chars to new str all chars are divided equaly between them 
		for(int j = 0 ; j < strlen(strArray[i]) ; j++)//go char by char of original string 
		{
			if(charNum == charPerNewStr)//if this str is full put null terminate at its end and reset char counter and move to next string becouse of even divisin when we reach the last char in the last new str we also finish the last char in last str of original arr and outer loop will finish
			{
				newStrArray[currentStr][charNum+1] = '\0';
				charNum = 0;
				currentStr++;
			}
			newStrArray[currentStr][charNum] = strArray[i][j];//instert char in place
			charNum++;
		}
	return newStrArray;
		
}

int main (int argc, char *argv[])
{

  int res = 0;
  
  int* temp;
  char** strArray = wordsToEvenNumOfCharsPerWord((argv + 1),(argc - 1),NUM_OF_THREADES);//we can divide the xor to 4 and give equal work for each thread because xor is commutative
  pthread_t thr[NUM_OF_THREADES];

  for (int i = 0; i < NUM_OF_THREADES; i++)
  	pthread_create(&(thr[i]),NULL,xorFunc,strArray[i]);
  for (int i = 0; i < NUM_OF_THREADES; i++)
  {
  	pthread_join(thr[i],(void*)&temp);
  	res += *temp;

  }
  
  printf ("xor of all strings: %d\n", res);
  return 0;
}
