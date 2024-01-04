/*

*/ 

import java.awt.Color;
import java.util.*;

public class GraphAlgorithms{

	/* FloodFillDFS(v, writer, fillColour)
	   Traverse the component the vertex v using DFS and set the colour 
	   of the pixels corresponding to all vertices encountered during the 
	   traversal to fillColour.
	   
	   To change the colour of a pixel at position (x,y) in the image to a 
	   colour c, use
			writer.setPixel(x,y,c);
	*/
	public static void FloodFillDFS(PixelVertex v, PixelWriter writer, Color fillColour){
		Stack<PixelVertex> st = new Stack<PixelVertex>();
		ArrayList<PixelVertex> visit = new ArrayList<PixelVertex>();
		writer.setPixel(v.getX(), v.getY(), fillColour);
		st.push(v);
		while (!st.isEmpty()){
			PixelVertex cur = st.pop();
			if (!visit.contains(cur)){
				visit.add(cur);
				writer.setPixel(cur.getX(), cur.getY(), fillColour);
				for(int i = 0; i < cur.getNeighbours().size(); i++){		
					st.push(cur.getNeighbours().get(i));
				}
			}
		}
		
	}
	
	/* FloodFillBFS(v, writer, fillColour)
	   Traverse the component the vertex v using BFS and set the colour 
	   of the pixels corresponding to all vertices encountered during the 
	   traversal to fillColour.
	   
	   To change the colour of a pixel at position (x,y) in the image to a 
	   colour c, use
			writer.setPixel(x,y,c);
	*/
	public static void FloodFillBFS(PixelVertex v, PixelWriter writer, Color fillColour){
		Queue<PixelVertex> st = new LinkedList<PixelVertex>();
		ArrayList<PixelVertex> visit = new ArrayList<PixelVertex>();
		writer.setPixel(v.getX(), v.getY(), fillColour);
		st.add(v);

		while (!st.isEmpty()){
			PixelVertex cur = st.remove();
			if (!visit.contains(cur)){
				visit.add(cur);
				writer.setPixel(cur.getX(), cur.getY(), fillColour);
				for(int i = 0; i < cur.getNeighbours().size(); i++){		
					st.add(cur.getNeighbours().get(i));
				}
			}
		}
	}
	
}