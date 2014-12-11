import java.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import java.util.Timer;
import java.util.TimerTask;
import javax.swing.SwingWorker;

class PageSimula
{
	/***
	 * Variable:
	 */
	final int MEMSIZE = 4;
	final int JOBSIZE = 320;
	final int PAGESIZE = 10;

	int[] instrNo =new int[JOBSIZE];
	int[] instrState = new int[JOBSIZE];

	int[][] memInstr = new int[MEMSIZE][PAGESIZE];
	int[][] memInstrFifo = new int[MEMSIZE][PAGESIZE];
	int[][] memInstrLru = new int[MEMSIZE][PAGESIZE];

	int missTimes;

	public PageSimula()
	{
	}

	void instrInit()
	{
		Random r = new Random();

		for (int i = 0; i < JOBSIZE; i++)
		{
			instrNo[i] = r.nextInt()%60;
			if (instrNo[i] < 0) instrNo[i] = 0 - instrNo[i];

			instrState[i] = 0;

			System.out.printf("%03d ", instrNo[i]);
			if (i % 32 == 0) System.out.printf("\n");
		}

		for (int i = 0; i < MEMSIZE; i++)
		{
			for (int j = 0; j < PAGESIZE; j++)
			{
				memInstr[i][j] = 0;
				memInstrFifo[i][j] = 0;
				memInstrLru[i][j] = 0;
			}
		}
	}

	boolean findInMem(int no, int[] pos)
	{
		for (int i = 0; i < MEMSIZE; i++)
		{
			for (int j = 0; j < PAGESIZE; j++)
			{
				if (no == memInstr[i][j])
				{
					pos[0] = i;
					pos[1] = j;

					return true;
				}
			}
		}

		return false;
	}

	boolean isMemFull(int[] pos)
	{
		for (int i = 0; i < MEMSIZE; i++)
		{
			for (int j = 0; j < PAGESIZE; j++)
			{
				if (memInstr[i][j] == 0)
				{
					pos[0] = i;
					pos[1] = j;

					return false;
				}
			}
		}

		return true;
	}

	boolean findMinimum(int vist[][], int[] pos)
	{
		int min = Integer.MAX_VALUE;

		for (int i = 0; i < MEMSIZE; i++)
		{
			for (int j = 0; j < PAGESIZE; j++)
			{
				if (min > vist[i][j])
				{
					min = vist[i][j];
					pos[0] = i;
					pos[1] = j;
				}
			}
		}

		return true;
	}

	void displayMem(int[][] vist)
	{
		for (int i = 0; i < MEMSIZE; i++)
		{
			for (int j = 0; j < PAGESIZE; j++)
			{
				System.out.printf("%03d ", vist[i][j]);
			}

			System.out.println();
		}

		System.out.println();
	}

	public int[][] getMemInstr()
	{
		return memInstr;
	}

	//mode = 0(FIFO), mode=1(LRU)
	// i = step now
	public int[] simulate(int mode, int i)
	{
		int next;
		int position[] = new int[2];
		// 0:hit, 1:replace add, 2:new add
		int state = -1; 

		{
			next = instrNo[i];

			if (findInMem(next, position))
			{
				// update access time
				memInstrLru[position[0]][position[1]] = i;

				state = 0;
			}
			else
			{
				// new add
				if (!isMemFull(position))
				{
					memInstr[position[0]][position[1]] = next;
					memInstrFifo[position[0]][position[1]] = i;
					memInstrLru[position[0]][position[1]] = i;

					state = 2;
				}
				else {
					// replace add
					if (mode == 0)
					{
						//fifo 
						findMinimum(memInstrFifo, position);
					}
					else 
					{
						//lru
						findMinimum(memInstrLru, position);
					}

					memInstr[position[0]][position[1]] = next;
					memInstrFifo[position[0]][position[1]] = i;
					memInstrLru[position[0]][position[1]] = i;

					state = 1;
				}
			}

			//i++;
			//displayMem(memInstr);
		}

		int[] ret = new int[3];
		ret[0] = position[0];
		ret[1] = position[1];
		ret[2] = state;

		return ret;
	}

}

/****
  图形显示部分
 ****/
class PageFrame extends JFrame
{
	public PageFrame()
	{
		setSize(450, 400);
		setTitle("Page Fault Simulation.");
		PagePanel panel = new PagePanel();
		add(panel);
	}
}

class PagePanel extends JPanel
{
	final int MEMSIZE = 4;
	final int PAGESIZE = 10;
	final int JOBSIZE = 320;
	int[][] memState = new int[MEMSIZE][PAGESIZE];

	int step = -1, mode;

	JLabel[][] myLabel = new JLabel[MEMSIZE][PAGESIZE];
	JPanel state = new JPanel();
	JLabel sm = new JLabel("灰框：表示HIT ,蓝框：表示替换。下一个指令:");

	PageSimula ps = new PageSimula();

	private static final Timer time = new Timer("RUNNER");

	public PagePanel()
	{
		setLayout(new BorderLayout());
		add(sm, BorderLayout.NORTH);

		state.setLayout(new GridLayout(10, 4));

		for (int i = 0; i < PAGESIZE; i++)
		{
			for (int j = 0; j < MEMSIZE; j++)
			{
				myLabel[j][i] = new JLabel("-", JLabel.CENTER);

				state.add(myLabel[j][i]);
			}
		}

		state.setBorder(BorderFactory.createTitledBorder(""));
		add(state, BorderLayout.CENTER);

		JPanel south = new JPanel();
		JButton fifo = new JButton("FIFO");
		JButton lru = new JButton("LRU");

		ActionListener cmd = new SimulateAction();
		fifo.addActionListener(cmd);
		lru.addActionListener(cmd);

		south.add(fifo);
		south.add(lru);

		add(south, BorderLayout.SOUTH);

		time.schedule(new TimerTask() {
				@Override
				public void run() { update();}
				}, 0, 100);
	}

	void update()
	{
		new SwingWorker<Object, Object>()
		{
			protected Object doInBackground() throws Exception
			{
				if (step >= JOBSIZE || step == -1)
				{
					//reset
					for (int i = 0; i < PAGESIZE; i++)
					{
						for (int j = 0; j < MEMSIZE; j++)
						{
							myLabel[j][i].setText("-");
						}
					}
					sm.setText("灰框：表示HIT, 蓝框：表示替换。下一个指令:");

					return null;
				}

				step ++;
				int[] ret = ps.simulate(mode, step);

				memState = ps.getMemInstr();

				sm.setText("灰框：表示HIT, 蓝框：表示替换。下一个指令[" + step + "]=" + memState[ret[0]][ret[1]] );

				if (ret[2] == 0)
				{
					myLabel[ret[0]][ret[1]]
						.setBorder(BorderFactory.createEtchedBorder());
				}
				else if (ret[2] == 1)
				{
					myLabel[ret[0]][ret[1]]
						.setBorder(BorderFactory.createLineBorder(Color.BLUE));
				}

				// not hit and replace
				if (ret[2] != 2)
				{
					try {
						Thread.sleep(100);
					}
					catch (Exception e)
					{
					}

					myLabel[ret[0]][ret[1]]
						.setBorder(BorderFactory.createEmptyBorder());
				}


				for (int i = 0; i < MEMSIZE; i++)
				{
					for (int j = 0; j < PAGESIZE; j++)
					{
						if (memState[i][j] != 0)
							myLabel[i][j].setText("" + memState[i][j]);
					}
				}


				return null;
			}
		}.execute();
	}

	private class SimulateAction implements ActionListener
	{
		public void actionPerformed(ActionEvent event)
		{
			String input = event.getActionCommand();

			mode = 0;
			if (input.equals("LRU"))
			{
				mode = 1;
			}

			step = 0;
			ps.instrInit();

		}
	}
}

/***
  控制部分
 ***/
public class PageSimulation
{
	public static void main(String[] args)
	{
		PageFrame frame = new PageFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
