/*
 * PixelVertex.java 
 * Used for Assignment 6 in CSC 225 - Summer 2020
 * You do not need to change this file.
 * A. Estey
 */

import java.awt.Color;
import java.util.LinkedList;

public class PixelVertex{

	private int x;
	private int y;
	private LinkedList<PixelVertex> neighbours;
	
	public PixelVertex(int x, int y) {
		this.x = x;
		this.y = y;
		neighbours = new LinkedList<PixelVertex>();
	}
	
	/* getX()
	   Return the x-coordinate of the pixel corresponding to this vertex.
	*/
	public int getX(){
		return this.x;
	}
	
	/* getY()
	   Return the y-coordinate of the pixel corresponding to this vertex.
	*/
	public int getY(){
		return this.y;
	}
	
	/* getNeighbours()
	   Return an array containing references to all neighbours of this vertex.
	   The size of the array must be equal to the degree of this vertex (and
	   the array may therefore contain no duplicates).
	*/
	public LinkedList<PixelVertex> getNeighbours(){
		return this.neighbours;
	}
		
	/* addNeighbour(newNeighbour)
	   Add the provided vertex as a neighbour of this vertex.
	*/
	public void addNeighbour(PixelVertex newNeighbour){
		if (!neighbours.contains(newNeighbour)) {
			neighbours.add(newNeighbour);
		}
	}
	
	/* removeNeighbour(neighbour)
	   If the provided vertex object is a neighbour of this vertex,
	   remove it from the list of neighbours.
	*/
	public void removeNeighbour(PixelVertex neighbour){
		neighbours.remove(neighbour);
	}
	
	/* getDegree()
	   Return the degree of this vertex. Since the graph is simple, 
	   the degree is equal to the number of distinct neighbours of this vertex.
	*/
	public int getDegree(){
		return neighbours.size();
	}
	
	/* isNeighbour(otherVertex)
	   Return true if the provided PixelVertex object is a neighbour of this
	   vertex and false otherwise.
	*/
	public boolean isNeighbour(PixelVertex otherVertex){
		return neighbours.contains(otherVertex);
	}
	
	public boolean equals(Object other) {
		if (other instanceof PixelVertex) {
			PixelVertex o = (PixelVertex) other;
			return this.x == o.getX() && this.y == o.getY();
		}
		return false;
	}
	
}