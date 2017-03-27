/******************************
 **	  ImageManipulator.h		 **
 **	Created by Charles Davis **
 **		CPSC 246-01			 **
 ******************************/

/******************************************************************************
 ** ImageManipulator.h is the header	 file for the ImageManipulator class	.	 **
 ** It has 11 filters that manipulate the colors of the given image			 **
 ** It has a function called filterImage(string image, int imageFilter) that **
 ** takes the input file, changes the color value based on the inputed image **
 ** filter integer and spits out the finished image to the given string		 **
 ** file name.  In this file there is a list of constant ints that a user    **
 ** can send through the image filter function.  They will be ran in this    **
 ** order that follows:														 **
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


#ifndef IMAGE_MANIPULATOR
#define IMAGE_MANIPULATOR

#include <string>
#include <fstream>

using namespace std;

class ImageManipulator
{
	/**************** Public constants for filter values *********************/

	public:
		const static int ZERO_RED			= 1;
		const static int ZERO_GREEN			= 2;
		const static int ZERO_BLUE			= 4;
		const static int INVERT_RED			= 8;
		const static int INVERT_GREEN		= 16;
		const static int INVERT_BLUE			= 32;
		const static int AVERAGE_RGB			= 64;
		const static int SWAP_RED_GREEN		= 128;
		const static int SWAP_GREEN_BLUE		= 256;
		const static int SWAP_BLUE_RED		= 512;
		const static int FIESTA_FILTER		= 1024;

		// Number of filters for the filterImage function to utilize
		const static int NUM_FILTERS			= 11;


		//Constructors

		ImageManipulator(string file);
	 /*************************************************************************
	  **	  Receives: nothing					     							 **
      **  Returns:  nothing												     **
      **																		 **
	  **	  Instatiates a new ImageManipulator object with all the attributes  **
	  **	  of the inputed file with string name.  It validates the header to  **
	  **	  makes sure that it is a legal PPM format file and that it is a P3  **
	  **	  magic number.  See the NetPBM wikipedia page for more information  **
	  **  about the PPM file format.										     **
	  ************************************************************************/

		~ImageManipulator();
	 /*************************************************************************
	  ** Receives: nothing                                                   **
	  ** Returns:  nothing                                                   **
	  **																	     **
	  ** The deconstructor for the ImageManipulator object.  It deletes the  **
	  ** dynamic memory attributes of the instantiated object.               **
	  ************************************************************************/

		ImageManipulator(const ImageManipulator &im);
	 /*************************************************************************
	  ** Receives: a reference to an ImageManipulator object				     **
	  ** Return:	   nothing                                                   **
	  **																		 **
	  ** Special copy constructor that copies all of the attributes of the   **
	  ** given object and instaniates a new object with all of the same		 **
	  ** values.																 **
	  ************************************************************************/

		ImageManipulator& operator = (const ImageManipulator& im);
	 /*************************************************************************
	  ** Receives: a reference to an ImageManipulator object                 **
	  ** Returns:  a reference to an ImageManipulator object                 **
	  **																	     **
	  ** Overloads the equal sign operator.  It assigns the passed object to **
	  ** its caller.  If both are exactly the same it just returns itself.   **
	  ************************************************************************/


		//main function

		void filterImage(string image, int imageFilter);
	 /*************************************************************************
	  ** Receives: a string and an int										 **
	  ** Returns:  nothing												     **
	  **																		 **
	  ** This function takes the objects input image and runs a series of    **
	  ** filters on it to change the color values.  It then sends all of the **
	  ** manipulated values to a new image file specified by the user.	     **
	  ************************************************************************/


	 /******************************* FILTERS *********************************
	  ** All filters receive nothing and return nothing.						 **
	  ************************************************************************/

		void zeroRed();		// zeros the red values of the given image
		void zeroGreen();	// zeros the green values of the given image
		void zeroBlue();    // zeros the blue values of the given image

		void invertRed();   // invert the red values of the given image
		void invertGreen(); // invert the green values of the given image
		void invertBlue();  // invert the blue values of the given image

		void averageRGB();  // averages the values of the given image
							// this creates a greyscale picture

		void swapRG();      // swaps the red and green values of the image
		void swapGB();	    // swaps the green and blue values of the image
		void swapBR();		// swaps the blue and red values of the image


		void fiestaFilter(); 
	 /*************************************************************************
	  ** Receives: nothing												     **
	  ** Returns:  nothing												     **
	  **																		 **
	  ** This is a proprietary filter created by the developer that does	     **
	  ** some funky things to the image.  It creates a grid of crazy color   **
	  ** changes.															 **
	  ************************************************************************/


		//accessors
		string getInFileName() const;	// returns inFileName of the instance
		string getOutFileName() const;  // returns outFileName of the instance
		string getMagicNumber() const;  // returns magic number of the instance
		int getHeight() const;			// returns pixel height of the instance
		int getWidth() const;			// returns pixel width of the instance
		int getMaxVal() const;          // returns max color of the instance
		int getCurrentRow() const;      // returns current row being processed
	

	private:

		ifstream inFile;		//Input File Stream
		ofstream outFile;	//Output File Stream


		string inFileName, outFileName;	
		string magicNumber;				//Code indicating the ppm file type

		//Pixel values of picture size, max color value and curr row for processing
		int height, width, maxVal, currentRow; 

		struct Pixel			
		{					//struct holding the 	
			int redVal;		//RGB values of the picture
			int greenVal;
			int blueVal;
		};

		Pixel* RGBArray;		//pointer to the first element in array of Pixels
};

#endif