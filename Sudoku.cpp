#include <iostream>
#include "Sudoku.h"
using namespace std;

Sudoku::Sudoku()
{
	numberOfAnswer = 0;
	for(int i = 0 ; i < sudokuSize ; i++){
		setMap(i,0);
		setQuestionMap(i,0);
		answer[i] = 0 ;
	}
	nonZero = 0;
}

void Sudoku::setMap(int index ,int value)
{
	map[index] = value ;
}

void Sudoku::setQuestionMap(int index ,int value)
{
	questionMap[index] = value ;
}

int Sudoku::getMap(int index)
{
	return map[index] ;
}

int Sudoku::getQuestionMap(int index)
{
	return questionMap[index] ;
}

bool Sudoku::isPossibleAnswer(int index)
{
	int row , col , cell ;
	row = index / 9;
	col = index % 9 ;
	cell = (row / 3 ) *  27  +(col / 3 ) * 3 ;
	for(int i = 0 ; i < 9 ;  i++){
		if(getQuestionMap(row*9 + i ) == getMap(index) )
			return false ;
		else if(getQuestionMap(col + i * 9 ) == getMap(index) )
			return false ;
		else if( (i+1) / 3 == 0 && getQuestionMap(cell + i ) == getMap(index))
			return false ;
		else if( (i+1) / 3 == 1 && getQuestionMap(cell + 9 + i %3 ) == getMap(index))
			return false ;
		else if( (i+1) / 3 == 2 && getQuestionMap(cell + 18 + i %3) == getMap(index))
			return false ;
	}
	return true ;
}

int Sudoku::getZeroIndex()
{
	for(int i  =  0 ; i < sudokuSize ; i++ )
	{
		if( map[i] == 0)
			return i;
	}
	return -1;
}

void Sudoku::showMap()
{
	for(int i = 0 ; i < sudokuSize ; ++i)
	{
		cout<<map[i]<<" ";
		if (i%9 == 8)
			cout << endl;
	}
}

void Sudoku::giveQuestion()
{
	int mapSeed[]= {
	6,4,5,3,1,2,9,7,8,9,7,8,6,4,5,3,1,2,3,1,2,9,7,8,6,4,5,
	4,5,6,1,2,3,7,8,9,7,8,9,4,5,6,1,2,3,1,2,3,7,8,9,4,5,6,
	5,6,4,2,3,1,8,9,7,8,9,7,5,6,4,2,3,1,2,3,1,8,9,7,5,6,4
	};	//initial a map
	for(int i = 0 ; i < sudokuSize ; i++)
		map[i] = mapSeed[i];

	//random change
	srand(time(NULL));
	changeNum(rand()%9 +1 , rand()%9 + 1);
	changeCol(rand()% 3 , rand()% 3);
	changeRow(rand()% 3 , rand()% 3);
	rotate(rand()%101);
	flip(rand()%2);
	//dig map
	int dig_time = 0 ;
	dig_time = rand () % 81 ;
	for(int i = 0 ; i < dig_time ; ++i)
		map[rand()%81] = 0 ;

	showMap();
}



void Sudoku::readIn()
{
	for(int i = 0 ; i < sudokuSize ; ++i){
		cin >>  map[i];
		setQuestionMap(i,map[i]);		
	}
}

void Sudoku::solveFunction()
{	if(numberOfAnswer < 2){
		int zeroIndex ;
		zeroIndex = getZeroIndex() ;
		if(zeroIndex == -1){
			if(numberOfAnswer <= 1){
				numberOfAnswer ++ ;
				for(int i = 0 ; i < sudokuSize ; i++)
					answer[i] =map[i];
				return ;
			}
		}
		for(int i = 1 ; i <= 9 ; i++){
			setMap(zeroIndex , i);
			if(isPossibleAnswer(zeroIndex)){
				setQuestionMap(zeroIndex ,i);
				solveFunction();
			}
			setQuestionMap(zeroIndex , 0); 
		}
		setMap(zeroIndex , 0 );
	}	
}

void Sudoku::solve(){

	int location ;
	for(int i =0 ; i < sudokuSize ; i++){
		if(map[i])
			nonZero ++ ;
		else{
			for(int j = 0  ; j < 9 ; j++){
				if(map[ (i/9) * 9 +j]  && !possibleNumber[i][map[(i/9)*9 + j] - 1 ] ){
					howManyImPossible[i]++ ;
					possibleNumber[i][map[(i/9)*9 + j] - 1 ] = true ;
				}
				if(map[ i%9 + j*9 ] && !possibleNumber[i][map[i%9 + j*9]-1] ) {
					howManyImPossible[i]++ ;
					possibleNumber[i][map[i%9 + j*9]-1] = true ;
				}
				location = map[((i/( 9* 3 ))*3+j/3)*9 + ( ( i %9 )/3) * 3+j%3] - 1;
				if (map[((i/(9*3))*3+j/3)*9 + ((i%9)/3)*3+j%3] && !possibleNumber[i][location]){
					howManyImPossible[i]++ ;
					possibleNumber[i][location] = true ;
				}
			}
		}
	}
	//enter the answer if there is just one possible number
	for(int i = 0 ; i < sudokuSize ; i++){	
		if (!map[i]  && howManyImPossible[i] ==8 ){
			howManyImPossible[i] = 0 ;
			nonZero ++;
			for (int  k = 0 ; k < 9 ; k ++){//find which number should full in
				if(!possibleNumber[i][k]){
					map[i] = k +1 ;
					questionMap[i] = k + 1 ;
					for(int j = 0  ; j < 9 ; j++){//update the possible imformation
						if(map[ (i/9) * 9 +j]  && !possibleNumber[i][map[(i/9)*9 + j] - 1 ] ){
							howManyImPossible[i]++ ;
							possibleNumber[i][map[(i/9)*9 + j] - 1 ] = true ;
						}
						if(map[ i%9 + j*9 ] && !possibleNumber[i][map[i%9 + j*9]-1] )  {
							howManyImPossible[i]++ ;
							possibleNumber[i][map[i%9 + j*9]-1] = true ;
						}
						location = map[((i/( 9* 3 ))*3+j/3)*9 + ( ( i %9 )/3) * 3+j%3] - 1;
						if (map[((i/(9*3))*3+j/3)*9 + ((i%9)/3)*3+j%3] && !possibleNumber[i][location]){
							howManyImPossible[i]++ ;
							possibleNumber[i][location] = true ;
						}
					}

				}
			}
			i = -1;//restart the loop until there is no any element has just one possible answer
		}
	}
	if(nonZero == 81 ){
		numberOfAnswer++;
	}

	solveFunction();
	//showMap() ;
	if(numberOfAnswer == 0)
		cout<<numberOfAnswer<<endl;
	else if(numberOfAnswer == 1){
		cout<<numberOfAnswer<<endl;
		for(int i = 0 ; i < sudokuSize ; ++i)
		{
			cout<<answer[i]<<" ";
			if (i%9 == 8)
				cout << endl;
		}
		
	}
	else
		cout<<numberOfAnswer<<endl;
}
void Sudoku::changeNum(int a , int b)
{	
	vector<int> indexA,indexB;
	for(int i = 0 ; i < sudokuSize ; ++i)//find Index of all A
	{
		if(map[i] == a)
			indexA.push_back(i);
		else if(map[i] == b)
			indexB.push_back(i);
	}
	for(int i = 0  ; i<indexA.size() ; ++i)
		map[indexA.at(i)] = b ;
	for(int i = 0 ; i<indexB.size() ; ++i )
		map[indexB.at(i)] = a;
}

void Sudoku::changeRow(int a, int b)
{
	int buffer[27];
	for(int i = 0 ; i < 27 ; ++i)
		buffer[i] = map[27*a+i];
	for(int i = 0 ; i < 27 ; ++i)
		map[27*a+i] = map[27*b+i];
	for(int i = 0 ; i < 27 ; ++i)
		map[27*b+i] = buffer[i];
}

void Sudoku::changeCol(int a,int b)
{
	int buffer[27];
	for(int j= 0 ; j < 3 ; ++j)
	{
		for(int i = 0 ; i < 9 ;++i)
			buffer[9*j+i] = map[3*a + j+ 9*i];
	}

	for(int j= 0 ; j < 3 ; ++j)
	{
		for(int i = 0 ; i < 9 ;++i)
			map[3*a + j+ 9*i] = map[3*b + j+ 9*i];
	}

	for(int j= 0 ; j < 3 ; ++j)
	{
		for(int i = 0 ; i < 9 ;++i)
			map[3*b+ j+ 9*i] = buffer[9*j+i] ;
	}	
}
void Sudoku::rotate(int  n)//rotate 90 degree
{
	int originalMap[9][9] ;
	int rotatedMap[9][9];
	for(int j = 0 ; j <  9 ; ++j)//transform int[] into int[][]
	{
		for(int i = 0 ; i < 9 ; ++i)
		{
			originalMap[i] [j] = map [i+j*9] ;
		}
	}
	for(int i = 0 ; i < n ; ++ i)
	{
		for(int x = 0 ; x < 9 ; x++)//rotate
		{
			for(int y = 0 ; y < 9 ; y++)
				rotatedMap[x][y] = originalMap[y][8-x];
		}
		for(int x = 0 ; x < 9 ; x++)//update originalMap[]
		{
			for(int y = 0 ; y < 9 ; y++)
				originalMap[x][y] = rotatedMap[x][y];
		}
	}

	for(int j = 0 ; j < 9; ++j)//update map
	{
		for(int i = 0 ; i< 9 ; ++i)
		{
			map[i+9*j] = rotatedMap[i][j];
		}
	}
}

void Sudoku::flip(int n)
{
	int originalMap[9][9] ;
	int flippedMap[9][9];
	for(int j = 0 ; j <  9 ; ++j)//transform int[] into int[][]
	{
		for(int i = 0 ; i < 9 ; ++i)
		{
			originalMap[i] [j] = map [i+j*9] ;
		}
	}

	if(n == 0)
	{
		for(int i = 0 ; i < 9 ; ++i)
		{
			for(int j = 0 ; j < 9 ;++j)
				flippedMap[i][j] = originalMap[i][8-j];
		}
	}
	else
	{
		for(int j = 0 ; j < 9 ; ++j)
		{
			for(int i = 0 ; i < 9 ; ++i )
				flippedMap[i][j] = originalMap[8-i][j];
		}
	}
	for(int j = 0 ; j < 9; ++j)//update map
	{
		for(int i = 0 ; i< 9 ; ++i)
		{
			map[i+9*j] = flippedMap[i][j];
		}
	}
}

void Sudoku::transform()
{
	readIn();
	srand(time(NULL));
	changeNum(rand()%9 +1 , rand()%9 + 1);
	changeCol(rand()% 3 , rand()% 3);
	changeRow(rand()% 3 , rand()% 3);
	rotate(rand()%101);
	flip(rand()%2);
	showMap();
}