/*
========================================================================================
COIS-4470H Modelling and Simulation
Assignment 1 Question 1
Simulation of Booting-up multiple server with failure probability

Language : C#
Written By : Punyaja Mishra, 0660001

The program will calculate the percentage of days that a company 
with 10 servers can have its business on time given every morning 
the servers are rebooted and if 1 or more servers fail to boot up
then the company can not do its business. The probability of a 
server failing to boot up are provided as 1%, 2% and 3%. The simulation 
will run on 2 cases :

Case 1 : No backup server
If 1 or more servers fail, no business on time

Case 2 : ONE backup server
If one of the 10 servers fail, business can stil be on time

========================================================================================
*/

using System;
using System.Linq;
using System.Collections.Generic;

public class A1Q1_4470
{
	public static void Main()
	{
		//defining constants
		int NUM_DAYS = 10000; // Number of days to simulate
		int NUM_SERVERS = 10; // Number of servers
		double FAIL_PERCENT = 3; // Probability as percent of time a boot up fials

		//Random function
		Random random = new Random();

		//defining variables
		int sum_fail=0; //counter for fail days
		bool fail; // boolean flag set to true or false
		double on_time; //percentage of days when company does business on time
						//on timee = all servers are successfully re-booted
		int fail_servers; //Number of servers failed on a particular day

		//output a "running suimulation" banner
		Console.WriteLine(" >>> The simulation is running for %d days... \n", NUM_DAYS);

		//Run the simulation for NUM_DAYS
		for (int i = 0; i < NUM_DAYS; i++)
		{
			//loop through all servers (NUM_SERVERS)
			// set fail to TRUE if bootup fails else 
			// setting fail to false before reboot for the day
			fail = false;

			//initializing the number of failed servers FOR THE DAY to 0
			fail_servers = 0;
			for (int j = 0; j < NUM_SERVERS; j++)
			{
				//generating random number to know whether the boot up failed
				//random number is generated between 1 and 100 
				double random_number = random.NextInt64(0,101);

				//since fail percent is 1%, if the random number is 0 or 1, that means the server boot up failed
				if (random_number <= FAIL_PERCENT)
                {
					fail = true;
					fail_servers++;
				}
					
			}
			//Increment the days with failure
			if (fail && fail_servers > 1)
			{
				sum_fail ++;
			}
		} //End of the Loop

		//Compute percent of days on which the company has business on time
		on_time = ((double)(NUM_DAYS - sum_fail) / (double)NUM_DAYS)*100;



		//Printing Results
		Console.WriteLine(" *** Results from server boot-up simulation ***\n");
		Console.WriteLine("Problem Input : \n");
		Console.WriteLine("\tNumber of days simulated = " + NUM_DAYS + " days \n");
		Console.WriteLine("\tNumber of servers = " + NUM_SERVERS + " days \n");
		Console.WriteLine("\tBoot-up failure percent = " + FAIL_PERCENT + "% \n");
		Console.WriteLine("Problem Output : \n");
		Console.WriteLine("\tDays the business is on time = " + Math.Round(on_time,2)  + "% \n");
	}
}
