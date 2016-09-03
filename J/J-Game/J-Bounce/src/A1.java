/*
 * Your name, UPI 
 *
 * Main control of actions
 */

/*
 *  =============================================================
 *  A1.java : Extends JFrame and contains a panel where
 *  shapes move around on the screen. Also contains start and stop
 *  buttons that starts animation and stops animation respectively.
 *  ==============================================================
 */

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.border.*;
import javax.swing.event.*;


public class A1 extends JFrame {
    AnimationPanel panel;  // panel for bouncing area
    JButton startButton, stopButton, createButton, borderButton, fillButton, areaButton;  //buttons to start and stop the animation
    JTextField xPosText, yPosText, heightText, widthText;
    JComboBox<ImageIcon> shapesComboBox, pathComboBox;

    /** main method for A1
     */
    public static void main(String[] args) {
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new A1();
            }
        });
    }

    /** constructor to initialise components
     */
    public A1() {
        super("Bouncing Application");
        panel = new AnimationPanel();
        add(panel, BorderLayout.CENTER);
        add(setUpToolsPanel(), BorderLayout.NORTH);
        add(setUpButtons(), BorderLayout.SOUTH);
        addComponentListener(
            new ComponentAdapter() { // resize the frame and reset all margins for all shapes
                public void componentResized(ComponentEvent componentEvent) {
                    panel.resetMarginSize();
            }
        });
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 500);
        Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension frameSize = getSize();
        setLocation((d.width - frameSize.width) / 2, (d.height - frameSize.height) / 2);
        setVisible(true);
    }

    /** Set up the tools panel
    * @return toolsPanel        the Panel
     */
    public JPanel setUpToolsPanel() {
        //Set up the shape combo box
        ImageIcon rectangleButtonIcon = createImageIcon("rectangle.gif");
        ImageIcon squareButtonIcon = createImageIcon("square.gif");
        shapesComboBox = new JComboBox<ImageIcon>(new ImageIcon[] {rectangleButtonIcon, squareButtonIcon} );
        shapesComboBox.setToolTipText("Set shape");
        shapesComboBox.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                //set the Current shape type based on the selection: 0 for Rectangle, 1 for Square etc
                panel.setCurrentShapeType(shapesComboBox.getSelectedIndex());
            }
        });
        //Set up the path combo box
        ImageIcon fallButtonIcon = createImageIcon("fall.gif");
        ImageIcon bounceButtonIcon = createImageIcon("bounce.gif");
        pathComboBox = new JComboBox<ImageIcon>(new ImageIcon[] {fallButtonIcon, bounceButtonIcon });
        pathComboBox.setToolTipText("Set Path");
        pathComboBox.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                //set the Current path type based on the selection from combo box: 0 for bouncing Path
                panel.setCurrentPathType(pathComboBox.getSelectedIndex());
            }
        });
        //Set up the xPos TextField
        xPosText = new JTextField("10");
        xPosText.setToolTipText("x-Position");
        xPosText.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                //complete this
                int x = Integer.parseInt(xPosText.getText());
                panel.setCurrentXPos(x);
            }
        });
        //Set up the yPos TextField
        yPosText = new JTextField("20");
        yPosText.setToolTipText("y-Position");
        yPosText.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                //complete this
                int y = Integer.parseInt(yPosText.getText());
                panel.setCurrentYPos(y);
            }
        });
        //Set up the height TextField
        heightText = new JTextField("20");
        heightText.setToolTipText("Set Height");
        heightText.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                //complete this
                int h = Integer.parseInt(heightText.getText());
                panel.setCurrentHeight(h);
            }
        });
        //Set up the width TextField
        widthText = new JTextField("50");
        widthText.setToolTipText("Set Width");
        widthText.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                //complete this
                int w = Integer.parseInt(widthText.getText());
                panel.setCurrentWidth(w);
            }
        });
        //Set up the create shape button
        createButton = new JButton("Create");
        createButton.setToolTipText("Create Shape");
        createButton.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                panel.createNewShape();
            }
        });
        //Set up the fill colour button
        fillButton = new JButton("Fill");
        fillButton.setToolTipText("Set Fill Color");
        fillButton.setForeground(Color.blue);
        fillButton.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                Color newColor = JColorChooser.showDialog(panel, "Fill Color", null);
                if ( newColor != null) {
                    fillButton.setForeground(newColor);
                    panel.setCurrentFillColor(newColor);
                }
            }
        });

        //Set up the border colour button
        borderButton = new JButton("Border");
        borderButton.setToolTipText("Set Border Color");
        borderButton.setForeground(Color.black);
        borderButton.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                Color newColor = JColorChooser.showDialog(panel, "Border Color", null);
                if ( newColor != null) {
                    borderButton.setForeground(newColor);
                    panel.setCurrentBorderColor(newColor);
                }
            }
        });

        //Set up the area colour button
        areaButton = new JButton("Area");
        areaButton.setToolTipText("Calculate Areas");
        areaButton.setForeground(Color.black);
        areaButton.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                //complete this
                int area = AnimationPanel.calculateArea(panel.getAllShapes());
                JOptionPane.showMessageDialog(null, "Area is "+area);
            }
        });
        JPanel toolsPanel = new JPanel();
        toolsPanel.setLayout(new BoxLayout(toolsPanel, BoxLayout.X_AXIS));
        toolsPanel.add(new JLabel(" Shape: ", JLabel.RIGHT));
        toolsPanel.add(shapesComboBox);
        toolsPanel.add(new JLabel(" Path: ", JLabel.RIGHT));
        toolsPanel.add(pathComboBox);
        toolsPanel.add( new JLabel(" X: ", JLabel.RIGHT));
        toolsPanel.add(xPosText);
        toolsPanel.add(new JLabel(" Y: ", JLabel.RIGHT));
        toolsPanel.add(yPosText);
        toolsPanel.add( new JLabel(" Height: ", JLabel.RIGHT));
        toolsPanel.add(heightText);
        toolsPanel.add(new JLabel(" Width: ", JLabel.RIGHT));
        toolsPanel.add(widthText);
        toolsPanel.add(createButton);
        toolsPanel.add(borderButton);
        toolsPanel.add(fillButton);
        toolsPanel.add(areaButton);
        return toolsPanel;
    }

    /** Set up the buttons panel
         * @return buttonPanel        the Panel
     */
    public JPanel setUpButtons() {
        JPanel buttonPanel= new JPanel(new FlowLayout());
        //Set up the start button
        startButton = new JButton("Start");
        startButton.setToolTipText("Start Animation");
        startButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                startButton.setEnabled(false);
                stopButton.setEnabled(true);
                panel.start();  //start the animation
            }
        });
        //Set up the stop button
        stopButton = new JButton("Stop");
        stopButton.setToolTipText("Stop Animation");
        stopButton.setEnabled(false);
        stopButton.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                stopButton.setEnabled(false);
                startButton.setEnabled(true); //stop the animation
                panel.stop();
             }
        });
        // Slider to adjust the speed of the animation
        JSlider slider = new JSlider(JSlider.HORIZONTAL, 0, 200, 30);
        slider.setToolTipText("Adjust Speed");
        slider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent e) {
             JSlider source = (JSlider)e.getSource();
             if (!source.getValueIsAdjusting()) {
                 int value = (int) (source.getValue());  // get the value from slider
                 TitledBorder tb = (TitledBorder) source.getBorder();
                 tb.setTitle("Anim delay = " + String.valueOf(value) + " ms"); //adjust the tilted border to indicate the speed of the animation
                 panel.adjustSpeed(value); //set the speed
                 source.repaint();
             }
            }
        });
        TitledBorder title = BorderFactory.createTitledBorder("Anim delay = 30 ms");
        slider.setBorder(title);
        // Add buttons and slider control
        buttonPanel.add(startButton);
        buttonPanel.add(stopButton);
        buttonPanel.add(slider);
        return buttonPanel;
    }

    /** create the imageIcon
     * @param  filename        the filename of the image
     * @return ImageIcon        the imageIcon
     */
    protected static ImageIcon createImageIcon(String filename) {
        java.net.URL imgURL = A1.class.getResource(filename);
        return new ImageIcon(imgURL);
    }
}

