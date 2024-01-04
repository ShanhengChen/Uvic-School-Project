/* PixelWriter.java
   CSC 225 - Summer 2019

   This file defines an interface for setting the colour values of pixels
   in the image. It is not necessary to implement this interface yourself 
   (your code in A3Algorithms.java will take instances of this interface
    as parameters)

   B. Bird - 07/06/2019
*/ 

import java.awt.Color;

public interface PixelWriter{
	/* setPixel(x,y,c)
	   Set the pixel at coordinates (x,y) in the image
	   to the provided color.
	*/
	void setPixel(int x, int y, Color c);
}