#ifndef __SHUFFLEBAG_H__
#define __SHUFFLEBAG_H__

#include "cocos2d.h"
USING_NS_CC;

template<typename T>
class ShuffleBag
{
public:
	ShuffleBag() : cursor_(-1) {}

	void add(const T& value)
	{
		data_.push_back(value);
		cursor_ = data_.size() - 1;
	}

	void add(const T& value, int count)
	{
		while (count-- > 0)
			add(value);
	}

	T next()
	{
		if (cursor_ < 1)
		{
			cursor_ = data_.size() - 1;
			return data_[0];
		}

		const int grab = rand() /*'cocos2dx rand() funcation' random() */ % (cursor_ + 1);
		const T temp = data_[grab];
		std::swap(data_[grab], data_[cursor_]);
		cursor_--;
		return temp;
	}

	int getBit();
	int getBagSize() { return cursor_ + 1; };

	void printProbabilities();
	std::map<T, int> getCountMap() { return countMap_; };
	std::map<T, float> getProbMap() { return probMap_; };

private:
	std::vector<T> data_;
	int cursor_;

	std::map<T, int> countMap_;
	std::map<T, float> probMap_;
};


class ShuffleExample : public cocos2d::Layer
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(ShuffleExample);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void createBag();

private:
	ShuffleBag<int>* _bag;
	void printProbMap();
};

#endif
