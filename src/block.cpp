# include "../include/block.h"

namespace vision{
  Block::Block(std::vector<double> position, Color color): distance(position[1]), angle(position[0]), cubeColor(color), dimension(0.0508) {}

    double Block::getDistance(){
        return distance;
    }

    double Block::getAngle(){
        return angle;
    }

    Color Block::getColor(){
        return cubeColor;
    }
}
