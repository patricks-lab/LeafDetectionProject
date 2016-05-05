#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

#define PI 3.14159265

using namespace std;
using namespace cv;

RNG rng(12345);

vector< vector<Point> > contours;

vector<vector <Point> > temp;

vector<Point> approx;

vector<float> res;

int bigCont = 0;

bool done = false;
bool doneselect = false;
Point maxa,maxb;
Point rmva,rmvb;
int here = 0;
float scaling;

Mat img;

//Distance between two points.
float dist(Point a,Point b)
{
   return sqrt(pow(a.x-b.x,2) + pow(a.y-b.y,2));
}

float ab2l(int p1x, int p1y, int p2x, int p2y)
{
  float out = atan2(p2y-p1y,p2x-p1x);
  return out;
}

/******
Returns: The Angle (DEGREES) formed with 3 points.
 /|\ /\
B o  o C
  | /
  |/
  o
  A
find the m/_ABC
******/
float angle3points(int leftx, int lefty, int midx, int midy, int rightx, int righty)
{
  float out;
  //Angle formed by angle /_ LeftMidRight
  out = ab2l(leftx,lefty,midx,midy) - ab2l(rightx,righty,midx,midy);
  if(out < 0) out += 2 * PI;
  return out * 180/PI;
}

/***
Refer to Notebook for more DETAIL!
***/
float findThirdPoint(int x1,int y1, int x2 ,int y2, int why)
{
  float m,b;
   m = float(y1-y2)/float(x1-x2);
   b = y1 - (m * x1);
   return (why-b)/m;
}

bool between(float a, int b, int c)
{
  //2 cases:
  if(a > b && a < c) return true;
  if(a > c && a < b) return true;
  return false;
}

/****
leafWidths: Select 100 equidistant points for Leaf Distances
****/
void leafWidths()
{
  //When processing, use smoother polygon!
  approxPolyDP(contours[0],approx,0.008*arcLength(contours[0],true),true);

  //Debugging
  vector<vector<Point> > n2;
  n2.push_back(approx);
  drawContours(img, n2, -1, Scalar(0,128,255));
  //Debugging

  Point centroid = Point(0,0);
  float perimeter = 0;
  float numOfPoints = 100;
  float moveBy = 0;
  float mvB = 0;

  float currX = approx[0].x;
  float currY = approx[0].y;

  //find the centroid..
  for(int i = 0; i < approx.size(); i++)
  {
    centroid.x += approx[i].x;
    centroid.y += approx[i].y;
    if(i != approx.size()-1) perimeter += dist(approx[i],approx[i+1]);
  }
  perimeter += dist(approx[approx.size()-1],approx[0]);

  centroid.x /= approx.size();
  centroid.y /= approx.size();

  circle(img,centroid,10,Scalar(0,255,255));

  moveBy = perimeter/numOfPoints;
  mvB = moveBy;

  /******
  Start determing KEYpoints on the polygon
  Case 1: all points (0,1),(1,2),....(99,100)
  ******/

  for(int i = 0; i < approx.size()-1; i++)
  {

    //circle(img,approx[i],5,Scalar(0,255,128));
   while(true)
   {
     //Delta a, Delta b
     //(ah)^2 + (bh)^2 = moveBy^2
     //Find h.

      /**
      Will move the point by perim/numOfPoints
      **/
     float a = approx[i+1].x - approx[i].x;
     float b = approx[i+1].y - approx[i].y;

     float h = sqrt(mvB*mvB/(a*a + b*b));

     if(mvB < moveBy)
     {
       mvB = moveBy;
     }

     currX += a*h;
     currY += b*h;



     /**
     Check if we have gone too far..
     **/
     if(!between(currX,approx[i].x,approx[i+1].x) && !between(currY,approx[i].y,approx[i+1].y))
     {
       //circle(img,Point(currX,currY),5,Scalar(0,255,128));
       //Deal with carryover.
       mvB -= dist(Point(currX-a*h, currY-b*h), approx[i+1]);

       currX = approx[i+1].x;
       currY = approx[i+1].y;

       break;
     } else {
       //Find dist between point and centroid
       circle(img,Point(currX,currY),5,Scalar(0,255,255));
       line(img,centroid,Point(currX,currY),Scalar(255,0,0));
       cout << dist(Point(currX,currY),centroid) << '\n';
       res.push_back(dist(Point(currX,currY),centroid));
     }

   }

  }


  /******
  Case 2: points (100,0)
  ******/

  mvB = moveBy;

  while(true)
  {
    //Delta a, Delta b
    //(ah)^2 + (bh)^2 = moveBy^2
    //Find h.

    /*
     Will move the point by perim/numOfPoints
    */
    float a = approx[0].x - approx[approx.size()-1].x;
    float b = approx[0].y - approx[approx.size()-1].y;

    float h = sqrt(mvB*mvB/(a*a + b*b));

    if(mvB < moveBy)
    {
      mvB= moveBy;
    }


    currX += a*h;
    currY += b*h;


    /*
    Check if we have gone too far..
    */
    if(!between(currX,approx[approx.size()-1].x,approx[0].x) && !between(currY,approx[approx.size()-1].y,approx[0].y))
    {
      //Deal with carryover.
      mvB -= dist(Point(currX, currY), approx[0]);

      currX = approx[0].x;
      currY = approx[0].y;

      break;
    } else {
      //Find dist between point and centroid
      circle(img,Point(currX,currY),5,Scalar(0,128,255));
      line(img,centroid,Point(currX,currY),Scalar(255,0,0));
      cout << dist(Point(currX,currY),centroid) << '\n';
      res.push_back(dist(Point(currX,currY),centroid));
    }

  }
  circle(img,approx[0],5,Scalar(0,0,255));
  circle(img,approx[approx.size()-1],5,Scalar(0,128,255));

}

/**
Calculates if a leaf is seratted using a formula, namely:

seratedness= ConvexArea-leafArea

**/
double calculateConvex()
{
  vector<Point> hull(contours[0].size());
  convexHull( Mat(contours[0]), hull, false );
  //Debugging
  vector<vector<Point> > h2;
  h2.push_back(hull);
  //**
  drawContours(img,h2,-1,Scalar(255,0,255));

  //Debugging
  double hullArea = contourArea(hull);
  double leafArea = contourArea(contours[0]);

  cout << hullArea << " " << leafArea << '\n';
  return hullArea-leafArea;
}



/****
Takes a leaf and finds the contour
(Segmenting green parts, filtering, etc..)
****/

void filtering()
{

  vector<Vec4i> hierarchy;

  vector<Point> approx;

  Mat mask;
  Mat src;
  int big_a = 0;


  cvtColor(img,src,CV_BGR2HSV);

  Scalar lowerb = Scalar(25,50,50);//Scalar(30,127,51);
  Scalar upperb = Scalar(82,255,255);//Scalar(82,255,255);

  inRange(src,lowerb,upperb,mask);

  //Debugging
  imshow("raw mask", mask);

  Mat kernel = Mat::ones(5*scaling,5*scaling,CV_8UC1);
  dilate(mask,mask,kernel);

  //Debugging
  imshow("PROCESSED mask", mask);

  findContours(mask, temp,hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

//imshow("Mask",mask);
  //Only extract biggest contour.
  for( int i = 0; i< temp.size(); i++ )
  {
    double a = contourArea(temp[i],false);
    if(a > big_a) {
      big_a = a;
      bigCont = i;
    }
  }

  //Debugging
  Mat fll = Mat::zeros( 800, 1000, CV_8UC3 );
  drawContours(fll, temp, bigCont, Scalar(0,128,255));
  imshow("found!", fll);

  double d = 0;
  double minDist = 100000000;
  int minIndex;

  //Check the closest point of the contour; the closest to maxA will be the start!
  for(int i = 0; i < temp[bigCont].size(); i++)
  {
    d = dist(temp[bigCont][i],maxa);
    if(d < minDist)
    {
      minDist = d;
      minIndex = i;
    }
  }

  vector<Point> temp2;
  for(int i = minIndex; i < temp[bigCont].size(); i++)
  {
    temp2.push_back(temp[bigCont][i]);
  }

  for(int i = 0; i < minIndex; i++)
  {
    temp2.push_back(temp[bigCont][i]);
  }

  contours.push_back(temp2);
}

/***
Takes in User defined Stem.
MAIN CODE!!
***/
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

    if(event == EVENT_RBUTTONDOWN)
     {
       /*
       here => number of times clicked
       Clicks 1 AND 2: set the major vein of the leaf
       Clicks 3 and 4: remove the stem of the leaf
       */
       here++;

       if(here == 1)
       {
         maxa = Point(x,y);
         circle(img,maxa,10,Scalar(255,255,255));
       }
       if(here == 2)
       {
         maxb = Point(x,y);
         circle(img,maxb,10,Scalar(255,255,255));
       }
       if(here == 3) {
         rmva = Point(x,y);
         circle(img,rmva,8,Scalar(0,0,255));
       }
       if(here == 4)
       {
         rmvb = Point(x,y);
         circle(img,rmvb,8,Scalar(0,0,255));
         doneselect = true;
       }



       if(doneselect)
       {
         rectangle(img,rmvb,rmva,Scalar(255,255,255),CV_FILLED);
         float dist = sqrt(pow(maxa.y-maxb.y,2)+pow(maxa.x-maxb.x,2));

         //Resize image
         float scaling = 600.0/dist;
         resize(img,img,Size(img.cols*scaling,img.rows*scaling));
         maxa = Point(maxa.x*scaling,maxa.y*scaling);
         maxb = Point(maxb.x*scaling,maxb.y*scaling);

         //Now that we have the scaling variable initialized, run a contour
         filtering();
         calculateConvex();

         line(img,maxa,maxb,Scalar(255,0,0));

         //In here, we will deal with the processing. Like green extraction, distances, etc.
         leafWidths();

         cout << "Distance= " << dist  << '\n';

       }

     }
}




/****
Main code
****/

int main( int argc, char** argv ) {

  cout << "I am compiled" << '\n';

 string in = argv[1];

  ofstream fout(in+".txt");

  //for different starting points on stem
  ofstream frev(in+".txt.reversed");

  ofstream comp(in+".txt.convex");

  //ofstream fsout(in+".txt.slope");

  //ofstream fsrev(in+".txt.slope.reversed");


  img = imread(in);

  namedWindow("My Window", 1);
   setMouseCallback("My Window", CallBackFunc, NULL);
  while(1)
  {
     imshow("My Window",img);
     if(waitKey(30)>=0) break;
  }

   //Convex hull difference
   //comp << calculateConvex() << '\n';

   //starting on TIP of leaf
   for(int i =0; i < res.size(); i++)
   {
     fout << res[i] << '\n';
   }

   //starting on BOTTOM of leaf
   for(int i = res.size()/2; i < res.size(); i++)
   {
     frev << res[i] << '\n';
   }
   for(int i = 0; i < res.size()/2; i++)
   {
     frev << res[i] << '\n';
   }


  //float dist = sqrt(pow(maxa.y-maxb.y,2)+pow(maxa.x-maxb.x,2));
  //double perimetersquare = pow(arcLength( contours[0], true ),2);
  //double area = contourArea(contours[0]);

  //Calculate density
  //comp << "P^2:" << perimetersquare << " A:" << area << " stemD:" << dist <<  '\n';
  //comp  << perimetersquare/4*PI*area << '\n';
  //comp  << PI*pow(dist,2)/4*area << '\n';
  return 0;
}
