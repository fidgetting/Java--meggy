/**
 * PA7GameOfLife.java
 * 
 * Canonical example for PA7.
 * 
 * The Game of Life is a popular celluar automaton.
 * You have a 2D grid of pixels that are either on or off.
 * For the next time step, a pixel counts the number of its neighbors
 * (all 8 since including diagonals) that are lit.  Based on that
 * number and the current value of the pixel, the next value of the
 * pixel is determined as follows:
 *      if the number of neighbors lit is less than 2 then
 *          pixel is turned off
 *      if the number of neighbors is 2 then
 *          pixel keeps current value
 *      if the number of neighbors lit is 3 exactly then
 *          pixel is turned on
 *      if the number of neighbors lit is over 3 then
 *          pixel is turned off
 * 
 * This program sets some initial set of pixels to on.
 * The auxiliary lights are used to indicate which columns had 
 * one or more lights kept or turned on.  
 * Each auxiliary light corresponds to a column.
 * 
 * New language features tested since PA6:
 *  -arrays
 *  -auxiliary lights, setAuxLEDs()
 *
 * MMS, 1/13/11, 2/21/11
 */

import meggy.Meggy;

class PA7GameOfLife {

  public static void main(String[] whatever){
    // Only want to construct one instance of class
    new GameOfLifeBoard().run();
  }
}

class GameOfLifeBoard {
  // When initializing off keep in mind that all of the
  // pixels on the board start at DARK by default.
  Meggy.Color on;
  Meggy.Color off;
  Meggy.Color [] board;


  // no initial values, nothing should happen
  public void initScreen1() {
    on = Meggy.Color.WHITE;
    off = Meggy.Color.DARK;
  }


  // all lights on, they should all turn off except
  // for the corners, which will turn off the next time step
  public void initFullScreen() {
    byte i;

    on = Meggy.Color.WHITE;
    off = Meggy.Color.DARK;
    i=(byte)0;
    while (i<(byte)8) {
      this.mazeRow((byte)0, (byte)7, i);
      i = (byte)(i+(byte)1);
    }
  }

  // blinker
  public void initBlinker() {
    on = Meggy.Color.VIOLET;
    off = Meggy.Color.DARK;
    Meggy.setPixel((byte)3, (byte)3, on);
    Meggy.setPixel((byte)3, (byte)4, on);
    Meggy.setPixel((byte)3, (byte)5, on);
  }


  // Run the Game of Life simulation
  public void run() {
    int x;
    int y;
    int numNeigh;
    int [] numOn;
    int bitarray;
    int i;

    board = new Meggy.Color [ (byte)8*(byte)8 ];
    numOn = new int [8];
    
    i = board.length;

    // Call whichever initial board desired
    this.initBlinker();

    while (true) {
      Meggy.delay(256);

      // copy values for board from the Meggy screen
      this.copyFromScreen();

      // Initialize number of lights turned on
      x=0;
      while (x<8) {
        numOn[x] = 0;
        x = x+1;
      }

      // Loop over screen to determine new x,y values.
      // Place new x,y value in board.
      x = 0;
      while (x<8) {
        y = 0;
        while (y<8) {
          numNeigh = this.numNeigh(x, y);
          if (numNeigh < 2) {
            Meggy.setPixel((byte)x,(byte)y,off);
          } else {
            if (numNeigh==3) {
              Meggy.setPixel((byte)x,(byte)y,on);
              numOn[x] = numOn[x] + 1;
            } else {
              if (3 < numNeigh) {
                Meggy.setPixel((byte)x,(byte)y,off);
              } else {
                // Check if this pixel is staying on.
                if (Meggy.getPixel((byte)x, (byte)y)==on) {
                  numOn[x] = numOn[x] + 1;
                } else {
                  // nothing happens
                }
              }
            }
          }
          y = y+1;
        }
        x = x+1;
      }

      // Set auxiliary light if the column it is associated with had 
      // 1 or more lights turned on or staying on.
      x = 0;
      bitarray = 0;
      while (x<8) {
        if (0<numOn[x]) {
          bitarray = bitarray + this.powerOf2(x);
        } else { 
        }
        x = x+1;
      }
      Meggy.setAuxLEDs(bitarray);
    }
  }

  // Returns 2^x.
  public int powerOf2(int x) {
    int i;
    int retval;
    i=0;
    retval = 1;
    while (i<x) {
      retval = (byte)retval*(byte)2;
      i = i+1;
    }
    return retval;
  }

  public boolean inBounds(int x, int y) {
    return ((0-1) < x) && (x < 8) && ((0-1) < y) && (y < 8);
  }

  public boolean isOn(int x, int y) {
    return this.inBounds(x,y) && this.getPixel(x,y)==on;
  }

  // Counts the number of neighbors for x,y in the screen that are on.
  public int numNeigh(int x, int y) {
    int retval;

    retval = 0;

    // ============= Start at top neighbor and go clockwise.
    // north
    if (this.isOn(x,y+1)) { retval = retval + 1; } else {}
    // northeast
    if (this.isOn(x+1,y+1)) { retval = retval + 1; } else {}
    // east
    if (this.isOn(x+1,y)) { retval = retval + 1; } else {}
    // southeast
    if (this.isOn(x+1,y-1)) { retval = retval + 1; } else {}
    // south
    if (this.isOn(x,y-1)) { retval = retval + 1; } else {}
    // southwest
    if (this.isOn(x-1,y-1)) { retval = retval + 1; } else {}
    // west
    if (this.isOn(x-1,y-0)) { retval = retval + 1; } else {}
    // northwest
    if (this.isOn(x-1,y+1)) { retval = retval + 1; } else {}

    return retval;
  }

  public void setPixel(int x, int y, Meggy.Color c) {
    board[(byte)x*(byte)8+y] = c;
  }

  public Meggy.Color getPixel(int x, int y) {
    return board[(byte)x*(byte)8+y];
  }

  public void copyFromScreen() {
    int x;
    int y;
    x = 0;
    while (x<8) {
      y = 0;
      while (y<8) {
        this.setPixel(x, y, Meggy.getPixel((byte)x, (byte)y));
        y = y+1;
      }
      x = x+1;
    }
  }

  // Starting and stopping at the given columns,
  // make the given row a maze border.
  // Assuming that startcol, endcol, and row are in bounds.
  public void mazeRow(byte startcol, byte endcol, byte row) {
    Meggy.setPixel(startcol, row, on);
    if (startcol<endcol) {
      this.mazeRow((byte)(startcol+(byte)1), endcol, row);
    } else {}
  }

  // Starting and stopping at the given rows,
  // make the given col a maze border.
  // Assuming that startrow, endrow, and col are in bounds.
  public void mazeCol(byte col, byte startrow, byte endrow) {
    Meggy.setPixel(col, startrow, on);
    if (startrow<endrow) {
      this.mazeRow(col,(byte)(startrow+(byte)1), endrow);
    } else {}
  }

}