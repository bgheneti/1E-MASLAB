#include <complex>   

enum Color{
  red,
  green
}

namespace vision{
    class Block{
        private:
	    const double dimension = .0508;
	    std::polar position;
	    Color cubeColor;
        public:
	    Block(std::polar pos, Color color);
	    double getPosition();
	    Color getColor();
    }
}
