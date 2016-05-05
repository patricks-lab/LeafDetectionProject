#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

vector<double> leaf1;
vector<double> leaf2;
double n = 95;

double absDist(int v1s, int v1e, int v2s, int v2e)
{

  double dist = 0;
  double demoniator = 0;
  double out = 0;
  //loop from v1s->v1e
  //           v2s->v2e
   for(int i = v1s, j = v2s; i < v1e; i++, j++)
   {
      dist += min(leaf1[i],leaf2[j]);
      demoniator += max(leaf1[i],leaf2[j]);
   }

   out = (dist/demoniator) * 100;

   return out;

}


int main (int argc, char *argv[])
{
  double maxVal = 0;
  if(argc != 3)
  {
    cout<<  argc << "must have 2 arguments!" << '\n';
    return 0;
  }

  int k = 5;
  double minVal = 1000000;
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
    minVal = min(minVal,min(l1,l2));
  }

  for(int i = 0; i < n; i++)
  {
    leaf1[i] -= minVal;
    leaf2[i] -= minVal;
  }

  //cout << "LEFT SPIN" << '\n';



  for(int i = 0; i < k; i++)
  {
    double val =  absDist(0,n-1-i,i,n-1);
     //cout << val << '\n';
     maxVal= max(maxVal, val);
  }

  //cout << "RIGHT SPIN" << '\n';

  for(int i = 0; i < k; i++)
  {
    double val =  absDist(i,n-1,0,n-1-i);
     //cout << val << '\n';
     maxVal= max(maxVal, val);
  }

  cout << maxVal << '\n';

  return 0;
}
