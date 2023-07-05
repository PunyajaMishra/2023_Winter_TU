using System;
using System.Linq;
using System.Collections.Generic;

public  static class Lab2_4470
{
    public static void Main()
    {
        //square of length 2; 0 <= x <= 2; 0 <= y <= 2
        double sx = 0.0, sy = 1.0;
        //circle of radius 1
        double radius = 1.0;


        //number of points inside the circle
        int inside_circle = 0;
        
        //initialising the pi value
        double pi;



        //random points (x,y) generated within the square -> in between 0 to 2
        double x,y;
        Random random = new Random();

        //loop counters
        int i = 0, j;
        
        //loop through different cases
        for(j=2; j<7; j++)
        {
            //number of pair of random numbers generated
            int no_pairs = (int)(Math.Pow(10, j));

            //loop no_pairs time and check if they lie inside circle. If true, then count++
            while (i<= no_pairs)
            {
                x = NextDouble(random, sx, sy);
                y = NextDouble(random, sx, sy);

                if (Math.Pow(x, 2) + Math.Pow(y, 2) <= radius)
                    inside_circle ++;
            
                i++;
            }

            //calculate pi value
            pi = ((double)(inside_circle) / (double)(no_pairs))*4;

            //print pi value
            Console.WriteLine("Random number of points = " + no_pairs + " pi = " + pi);
        }

    }

    public static double NextDouble(this Random random, double minimum, double maximum)
    {
        return random.NextDouble() * (maximum - minimum) + minimum;
    }
}