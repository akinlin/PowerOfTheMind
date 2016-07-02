#include "ShuffleBag.h"

template<typename T>
int ShuffleBag<T>::getBit()
{
	auto value = next();
	return 0;
}

template<typename T>
void ShuffleBag<T>::printProbabilities()
{
	std::string probHeader = "turn ";

	std::stringstream turn;
	turn << probHeader << (data_.size() - 1) - cursor_;
	probHeader = turn.str();

	std::stringstream vectorValues;

	countMap_.clear();
	std::string values = " [ ";
	for (int i = 0; i < cursor_ + 1; i++)
	{
		vectorValues << i << ":" << data_[i] << " ";
		countMap_[data_[i]]++;
		values += vectorValues.str();
		vectorValues.str("");
	}
	vectorValues << "] - " << cursor_ + 1;
	values += vectorValues.str();
	probHeader += values;

	// build probabilities table
	typedef std::map<T, int>::iterator it_type;
	for (it_type iterator = countMap_.begin(); iterator != countMap_.end(); iterator++)
	{
		float count = iterator->second;
		float percentage = (num / total) * 100;
		probMap_[iterator->first] = percentage;
	}

	log("%s", probHeader.c_str());
}

bool ShuffleExample::init()
{
	if (!Layer::init())
    { 
        return false;
    }

	createBag();

    return true;
}

void ShuffleExample::createBag()
{
	_bag = new ShuffleBag<int>();
	_bag->add(1, 2);
	_bag->add(2, 3);
	_bag->add(3, 1);
	_bag->add(4, 4);

	for (int i = 0; i < 10; i++)
	{
		_bag->printProbabilities();
		printProbMap();
		log("Value chosen: %d", _bag->next());
		log("");
	}
}

void ShuffleExample::printProbMap()
{
	auto probMap = _bag->getProbMap();
	float total = _bag->getBagSize();

	typedef std::map<int, float>::iterator it_type;
	for (it_type iterator = probMap.begin(); iterator != probMap.end(); iterator++)
	{
		int value = iterator->first;
		float percentage = iterator->second;
		log("value %d - %.0f%%", value, percentage);
	}
}