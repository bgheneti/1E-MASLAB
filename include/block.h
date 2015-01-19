#include <complex>   

enum Color{
  red,
  green
};

namespace vision{
    class Block{
        private:
	    const double dimension;
	    std::complex<double> position;
	    Color cubeColor;
        public:
	    Block(std::complex<double> pos, Color color);
	    std::complex<double> getPosition();
	    Color getColor();
    };
}
