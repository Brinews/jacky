import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class PlayGame2048Panel extends JPanel
{
    private Grid[][] grid;

    private int value, moveTimes ;
    private boolean  newNum;
    private static final int SIZE = 4;
    private int[][] board;

    public PlayGame2048Panel()
    {
        setPreferredSize (new Dimension(320, 320));
        //setBackground (Color.white);

        setFocusable(true);

        setLayout(new GridLayout(SIZE, SIZE));

        grid = new Grid[SIZE][SIZE];
        board = new int[SIZE][SIZE];

        for( int i = 0; i<SIZE; i++)
        {
            for( int j = 0; j<SIZE;j++)
            {
                grid[i][j] = new Grid();
                add(grid[i][j]);
            }
        }

        newNum = true;
        moveTimes = 0;

        for( int i= 0; i<2;i++)
        {
            randomGrid();
        }

        addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                syncBoard();
                switch(e.getKeyCode()) {
                    case KeyEvent.VK_LEFT:
                        moveUp(board);
                        break;
                    case KeyEvent.VK_UP:
                        moveLeft(board);
                        break;
                    case KeyEvent.VK_RIGHT:
                        moveDown(board);
                        break;
                    case KeyEvent.VK_DOWN:
                        moveRight(board);
                        break;
                }
                checkStatus();
            }
        });

        setVisible(true);
    }

    public void syncBoard() 
    {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = grid[i][j].getValue();
                //System.out.print(board[i][j] + " ");
            }
            //System.out.println();
        }
    }

    public void syncGrid()
    {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                grid[i][j].setValue(board[i][j]);
            }
        }
    }

    public void randomGrid()
    {
        if (newNum == true)
        {
            int i, j;

            do {
                i = (int)(Math.random()*SIZE);
                j = (int)(Math.random()*SIZE);
            } while (grid[i][j].getValue() != 0);

            //System.out.println(i+","+j);

            if (grid[i][j].getValue() == 0)
            {
                if( Math.random() < 0.5)
                {
                    grid[i][j].setValue(2);
                }
                else
                {
                    grid[i][j].setValue(4);
                }
            }
        }
    }

    int findTarget(int[] array, int x, int stop) {
        int t;
        // if the position is already on the first, don't evaluate
        if (x == 0) {
            return x;
        }
        for (t = x - 1; t >= 0; t--) {
            if (array[t] != 0) {
                if (array[t] != array[x]) {
                    // merge is not possible, take next position
                    return t + 1;
                }
                return t;
            } else {
                // we should not slide further, return this one
                if (t == stop) {
                    return t;
                }
            }
        }
        // we did not find a
        return x;
    }

    boolean slideArray(int[] array) {
        boolean success = false;
        int x, t, stop = 0;

        for (x = 0; x < SIZE; x++) {
            if (array[x] != 0) {
                t = findTarget(array, x, stop);
                // if target is not original position, then move or merge
                if (t != x) {
                    // if target is zero, this is a move
                    if (array[t] == 0) {
                        array[t] = array[x];
                    } else if (array[t] == array[x]) {
                        // merge (increase power of two)
                        array[t] *= 2;
                        // set stop to avoid double merge
                        stop = t + 1;
                    }
                    array[x] = 0;
                    success = true;
                }
            }
        }
        return success;
    }

    void rotateBoard(int[][] board) {
        int i, j, n = SIZE;
        int tmp;
        for (i = 0; i < n / 2; i++) {
            for (j = i; j < n - i - 1; j++) {
                tmp = board[i][j];
                board[i][j] = board[j][n - i - 1];
                board[j][n - i - 1] = board[n - i - 1][n - j - 1];
                board[n - i - 1][n - j - 1] = board[n - j - 1][i];
                board[n - j - 1][i] = tmp;
            }
        }
    }

    boolean moveUp(int[][] board) {
        boolean success = false;
        int x;
        for (x = 0; x < SIZE; x++) {
            success |= slideArray(board[x]);
        }
        return success;
    }

    boolean moveLeft(int[][] board) {
        boolean success;
        rotateBoard(board);
        success = moveUp(board);
        rotateBoard(board);
        rotateBoard(board);
        rotateBoard(board);
        return success;
    }

    boolean moveDown(int[][] board) {
        boolean success;
        rotateBoard(board);
        rotateBoard(board);
        success = moveUp(board);
        rotateBoard(board);
        rotateBoard(board);
        return success;
    }

    boolean moveRight(int[][] board) {
        boolean success;
        rotateBoard(board);
        rotateBoard(board);
        rotateBoard(board);
        success = moveUp(board);
        rotateBoard(board);
        return success;
    }

    public void checkStatus()
    {
        int sum = 0;
        for( int i = 0; i<SIZE; i++)
        {
            for( int j = 0; j<SIZE; j++)
            {
                if( grid[i][j].getValue()!=0)
                {
                    sum++;
                }
            }
            if(sum ==SIZE*SIZE)
            {
                newNum = false;
                //System.out.println("\n\nover");
            }
        }

        if(newNum == false)
        {
            grid[1][0].setText("g");
            grid[1][1].setText("a");
            grid[1][2].setText("m");
            grid[1][3].setText("e");
            grid[2][0].setText("o");
            grid[2][1].setText("v");
            grid[2][2].setText("e");
            grid[2][3].setText("r");
            grid[2][3].addMouseListener(new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    restart();
                }
            });
        } else {
            syncGrid();
            randomGrid();
        }
    }

    public void win()
    {
        grid[2][1].setText("y");
        grid[2][2].setText("o");
        grid[2][3].setText("u");
        grid[3][1].setText("w");
        grid[3][2].setText("i");
        grid[3][3].setText("n");
        grid[3][3].addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                restart();
            }
        });
    }

    public void restart()
    {
        newNum = true;

        moveTimes = 0;
        for( int i = 0; i <SIZE; i++)
        {
            for( int j =0; j<SIZE; j++)
            {
                grid[i][j].setValue(0);
            }
        }

        for( int i = 0; i <2; i++)
        {
            randomGrid();
        }
    }
}

