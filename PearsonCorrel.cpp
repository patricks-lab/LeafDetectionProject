#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

vector<double> leaf1;
vector<double> leaf2;
double n = 95;

double pearsonCorrel(int v1s, int v1e, int v2s, int v2e)
{


   //Calculate values
   double sumxy = 0;
   double sumx= 0;
   double sumy= 0;
   double sumxsq = 0;
   double sumysq = 0;


  //loop from v1s->v1e
  //           v2s->v2e
   for(int i = v1s, j = v2s; i < v1e; i++, j++)
   {
     //cout << i << " " << j << '\n';
     //cout << "Here" << '\n';
     sumxy += leaf1[i]*leaf2[j];
     sumx += leaf1[i];
     sumy += leaf2[j];
     sumxsq += leaf1[i]*leaf1[i];
     sumysq += leaf2[j] * leaf2[j];
   }

   double top = sumxy - (sumx*sumy/n);
   double bottom = sqrt((sumxsq - sumx*sumx/n)*(sumysq - sumy*sumy/n));
   //cout << sumxy << " " << sumx << '\n';
   return top/bottom;

}


int main (int argc, char *argv[])
{
  double maxVal = -1;
  if(argc != 3)
  {
    cout<<  argc << "must have 2 arguments!" << '\n';
    return 0;
  }

  int k = 10;
  //cout << "How many shift (left right)?" << '\n';
  //cin >> k;

  ifstream fin1(argv[1]);
  ifstream fin2(argv[2]);

  for(int i = 0; i < n; i++)
  {
    double l1, l2;
    fin1 >> l1;
    fin2 >> l2;
    leaf1.push_back(l1);
    leaf2.push_back(l2);
  }

  //cout << "LEFT SPIN" << '\n';

  for(int i = 0; i < k; i++)
  {
    double val =  pearsonCorrel(0,n-1-i,i,n-1);
     //cout << val << '\n';
     maxVal= max(maxVal, val);
  }

  //cout << "RIGHT SPIN" << '\n';

  for(int i = 0; i < k; i++)
  {
    double val =  pearsonCorrel(i,n-1,0,n-1-i);
     //cout << val << '\n';
     maxVal= max(maxVal, val);
  }

  cout << maxVal << '\n';

  return 0;
}
