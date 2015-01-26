#ifndef INCLUDE_BLOCK_H_
#define INCLUDE_BLOCK_H_

#include <complex>   
#include <vector>

namespace vision{

    enum Color{
        red,
	green
    };

    class Block{
        private:
	    const double dimension;
	    double distance;
	    double angle;
	    Color cubeColor;
        public:
	    Block(std::vector<double> position, Color color);
	    double getDistance();
	    double getAngle();
	    Color getColor();
    };
}

#endif
