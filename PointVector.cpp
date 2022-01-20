#include "PointVector.hpp"

//constructor
PointVector::PointVector(){
    PointVector::size = 0;
}

void PointVector::addPoint(float x, float y){
    Point new_point = {x, y};
    PointVector::vectorPoint.push_back(new_point);
    PointVector::size++;
}

float PointVector::getDistance(int id1, int id2){
    Point p1 = PointVector::vectorPoint.at(id1);
    Point p2 = PointVector::vectorPoint.at(id2);
    auto dx = p2.x - p1.x;
    auto dy = p2.y - p1.y;
    return dx*dx + dy*dy;
}