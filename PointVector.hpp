#include <vector>
#include <cmath>

struct Point
{   
    float x;
    float y;
} ;

class PointVector{
    std::vector<Point> vectorPoint;

    public:
        //size of the vector
        int size;

        /**
         * Create a PointVector object
         */
        PointVector();

        /**
         * Add a point to the vector
         *
         * @param x coordinate x of the new point
         * @param y coordinate y of the new point
         */
        void addPoint(float x, float y);
        
        /**
         * compute the distance between the points in position id1 and id2 
         * Note: you should square root the returned value to
         *
         * @param id1 index of the first point in the pointVector
         * @param id2 index of the second point in the pointVector
         * @throws std::out_of_range exception if id1 or id2 are not in bounds
         * @return the distance (without square root) between the two points
         */
        float getDistance(int id1, int id2);
};