using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FinalDegree
{
    public class pfd_course
    {
        public string courseName;
        public int courseCredit;

        public string planName1;
        public int planWeight1;

        public string planName2;
        public int planWeight2;

        public string planName3;
        public int planWeight3;

        public string planName4;
        public int planWeight4;

        public double courseTotal;

        public pfd_course()
        {
            pfd_clear();
        }

        public void pfd_clear()
        {
            courseName = ""; courseCredit = 0;
            planName1 = ""; planWeight1 = 0;
            planName2 = ""; planWeight2 = 0;
            planName3 = ""; planWeight3 = 0;
            planName4 = ""; planWeight4 = 0;
            courseTotal = 0.0;
        }
    }
}
