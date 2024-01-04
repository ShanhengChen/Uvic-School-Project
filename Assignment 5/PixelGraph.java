/*
 * PixelGraph.java 
 * Used for Assignment 6 in CSC 225 - Summer 2020
 * You do not need to change this file.
 * A. Estey
 */

import java.awt.Color;

public class PixelGraph{

	PixelVertex[][] data;
	
	/* PixelGraph constructor
	   Given a 2d array of colour values (where element [x][y] is the colour 
	   of the pixel at position (x,y) in the image), this initializes a matrix
	   of PixelVertex's corresponding to each pixel in the image. The neighbours
	   of each vertex are adjacent vertices of the SAME colour.
	*/
	public PixelGraph(Color[][] imagePixels){
		int height = imagePixels.length;
		int width = imagePixels[0].length;
		
		data = new PixelVertex[height][width];
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				data[y][x] = new PixelVertex(x, y);
			}
		}
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				addNeighbours(x, y, imagePixels);
			}
		}	
		
	}
	
	/* addNeighbours(x, y, imagePixels)
	   adds neighbours to the PixelVertex at coordings (x,y)
	   Adjacent vertices are only added as neighbours if they are the
	   SAME colour as the vertex.
	*/
	public void addNeighbours(int x, int y, Color[][] imagePixels) {
		if (y > 0 && imagePixels[y-1][x].equals(imagePixels[y][x])) {
			data[y][x].addNeighbour(data[y-1][x]);
		}
		if (x > 0 && imagePixels[y][x-1].equals(imagePixels[y][x])) {
			data[y][x].addNeighbour(data[y][x-1]);
		}
		if (y+1 < imagePixels.length && imagePixels[y+1][x].equals(imagePixels[y][x])) {
			data[y][x].addNeighbour(data[y+1][x]);
		}
		if (x+1 < imagePixels[y].length && imagePixels[y][x+1].equals(imagePixels[y][x])) {
			data[y][x].addNeighbour(data[y][x+1]);
		}
	}
	
	/* getPixelVertex(x,y)
	   Given an (x,y) coordinate pair, return the PixelVertex object 
	   corresponding to the pixel at the provided coordinates.
	   This method is not required to perform any error checking (and you may
	   assume that the provided (x,y) pair is always a valid point in the 
	   image).
	*/
	public PixelVertex getPixelVertex(int x, int y){
		return data[y][x];
	}
	
	/* getWidth()
	   Return the width of the image corresponding to this PixelGraph 
	   object.
	*/
	public int getWidth(){
		if (data.length > 0) {
			return data[0].length;
		}
		return 0;
	}
	
	/* getHeight()
	   Return the height of the image corresponding to this PixelGraph 
	   object.
	*/
	public int getHeight(){
		return data.length;
	}
	
}