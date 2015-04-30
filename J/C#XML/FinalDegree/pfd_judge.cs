using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FinalDegree
{
    class pfd_judge
    {
        static string[] results = 
                 { "1st class degree", "upper second class degree",
                   "lower second class degree", "third class degree" };

        public static string getNextLevel(double A, double B)
        {
            if (A >= 70)
            {
                if (B >= 60)
                {
                    return results[0];
                }
                else if (B >= 50)
                {
                    return results[1];
                }
            }
            else if ( A >= 60)
            {
                if (B < 60 && B >= 50) return results[1];
            }
            else if (A >= 50)
            {
                if (B < 50 && B >= 40) return results[2];
            }
            else if (A >= 40)
            {
                if (B < 40) return results[3];
            }

            return "";
        }
    }
}
