#include "PointVector.hpp"

//constructor
PointVector::PointVector(){
    PointVector::size = 0;
}

void PointVector::addPoint(Point p){
    PointVector::vectorPoint.push_back(p);
    PointVector::size++;
}

float PointVector::getDistance(int id1, int id2){
    Point p1 = PointVector::vectorPoint.at(id1);
    Point p2 = PointVector::vectorPoint.at(id2);
    return std::sqrt(std::pow(p2.x - p1.x,2) + std::pow(p2.y - p1.y,2));
}