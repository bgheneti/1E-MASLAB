#ifndef INCLUDE_POINT_H_
#define INCLUDE_POINT_H_
#include <cmath.h>
namespace utils {
    class Point {
        private:
            const float x;
            const float y;
        public:
            Point(float x, float y) {
                x = x;
                y = y;
            }
            float getX() {
                return x;
            }
            float getY() {
                return y;
            }
            float getDistanceTo(utils::Point otherPoint) {
                float delX = otherPoint.getX() - x;
                float delY = otherPoint.getY() - y;
                return sqrt(delX*delX + delY*delY);
            }

}


#endif
