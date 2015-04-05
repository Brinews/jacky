/*
 * Based on i/o subroutines in C provided by G. Gimel'farb
 * Ported to Java by Andrew Probert: 10 March 2011
 * 
 * Updated by jnie014 to read files as bytes, rather than text: 16 March 2011
 * 
 * Functions for reading in and saving grayscale PGM and colour PPM format files.
 * 
 */


public class PGM_PPM_Handler {

	/*
	 **     i/o images in pgm/ppm format
	 */

	/*
	 *   name -- an image file specification
	 *   id -- the type of image (an output external variable)
	 *   row  -- the number of image rows (an output external variable) 
	 *   col  -- the number of image columns (an output external variable) 
	 *--------------------------------------------------------------------------
	 *  Terminates the program if the file can't be open or is not in pgm format 
	 */
	
	static char[] readFilePGM_PPM( String name, int [] id,
			int[] row, int[] col ) {
		String str;
		java.io.BufferedReader bReader;
		int i, j, k, k2;
		char[] crowstr = new char[10];
		for (i = 0; i < 10; i++) crowstr[i] = 0;
		char [] ccolstr = new char[10];
		for (i = 0; i < 10; i++) ccolstr[i] = 0;
		String rowstr;
		String colstr;
		int base, maxpts, column, line;
		char[] image;

		byte[] asBytes = new byte[10000]; // Store all data read from file as bytes
		java.io.FileInputStream fis = null;
		java.io.BufferedInputStream bis = null;
		int byteCount = 0; // Counts the number of bytes in the header information
		int fileSize = 0; // Stores the size of the whole file
		
		try {
			fis = new java.io.FileInputStream(name);
			bis = new java.io.BufferedInputStream(fis);
			fileSize = fis.available();
			//System.out.println("Total number of bytes in file: "+fileSize);
			asBytes = new byte[fileSize];
			fis.read(asBytes);

		} catch (Exception e) { System.out.println("Error: "+e.getMessage());
		} finally {
			try {
				fis.close();
				bis.close();
			} catch (Exception e) {}
		}
		
		java.io.ByteArrayInputStream bais = new java.io.ByteArrayInputStream(asBytes); // A stream of bytes, created from the byte[] above 
		
		try {
			bReader = new java.io.BufferedReader(new java.io.InputStreamReader(bais));

			str = bReader.readLine(); byteCount += str.getBytes().length; byteCount++;
			str = str.trim();
			if ( !str.equals("P5") && !str.equals("P6")) {
				System.out.println( "\n readFilePGM_PPM(): file " + name + " is neither a PGM nor a PPM file; its magic key is " + str);
				return null;
			}

			if (str.equals("P5")) id[0] = 1;
			if (str.equals("P6")) id[0] = 3;

			do { str = bReader.readLine(); byteCount += str.getBytes().length; byteCount++;} while( str.charAt(0)=='#' ); 
			//System.out.println(str);

			k = 0;
			char temp = str.charAt(k);
			do { temp = str.charAt(k); 
			ccolstr[k] = temp;
			k++;
			} while( temp != ' ' );
			ccolstr[k]='\0';
			colstr = String.valueOf(ccolstr).trim();
			//System.out.println(colstr);

			//k++;
			k2=0;
			temp = str.charAt(k2 + k);
			do{ temp = str.charAt(k2 + k);
			crowstr[k2] = temp;
			k2++;
			} while( (k2 + k) < str.length() );
			crowstr[k2] = '\0';
			rowstr = String.valueOf(crowstr).trim();
			//System.out.println(rowstr);

			line = row[0] = Integer.valueOf( String.valueOf(rowstr) ).intValue();
			column = col[0] = Integer.valueOf( String.valueOf(colstr) ).intValue();

			//maxpts= (row[0]) * column;

			if ( id[0] == 3 ) column *= 3;
			maxpts= line * column;		    

			image = new char[maxpts];

			str = bReader.readLine(); byteCount += str.getBytes().length; byteCount++;

			// Start reading byte values of image
			bais = new java.io.ByteArrayInputStream(asBytes, byteCount, fileSize);
			bis = new java.io.BufferedInputStream(bais);
			for( i = 0, base = 0; i < line; i++, base += column ) {
				for( j = 0; j < column; j++ ) {
					image[ base + j ] = (char)bis.read();
				}
				//System.out.print("\n");
			}
		} catch (Exception e) {
			System.out.println("Error: "+e.getMessage());
			return null;
		} finally {
			try {
				bais.close();
			} catch (Exception e) {}
		}
		//System.out.println(name+" read successfully. Header size was: "+byteCount+". Data size was: "+(fileSize-byteCount));
		return image;
	}

	/*
	 *   name  -- image file specification
	 *   image -- image array to output
	 *   id    -- type of image
	 *   row   -- number of image rows 
	 *   col   -- number of image columns 
	 *-----------------------------------------------------------------
	 *  Terminates the program if the file can't be open 
	 */
	static int saveFilePGM_PPM( String name, char[] image, int id,
			int row, int col) {
		
		java.io.FileOutputStream fos = null; // Writes as bytes, rather than as text
		String output;
		int base, i, j, len;

		if ( ( id != 1 ) && (id != 3 ) ) {
			System.out.println( "saveFilePGM_PPM(): wrong image identifier; can't write into the file " + name );
			return -1;
		}	    

		try {
			fos = new java.io.FileOutputStream(name);
			

			if (id == 1)
				output = ("P5\n" + col + " " + row + "\n255\n");
			else
				output = ("P6\n" + col + " " + row + "\n255\n");
			fos.write(output.getBytes());
			
			len = col;
			if (id == 3) len *= 3;
			base = 0;
			for( i = 0; i < row; i++ ) {
				for( j = 0; j < len; j++) {
					fos.write(image[ base + j ]); // Write the char out to the file (converts to byte)
				}
				base += len;
			}
			fos.close();
		}
		catch (java.io.IOException ioe) {
			System.out.println( "\n readFilePGM(): error opening file " + name ); 
			return 0;
		}


		return( 1 );
	}
}
