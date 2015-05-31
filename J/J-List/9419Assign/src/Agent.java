/*********************************************
 *  Agent.java 
 *  Sample Agent for Text-Based Adventure Game
 *  COMP3411 Artificial Intelligence
 *  UNSW Session 1, 2012
*/

import java.util.*;
import java.io.*;
import java.net.*;

public class Agent {

   public String[] solutions = {
       "FFRRFFLFFFFFBFRFFLFRRFCFFCFFFFFFCFFCFFFRFFFFFFRFFFFFFFFFFFFFFRFFRRFFLFFFFFFFLFFFF",
       "FFFLFFFRFFFFFFFFFFRRFFFFFFFFFFFFRRRFFFRF",
       "FFLFFLFFFFFFFFRFFFFFFLFFRFFFFFFFFFFFFLFFRFFLFFRFFFFRFFFFRFFLFFFFFFFFRFFRRFFLFFFFFFFFRFFLFFFFLFFFFLFFRFFLFFRFFFFFFFFFFFFLFFRFFFFFFLFFFFFFFFRFFRFF",
       "LFFFFLFFRFFLFFRFFFFFFFFLFFRFFFFFFRFFLFFRFFFFFFRFFLFFFFRFFFFCFFRFFLFFLFFRFFFFRFFLFFFFFFFFLFFLFFLLFFRFFRFFFFFFFFRFFLFFFFLFFRFFRFFLFFLFFRFFLFFFFFFLFFRFFLFFFFFFFFFFLFFRFFLFFRFFFF",
       "LFFRFFLFFLFFFFRFFLFFFFLFFLFFRFFFFRRFFFFLFFLFFRFFFFCFCFCFCFCFCFLFFLLFFRFFFFFFFFFFLFFFFRFFFFBFFFFLFFLFFRFFLLFFLFFRFFRFFFFLFFLFFFFRFFRFFLFF",
       "FRFFFFFFFFFFFFFFRFFFFLLFFFFLFFFFFFFFFFFFFFLF",
       "FFRFFFRFFFFFFFFFFFFFFFFFFRFFFFFFFFFRFFFFLLFFFFRFFRFFFFFFFFFFFFFFFRCFFLFLLFRFFLFFFFFFFFFFFFFFFLFFFFFFFFLFBFFFRRFFFFLFFFLFFFFFFFFFFFFFFFFLFFFF",
       "RFFLFFRFFFFRFFFFFFFFLFFFFFFFFFFRFFRFFFFLFFFFFFFFRFFFFLFFFFRFFLFFLFFFFRFFLFFFFFFRFFRFFFFF",
       "FFRFLFFRFFBFRFFFFRRFFFFBFFRRFFRFFFFLFFFF",
       "LFFFFLFFFFFFFFFFFLFFFFFFFFRFFFFLFFFFFFFLFFFFFFFFLFFFFBFLFLLFLBFFBFFLLFLFLLFLFFFLFFFFFFFLFFFFFFFLBFBFFLLFFF",
       "FFFFFFFFFFFFFRFFFFLFFFF"
   };

   public String[] patterns = {
       "  d         ^            ",
       "    ~    ~  ^ ~    ~~~~~~",
       "   *~ * *~ *^*~ ***~   *~",
       " *   ** **  ^   ***      ",
       " * ~~ **~~  ^~~***~~   ~~",
       "~~~~~~   B  ^  ~   ~~~~~~",
       "~~ B~~   ~~ ^ ~~   ~~~ ~~",
       "~* * ~*T* ~*^  ~* * ~*   ",
       "~~ d*~~  *~~^ *~~   ~~   ",
       "~~~~~       ^   *** *****",
   };

   private static int start = -1, steps = 0;

   public boolean stringComp(String from, String to)
   {
       for (int i = 0; i < from.length(); i++)
           if (from.charAt(i) != to.charAt(i)) {
               return false;
           }

       return true;
   }

   public char get_action( char view[][] ) {

      // REPLACE THIS CODE WITH AI TO CHOOSE ACTION

      /* found g, try to reach it */

      /* found d or a, try to reach it */

      /* found * or T */

       if (true) {

           if (start == -1) {
               String s = "";

               for (int i = 0; i < 5; i++)
                   for (int j = 0; j < 5; j++) {
                       if (i == 2 && j == 2)
                           s += '^';
                       else
                           s += view[i][j];
                   }

               start = solutions.length - 1;

               for (int i = 0; i < patterns.length; i++)
                   if (stringComp(s, patterns[i])) {
                       start = i;
                       break;
                   }
           }

           if (start != -1) {
               return solutions[start].charAt(steps++);
           }

       }

       int ch=0;

       System.out.print("Enter Action(s): ");

       try {
           while ( ch != -1 ) {
               // read character from keyboard
               ch  = System.in.read();

               switch( ch ) { // if character is a valid action, return it
                   case 'F': case 'L': case 'R': case 'C': case 'B':
                   case 'f': case 'l': case 'r': case 'c': case 'b':
                       return((char) ch );
               }
           }
       }
       catch (IOException e) {
           System.out.println ("IO error:" + e );
       }

       return 0;
   }

   void print_view( char view[][] )
   {
      int i,j;

      System.out.println("\n+-----+");
      for( i=0; i < 5; i++ ) {
         System.out.print("|");
         for( j=0; j < 5; j++ ) {
            if(( i == 2 )&&( j == 2 )) {
               System.out.print('^');
            }
            else {
               System.out.print( view[i][j] );
            }
         }
         System.out.println("|");
      }
      System.out.println("+-----+");
   }

   public static void main( String[] args )
   {
      InputStream in  = null;
      OutputStream out= null;
      Socket socket   = null;
      Agent  agent    = new Agent();
      char   view[][] = new char[5][5];
      char   action   = 'F';
      int port;
      int ch;
      int i,j;

      if( args.length < 2 ) {
         System.out.println("Usage: java Agent -p <port>\n");
         System.exit(-1);
      }

      port = Integer.parseInt( args[1] );

      try { // open socket to Game Engine
         socket = new Socket( "localhost", port );
         in  = socket.getInputStream();
         out = socket.getOutputStream();
      }
      catch( IOException e ) {
         System.out.println("Could not bind to port: "+port);
         System.exit(-1);
      }

      try { // scan 5-by-5 wintow around current location
         while( true ) {
            for( i=0; i < 5; i++ ) {
               for( j=0; j < 5; j++ ) {
                  if( !(( i == 2 )&&( j == 2 ))) {
                     ch = in.read();
                     if( ch == -1 ) {
                        System.exit(-1);
                     }
                     view[i][j] = (char) ch;
                  }
               }
            }
            //agent.print_view( view ); // COMMENT THIS OUT BEFORE SUBMISSION
            action = agent.get_action( view );
            out.write( action );
         }
      }
      catch( IOException e ) {
         System.out.println("Lost connection to port: "+ port );
         System.exit(-1);
      }
      finally {
         try {
            socket.close();
         }
         catch( IOException e ) {}
      }
   }
}
