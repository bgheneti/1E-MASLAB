#include "../include/localizer.h"
#include <stdlib.h>
#include <math.h>
#include <vector>

namespace map{
  Localizer::Localizer(Map m, int irR1, int usTR1, int usER1, int irR2, int usTR2, int usER2, int irR3, int usTR3, int usER3):r1(irR1, usTR1, usER1), r2(irR2, usTR2, usER2), r3(irR3, usTR3, usER3), obsModel(m){
    sampleSize = 10000;
    for (int i = 0; i < sampleSize; i++){
      Particle p;
      p.weight = 1;
      utils::Point pt = m.getLocation();
      p.x = pt.x;
      p.y = pt.y;
      p.th = double(rand()%360);//0 degrees is upwards
      particles.push_back(p);
    }
    xMax = obsModel.getXMax();
    yMax = obsModel.getYMax();
    xMin = 0;
    yMin = 0;
  }
  
  void Localizer::updatePos(double dx, double dy, double dth){
    int baselineChange = int(dx); //the amount that every particle shifts
    int len = particles.size();
    
    //update x
    for (int j = 0; j < len; j++){
      Particle p = particles[j];
      p.x += baselineChange;
      Particle p2;
      p2.x = p.x + dx/std::abs(dx);
      p2.y = p.y;
      p2.th = p.th;
      p2.weight = std::abs(dx-baselineChange);
      p.weight = 1-p2.weight;
      particles.push_back(p2);
    }
    
    baselineChange = int(dy);
    len = particles.size();
    //update y
    for (int j = 0; j < len; j++){
      Particle p = particles[j];
      p.y += baselineChange;
      Particle p2;
      p2.x = p.x;
      p2.y = p.y + dy/std::abs(dy);
      p2.th = p.th;
      p2.weight = std::abs(dy-baselineChange);
      p.weight = 1-p2.weight;
      particles.push_back(p2);
    }
    
    baselineChange = int(dth);
    len = particles.size();
    
    //update th
    for (int j = 0; j < len; j++){
      Particle p = particles[j];
      p.th += baselineChange;
      Particle p2;
      p2.x = p.x;
      p2.y = p.y;
      p2.th = p.th + dth/std::abs(dth);
      p2.weight = std::abs(dth-baselineChange);
      p.weight = 1-p2.weight;
      particles.push_back(p2);
    }
  }
  
  void Localizer::updateWeights(double l1, double l2, double l3){ 
    double sig = 0;
    //loop through particles
    for (int i = 0; i < particles.size(); i++){
      Particle p;
      p = particles.at(i);
      //adjust weight based on observation
      p.weight = obsModel.getPObsGivenPos(p, l1, l2, l3);
      //sum up new weights
      sig += p.weight;
    }
    //loop through again to renormalize
    for (int j = 0; j < particles.size(); j++){
      Particle p;
      p = particles.at(j);
      p.weight /= sig;
    }
  }
  
  void Localizer::step(){
    std::vector<Particle> newParticles;
    sampleSize *= 0.99;
    
    //the probabilities (markov chain) of an increase/decrease in x/y/th
    double xTransition = 0.025;
    double yTransition = 0.025;
    double thTransition = 0.1;
    
    //var for summing and normalization
    double sig = 0;
    
    //loop through and add particles to new vector
    for (int i = 0; i < particles.size(); i++){
      if (particles[i].weight > 1.0/sampleSize){
        Particle oldParticle = particles[i];
        for (int dx = -1; dx <= 1; dx++){
          for (int dy = -1; dy <= 1; dy++){
            for (int dth = -1; dth <= 1; dth++){
              double p = 1;
              if (dx) {p*=xTransition;}
              else {p*=(1-2*xTransition);}
              if (dy) {p*=yTransition;}
              else {p*=(1-2*yTransition);}
              if (dth) {p*=thTransition;}
              else {p*=(1-2*thTransition);}
              
              for (int j = 0; j < int(sampleSize*p+0.5); j++){
                Particle newParticle;
                newParticle.x = std::min(std::max(oldParticle.x + dx, xMin), xMax);
                newParticle.y = std::min(std::max(oldParticle.y + dy, yMin), yMax);
                newParticle.th = fmod((oldParticle.th + dth), 360.0);
                newParticle.weight = 1;
                newParticles.push_back(newParticle);
                sig += newParticle.weight;
              }
            }
          }
        }
        
      }
    }
    
    particles = newParticles;
    //loop through again to renormalize
    for (int j = 0; j < particles.size(); j++){
      Particle p = particles.at(j);
      p.weight /= sig;
    }
  }
}
