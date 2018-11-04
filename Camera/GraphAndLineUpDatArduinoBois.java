import java.awt.*;
import java.awt.event.*;
import java.util.*;

import javax.swing.*;

import org.jfree.chart.*;
import org.jfree.data.xy.*;

import com.fazecast.jSerialComm.*;

public class GraphAndLineUpDatArduinoBois extends JFrame
{
	private int windowWidth,windowHeight;
	private JFrame killMeNow;
	private JComboBox<String> portOptions;
	private JButton connectBtn;
	private SerialPort chosenPort;
	private XYSeries data;
	private XYSeriesCollection dataSet;
	private int numOfDataPoints;
	
	public GraphAndLineUpDatArduinoBois()
	{
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		windowWidth = 35*(int)screenSize.getWidth()/420;
		windowHeight = 30*(int)screenSize.getHeight()/420;
		
		setLayout(new BorderLayout());
		setSize (10*windowWidth,10*windowHeight+100);
		setTitle ("Get Coding Rae"); // best title of them all
		
		numOfDataPoints = 0; // there are 0 points to graph at the start
		
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
		
		// Add the above items to the top of the screen
		topThings.add(portOptions);
		topThings.add(connectBtn);
				
		// add the topThings to the top of the JFrame
		add(topThings,BorderLayout.NORTH);
		
		// Now let's create a line graph (jokes we'll need to do a histogram somehow)
		data = new XYSeries("");
		// Apparently line graphs can draw multiple series so gotta collect 'em all
		dataSet = new XYSeriesCollection(data);
		
		// now graph it!
		JFreeChart chart = ChartFactory.createXYLineChart("Pretty Lit", "Good Times(s)", "Read it", dataSet);
		// add the chart to the JFrame
		add(new ChartPanel(chart),BorderLayout.CENTER);
		
		setDefaultCloseOperation (JFrame.EXIT_ON_CLOSE);
		setLocationRelativeTo (null);
	}
	
	class ButtonListener implements ActionListener
	{
		public void actionPerformed (ActionEvent e)
		{
			if (e.getActionCommand().equals("Connect"))
			{
				try
				{
				String portName = portOptions.getSelectedItem().toString();
				// attempt to connect to the serial port specified by user
				chosenPort = SerialPort.getCommPort(portName);				
				chosenPort.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);
				
				if (chosenPort.openPort()) // if the port has been opened
				{
					
					// set option to disconnect
					connectBtn.setText("Disconnect");
					// prevent user from choosing any port names
					portOptions.setEnabled(false);
					
				}
				
				
				Thread thread = new Thread()
				{
					@Override public void run() 
					{
						Scanner input = new Scanner(chosenPort.getInputStream());
						while(input.hasNextLine()) 
						{
							try 
							{
								String reading = input.nextLine();
								int value = Integer.parseInt(reading);
								if (value <= 320)
									data.add(numOfDataPoints++, value);
							} 
							catch(Exception e) 
							{
							
							}
						}
						input.close();
					}
				};
				thread.start();
				}
				catch (Exception f)
				{}
			}
			else
			{
				chosenPort.closePort();
				portOptions.setEnabled(true);
				connectBtn.setText("Connect");
				data.clear();
				numOfDataPoints = 0;
			}
		}
	}
	
	public static void main(String[] args) 
	{
		GraphAndLineUpDatArduinoBois whyAmIDoingGui = new GraphAndLineUpDatArduinoBois();
		whyAmIDoingGui.setVisible(true);
	}

}
