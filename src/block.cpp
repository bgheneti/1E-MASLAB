# include "../include/block.h"

namespace vision{
    Block::Block(std::complex<double> pos, Color color): position(pos), cubeColor(color), dimension(0.0508) {}

    std::complex<double> Block::getPosition(){
        return position;
    }
    Color Block::getColor(){
        return cubeColor;
    }
}
