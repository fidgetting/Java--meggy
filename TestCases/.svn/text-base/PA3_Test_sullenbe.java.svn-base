/**
 * CS453 Homework 3, Due 23 Feb 2011, 2359
 * PA3_Test_sullenbe.java
 * @author sullenbe
 *
 * To run, in a command prompt type:
 * >java -jar ~cs453/public/MJ_PA3.jar PA3_Test_sullenbe.java
 *
 * Make sure the meggy package is accessible.
 *
 */
import meggy.Meggy;

class PA3_Test_sullenbe {
    public static void main(String[] args) {
    	/** Goal	::=	"import" "meggy.Meggy" ";" MainClass <EOF>
		 *	MainClass	::=	"class" Identifier "{" "public" "static" "void" "main" "(" "String" "[" "]" Identifier ")" "{" Statement "}" "}"
		 *	Statement	::=	"{" ( Statement )* "}"
		 *	|	"Meggy.setPixel" "(" Expression "," Expression "," Expression ")" ";"
		 *	Expression	::=	Expression ( "+" | "-" | "*" ) Expression
		 *	|	"(" "byte" ")" Expression
		 *	|	<INT_LITERAL>
		 *	|	<COLOR_LITERAL>
		 *	|	"(" Expression ")"
		 */

		//Set pixel at 1,6 to GREEN
		//Test extra nested {{}}'s
		{{{{{Meggy.setPixel( (byte)1, (byte)6, Meggy.Color.GREEN );}}}}

		//Set pixel at 1,5 to VIOLET
		//Test extra (byte)'s
		{Meggy.setPixel( (byte)(byte)(byte)(byte)1, (byte)(byte)(byte)(byte)5, Meggy.Color.VIOLET );}

		//Set pixel at 2,7 to RED
		//Test byte addition
		{Meggy.setPixel( (byte)((byte)1+(byte)1), (byte)((byte)3 + (byte)4), Meggy.Color.RED );}

		//Set pixel at 3,7 to BLUE
		//Test byte subtraction
		{Meggy.setPixel( (byte)((byte)7-(byte)4), (byte)((byte)20-(byte)13), Meggy.Color.BLUE );}

		//Set pixel at 3,3 to GREEN
		//Test integer addition
		{Meggy.setPixel( (byte)(1+2), (byte)(0+1+1+1), Meggy.Color.GREEN );}

		//Set pixel at 3,2 to VIOLET
		//Test integer subtraction
		{Meggy.setPixel( (byte)(10-7), (byte)(192-180-10), Meggy.Color.VIOLET );}

		//Set pixel at 3,0 to YELLOW
		//Test association (if equal, should cancel each other out)
		{Meggy.setPixel( (byte)((1 + 7 - 5)-(1-5+7)+3), (byte)((2+7)-(7+2)), Meggy.Color.YELLOW );}

		//Set pixel at 4,7 to YELLOW
		//Test (byte) multiplication
		{Meggy.setPixel( (byte)((byte)2*(byte)2), (byte)((byte)1*(byte)7), Meggy.Color.YELLOW );}

		//Set pixel at 4,4 to BLUE
		//Test precendence (first two args should equal each other)
		{Meggy.setPixel( (byte)((byte)1*(byte)3+1), (byte)(1+(byte)3*(byte)1), Meggy.Color.BLUE );}

		//Set pixel at 5,6 to RED
		//Normal test
		{Meggy.setPixel( (byte)5, (byte)6, Meggy.Color.RED );}

		//Set pixel at 5,5 to GREEN
		//Stronger precedence test (Should cancel self out). Uses byte add., sub.
		{Meggy.setPixel( (byte)(((byte)9*(byte)3+1)-(1+(byte)9*(byte)3)+5), (byte)(((byte)25756*(byte)56+1011)-(1011+(byte)25756*(byte)56)+5), Meggy.Color.RED );}

		}
    }
}
