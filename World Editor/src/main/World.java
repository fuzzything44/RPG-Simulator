package main;

import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

import org.newdawn.slick.Image;


public class World {

	public LinkedList<LinkedList<WorldChunk>> chunks = new LinkedList<LinkedList<WorldChunk>>();
	public int chunkResX = 50;
	public int chunkResY = 50;

	public World() {
		
	}
	
	public void save(String saveFolder) {
		BufferedImage backgroundSave = new BufferedImage(chunkResX * chunks.size(), chunkResY * chunks.getFirst().size(), BufferedImage.TYPE_INT_RGB);

		Map<Image, Integer> imageMap = new HashMap<Image, Integer>();
		// Looping through every chunk and then all background parts of the chunks.
		for (int x = 0; x < chunks.size(); x++) {
			for (int y = 0; y < chunks.get(x).size(); y++) {
				// These two looped through the chunks.
				
				for (int chunkX = 0; chunkX < chunkResX; chunkX++) {
					for (int chunkY = 0; chunkY < chunkResY; chunkY++) {
						// End of the mass of loops. These two looped through the background chunk arrays.
						
						if (imageMap.get(chunks.get(x).get(y).background[chunkX][chunkY]) != null) {
							// If the image is not new.
							backgroundSave.setRGB( (x * chunkResX) + chunkX,// x is what chunk so this gives the actor absolute position
									(y * chunkResY) + chunkY,	// gives actor absolute y value
									imageMap.get(
											chunks.get(x).get(y).background[chunkX][chunkY]	// Gives the background image at the coordinate.
											));
						} else {
							
							boolean pixelAdded = false;

							while (!pixelAdded) {
								int r = (int) Math.round(Math.random() * 255);
								int g = (int) Math.round(Math.random() * 255);
								int b = (int) Math.round(Math.random() * 255);
								int rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);
								// Creates a random color as an int.
								
								if (!imageMap.containsValue(rgb) ) {
									// If the color hasn't been used yet.
									backgroundSave.setRGB( (x * chunkResX) + chunkX, (y * chunkResY) + chunkY, rgb);
									// Add the pixel to the image.
									
									imageMap.put(chunks.get(x).get(y).background[chunkX][chunkY], rgb);
									// This color is now keyed. 
								}
							}// End while
							
						}// End if/else
					}// End chunkY for
				}
			}// End y for
		}// End x for
		// TODO: Save Image here
		
		// TODO: Create text file here.
		try {
			BufferedWriter textFile = new BufferedWriter(new FileWriter(saveFolder + "main.world") );
			textFile.write("Now we save the text file!");
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}// End save
}
