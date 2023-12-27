#include "gain.h"
#include <iostream>

Gain::Gain(float gainValue)
{
    this->gain_value = gainValue;
}

float Gain::getGainValue() const {
    return this->gain_value;
}

float Gain::update(float input) {
	return input*(this->getGainValue());
}
