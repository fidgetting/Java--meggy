
import meggy.Meggy;

class PA5Cylon {

  public static void main(String[] whatever){
    // Not actually constructing object in PA5 compiler,
    {
      Meggy.setPixel((byte) 0, (byte) 0, Meggy.Color.RED);

      while (true) {

        if(Meggy.checkButton(Meggy.Button.Up)){
          if(Meggy.checkButton(Meggy.Button.Right)){
            Meggy.setPixel((byte) 0, (byte) 0, Meggy.Color.DARK);
            Meggy.setPixel((byte) 0, (byte) 7, Meggy.Color.DARK);
            new Cylon().pattern((byte) 4);
          } else {
            Meggy.setPixel((byte) 0, (byte) 0, Meggy.Color.DARK);
            Meggy.setPixel((byte) 0, (byte) 4, Meggy.Color.DARK);
            new Cylon().pattern((byte) 7);
          }
        } else {

          if(Meggy.checkButton(Meggy.Button.Down)){
            Meggy.setPixel((byte) 0, (byte) 4, Meggy.Color.DARK);
            Meggy.setPixel((byte) 0, (byte) 7, Meggy.Color.DARK);
            new Cylon().pattern((byte) 0);
          } else {

            //no button clicked, so repeat prev cylon

            if(Meggy.getPixel((byte)0, (byte)0) == Meggy.Color.RED){
              new Cylon().pattern((byte) 0);
            } else {
              if(Meggy.getPixel((byte)0, (byte)4) == Meggy.Color.RED){
                new Cylon().pattern((byte) 4);
              } else {
                if(Meggy.getPixel((byte)0, (byte)7) == Meggy.Color.RED){
                  new Cylon().pattern((byte) 7);
                } else {}
              }
            }
          } 
        }
        Meggy.delay(100);
      }
    }
  }
}



class Cylon {

  public void pattern(byte row) {
    this.moveRight((byte) 0, row);
    this.moveLeft((byte) 7, row);
  }

  public void moveRight(byte x, byte y){


    if (this.inBounds(x,y)) {
      Meggy.setPixel(x, y, Meggy.Color.RED);
    } else {}

    if (this.inBounds((byte)(x-(byte)1),y)) {
      Meggy.setPixel((byte)(x-(byte)1), y, Meggy.Color.DARK);
    } else {}

    Meggy.delay(10);


    if (this.inBounds((byte)(x+(byte)1),y)) {
      this.moveRight((byte)(x+(byte)1),y);
    } else {}
  }

  public void moveLeft(byte x, byte y){

    if (this.inBounds(x,y)) {
      Meggy.setPixel(x, y, Meggy.Color.RED);
    } else {}

    if (this.inBounds((byte)(x+(byte)1),y)) {
      Meggy.setPixel((byte)(x+(byte)1), y, Meggy.Color.DARK);
    } else {}

    Meggy.delay(10);

    if (this.inBounds((byte)(x-(byte)1),y)) {
      this.moveLeft((byte)(x-(byte)1),y);
    } else {}
  }

  public boolean inBounds(byte x, byte y) {
    return ((byte)(0-1) < x) && (x < (byte)8) && ((byte)(0-1) < y) && (y < (byte)8);
  }
}
