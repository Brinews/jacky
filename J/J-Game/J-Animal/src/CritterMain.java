// Stuart Reges
// 9/6/00
//
// CritterMain provides method main for a simple simulation program.

public class CritterMain {
    public static void main(String[] args) {
        CritterFrame frame = new CritterFrame();

        frame.add(25, Bird.class);
        frame.add(25, Frog.class);
        frame.add(25, Mouse.class);
        frame.add(25, Turtle.class);
        frame.add(25, Wolf.class);

        frame.start();
    }
}
