#include "../include/localizer.h"

namespace map{
  Localizer::Localizer(Map m, int irR1, int usTR1, int usER1, int irR2, int usTR2, int usER2, int irR3, int usTR3, int usER3):r1(irR1, usTR1, usER1), r2(irR2, usTR2, usER2), r3(irR3, usTR3, usER3), obsModel(m){
    
  }
  
  void Localizer::updateWeights(double l1, double l2, double l3){
    Particle p; 
    double sig = 0;
    //loop through particles
    for (int i = 0; i < particles.size(); i++){
      p = particles.at(i);
      //adjust weight based on observation
      p.weight = obsModel.getPObsGivenPos(p, l1, l2, l3);
      //sum up new weights
      sig += p.weight;
    }
    //loop through again to renormalize
    for (int j = 0; j < particles.size(); j++){
      p = particles.at(j);
      p.weight /= sig;
    }
  }
  
  void Localizer::step(){
    
  }
}
