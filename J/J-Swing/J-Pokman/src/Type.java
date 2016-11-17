/**
 * Type class
 * Author: coder
 * Date: 2016-11-10
 */
public class Type {
    private String typeName;
    public Type() {}
    public Type(String name) { typeName = name; }
}

class Fire extends Type {
    public Fire() { super(); }
    public Fire(String n) { super(n); }
}

class Water extends Type {
    public Water() { super(); }
    public Water(String n) { super(n); }
}

class Grass extends Type {
    public Grass() { super(); }
    public Grass(String n) { super(n); }
}
