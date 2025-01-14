#include <iostream>
#include <map>
#include <vector>

#include "prqueue.h"

using namespace std;

int main()
{
	map<int, vector<int>> mp;
    vector<int> vals = {15, 16, 17, 6, 7, 8, 9, 2, 1};

    vector<int> priorities = {1, 2, 9, 3, 5, 6, 7, 8, 4};
    prqueue<int> pq;

    for (int i = 0; i < vals.size(); i++) {
        pq.enqueue(vals[i], priorities[i]);
        mp[priorities[i]].push_back(vals[i]);
    }

	cout << pq.size() << endl;
	cout << vals.size() << endl;

	return 0;
}
