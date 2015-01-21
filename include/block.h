#ifndef INCLUDE_BLOCK_H_
#define INCLUDE_BLOCK_H_

#include <complex>   

namespace vision{

    enum Color{
        red,
	green
    };

    class Block{
        private:
	    const double dimension;
	    std::complex<double> position;
	    Color cubeColor;
        public:
	    Block();
	    Block(std::complex<double> pos, Color color);
	    std::complex<double> getPosition();
	    Color getColor();
    };
}

#endif
