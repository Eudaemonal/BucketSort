#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <vector>

class BucketSort {
public:
	void sort(unsigned int numCores);
	std::vector<unsigned int> numbersToSort;

private:
	bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow = 0) const;
	void sortBucket();
	unsigned int getFirstDigit(unsigned int x) const;
	std::pair<unsigned int, bool> getBucketIndex();

	std::vector<std::vector<unsigned int>> buckets {};
	unsigned int currBucketIndex = 0;
	std::mutex currBucketIndexMutex {};

};


#endif /* BUCKET_SORT_H */
