//
// Created by ben on 11/11/18.
//

#include "Observable.h"
#include <vector>

#include <iostream>

using namespace state;



Observable::~Observable (){

}

bool Observable::addObserver (Observer * new_obs){
    std::cout<<"adding new observer at addr "<<&observers<<std::endl;
    this->observers.push_back(new_obs);
    return true;
}

bool const Observable::removeObserver (Observer * old_obs){
    for(unsigned int index = 0; index < observers.size(); index ++)
    {
        if(&observers[index] == &old_obs)
        {
            observers.erase(observers.begin()+index);
            return true;
        }
    }
    return false;
}

bool const Observable::removeAllObservers (){
    observers.erase(observers.begin(),observers.end());
    return true;
}

bool Observable::notifyObservers (const Event& event){
    for(auto observer : observers)
    {
        observer->stateChanged(event);
    }
    return true;
}