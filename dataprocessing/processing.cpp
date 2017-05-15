#include <fstream>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* *argv)
{
   char *indata;
   indata  = strdup(argv[1]);
   char *outdata;
   outdata  = strdup(argv[2]);
   float zmin = atof(argv[3]);
   float zmax = atof(argv[4]);
   float xmin = atof(argv[5]);
   float xmax = atof(argv[6]);
   float ymin = atof(argv[7]);
   float ymax = atof(argv[8]);

   zmin = zmin * 0.25 -10;
   zmax = zmax * 0.25 -10;

   ifstream infile(indata);
   ofstream outfile(outdata);

   double p[9];
   while(infile>>p[0]>>p[1]>>p[2]>>p[3]>>p[4]>>p[5]>>p[6]>>p[7])//>>p[8])
   {
       if(p[2] > zmin && p[2] < zmax && p[0] > xmin && p[0] < xmax && p[1] > ymin && p[1] < ymax)
       {
          outfile<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3]
           <<" "<<p[4]<<" "<<p[5]<<" "<<p[6]<<" "<<p[7]<<endl;//" "<<p[8]<<endl;
       }
   }

   outfile.close();
   infile.close();

   return 0;
}
