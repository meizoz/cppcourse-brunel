#include "Neuron.hpp"
#include "Constants.hpp"
#include <cmath>

const double A = exp(-(DT/TAU)); // V(t+h)= A*V(t) + extCur*B
const double B = R*(1-A);

Neuron::Neuron(std::vector<int> targs) : pot(0), refractTimer(-1)
{
		targets = targs;
		
		for(int i(0);i<recievedBuffer.size();i++)
		{
			recievedBuffer[i] = 0.0;
		}
}

Neuron::Neuron() : pot(0), refractTimer(-1), isInhib(false)
{
	for(int i(0);i<recievedBuffer.size();i++)
		{
			recievedBuffer[i] = 0.0;
		}
}

double Neuron::getPot() const
{
	return pot;
}

bool Neuron::isInhibitory() const
{
	return isInhib;
}

void Neuron::setNeuronType(bool inhib)
{
	isInhib = inhib;
}

void Neuron::addPotential(double extPot)
{
	pot += extPot;
}

void Neuron::setTargets(std::vector<int> tar)
{
	targets = tar;
}

void Neuron::addTarget(int tar)
{
	targets.push_back(tar);
}

std::vector<int> Neuron::getTargets() const
{
	return targets;
}

void Neuron::recieve(int t, int delay, bool isInhibitory)
{
	if(isInhibitory)
	{
		recievedBuffer[(t+delay) % (D+1)] += JI;
	}
	else
	{
		recievedBuffer[(t+delay) % (D+1)] += J;
	}
}

bool Neuron::isRefractory()const
{
	if(refractTimer >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Neuron::update(int t, double extCur)
{
	
	if(!isRefractory())
	{	
		pot = A*pot + extCur*B; //update of potential (pot)
		
		pot += recievedBuffer[t % (D+1)];
		recievedBuffer[t % (D+1)] = 0.0;
		
	}
	else
	{
		 refractTimer -= DT;
		 recievedBuffer[t % (D+1)] = 0.0;
	}
	
	if(pot > NEUR_THRESHOLD) //condition for spike
	{
		//spikes.push_back(t);
		pot = 0;
		refractTimer = 2;
		
		return true;	
	}
	 
	
	
	return false;
}
