#include "BucketSort.h"

#include <algorithm>
#include <cmath>



void BucketSort::sort(unsigned int numCores){
	const auto numThreads = numCores - 1;
	buckets.clear();
	buckets.resize(10);

	for(const auto& num: numbersToSort){
		auto firstDigit = getFirstDigit(num);
		buckets[firstDigit].push_back(num);
	}

	auto threads = std::vector<std::thread>{};
	for(unsigned int i = 0; i<numThreads; ++i){
		threads.push_back(std::thread{[this]() {this->sortBucket(); }});
	}
	sortBucket();

	for(auto& t:threads){
		if(t.joinable()){
			t.join();
		}
	}
}


bool BucketSort::aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) const {
    
        if (x == y) return false;
	
	unsigned int a = x;
        unsigned int b = y;
    
	if (pow == 0) {
                while (a / 10 > 0) {
                        a = a / 10; 
                }   
                while (b / 10 > 0) {
                        b = b / 10;
                }
        } else {
        	while (a / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
                        a = a / 10;
                }
                while (b / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
                        b = b / 10;
                }
        }

        if (a == b)
                return aLessB(x,y,pow + 1);  // recurse if this digit is the same 
        else
                return a < b;
}





void BucketSort::sortBucket() {
	auto p = getBucketIndex();
	while(p.second !=false){
		auto& bucket = buckets[p.first];
        	std::sort(bucket.begin(),bucket.end(), [this](const unsigned int& x, const unsigned int& y){
          	      return aLessB(x,y,0);
		} );

		unsigned int pos = 0;
		for(unsigned int i=0; i<p.first; ++i){
			pos+=buckets[i].size();
		}

		for(unsigned int i=0;i<bucket.size(); ++i){
			numbersToSort[pos] = bucket[i];
			++pos;
		}

		p = getBucketIndex();
	}
}


unsigned int BucketSort::getFirstDigit(unsigned int x) const {
	while(x >= 10){
		x/=10;
	}
	return x;
}



std::pair<unsigned int, bool> BucketSort::getBucketIndex() {
	std::lock_guard<std::mutex> guard(currBucketIndexMutex);
	if(currBucketIndex < buckets.size()){
		return std::make_pair(currBucketIndex++, true);
	}
	return std::make_pair(currBucketIndex, false);
}


