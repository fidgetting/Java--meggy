// HW 3 Jef Mallal
/*
Goal 	::= 	"import" "Meggy.meggy" ";" MainClass <EOF>
MainClass 	::= 	"class" Identifier "{" "public" "static" "void" "main" "(" "String" "[" "]" Identifier ")" "{" Statement "}" "}"
Statement 	::= 	"{" ( Statement )* "}"
	| 	"Meggy.setPixel" "(" Expression "," Expression "," Expression ")" ";"
Expression 	::= 	Expression ( "+" | "-" | "*" ) Expression
	| 	"(" "byte" ")" Expression
	| 	<INT_LITERAL>
	| 	<COLOR_LITERAL>
	| 	"(" Expression ")"
*/

import meggy.Meggy;

class PA3Test_mallal {

    public static void main(String[] args) {{
        //Pixel 0,0 to Red
        Meggy.setPixel( (byte)0, (byte)0, Meggy.Color.RED );
        //Pixel 7,0 to Orange
        //                     7            0         ORANGE
        //byte addition                     
        Meggy.setPixel( (byte)((byte)3+(byte)4), (byte)0, Meggy.Color.ORANGE );
        //Pixel 0,7 to Yellow
        //                  0            7                   3 (YELLOW)
        //byte addition                  
        {{{{{{{{Meggy.setPixel( (byte)0, (byte)((byte)4+(byte)3), Meggy.Color.YELLOW );}}}}}}}}
        //Pixel 7,7 to Green
        //                     7          7   4 (GREEN)
        //byte subtraction and integer subtraction
        Meggy.setPixel( (byte)((byte)10-(byte)3), (byte)(10-3), Meggy.Color.GREEN );
        //Pixel 3,3 to Blue
        //                 3          3           BLUE
        // "(" Expression ")"
        Meggy.setPixel( (byte)(((3))), (byte)(((((3))))), Meggy.Color.BLUE );
        //Pixel 4,4 to Violet
        //                    4                 4        6(VIOLET)
        //byte multiplication
        Meggy.setPixel( (byte)((byte)2*(byte)2), (byte)((byte)1*(byte)4), Meggy.Color.VIOLET );
        //Pixel 3,4 to White
        //                       3                    4              WHITE
        //integer addition and subtraction
        Meggy.setPixel( (byte)(64-32-16-8-4-1), (byte)((1 + 3 - 2)*2), Meggy.Color.WHITE );
        //Pixel 4,3 to Dark
        //                  4      3      7(DARK)
        //integer addition multiplaction subtraction and presidence order
        //doesn't work on test compiler, commenting out
//        Meggy.setPixel( (byte)(2*2), (byte)(6-2*2-1), Meggy.Color.DARK );
    }}
}
