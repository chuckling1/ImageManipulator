/******************************
 **	  ImageManipulator.cpp	 **
 **	Created by Charles Davis **
 **		CPSC 246-01			 **
 ******************************/

/******************************************************************************
 ** ImageManipulator.cpp is the implementation file for the ImageManipulator **
 ** class.  It has 11 filters that manipulate the colors of the given image  **
 ** It has a function called filterImage(string image, int imageFilter) that **
 ** takes the input file, changes the color value based on the inputed image **
 ** filter integer and spits out the finished image to the given string		 **
 ** file name.  In the header file there is a list of constant ints that a   **
 ** a user can send through the image filter function.  They will be ran in  **
 ** this order:																 **
 **																			 **
 **		1. ZERO_RED		5. INVERT_GREEN		9. SWAP_GREEN_BLUE				 **
 **		2. ZERO_GREEN	6. INVERT_BLUE		10. SWAP_BLUE_RED			     **
 **		3. ZERO_BLUE		7. AVERAGE_RGB		11. FIESTA_FILTER				 **
 **		4. INVERT_RED	8. SWAP_RED_GREEN									 **
 **																			 **
 ** So if a user wanted to zero the red and then swap the blue and red		 **
 ** the function call would be:												 **
 ** myImage.filterImage("myPic.ppm,myImage.ZERO_RED + myImage.SWAP_RED_BLUE) **
 **																			 **
 ** and even if the values were reverse they would still be executed in the  **
 ** same order.																 **
 ** The fiest filter is a special filter created by the developer that does  **
 ** some crazy stuff to the image by adding to the color values and swapping **
 ** colors in rows and columns creating a really crazy looking final image   **
 *****************************************************************************/



#include "ImageManipulator.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;




/******************************** Constructor ********************************/
	ImageManipulator::ImageManipulator(string file)
	{
		// The constructor opens the given file, validates that the file exists
		// checks the magic number, reads and ignores the comments, gets the
		// height, width and max color value and assigns them to the instance
		// variables of the object then creates a new Pixel array with size
		// height and width then initializes the array values all to zero


		string tempLine;			// tempLine for reading comment line 
		inFileName = file;		// set the filename for the object

		inFile.open(inFileName); // open the file


		if(inFile.is_open())		// if the file is open
		{
			getline(inFile, magicNumber);		//read in magic number
			
			if(magicNumber == "P3")		// if it is a valid number
			{
				getline(inFile, tempLine);	//get the comment line

				if(tempLine.at(0) == '#')	// if comment is valid comment
				{
					inFile >> width;			// read in width and height
					inFile >> height;
					
					if(width > 0 && height > 0)		// if the width and height
					{								// aren't negative
						inFile >> maxVal;			// read in max color value

						if(maxVal != 255)	// for our purposes the maxVal
						{					// should be 255. If it isn't exit
							cerr << "Invalid Max Color";
							exit(1);
						}

					}
					else							// negative size found. exit
					{ 
						cerr << "Invalid size"; 
						exit(1);
					}

				}
				else								// no comment found
				{ 
					cerr << "Invalid Header"; 
					exit(1);
				}

			}
			else									// bad magic number
			{ 
				cerr << "Invalid Header";
				exit(1);
			}

		}										// file isn't open
		else 
		{ 
			cerr << "Error opening file"; 
			exit(1);
		}



		RGBArray = new Pixel[width * height];		// alloc memory for array

		for(int i = 0; i < (width * height); i++)	// init array
		{
			RGBArray[i].redVal = 0;
			RGBArray[i].greenVal = 0;
			RGBArray[i].blueVal = 0;
		}
	}




/********************************* Destructor ********************************/
	ImageManipulator::~ImageManipulator()
	{
		//Delete dynamic memory stuff
			delete [] RGBArray;
	}
	



/***************************** Copy Constructor ******************************/
	ImageManipulator::ImageManipulator(const ImageManipulator &im)
	{
		// copies the values of the instantiated object im to the new object
		// this.  


		//copy all relavant values to new object.
		inFileName = im.inFileName;
		magicNumber = im.magicNumber;
		height = im.height;
		width = im.width;
		maxVal = im.maxVal;
	
		// create new Pixel array with size width * height
		RGBArray = new Pixel[width*height];

		//instantiate the new array with a copy of the other objects values
		for(int i = 0; i < (width*height); i++)
		{
			RGBArray[i].redVal = im.RGBArray[i].redVal;
			RGBArray[i].greenVal = im.RGBArray[i].greenVal;
			RGBArray[i].blueVal = im.RGBArray[i].blueVal;
		}
	
	}




/*************************** Operator = Overload *****************************/
	ImageManipulator& ImageManipulator::operator = (const ImageManipulator& im)
	{
		// assings the values of the instantiate object passed to the function
		// and returns this object.

		if(this != &im)		// check to see if the user passed the same exact
		{					// object if not do the assignments
			inFileName = im.inFileName;
			magicNumber = im.magicNumber;
			height = im.height;
			width = im.width;
			maxVal = im.maxVal;

			delete [] RGBArray;		// delete the current array to make room
									// for the new one

			RGBArray = new Pixel[width * height];	//create a new array

			// instantiate the new array with other object's values
			for(int i = 0; i < (width * height); i++) 
			{
				RGBArray[i] = im.RGBArray[i];
			}
		}

		return *this;	// return the object to the caller
	}




/******************************** Filter Image *******************************/
	void ImageManipulator::filterImage(string image, int imageFilter)
	{
		// takes the given string for the output image name and filters the 
		// input image that the object was constructed with. By checking the 
		// bits of the imageFilter integer it runs each filter on the image
		// in a specific order. 


		string tempLine;		// tempLine for getting rid of header
		int aryIndx, bitMask;	//ary index and bitmask for checking filter val
		int tempRed = 0, tempGreen = 0, tempBlue = 0;
		
		currentRow = 0;		// set current row to zero to start at the top of
							// the image.
		
		outFileName = image;		// set the name of the outFile

		outFile.open(outFileName);  // open the outFile for sending values

		
		if(outFile.is_open())		// if outFile is open send it new header
		{
			outFile << magicNumber << "\n";
			outFile << "#Generated by ImageManipulator 2014. ";
			outFile << "Created by Charles Davis\n";
			outFile << width << " " << height << "\n";
			outFile << maxVal << "\n";
		}
		else	
		{
			cerr << "Error opening out file";
			exit(1);
		}

		inFile.open(inFileName);		// open inFile


		if(inFile.is_open())			// if there wasn't an error
		{
			getline(inFile, tempLine); //get magic number out of way	
			getline(inFile, tempLine); //get comments out of the way
			getline(inFile, tempLine);  // get height and width out of the way
			getline(inFile, tempLine); // get maxVal out of the way
		}
		else								// there was an error. exit
		{ 
			cerr << "Error opening file"; 
			exit(1);
		}
		
		cout << "Processing " << getInFileName();
		cout << " to " << outFileName << endl;

		while(currentRow < height)		//for every rown in the image
		{

			// for each column in the row
			 
			for(int currentCol = 0; currentCol < width; currentCol++)
			{
				// treating a 1d array as a 2d calculate the index
				aryIndx = currentRow * width + currentCol;  

				inFile >> tempRed;		// get red value
				inFile >> tempGreen;		// get green value
				inFile >> tempBlue;		// get blue value

				RGBArray[aryIndx].redVal = tempRed;		// set red
				RGBArray[aryIndx].greenVal = tempGreen; // set green
				RGBArray[aryIndx].blueVal = tempBlue;   // set blue

			}

				// for each possible filter

				for(int i = 0; i < NUM_FILTERS; i++)
				{
					//calculate the bitmask by shifting the bit i places
					bitMask = (1 << i);

					// bitwise & will check the imageFilter int for the bitMask

					if(imageFilter & bitMask)
					{
						// if it exists do a switch
						switch(bitMask)
						{
							case ZERO_RED:
								zeroRed();			break;
							case ZERO_GREEN:	
								zeroGreen();			break;
							case ZERO_BLUE:
								zeroBlue();			break;
							case INVERT_RED:	
								invertRed();			break;
							case INVERT_GREEN:
								invertGreen();		break;
							case INVERT_BLUE	:
								invertBlue();		break;
							case AVERAGE_RGB	:
								averageRGB();       break;
							case SWAP_RED_GREEN:	
								swapRG();			break;
							case SWAP_GREEN_BLUE	:
								swapGB();			break;
							case SWAP_BLUE_RED:	
								swapBR();			break;
							case FIESTA_FILTER:
								fiestaFilter();		break;

							default:					break;
						}
				
					}

				}

				// for each column in the current row print out the RGB values
				for(int currentCol = 0; currentCol < width; currentCol++)
				{
					// calculate the 1d version of 2d index
					aryIndx = currentRow * width + currentCol;

					//print out value and a space between them
					outFile << RGBArray[aryIndx].redVal << " ";
					outFile << RGBArray[aryIndx].greenVal << " ";
					outFile << RGBArray[aryIndx].blueVal << " ";


					// for every 5 triplets print out a newline to the file
					if((currentCol+1) % 5 == 0)
					{
						outFile << "\n";
					}

				}

				currentRow++;
		}

		inFile.close();		// close in and out files 
		outFile.close();
	}




/********************************** zeroRed **********************************/
	void ImageManipulator::zeroRed()
	{
		// zeros out the red element of the pixel array

		int currentCol = 0, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			// zero out red value
			RGBArray[aryIndx].redVal = 0;
			currentCol++;
		}
	}




/********************************* zeroGreen *********************************/
	void ImageManipulator::zeroGreen()
	{
		// zeros out the green element of the pixel array

		int currentCol = 0, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			// zero out the green value
			RGBArray[aryIndx].greenVal = 0;
			currentCol++;
		}
	}




/********************************** zeroBlue *********************************/
	void ImageManipulator::zeroBlue()
	{
		// zeros out the blue element of the pixel array

		int currentCol = 0, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			//zero out the blue value
			RGBArray[aryIndx].blueVal = 0;
			currentCol++;
		}	 
	}




/********************************* invertRed *********************************/
	void ImageManipulator::invertRed()
	{
		// inverts the red value of the current row in the array

		
		int currentCol = 0, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			// invert red value.  maxVal - currentRedVal
			RGBArray[aryIndx].redVal = maxVal - RGBArray[aryIndx].redVal;
			currentCol++;
		}	 
	}




/******************************** invertGreen ********************************/
	void ImageManipulator::invertGreen()
	{
		// inverts the green value of the current row in the array

		int currentCol = 0, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentCol * width + currentCol;

			// invert green value. maxVal - currentGreenVal
			RGBArray[aryIndx].greenVal = maxVal - RGBArray[aryIndx].greenVal;
			currentCol++;
		}	 

	}




/********************************* invertBlue ********************************/
	void ImageManipulator::invertBlue()
	{
		// inverts the blue value of the current row in the array

		int currentCol = 0, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			// invert blue value. maxVal - currentBlueVal
			RGBArray[aryIndx].blueVal = maxVal - RGBArray[aryIndx].blueVal;
			currentCol++;
		}	 

	}




/******************************** averageRGB *********************************/
	void ImageManipulator::averageRGB()
	{
		// averages the three color values and assigns them to the current
		// elements in the array.  This creates a grey scale picture

		int currentCol = 0, averageColor, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			// add all three colors and get the average
			averageColor = (RGBArray[aryIndx].redVal + 
							RGBArray[aryIndx].greenVal + 
							RGBArray[aryIndx].blueVal) / 3;


			// assign the average to each position in the pixel array	
			RGBArray[aryIndx].redVal = averageColor; 
			RGBArray[aryIndx].greenVal = averageColor; 
			RGBArray[aryIndx].blueVal = averageColor;

			currentCol++;
		}	 
	}




/********************************** swapRG ***********************************/
	void ImageManipulator::swapRG()
	{
		// swaps the red and green values of the pixels in the array

		int currentCol = 0, swapTemp, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			// swap the red and green values
			swapTemp = RGBArray[aryIndx].redVal;
			RGBArray[aryIndx].redVal = RGBArray[aryIndx].greenVal;
			RGBArray[aryIndx].greenVal = swapTemp;

			currentCol++;
		}

	}




/*********************************** swapGB **********************************/
	void ImageManipulator::swapGB()
	{
		// swaps the green and blue values of the pixels in the array

		int currentCol = 0, swapTemp, aryIndx;

		// for every element the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			// swap the green and blue values
			swapTemp = RGBArray[aryIndx].greenVal;
			RGBArray[aryIndx].greenVal = RGBArray[aryIndx].blueVal;
			RGBArray[aryIndx].blueVal = swapTemp;

			currentCol++;
		}

	}




/********************************** swapBR ***********************************/
	void ImageManipulator::swapBR()
	{
		// swaps the blue and red values of the pixels in the array
		int currentCol = 0, swapTemp, aryIndx;

		// for every element in the current row
		while(currentCol < width)
		{
			aryIndx = currentRow * width + currentCol;

			// swap the blue and red values  
			swapTemp = RGBArray[aryIndx].blueVal;
			RGBArray[aryIndx].blueVal = RGBArray[aryIndx].redVal;
			RGBArray[aryIndx].redVal = swapTemp;

			currentCol++;
		}
	}




/******************************** fiestaFilter *******************************/
	void ImageManipulator::fiestaFilter()
	{
		// this is the custom crazy filter created by me.  It turns the image
		// into a crazy looking grid with 7 rows and 7 columns of different 
		// value manipulations. 


		int tempRed, tempGreen, tempBlue;
		int currentCol = 0, cutOne, seventhCut, aryIndx;

		// cut the width of image into seven columns
		cutOne = width / 7;

		//cut the height of the image into seven rows
		seventhCut = height / 7;


		// for each element in the current row
		while(currentCol < width)
		{

			// if the current row is the first, third, fifth or seventh row
			if(currentRow < seventhCut || 
			  (currentRow >= (seventhCut * 2) && currentRow < (seventhCut * 3)) ||
			  (currentRow >= (seventhCut * 4) && currentRow < (seventhCut * 5)) ||
			  (currentRow >= (seventhCut * 6)) && currentRow < height)
				{

					// swap the blue and red pixels
					swapBR();
				}



			//Red and Green
			// for the first cut
			for(int colCount = 0; colCount < cutOne; colCount++)
			{
				// calculate the current row and column in a 1d array
				aryIndx = currentRow * width + colCount;
				

				// Red

				// add 5 to the red value
				tempRed = RGBArray[aryIndx].redVal + 5;

				// if the add exceeds the max value circle back to begining
				if(tempRed > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].redVal = 0;

					// add the difference
					RGBArray[aryIndx].redVal +=
					(tempRed - maxVal);

				}
				else
				{	// otherwise add 5
					RGBArray[aryIndx].redVal += 5;	
				}

				//Green

				// add 10 to green value
				tempGreen = RGBArray[aryIndx].greenVal + 10;

				// if the add exceeds the max value circle back to begining
				if(tempGreen > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].greenVal = 0;

					// add the difference
					RGBArray[aryIndx].greenVal +=
					(tempGreen - maxVal);
				}
				else
				{	// other wise add 5
					RGBArray[aryIndx].greenVal += 10;	
				}

			}


			//green and blue 
			// for the second cut
			for(int colCount = cutOne; colCount < (cutOne*2); colCount++)
			{
				// calculate the current row and column in 1d array
				aryIndx = currentRow * width + colCount;


				//green

				// add 15 to green value
				tempGreen = RGBArray[aryIndx].greenVal + 15;

				//if the add exceeds the max value circle back to begining
				if(tempGreen > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].greenVal = 0;

					// add the difference
					RGBArray[aryIndx].greenVal +=
					(tempGreen - maxVal);

				}
				else
				{	// otherwise add 15
					RGBArray[aryIndx].greenVal += 15;	
				}


				//blue

				// add 20 to the blue value
				tempBlue = RGBArray[aryIndx].blueVal + 20;

				// if the add exceeds the max value circle back to the begining
				if(tempBlue > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].blueVal = 0;

					// add the difference
					RGBArray[aryIndx].blueVal +=
					(tempBlue - maxVal);
				}
				else
				{	// otheriwse add 20
					RGBArray[aryIndx].blueVal += 20;	
				}

			}


			//blue and red for the third cut
			for(int colCount = (cutOne*2); colCount < (cutOne*3); colCount++)
			{
				// calculate the row and column in the 1d array.
				aryIndx = currentRow * width + colCount;

				//blue

				// add 25 to the blue value
				tempBlue = RGBArray[aryIndx].blueVal + 25;

				// if add exceed the max value circle back to 0
				if(tempBlue > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].blueVal = 0;

					// add the difference
					RGBArray[aryIndx].blueVal +=
					(tempBlue - maxVal);

				}
				else
				{   // otherwise add 25
					RGBArray[aryIndx].blueVal += 25;	
				}

				//red
				// add 30 to red value
				tempRed = RGBArray[aryIndx].redVal + 30;

				// if add exceeds the max value circle back to begining
				if(tempRed > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].redVal = 0;

					// add the difference
					RGBArray[aryIndx].redVal +=
					(tempRed - maxVal);
				}
				else
				{   // otherwise add 30
					RGBArray[aryIndx].redVal += 30;	
				}

			}
				
			// for the 4th cut red and green
			for(int colCount = (cutOne*3); colCount < (cutOne*4); colCount++)
			{
				// calculate the row and column in the 1d array.
				aryIndx = currentRow * width + colCount;


				//red
				// add 15 to red value
				tempRed = RGBArray[aryIndx].redVal + 15;

				// if add exceeds max value circle back to begining
				if(tempRed > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].redVal = 0;

					// add the difference
					RGBArray[aryIndx].redVal +=
					(tempRed - maxVal);
				}
				else
				{	// otherwise add 15
					RGBArray[aryIndx].redVal += 15;	
				}


				//green
				// add 30 to green value
				tempGreen= RGBArray[aryIndx].greenVal + 30;

				// if add exceeds the max value circle back to the begining
				if(tempGreen > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].greenVal = 0;

					// add the difference
					RGBArray[aryIndx].greenVal +=
					(tempGreen - maxVal);

				}
				else
				{	// otherwise add 30
					RGBArray[aryIndx].greenVal += 30;	
				}


				//blue
				// add 45 to the blue
				tempBlue = RGBArray[aryIndx].blueVal + 45;

				// if add exceeds the max value circle back the begining
				if(tempBlue > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].blueVal = 0;

					// add the difference
					RGBArray[aryIndx].blueVal +=
					(tempBlue - maxVal);

				}
				else
				{	// otherwise just add 45
					RGBArray[aryIndx].blueVal += 45;	
				}


			}

			//blue and red for the 5th cut
			for(int colCount = (cutOne*4); colCount < (cutOne*5); colCount++)
			{
				// calculate the row in column in 1d like it is a 2d array
				aryIndx = currentRow * width + colCount;

				//blue
				// add 25 to blue value
				tempBlue = RGBArray[aryIndx].blueVal + 25;

				// if add exceeds the max value circle back to the begining
				if(tempBlue > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].blueVal = 0;

					// add the difference
					RGBArray[aryIndx].blueVal +=
					(tempBlue - maxVal);

				}
				else
				{	// otherwise add 25
					RGBArray[aryIndx].blueVal += 25;	
				}

				//red
				// add 30 to the red value
				tempRed = RGBArray[aryIndx].redVal + 30;

				// if add exceeds the max value circle back to the begining
				if(tempRed > maxVal)
				{
					// set it to 0 
					RGBArray[aryIndx].redVal = 0;
					
					// add the difference
					RGBArray[aryIndx].redVal +=
					(tempRed - maxVal);
				}
				else
				{	// otherwise add 30
					RGBArray[aryIndx].redVal += 30;	
				}

			}

			//green and blue forthe 6th cut
			for(int colCount = (cutOne*5); colCount < (cutOne*6); colCount++)
			{
				// calculate the row and column in a 1d array as a 2d
				aryIndx = currentRow * width + colCount;

				//green
				// add 15 to the green value
				tempGreen = RGBArray[aryIndx].greenVal + 15;

				// if add exceeds the max value circle back to the begining
				if(tempGreen > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].greenVal = 0;

					// add the difference
					RGBArray[aryIndx].greenVal +=
					(tempGreen - maxVal);

				}
				else
				{	// otherwise add 15
					RGBArray[aryIndx].greenVal += 15;	
				}

				//blue
				// add 20 to the blue value
				tempBlue = RGBArray[aryIndx].blueVal + 20;

				// if add exceeds max value circle back to the begining
				if(tempBlue > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].blueVal = 0;

					// add the difference
					RGBArray[aryIndx].blueVal +=
					(tempBlue - maxVal);
				}
				else
				{	// otherwise add 20
					RGBArray[aryIndx].blueVal += 20;	
				}

			}

			// for the last cut red and green
			for(int colCount = (cutOne*6); colCount < width; colCount++)
			{
				// calculate the row in coloumn in 1d as if it were a 2d
				aryIndx = currentRow * width + colCount;

				// Red
				// add 5 to the red value
				tempRed = RGBArray[aryIndx].redVal + 5;

				// if add exceeds the max value circle back to the begining
				if(tempRed > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].redVal = 0;

					// add the difference
					RGBArray[aryIndx].redVal +=
					(tempRed - maxVal);

				}
				else
				{	// otherwise add 5
					RGBArray[aryIndx].redVal += 5;	
				}


				//Green
				// add 10 to the green value
				tempGreen = RGBArray[aryIndx].greenVal + 10;

				// if add exceeds max value circle back to the begining
				if(tempGreen > maxVal)
				{
					// set it to 0
					RGBArray[aryIndx].greenVal = 0;

					// add the difference
					RGBArray[aryIndx].greenVal +=
					(tempGreen - maxVal);
				}
				else
				{	// otherwise add 10
					RGBArray[aryIndx].greenVal += 10;	
				}

			}

			currentCol++;	// next column element in row
		}

		// for the whole row
		for(int colCount = 0; colCount < width; colCount++)
		{
			// calculate the row and column in 1d array as 2d
			aryIndx = currentRow * width + colCount;

			// add 75 to all three
			tempRed = RGBArray[aryIndx].redVal + 75;
			tempGreen = RGBArray[aryIndx].greenVal + 75; 
			tempBlue = RGBArray[aryIndx].blueVal + 75;
		
			// cap all the values at max value
			if(tempRed >= maxVal)
			{ 
				RGBArray[aryIndx].redVal = maxVal;
			}
			if(tempGreen >= maxVal)
			{
				RGBArray[aryIndx].greenVal = maxVal;
			}
			if(tempBlue >= maxVal)
			{
				RGBArray[aryIndx].blueVal = maxVal;
			}

		}

	}





/******************************* GETTERS *************************************/



/******************************* getInFileName *******************************/
	string ImageManipulator::getInFileName() const { return inFileName; }




/******************************* getOutFileName ******************************/
	string ImageManipulator::getOutFileName() const { return outFileName; }




/******************************* getMagicNumber ******************************/
	string ImageManipulator::getMagicNumber() const { return magicNumber; }




/********************************* getHeight *********************************/
	int ImageManipulator::getHeight() const { return height; }




/********************************** getWidth *********************************/
	int ImageManipulator::getWidth() const { return width; } 




/********************************* getMaxVal *********************************/
	int ImageManipulator::getMaxVal() const { return maxVal; }





/******************************* getCurrentRow *******************************/
	int ImageManipulator::getCurrentRow () const { return currentRow; }


