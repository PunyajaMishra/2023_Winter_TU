/*
========================================================================================
COIS-4470H Modelling and Simulation
Assignment 1 Question 2
Simulation of Newspaper Seller's Problem 

Language : C#
Written By : Punyaja Mishra, 0660001

The program for a given number of newspapers bought for the day simulates 
the total profit earned for 1000, 10000, 100000 days respectively. The 
day can be either good, fair or poor on random. Each day has a different 
demand based on what type of day it is. If the demand is less than the 
newspapers bought then the unsold newspapers are recycled. Vice versa
if we have more demand than the number of newspapers bought, we suffer a 
loss in our profit. 

========================================================================================
*/
using System;
using System.Linq;
using System.Collections.Generic;

public class A1Q2_4470
{
    public static void Main()
    {
        int NUM_DAYS = 0; //number of days to simulate
        int NUM_BUY = 0; //number of newspapers to buy 
        double totalProfit = 0; //Total profit
        double priceBuy = 0.33; //price of paper when buying
        double priceSell = 0.50; //price of newspapers when selling
        double priceRecycle = 0.05; //price of newspaper when recycling
        int dayDemand; //demand for the day
        int randDayType; //type of day - good, fair or poor

        Random random = new Random(); //creates an object of random class that will be used to create random integers

        //ask user to input the number of newspapers to buy
        Console.WriteLine("Enter the number of papers to buy: \n");
        NUM_BUY = Convert.ToInt16(Console.ReadLine());
        //ask user to enter the number of days to stimulate
        Console.WriteLine("Enter the number of days to stimulate: \n");
        NUM_DAYS = Convert.ToInt32(Console.ReadLine());

        Console.WriteLine("This program stimulates " + NUM_DAYS + " days with " + NUM_BUY + " papers to buy each day\n");

        //iterate NUM_DAYS times
        for (int i = 0; i < NUM_DAYS; i++)
        {

            //Generate integer between 1 and 100 for determining the day type
            randDayType = (int)(random.NextInt64(100) + 1);

            //If it is a Good Day
            if (randDayType <= 35)
            {
                //call the function to get the demand and sent the random number generated as a paramter
                int randGoodDay = (int)(random.NextInt64(100) + 1);
                dayDemand = demandGood(randGoodDay);
            }
            //if it is a fair day
            else if (randDayType <= 80)
            {
                //call the function to get the demand and sent the random number generated as a paramter
                int randFairDay = (int)(random.NextInt64(100) + 1);
                dayDemand = demandFair(randFairDay);
            }
            //if it is a poor day
            else
            {
                //call the function to get the demand and sent the random number generated as a paramter
                int randPoorDay = (int)(random.NextInt64(100) + 1);
                dayDemand = demandPoor(randPoorDay);
            }
            totalProfit += profit(dayDemand, NUM_BUY, priceBuy, priceSell, priceRecycle);

        } //end of loop

        //Printing Results
        Console.WriteLine("Number of Days : " + NUM_DAYS);
        Console.WriteLine("Number of Newspapers Purchased : " + NUM_BUY);
        Console.WriteLine("Total Profit : " + (int)(totalProfit));
    } //end of main

    //calculating profit
    public static double profit(double dayDemand, double numBuy, double priceBuy, double priceSell, double priceRecycle)
    {
        double p = priceSell - priceBuy; //profit from each paper
        if (dayDemand > numBuy)
            return numBuy * p - (dayDemand - numBuy) * p;
        return dayDemand * p - (numBuy - dayDemand) * priceRecycle;
    }

    public static int demandGood(int random_number)
    {
        int demand;

        if (random_number <= 3) //demand is 40
            demand = 40;
        else if (random_number <= 8) //demand is 50
            demand = 50;
        else if (random_number <= 23) //demand is 60
            demand = 60;
        else if (random_number <= 43) //demand is 70
            demand = 70;
        else if (random_number <= 78) //demand is 80
            demand = 80;
        else if (random_number <= 93) //demand is 90
            demand = 90;
        else //demand is 100
            demand = 100;

        return demand;
    }

    public static int demandFair(int random_number)
    {
        int demand;

        if (random_number <= 10) //demand is 40
            demand = 40;
        else if (random_number <= 28) //demand is 50
            demand = 50;
        else if (random_number <= 68) //demand is 60
            demand = 60;
        else if (random_number <= 88) //demand is 70
            demand = 70;
        else if (random_number <= 96) //demand is 80
            demand = 80;
        else //demand is 90
            demand = 90;

        return demand;
    }

    public static int demandPoor(int random_number)
    {
        int demand;

        if (random_number <= 44) //demand is 40
            demand = 40;
        else if (random_number <= 66) //demand is 50
            demand = 50;
        else if (random_number <= 82) //demand is 60
            demand = 60;
        else if (random_number <= 94) //demand is 70
            demand = 70;
        else //demand is 80
            demand = 80;

        return demand;
    }
}