package parser;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Class to simplify parsing and extracting of query data for WHERE clauses
 */
public class WhereArgs {
    // An enum to represent the different types of comparisons that can be made in our WHERE clauses
    // We can compare less than, less than or equal to, greater than or equal to, greater than, equal and not equal.
    public static enum Comparison {
        GEQ(">="),
        LEQ("<="),
        LESS("<"),
        GREATER(">"),
        EQUAL("="),
        NOT_EQUAL("<>");

        private String symbol;

        Comparison(String symbol) {
            this.symbol = symbol;
        }

        public String getSymbol() {
            return symbol;
        }

        public static Comparison find(String value) {
            for(Comparison c : Comparison.values()) {
                if(c.getSymbol().equals(value)){
                    return c;
                }
            }
            return null;
        }
    }

    // A class to provide an easy way to accessed parsed WHERE clause information
    // Once a where clause has been parsed, it will be assigned here and you can access the values with the getters below.
    // Note: The getValue() method will return the comparison value AS A STRING. You will need to convert it into the
    //       appropriate object type first before you can use it. See Type.parseType(String)
    public static class WhereTuple {
        private String column;
        private Comparison sign;
        private String value;

        public WhereTuple(String column, Comparison sign, String value) {
            this.column = column;
            this.sign = sign;
            this.value = value;
        }

        public String getColumn() {
            return column;
        }

        public Comparison getSign() {
            return sign;
        }

        public String getValue() {
            return value;
        }
    }

    // The pattern that represents the format of a single where clause (<column> <comparison_operator> <value>)
    static Pattern WHERE_CLAUSE = Pattern.compile("(\\w+) (>=|<>|<=|=|>|<) ([\\w\\.]+)");

    /**
     * Parses a whole WHERE clause string, e.g. (age >= 10 AND speed < 20.0 AND name = john) into a list of WhereTuples.
     * It splits the input string on their ANDs and attempts to match on the expected format of the where clause.
     */
    public static WhereTuple[] parse(String command) {
        String[] contents = command.split(" AND ");
        WhereTuple[] whereArgs = new WhereTuple[contents.length];
        // Find each WHERE condition clause in the query string and add it to the array.
        for(int i = 0; i < contents.length; i++) {
            Matcher matcher = WHERE_CLAUSE.matcher(contents[i]);
            if(!matcher.matches()) {
                throw new AssertionError("Invalid match found");
            }
            whereArgs[i] = new WhereTuple(matcher.group(1), Comparison.find(matcher.group(2)), matcher.group(3));
        }
        return whereArgs;
    }
}
