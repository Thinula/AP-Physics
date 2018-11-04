import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import org.jfree.chart.*;
import org.jfree.data.category.DefaultCategoryDataset;
import com.fazecast.jSerialComm.*;

// HOLY SHIT THIS ENTIRE CODE WORKS!!!! (KIND OF)

public class Clutching extends JFrame
{
	private int windowWidth,windowHeight,rowNum,colNum,value,lightVal;
	private double ratio;
	// highIntense and lowIntense are made up values for the time being
	private final int highIntense = 1023,lowIntense = 0;
	static JFrame killMeNow;
	private JComboBox<String> portOptions;
	private JButton connectBtn;
	private SerialPort chosenPort;
	private final int squareLength = 1;
	static Scanner input;
	
	public Clutching()
	{		
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		windowWidth = 35*(int)screenSize.getWidth()/420;
		windowHeight = 30*(int)screenSize.getHeight()/420;
		
		JPanel topThings = new JPanel(); // pane at top
		
		portOptions = new JComboBox<String>();
		connectBtn = new JButton("Connect");
		
		// create an array to hold all the serial ports
		SerialPort[] portNames = SerialPort.getCommPorts();
		// loop through this array and add the name to the portOptions comboBox
		for (int i = 0; i < portNames.length; i++)
			portOptions.addItem(portNames[i].getSystemPortName());
		
		// Button Listener for "Connect" Button
		ButtonListener btn = new ButtonListener();
		connectBtn.addActionListener(btn);
		
		rowNum = 0;
		colNum = 0;		
		
		// Add the above items to the top of the screen
		topThings.add(portOptions);
		topThings.add(connectBtn);
				
		// add the topThings to the top of the JFrame
		add(topThings,BorderLayout.NORTH);
		
		String portName = portOptions.getSelectedItem().toString();
		System.out.println(portName);
		// attempt to connect to the serial port specified by user
		chosenPort = SerialPort.getCommPort("COM4");				
		chosenPort.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);
		
		if (chosenPort.openPort()) // if the port has been opened
		{
			
			// set option to disconnect
			connectBtn.setText("Disconnect");
			// prevent user from choosing any port names
			portOptions.setEnabled(false);
			System.out.println("Heh troll");	
			
			repaint();
		}
		
		setLayout(new BorderLayout());
		setSize (10*windowWidth,10*windowHeight+100);
		setTitle ("Get Coding Rae"); // best title of them all
		setDefaultCloseOperation (JFrame.EXIT_ON_CLOSE);
		setLocationRelativeTo (null);
	}
	
	class ButtonListener implements ActionListener
	{
		public void actionPerformed (ActionEvent e)
		{
			if (e.getActionCommand().equals("Connect"))
			{	
				System.out.println("Why tho");
			}
												
		}
	}
	
	public void paint(Graphics g)
	{		
		int value;
		Color col; 
		Scanner input;
		
		//Scanner input = new Scanner(chosenPort.getInputStream());
		try
		{
			input = new Scanner(chosenPort.getInputStream());
		}
		catch(Exception e)
		{
			System.out.println("RIP");
			return;
		}
		while(input.hasNextLine() && rowNum < 300 && colNum < 300) 
		{
			try 
			{
				String reading = input.nextLine();
				value = Integer.parseInt(reading);
				
				System.out.println(value + " " + rowNum + " " + colNum);
				
				if (value < 0)
					col = new Color(0,0,0);
				else if (value < 256)
					col = new Color(value,value,value); 
				else
					col = new Color(255,255,255);
				
				g.setColor(col);
				g.fillRect(colNum, rowNum, squareLength, squareLength);
				
				colNum ++;
				
				if (colNum == 300 && rowNum < 300)
				{
					colNum = 0;
					rowNum ++;
				}
			} 
			catch(Exception e) 
			{
			
			}
		}
		input.close();
	}
	
	public static void main(String[] args) 
	{
		Clutching camera = new Clutching();
		camera.setVisible(true);
	}

}