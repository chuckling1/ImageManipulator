/******************************
 **			main.cpp			 **
 **	Created by Charles Davis **
 **		CPSC 246-01			 **
 ******************************

 ******************************************************************************
 ** The main file of the ImageManipulator.exe.  This program prints out the  **
 ** initial information and then creates an ImageManipulator object to		 **
 ** manipulate my SRU ID picture, Davis.ppm. I chose to have it run	the	     **
 ** fiesta filter I created.												     **
 *****************************************************************************/

#include "ImageManipulator.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

void init();
void getFilter(int& filter);

int main()
{
	init();
	
	string inFileName, outFileName;
	int userFilter = 0;

	getFilter(userFilter);

	cout << "\nEnter image file name to be manipulated: ";
	cin >> inFileName;

	cout << "Enter output file name: ";
	cin >> outFileName;

	ImageManipulator myImage = ImageManipulator(inFileName);
		
	myImage.filterImage(outFileName , userFilter);

	return 0;

}


/*********************************** Init ************************************/
void init()
{
	cout << "\n\n====================================================" << endl;
	cout << "   Charles Davis - CPSC 246.01 - ImageManipulator   " << endl;
	cout << "====================================================\n\n" << endl;
}


void getFilter(int &filter)
{
	int userInput;
	
	do
	{
		cout << "\n 1) Zero Red			7)  Average RGB"		 << endl;
		cout << " 2) Zero Green			8)  Swap Red Green"  << endl;
		cout <<	" 3) Zero Blue			9)  Swap Green Blue" << endl;
		cout << " 4) Invert Red			10) Swap Blue Red"	 << endl;
		cout <<	" 5) Invert Green		11) Fiesta Filter"	 << endl;
		cout <<	" 6) Invert Blue\n"							 << endl;

		cout << "Which filters would you like to apply?"		 << endl; 
		cout << "Choose one at a time.  Enter -1 to stop"	 << endl;
		cin >>  userInput;

		if(userInput >= 1 && userInput <= 11)
		{
			switch(userInput)
			{
				case 1: 
					filter = filter | ImageManipulator::ZERO_RED; 	
					break;
				case 2: 
					filter = filter | ImageManipulator::ZERO_GREEN; 	
					break;
				case 3: 
					filter = filter | ImageManipulator::ZERO_BLUE; 	
					break;
				case 4: 
					filter = filter | ImageManipulator::INVERT_RED; 	
					break;
				case 5: 
					filter = filter | ImageManipulator::INVERT_GREEN; 	
					break;
				case 6: 
					filter = filter | ImageManipulator::INVERT_BLUE; 	
					break;
				case 7: 
					filter = filter | ImageManipulator::AVERAGE_RGB; 	
					break;
				case 8:
					filter = filter | ImageManipulator::SWAP_RED_GREEN; 	
					break;
				case 9:
					filter = filter | ImageManipulator::SWAP_GREEN_BLUE; 	
					break;
				case 10:
					filter = filter | ImageManipulator::SWAP_BLUE_RED; 	
					break;
				case 11:
					filter = filter | ImageManipulator::FIESTA_FILTER; 	
					break;
				default:					break;
			}
		}

	}while(userInput != -1);

		cout << "\n\n\nYou chose: \n" << endl;  

		for(int i = 0; i < ImageManipulator::NUM_FILTERS; i++)
		{
			int bitMask = 1 << i;

			if(filter & bitMask)
			{
				switch(bitMask)
				{
					case ImageManipulator::ZERO_RED:
						cout << " Zero Red";	
						break;
					case ImageManipulator::ZERO_GREEN:	
						cout << " Zero Green";	
						break;
					case ImageManipulator::ZERO_BLUE:
						cout << " Zero Blue";
						break;
					case ImageManipulator::INVERT_RED:	
						cout << " Invert Red";
						break;
					case ImageManipulator::INVERT_GREEN:
						cout << " Invert Green"	;
						break;
					case ImageManipulator::INVERT_BLUE:
						cout << " Invert Blue";
						break;
					case ImageManipulator::AVERAGE_RGB:
						cout << " Average RGB";
						break;
					case ImageManipulator::SWAP_RED_GREEN:	
						cout << " Swap Red Green";
						break;
					case ImageManipulator::SWAP_GREEN_BLUE:
						cout << " Swap Green Blue";	
						break;
					case ImageManipulator::SWAP_BLUE_RED:	
						cout << " Swap Blue Red";
						break;
					case ImageManipulator::FIESTA_FILTER:
						cout << " Fiesta Filter";
						break;
					default:					break;
				}

				cout << endl;
			}

			
		}

}