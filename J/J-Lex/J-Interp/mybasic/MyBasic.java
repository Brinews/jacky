import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.charset.Charset;
import java.util.*;

/**
 * 微型解释器
 * 支持语句: 
 *   ASSIGN variable expression
 *   DEFINE func 
 *   END
 *   CALL func
 *  
 *  expression支持：+,-,*,/四种运算，及带括号
 *
 * 增强功能：
 *  支持注释, '为注释字符
 *  支持字符串常量赋值
 * 
 */

public class MyBasic {

    /**
     * 运行方式：MyBasic <脚本文件>
     * <脚本文件> 格式：每行一条语句， 关键字:DEFINE END ASSIGN CALL
     * @param args Command-line arguments.
     */
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: MyBasic <script>");
            return;
        }
        
        // 读取文件
        String contents = readScript(args[0]);
        
        // 解释执行
        MyBasic basic = new MyBasic();
        basic.interpret(contents);
    }
    
    /**
     * 词法分析处理
     */
    private static List<Token> doTokenize(String source) {
        List<Token> tokens = new ArrayList<Token>();
        
        String token = "";
        DFAState state = DFAState.DEFAULT;
        
        String charTokens = "\n=+-*/<>()";
        TokenType[] tokenTypes = { TokenType.LINE, TokenType.EQUALS,
            TokenType.OPERATOR, TokenType.OPERATOR, TokenType.OPERATOR,
            TokenType.OPERATOR, TokenType.OPERATOR, TokenType.OPERATOR,
            TokenType.LEFT_PAREN, TokenType.RIGHT_PAREN
        };
        
        // 依次扫描每个字符, 有限状态机
        for (int i = 0; i < source.length(); i++) {
            char c = source.charAt(i);
            switch (state) {
            case DEFAULT:
                if (charTokens.indexOf(c) != -1) {
                    tokens.add(new Token(Character.toString(c),
                        tokenTypes[charTokens.indexOf(c)]));
                } else if (Character.isLetter(c)) {
                    token += c;
                    state = DFAState.SYMBOL;
                } else if (Character.isDigit(c)) {
                    token += c;
                    state = DFAState.NUMBER;
                } else if (c == '"') {
                    state = DFAState.STRING;
                } else if (c == '\'') {
                    state = DFAState.COMMENT;
                }
                break;
                
            case SYMBOL:
                if (Character.isLetterOrDigit(c)) {
                    token += c;
                } else {
                    tokens.add(new Token(token, TokenType.SYMBOL));
                    token = "";
                    state = DFAState.DEFAULT;
                    i--; 
                }
                break;
                
            case NUMBER:
                if (Character.isDigit(c)) {
                    token += c;
                } else {
                    tokens.add(new Token(token, TokenType.NUMBER));
                    token = "";
                    state = DFAState.DEFAULT;
                    i--; 
                }
                break;
                
            case STRING:
                if (c == '"') {
                    tokens.add(new Token(token, TokenType.STRING));
                    token = "";
                    state = DFAState.DEFAULT;
                } else {
                    token += c;
                }
                break;
                
            case COMMENT:
                if (c == '\n') {
                    state = DFAState.DEFAULT;
                }
                break;
            }
        }
        
        return tokens;
    }


    /**
     * TOKEN类型
     */
    private enum TokenType {
        SYMBOL, NUMBER, STRING, LINE,
        EQUALS, OPERATOR, LEFT_PAREN, RIGHT_PAREN, EOF
    }
    
    /**
     * TOKEN类
     */
    private static class Token {
        public Token(String text, TokenType type) {
            this.text = text;
            this.type = type;
        }
        
        public final String text;
        public final TokenType type;
    }
    
    /**
     * 有限状态机状态类型
     */
    private enum DFAState {
        DEFAULT, SYMBOL, NUMBER, STRING, COMMENT
    }

    /**
     * 语法分析，生成语法分析树，采用递归下降分析法
     */
    private class Parser {
        public Parser(List<Token> tokens) {
            this.tokens = tokens;
            position = 0;
        }
        
        /**
         * 分析器
         * @param  functions 函数Hash表
         * @return           语句列表
         */
        public List<Statement> doParse(Map<String, Integer> functions) {
            List<Statement> statements = new ArrayList<Statement>();
            
            while (true) {
                while (tokenMatch(TokenType.LINE));
                
                if (tokenMatch("DEFINE")) { // 函数定义
                    functions.put(fetch(TokenType.SYMBOL).text, 
                            statements.size()+1);
                    statements.add(new DefinitionStatement());
                } else if (tokenMatch("ASSIGN") && tokenMatch(TokenType.SYMBOL)) {
                    // 赋值
                    String name = fetchahead(1).text;
                    Expression value = expression();
                    statements.add(new AssignStatement(name, value));
                } else if (tokenMatch("CALL")) {
                    // 调用
                    statements.add(new CallStatement(
                        fetch(TokenType.SYMBOL).text));
                } else if (tokenMatch("END")) {
                    statements.add(new EndCallStatement());
                } else break;
            }
            
            return statements;
        }
        
        
        /**
         * 表达式解析, 递归解析
         * @return 解析表达式
         */
        private Expression expression() {
            return optor();
        }
        
        /**
         * 表达式解析器
         *
         * 按照表达式从左往右生成表达式树
         * @return 解析树
         */
        private Expression optor() {
            Expression expression = atomic();
            
            while (tokenMatch(TokenType.OPERATOR) ||
                   tokenMatch(TokenType.EQUALS)) {
                char optor = fetchahead(1).text.charAt(0);
                Expression right = atomic();
                expression = new OptorExpression(expression, optor, right);
            }
            
            return expression;
        }
        
        /**
         * 类型匹配
         * @return 表达式
         */
        private Expression atomic() {
            if (tokenMatch(TokenType.SYMBOL)) {
                return new VariableExpression(fetchahead(1).text);
            } else if (tokenMatch(TokenType.NUMBER)) {
                return new NumberValue(Double.parseDouble(fetchahead(1).text));
            } else if (tokenMatch(TokenType.STRING)) {
                return new StringValue(fetchahead(1).text);
            } else if (tokenMatch(TokenType.LEFT_PAREN)) {
                Expression expression = expression();
                fetch(TokenType.RIGHT_PAREN);
                return expression;
            }
            throw new Error("Couldn't parse :(");
        }
        
        /**
         * 类型匹配
         * @param  type1 类型1
         * @param  type2 类型2
         * @return       True都相同
         */
        private boolean tokenMatch(TokenType type1, TokenType type2) {
            if (get(0).type != type1) return false;
            if (get(1).type != type2) return false;
            position += 2;
            return true;
        }
        
        /**
         * 类型匹配
         * @param  type  类型
         * @return       True相同
         */
        private boolean tokenMatch(TokenType type) {
            if (get(0).type != type) return false;
            position++;
            return true;
        }
        
        /**
         * 类型匹配
         * @param  name  名称匹配
         * @return       True相同
         */
        private boolean tokenMatch(String name) {
            if (get(0).type != TokenType.SYMBOL) return false;
            if (!get(0).text.equals(name)) return false;
            position++;
            return true;
        }
        
        /**
         * 字符串匹配
         * @param  type  类型
         * @return       返回该类型的对象
         */
        private Token fetch(TokenType type) {
            if (get(0).type != type) throw new Error("Expected " + type + ".");
            return tokens.get(position++);
        }
        
        /**
         * 字符串匹配
         * @param  name  下一个token名称
         * @return       token
         */
        private Token fetch(String name) {
            if (!tokenMatch(name)) throw new Error("Expected " + name + ".");
            return fetchahead(1);
        }

        /**
         * 取Token
         * @param  offset 偏移量
         * @return        Token
         */
        private Token fetchahead(int offset) {
            return tokens.get(position - offset);
        }
        
        /**
         * 取Token
         * @param  offset 偏移量
         * @return        Token 
         */
        private Token get(int offset) {
            if (position + offset >= tokens.size()) {
                return new Token("", TokenType.EOF);
            }
            return tokens.get(position + offset);
        }
        
        private final List<Token> tokens;
        private int position;
    }
    

    //  语法树结构

    /**
     * 语句接口
     */
    public interface Statement {
        /**
         * 解释执行过程
         */
        void execute();
    }

    /**
     * 表达式接口
     */
    public interface Expression {
        /**
         * 解释器求值过程
         * 
         * @return 计算结果
         */
        Value evaluate();
    }
    
    /**
     * 赋值语句，计算表达式数值并保存
     */
    public class AssignStatement implements Statement {
        public AssignStatement(String name, Expression value) {
            this.name = name;
            this.value = value;
        }
        
        public void execute() {
            if (!inDefinition) {
                System.out.print("Assigning " + value.evaluate().toString() );
                variables.put(name, value.evaluate());
                System.out.println(" to " + name);
            }
        }

        private final String name;
        private final Expression value;
    }
    
    /**
     * 调用语句
     */
    public class CallStatement implements Statement {
        public CallStatement(String funcname) {
            this.funcname = funcname;
        }
        
        public void execute() {
            if (functions.containsKey(funcname)) {
                System.out.println("Calling "+funcname);
                callStack.push(currentStatement);
                currentStatement = functions.get(funcname).intValue();
                inDefinition = false;
            }
        }

        private final String funcname;
    }

    /**
     * 定义结束语句
     */
    public class EndCallStatement implements Statement {
        public EndCallStatement() {
        }
        
        public void execute() {
            if (!inDefinition) {
                currentStatement = (Integer)callStack.pop();
            }
            inDefinition = false;
        }
    }

    /**
     * 过程定义
     */
    public class DefinitionStatement implements Statement {
        public DefinitionStatement() {
        }
        
        public void execute() {
            inDefinition = true;
        }
    }
    
    /**
     * 变量表达式
     */
    public class VariableExpression implements Expression {
        public VariableExpression(String name) {
            this.name = name;
        }
        
        public Value evaluate() {
            if (variables.containsKey(name)) {
                return variables.get(name);
            }
            return new NumberValue(0);
        }
        
        private final String name;
    }
    
    /**
     * 操作符表达式
     */
    public class OptorExpression implements Expression {
        public OptorExpression(Expression left, char optor,
                                  Expression right) {
            this.left = left;
            this.optor = optor;
            this.right = right;
        }
        
        public Value evaluate() {
            Value leftVal = left.evaluate();
            Value rightVal = right.evaluate();
            
            switch (optor) {
            case '=':
                if (leftVal instanceof NumberValue) {
                    return new NumberValue((leftVal.toNumber() ==
                                            rightVal.toNumber()) ? 1 : 0);
                } else {
                    return new NumberValue(leftVal.toString().equals(
                                           rightVal.toString()) ? 1 : 0);
                }
            case '+':
                if (leftVal instanceof NumberValue) {
                    return new NumberValue(leftVal.toNumber() +
                                           rightVal.toNumber());
                } else {
                    return new StringValue(leftVal.toString() +
                            rightVal.toString());
                }
            case '-':
                return new NumberValue(leftVal.toNumber() -
                        rightVal.toNumber());
            case '*':
                return new NumberValue(leftVal.toNumber() *
                        rightVal.toNumber());
            case '/':
                return new NumberValue(leftVal.toNumber() /
                        rightVal.toNumber());
            }
            throw new Error("Unknown optor.");
        }
        
        private final Expression left;
        private final char optor;
        private final Expression right;
    }
    
    /**
     * 变量对象接口
     */
    public interface Value extends Expression {
        /**
         * 序列化
         */
        String toString();
        
        /**
         * 取值
         */
        double toNumber();
    }
    
    /**
     * 数值变量
     */
    public class NumberValue implements Value {
        public NumberValue(double value) {
            this.value = value;
        }
        
        @Override public String toString() { return Double.toString(value); }
        public double toNumber() { return value; }
        public Value evaluate() { return this; }

        private final double value;
    }
    
    /**
     * 字符串变量
     */
    public class StringValue implements Value {
        public StringValue(String value) {
            this.value = value;
        }
        
        @Override public String toString() { return value; }
        public double toNumber() { return Double.parseDouble(value); }
        public Value evaluate() { return this; }

        private final String value;
    }

    /**
     * 初始化：准备变量hash和函数hash
     */
    public MyBasic() {
        variables = new HashMap<String, Value>();
        functions = new HashMap<String, Integer>();
        callStack = new Stack();
        inDefinition = false;
    }

    /**
     * 
     * 根据语法树进行解释执行
     * @param source 脚本文件
     */
    public void interpret(String source) {
        // 词法分析
        List<Token> tokens = doTokenize(source);
        
        // 语法分析
        Parser parser = new Parser(tokens);
        List<Statement> statements = parser.doParse(functions);
        
        // 解释执行
        currentStatement = 0;
        while (currentStatement < statements.size()) {
            int thisStatement = currentStatement;
            currentStatement++;
            statements.get(thisStatement).execute();
        }
    }
    
    /**
     * 完整读入脚本，返回成一串字符
     * @param  path  文件名
     * @return       文件内容
     * @throws       IOException
     */
    private static String readScript(String path) {
        try {
            FileInputStream stream = new FileInputStream(path);
            
            try {
                InputStreamReader input = new InputStreamReader(stream, Charset.defaultCharset());
                Reader reader = new BufferedReader(input);
                
                StringBuilder builder = new StringBuilder();
                char[] buffer = new char[1024]; /* 每行最大1024长 */
                int read;
                
                while ((read = reader.read(buffer, 0, buffer.length)) > 0) {
                    builder.append(buffer, 0, read);
                }
                
                builder.append("\n");
                
                return builder.toString();
            } finally {
                stream.close();
            }
        } catch (IOException ex) {
            return null;
        }
    }

    /*
     * 使用的数据结构
     */
    private final Map<String, Value> variables;
    private final Map<String, Integer> functions;
    private int currentStatement;
    private Stack callStack;
    private boolean inDefinition;
    
}

