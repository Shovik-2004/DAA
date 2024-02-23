#include<iostream>
#include<set>
#include<vector>
using namespace std;
struct point
{ 
   int x, y;
   bool operator==(point p2){
      if(x == p2.x && y == p2.y)
         return 1;
      return 0;
   }
   bool operator<(const point &p2)const{ 
      return true;
   }
};
int crossProduct(point a, point b, point c)
{ 
   int y1 = a.y - b.y;
   int y2 = a.y - c.y;
   int x1 = a.x - b.x;
   int x2 = a.x - c.x;
   return y2*x1 - y1*x2; 
int distance(point a, point b, point c)
{
   int y1 = a.y - b.y;
   int y2 = a.y - c.y;
   int x1 = a.x - b.x;
   int x2 = a.x - c.x;
   int item1 = (y1*y1 + x1*x1);
   int item2 = (y2*y2 + x2*x2);
   if(item1 == item2)
      return 0; 
   else if(item1 < item2)
      return -1; 
   return 1; 
}
set<point> findConvexHull(point points[], int n)
{
   point start = points[0];
   for(int i = 1; i<n; i++)
   { 
      if(points[i].x < start.x)
         start = points[i];
   }
   point current = start;
   set<point> result; 
   result.insert(start);
   vector<point> *collinearPoints = new vector<point>;
   while(true)
   {
      point nextTarget = points[0];
      for(int i = 1; i<n; i++)
	  {
         if(points[i] == current) 
            continue;
         int val = crossProduct(current, nextTarget, points[i]);
         if(val > 0){ 
            nextTarget = points[i];
            collinearPoints = new vector<point>;
         }else if(val == 0)
		 { 
            if(distance(current, nextTarget, points[i]) < 0)
			{ 
               collinearPoints->push_back(nextTarget);
               nextTarget = points[i];
            }else
			{
               collinearPoints->push_back(points[i]); 
            }
         }
      }
      vector<point>::iterator it;
      for(it = collinearPoints->begin(); it != collinearPoints->end(); it++)
	  {
         result.insert(*it); 
      }
      if(nextTarget == start)
         break;
      result.insert(nextTarget);
      current = nextTarget;
   }
   return result;
}
int main()
{
   point points[] = {
      {-7,8},{-4,6},{2,6},{6,4},{8,6},{7,-2},{4,-6},{8,-7},{0,0},
      {3,-2},{6,-10},{0,-6},{-9,-5},{-8,-2},{-8,0},{-10,3},{-2,2},{-10,4}};
      int n = 18;
      set<point> result;
      result = findConvexHull(points, n);
      cout << "Boundary points of convex hull are: "<<endl;
      set<point>::iterator it;
      for(it = result.begin(); it!=result.end(); it++)
         cout << "(" << it->x << ", " <<it->y <<") ";
}
