
import meggy.Meggy;

class PA7simple {
  public static void main(String[] whatever) {
    new oth().run();
  }
}

class oth {
  a var;
  
  public void run() {
    int[] i;
    int   j;
    i = new int[10];
    j = i.length;
    
    i[0] = 0;
    i[1] = 1;
    i[2] = 2;
    
    Meggy.setPixel((byte)i[0], (byte)i[1], Meggy.Color.BLUE);
    Meggy.setAuxLEDs(136);
    
    var.get().func();
  }
}

class a {
  b var;
  
  public b get() {
    return var;
  }
}

class b {
  public void func() {
    
  }
}
