/*
 * Author: coder
 * Date: 2016-11-12
 */
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;

public class PokemonGoGui {
   public static void main(String[] args) {
      PokePlayerFrame frame = new PokePlayerFrame();
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      frame.setVisible(true);
   }
}

/*
 * Record thread state class
 */ 
class StatusRunnable implements Runnable {
   public StatusRunnable(PokePlayer pokeplayer, JLabel healthLabel, JLabel potionLabel, JLabel statusLabel, JLabel pokemonLabel) {
      this.healthLabel = healthLabel;
      this.potionLabel = potionLabel;
      this.statusLabel = statusLabel;
      this.pokemonLabel = pokemonLabel;
      this.pokeplayer = pokeplayer;
   }

   public void run() {
      String sta = "";
      String spe = "";
      while (true) {
         healthLabel.setText("" + pokeplayer.getPlayer().getTotalHealth());
         potionLabel.setText("" + pokeplayer.getPlayer().getPotion().getHealVal());
         statusLabel.setText(pokeplayer.mesg);
         pokemonLabel.setText("" + pokeplayer.getPlayer().getPokemonNum());
         try {
            Thread.sleep(100);
         } catch (Exception e) {
         }
      }
   }

   private JLabel statusLabel;
   private JLabel potionLabel;
   private JLabel healthLabel;
   private JLabel pokemonLabel;
   private PokePlayer pokeplayer;
}

// ----------player move
class PokePlayerRunnable implements Runnable {
   public PokePlayerRunnable(PokePlayer pokeplayer, Component component) {
      this.pokeplayer = pokeplayer;
      this.component = component;
   }

   public void run() {
      while (true) {
         try {
            pokeplayer.move();
            component.repaint();
            Thread.sleep(pokeplayer.speed);
         } catch (Exception e) {
         }
      }
   }

   private PokePlayer pokeplayer;
   private Component component;
}

class PokePlayer {
   boolean isRun;// ---------is running
   ArrayList<Node> body;// -----pokemons
   Node wildpokemon;// --------wild pokemon
   int direction;// --------direction
   int score;
   int status;
   int speed;
   String mesg;

   static int wildpokeno = 0;
   Player role; // the player
   Pokemon wildpm; // wild pokemon

   public static final int SLOW = 200;
   public static final int MID = 100;
   public static final int FAST = 50;

   public static final int RUNNING = 1;
   public static final int PAUSED = 2;
   public static final int GAMEOVER = 3;

   public static final int WIN = 0;
   public static final int LOST = 1;

   public static final int LEFT = 1;
   public static final int UP = 2;
   public static final int RIGHT = 3;
   public static final int DOWN = 4;

   public PokePlayer() {
      speed = PokePlayer.SLOW;
      score = 0;
      isRun = false;
      //status = PokePlayer.PAUSED;
      direction = PokePlayer.RIGHT;
      body = new ArrayList<Node>();
      role = new Player("Hero");
      body.add(new Node(60, 20));
      randWildPokemon();
   }

   public Player getPlayer() {
       return role;
   }

   public String getMesg() { return mesg; }

   /*
    * 0: fire
    * 1: grass
    * 2: water
    */
   public int getPokemonType(int i) {
       Type t = role.getPokemon(i).getType1();
       if (t instanceof Fire) return 0;
       if (t instanceof Grass) return 1;
       if (t instanceof Water) return 2;
       return 3;
   }

   // ------------capture a wild-pokemon
   private boolean meetPokemon() {

      Node head = body.get(0);
      if (direction == PokePlayer.RIGHT && (head.x + Node.W) == wildpokemon.x
            && head.y == wildpokemon.y)
         return true;
      if (direction == PokePlayer.LEFT && (head.x - Node.W) == wildpokemon.x
            && head.y == wildpokemon.y)
         return true;
      if (direction == PokePlayer.UP && head.x == wildpokemon.x
            && (head.y - Node.H) == wildpokemon.y)
         return true;
      if (direction == PokePlayer.DOWN && head.x == wildpokemon.x
            && (head.y + Node.H) == wildpokemon.y)
         return true;
      else
         return false;
   }

   // ----------collsion or not
   private boolean isCollsion() {
      Node node = body.get(0);
      // ------------bound collsion
      if (direction == PokePlayer.RIGHT && node.x == 280)
         return true;
      if (direction == PokePlayer.UP && node.y == 0)
         return true;
      if (direction == PokePlayer.LEFT && node.x == 0)
         return true;
      if (direction == PokePlayer.DOWN && node.y == 380)
         return true;
      // --------------
      Node temp = null;
      int i = 0;
      for (i = 3; i < body.size(); i++) {
         temp = body.get(i);
         if (temp.x == node.x && temp.y == node.y)
            break;
      }
      if (i < body.size())
         return true;
      else
         return false;
   }

   // -------rand wild pokemon
   public void randWildPokemon() {
      /* no of wild-pokemon */
      wildpokeno++;

      boolean isInBody = true;
      int x = 0, y = 0;
      int X = 0, Y = 0;
      int i = 0;
      while (isInBody) {
         x = (int) (Math.random() * 15);
         y = (int) (Math.random() * 20);
         X = x * Node.W;
         Y = y * Node.H;
         for (i = 0; i < body.size(); i++) {
            if (X == body.get(i).x && Y == body.get(i).y)
               break;
         }
         if (i < body.size())
            isInBody = true;
         else
            isInBody = false;
      }
      wildpokemon = new Node(X, Y);

      Type ptype = new Fire("fire");
      double r = Math.random();
      if (r < 0.3) {
          ptype = new Grass("grass");
      } else if (r > 0.6) {
          ptype = new Water("water");
      }

      Type atype = new Fire("fire");
      r = Math.random();
      if (r < 0.3) {
          atype = new Grass("grass");
      } else if (r > 0.6) {
          atype = new Water("water");
      }

      wildpm = new Pokemon("wildpoke"+wildpokeno, Math.random()*16+1.0, "abi", (int)(Math.random()*20), ptype, atype);

   }

   // ---------change move direction
   public void changeDerection(int newDer) {
      if (direction % 2 != newDer % 2 || direction == newDer) {
          // should not move reverse
          direction = newDer;
          isRun = true;
      }
   }

   public boolean capturePokemon() {
       if (meetPokemon()) {
           if (role.capturePokemon(wildpm)) {
               body.add(0, wildpokemon);
               randWildPokemon();
               mesg = "Capture Success";
               return true;
           }
       }
       mesg = "Capture Failed";
       return false;
   }

   public void battlePokemon() {
       int k = 0;
       //k = getChoose();
       if (meetPokemon()) {
           int ret = role.fightPokemon(k, wildpm);
           if (ret == 0) {
               mesg = "Fight: Kill It";
               randWildPokemon();
           } else if (ret == 1) {
               mesg = "Fight: Be Killed";
               body.remove(k);
           } else {
               mesg = "Fight One Times";
           }
       }
   }

   /*
    * each times heal 10 health
    */
   public void healPokemon() {
       int k = 0;
       //k = getChoose();
       if (role.healPokemon(k)) {
           mesg = "Heal Success";
       }
   }

   public void move() {

      if (isCollsion()) {// ---------
         isRun = false;
         //status = PokePlayer.GAMEOVER;// -----
      } else if (isRun) {// ----move player
         Node node = body.get(0);
         int X = node.x;
         int Y = node.y;
         // ------------forward
         switch (direction) {
            case 1:
               X -= Node.W;
               break;
            case 2:
               Y -= Node.H;
               break;
            case 3:
               X += Node.W;
               break;
            case 4:
               Y += Node.H;
               break;
         }
         body.add(0, new Node(X, Y));
         body.remove(body.size() - 1);
         isRun = false;
      }
   }
}

// ---------pokemon node
class Node {
   public static final int W = 20;
   public static final int H = 20;
   int x;
   int y;

   public Node(int x, int y) {
      this.x = x;
      this.y = y;
   }
}

// ------painter
class PokePlayerPanel extends JPanel {
   PokePlayer pokeplayer;

   public PokePlayerPanel(PokePlayer pokeplayer) {
      this.pokeplayer = pokeplayer;
   }

   public void paintComponent(Graphics g) {
      super.paintComponent(g);

      Node node = null;
      for (int i = 0; i < pokeplayer.body.size(); i++) { // pokeplayer
         int type = pokeplayer.getPokemonType(i);
         if (type == 0)
            g.setColor(Color.red);
         else if (type == 1)
            g.setColor(Color.green);
         else if (type == 2)
            g.setColor(Color.blue);

         node = pokeplayer.body.get(i);
         g.fillRect(node.x, node.y, node.H, node.W);// wild-pokeplayer 

         if (i == 0) {
             /* mark header */
             g.setColor(Color.white);
             g.fillOval(node.x, node.y, 10, 10);
         }
      }

      node = pokeplayer.wildpokemon;

      if (pokeplayer.wildpm.getType1() instanceof Fire)
          g.setColor(Color.red);
      if (pokeplayer.wildpm.getType1() instanceof Grass)
          g.setColor(Color.green);
      if (pokeplayer.wildpm.getType1() instanceof Water)
          g.setColor(Color.blue);

      g.fillRect(node.x, node.y, node.H, node.W);
   }

   public Player getPlayer() {
       return pokeplayer.getPlayer();
   }
}

/* main frame */
class PokePlayerFrame extends JFrame {
   private JLabel healthLabel;
   private JLabel pokemonsLabel;
   private JLabel potionLabel;
   private JLabel statusLabel;
   private JPanel pokeplayerPanel;
   private PokePlayer pokeplayer;
   private JMenuBar bar;

   JMenu gameMenu;
   JMenu helpMenu;
   JMenu speedMenu;
   JMenu actionMenu;

   JMenuItem newItem;
   /*
   JMenuItem pauseItem;
   JMenuItem beginItem;
   */
   JMenuItem helpItem;
   JMenuItem aboutItem;
   JMenuItem slowItem;
   JMenuItem midItem;
   JMenuItem fastItem;

   JMenuItem fightItem;
   JMenuItem captureItem;
   JMenuItem healItem;

   private PokeHealPanel dialog = null;

   public PokePlayerFrame() {
      init();
      ActionListener l = new ActionListener() {
         public void actionPerformed(ActionEvent e) {
             /*
            if (e.getSource() == pauseItem)
               pokeplayer.isRun = false;
            if (e.getSource() == beginItem)
               pokeplayer.isRun = true;
               */
            if (e.getSource() == newItem) {
               newGame();
            }
            // ------------speed control
            if (e.getSource() == slowItem) {
               pokeplayer.speed = PokePlayer.SLOW;
            }
            if (e.getSource() == midItem) {
               pokeplayer.speed = PokePlayer.MID;
            }
            if (e.getSource() == fastItem) {
               pokeplayer.speed = PokePlayer.FAST;
            }
            if (e.getSource() == healItem) {
                // if first time, construct dialog
                if (dialog == null)
                    dialog = new PokeHealPanel();

                pokeplayer.healPokemon();
            }

            if (e.getSource() == fightItem) {
                pokeplayer.battlePokemon();
            }
            if (e.getSource() == captureItem) {
                pokeplayer.capturePokemon();
            }
         }
      };
      /*
      pauseItem.addActionListener(l);
      beginItem.addActionListener(l);
      */
      newItem.addActionListener(l);
      aboutItem.addActionListener(l);
      slowItem.addActionListener(l);
      midItem.addActionListener(l);
      fastItem.addActionListener(l);

      fightItem.addActionListener(l);
      captureItem.addActionListener(l);
      healItem.addActionListener(l);

      addKeyListener(new KeyListener() {
         public void keyPressed(KeyEvent e) {
            switch (e.getKeyCode()) {
               // ------------change direction
               case KeyEvent.VK_DOWN:// 
                  pokeplayer.changeDerection(PokePlayer.DOWN);
                  break;
               case KeyEvent.VK_UP:// 
                  pokeplayer.changeDerection(PokePlayer.UP);
                  break;
               case KeyEvent.VK_LEFT:// 
                  pokeplayer.changeDerection(PokePlayer.LEFT);
                  break;
               case KeyEvent.VK_RIGHT:// 
                  pokeplayer.changeDerection(PokePlayer.RIGHT);
                  break;
                  // space, pause or not
               case KeyEvent.VK_SPACE:// 
                  if (pokeplayer.isRun == true) {
                          pokeplayer.isRun = false;
                          //pokeplayer.status = PokePlayer.PAUSED;
                          break;
                  }
                  if (pokeplayer.isRun == false) {
                          pokeplayer.isRun = true;
                          pokeplayer.status = PokePlayer.RUNNING;
                          break;
                  }
            }
         }

         public void keyReleased(KeyEvent k) {
         }

         public void keyTyped(KeyEvent k) {
         }
      });
   }

   private void init() {
      pokeplayer = new PokePlayer();
      setSize(460, 460);
      setLayout(null);
      this.setResizable(false);
      bar = new JMenuBar();
      gameMenu = new JMenu("Game");
      newItem = new JMenuItem("New Game");
      gameMenu.add(newItem);

      helpMenu = new JMenu("Help");
      aboutItem = new JMenuItem("About");
      helpMenu.add(aboutItem);

      speedMenu = new JMenu("Speed");
      slowItem = new JMenuItem("Slow");
      fastItem = new JMenuItem("Fast");
      midItem = new JMenuItem("Middle");
      speedMenu.add(slowItem);
      speedMenu.add(midItem);
      speedMenu.add(fastItem);

      actionMenu = new JMenu("Action");
      fightItem = new JMenuItem("Fight");
      captureItem = new JMenuItem("Capture");
      healItem = new JMenuItem("Heal");
      actionMenu.add(fightItem);
      actionMenu.add(captureItem);
      actionMenu.add(healItem);

      bar.add(gameMenu);
      bar.add(speedMenu);
      bar.add(actionMenu);
      bar.add(helpMenu);

      setJMenuBar(bar);

      healthLabel = new JLabel();
      potionLabel = new JLabel();
      statusLabel = new JLabel();
      pokeplayerPanel = new JPanel();
      pokemonsLabel = new JLabel();

      pokeplayerPanel.setBounds(0, 0, 300, 400);
      pokeplayerPanel.setBorder(BorderFactory.createLineBorder(Color.darkGray));
      add(pokeplayerPanel);

      healthLabel.setBounds(310, 25, 60, 20);
      add(healthLabel);
      pokemonsLabel.setBounds(310, 75, 60, 20);
      add(pokemonsLabel);
      potionLabel.setBounds(310, 125, 60, 20);
      add(potionLabel);
      statusLabel.setBounds(310, 360, 140, 20);
      add(statusLabel);

      JLabel temp = new JLabel("Total Health");
      temp.setBounds(310, 5, 80, 20);
      add(temp);
      temp = new JLabel("Potion");
      temp.setBounds(310, 105, 60, 20);
      add(temp);
      temp = new JLabel("Pokemons(N)");
      temp.setBounds(310, 55, 80, 20);
      add(temp);

      temp = new JLabel("<html>Instructions:<br>Red Box: Fire Pokemon<br>Green: Grass Pokemon<br>Blue: Water Pokemon<br><font color=red>OperationKeys:</font><br>(UP,DOWN,LEFT,RIGHT)<br><font color=red>Actions:</font><br>(Fight,Capture,Heal)</html>");
      temp.setBounds(310, 145, 140, 180);
      add(temp);

      temp = new JLabel("<html><font color=red>Status:</font></html>");
      temp.setBounds(310, 340, 140, 20);
      add(temp);
   }

   private void newGame() {
      this.remove(pokeplayerPanel);
      this.remove(healthLabel);
      this.remove(potionLabel);
      this.remove(statusLabel);

      healthLabel = new JLabel();
      potionLabel = new JLabel();
      statusLabel = new JLabel();

      pokeplayer = new PokePlayer();
      pokeplayerPanel = new PokePlayerPanel(pokeplayer);

      pokeplayerPanel.setBounds(0, 0, 300, 400);
      pokeplayerPanel.setBorder(BorderFactory.createLineBorder(Color.darkGray));

      Runnable r1 = new PokePlayerRunnable(pokeplayer, pokeplayerPanel);
      Runnable r2 = new StatusRunnable(pokeplayer, healthLabel, potionLabel, statusLabel, pokemonsLabel);
      Thread t1 = new Thread(r1);
      Thread t2 = new Thread(r2);
      t1.start();
      t2.start();

      add(pokeplayerPanel);

      pokemonsLabel.setText(""+pokeplayer.getPlayer().getPokemonNum());
      healthLabel.setText(""+pokeplayer.getPlayer().getTotalHealth());
      potionLabel.setText(""+pokeplayer.getPlayer().getPotion().getHealVal());
      statusLabel.setText(pokeplayer.getMesg());

      healthLabel.setBounds(310, 25, 60, 20);
      add(healthLabel);
      potionLabel.setBounds(310, 125, 60, 20);
      add(potionLabel);
      statusLabel.setBounds(310, 360, 140, 20);
      add(statusLabel);
   }
}
