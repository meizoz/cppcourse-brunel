#include "Neuron.hpp"
#include "Constants.hpp"
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <random>
using namespace std;

int main()
{
	
	//number of neurons------------------------------------------------------------------------------------------------------------------------------------------------------
	
	
	const int nbrENeurons(10000);
	const int nbrINeurons(2500);
	
	//initialisation----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	const int nbrNeurons = nbrENeurons + nbrINeurons; //simulation of 12'500 neurons
	const int inhibitoryBegin (nbrENeurons); // 10'000 exitatory and 2'500 inhibitory : the firste inhibitory is brain[10000]
	
	const int nbrEConnections = nbrENeurons/10; //10 percent connection
	const int nbrIConnections = nbrINeurons/10;
	
	array<Neuron,nbrNeurons> brain; 
	
	for(int i(inhibitoryBegin);i<brain.size();i++) //set the inhibitory neurons
	{
		brain[i].setNeuronType(true);
	}
	
	std::random_device rd;    
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni1(0,inhibitoryBegin-1);
	std::uniform_int_distribution<int> uni2(inhibitoryBegin,brain.size()-1); //set the random number generators
	
	
	
	for(int n(0);n<brain.size();n++) //every neuron recieves the connection of 1000 exitatory neuron and 250 inhibitory ones (10 percent connection)
	{
		for(int i(0);i<nbrEConnections;i++)
		{
			int target;
			
			do
			{
				target = uni1(rng);
				
			} while(target == n);
			
			brain[target].addTarget(n);
		}
		
		for(int j(0);j<nbrIConnections;j++)
		{
			int target;
			do
			{
				target = uni2(rng);
				
			} while(target == n);
			
			brain[target].addTarget(n);
		}
	}
	
	
	
	
	//Simulation---------------------------------------------------------------------------------------------------------------------------------------
	
	int nbrCycles(12000);
	
	std::random_device ra;
	std::mt19937 gen(ra());
	std::poisson_distribution<> poi(NU_EXT*DT); //poisson generation to add random spikes from other parts of the brain at every time steps
	
	ofstream file1;
	file1.open("spikes.txt");
	int spikesCount(0);
	
	for(int i(0);i<nbrCycles;i++)
	{
		for(int j(0);j<brain.size();j++)
		{
			brain[j].addPotential(J*poi(gen)); //addition of the random spikes
			vector<int> tar(brain[j].update(i,0)); 
			
			if(!tar.empty())
			{
				spikesCount++;
				
				for(int k(0);k<tar.size();k++)
				{
					brain[tar[k]].recieve(i,D,brain[j].isInhibitory());
				}
			}
		}
		
		if(file1.is_open())
		{
			file1<<i*DT<<" "<<spikesCount<<endl;
		}
		spikesCount=0;
	}
	
	file1.close();
}
