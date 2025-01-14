#include "prqueue.h"

#include "gtest/gtest.h"

using namespace std;

template <typename T>

string map_as_string(const map<T, vector<int>>& m) {
    ostringstream ss;
    for (const auto &e : m) {
        int priority = e.first;
        vector<int> values = e.second;
        for (size_t j = 0; j < values.size(); j++) {
            ss << priority << " value: " << values[j] << endl;
        }
    }
    return ss.str();
}

TEST(Contructor, Empty) {
    prqueue<int> pq;
    EXPECT_EQ(pq.size(), 0);
}

TEST(Enque, OneElem) {
    prqueue<int> pq;
    pq.enqueue(24, 1);
    EXPECT_EQ(pq.size(), 1);
}

TEST(Enque, ThreeElems) {
    prqueue<int> pq;
    pq.enqueue(75, 7);
    pq.enqueue(24, 9);
    pq.enqueue(15, 3);
    EXPECT_EQ(pq.size(), 3);
}

TEST(Dequeue, DequeueANDPeekOne) {
    prqueue<int> pq;
    pq.enqueue(75, 7);
    EXPECT_EQ(pq.peek(), 75);
    EXPECT_EQ(pq.dequeue(), 75);
    pq.enqueue(24, 9);
    EXPECT_EQ(pq.peek(), 24);
    EXPECT_EQ(pq.dequeue(), 24);
    pq.enqueue(15, 3);
    EXPECT_EQ(pq.peek(), 15);
    pq.dequeue();
    EXPECT_EQ(pq.peek(), 0);
    EXPECT_EQ(pq.dequeue(), 0);
    EXPECT_EQ(pq.size(), 0);
    EXPECT_EQ(pq.getRoot(), nullptr);
}

TEST(Dequeue, DequeueANDPeekAll) {
    prqueue<int> pq;
    EXPECT_EQ(pq.peek(), 0);
    EXPECT_EQ(pq.dequeue(), 0);
    EXPECT_EQ(pq.getRoot(), nullptr);
    pq.enqueue(75, 7);
    pq.enqueue(24, 9);
    pq.enqueue(15, 3);
    EXPECT_EQ(pq.peek(), 15);
    pq.dequeue();
    EXPECT_EQ(pq.size(), 2);
    pq.clear();
    EXPECT_EQ(pq.peek(), 0);
    EXPECT_EQ(pq.dequeue(), 0);
    EXPECT_EQ(pq.size(), 0);
    EXPECT_EQ(pq.getRoot(), nullptr);
}

// TEST(Dequeue, DequeueThenEnqueue) {
//     prqueue<int> pq;
//     map<int, vector<int>> mp;
//     vector<int> vals = {15, 16, 17, 6, 7, 8, 9, 2, 1};
//     vector<int> priorities = {1, 2, 3, 6, 9, 11, 4, 5, 7};
//     for (int i = 0; i < vals.size(); i++) {
//         pq.enqueue(vals[i], priorities[i]);
//         mp[priorities[i]].push_back(vals[i]);
//     }
//     EXPECT_EQ(pq.size(), 9);
//     pq.dequeue();
//     pq.dequeue();
//     pq.enqueue(14, 16);
//     EXPECT_EQ(pq.size(), 8);
// }

TEST(Dequeue, DequeueANDPeek) {
    prqueue<int> pq;
    map<int, vector<int>> mp;
    vector<int> vals = {15, 16, 17, 6, 7, 8, 9, 2, 1};
    vector<int> priorities = {1, 2, 3, 6, 9, 11, 4, 5, 7};
    for (int i = 0; i < vals.size(); i++) {
        pq.enqueue(vals[i], priorities[i]);
        mp[priorities[i]].push_back(vals[i]);
    }
    EXPECT_EQ(pq.dequeue(), 15); 
    EXPECT_EQ(pq.size(), 8); 
    EXPECT_EQ(pq.peek(), 16);
}

TEST(Clear, Clear) {
    prqueue<int> pq;
    pq.enqueue(75, 7);
    pq.enqueue(24, 9);
    pq.enqueue(15, 3);
    pq.clear();
    EXPECT_EQ(pq.size(), 0);
    EXPECT_EQ(pq.getRoot(), nullptr);
}

TEST(AsString, AsString) {
    map<int, vector<int>> mp;
    vector<int> vals = {15, 16, 17, 6, 7, 8, 9, 2, 1};
    // Note that this uses duplicate priorities!
    vector<int> priorities = {1, 2, 3, 2, 2, 2, 2, 3, 3};
    prqueue<int> pq;
    for (int i = 0; i < vals.size(); i++) {
        pq.enqueue(vals[i], priorities[i]);
        mp[priorities[i]].push_back(vals[i]);
    }
    EXPECT_EQ(pq.size(), vals.size());
    EXPECT_EQ(pq.as_string(), map_as_string(mp));
}

TEST(BeginNext, BeginNext) {
    map<int, vector<int>> mp;
    vector<int> vals = {15, 17, 6, 7, 2};
    // Note that this uses duplicate priorities!
    vector<int> priorities = {1, 3, 2, 2, 3};
    prqueue<int> pq;
    for (int i = 0; i < vals.size(); i++) {
        pq.enqueue(vals[i], priorities[i]);
        mp[priorities[i]].push_back(vals[i]);
    }
    pq.begin();
    int value;
    int priority;
    EXPECT_EQ(true, pq.next(value, priority));
    EXPECT_EQ(vals[0], value);
    EXPECT_EQ(priorities[0], priority);
    
    EXPECT_EQ(true, pq.next(value, priority));
    EXPECT_EQ(vals[2], value);
    EXPECT_EQ(priorities[2], priority);
    
    EXPECT_EQ(true, pq.next(value, priority));
    EXPECT_EQ(vals[3], value);
    EXPECT_EQ(priorities[3], priority);

    EXPECT_EQ(true, pq.next(value, priority));
    EXPECT_EQ(vals[1], value);
    EXPECT_EQ(priorities[1], priority);

    EXPECT_EQ(true, pq.next(value, priority));
    EXPECT_EQ(vals[4], value);
    EXPECT_EQ(priorities[4], priority);

    EXPECT_EQ(false, pq.next(value, priority));
}

TEST(Copy, CopyConstructor) {
    map<int, vector<int>> mp;
    vector<int> vals = {15, 16, 17, 6, 7, 8, 9, 2, 1};
    // Note that this uses duplicate priorities!
    vector<int> priorities = {1, 2, 3, 2, 2, 2, 2, 3, 3};
    prqueue<int> pq;
    for (int i = 0; i < vals.size(); i++) {
        pq.enqueue(vals[i], priorities[i]);
        mp[priorities[i]].push_back(vals[i]);
    }
    prqueue<int> newpq;
    newpq.enqueue(5,5);

    EXPECT_FALSE(pq.size() == newpq.size());
    EXPECT_FALSE(pq.as_string() == newpq.as_string());
    EXPECT_FALSE(map_as_string(mp) == newpq.as_string());

    newpq = pq;

    EXPECT_EQ(pq.size(), newpq.size());
    EXPECT_EQ(pq.as_string(), newpq.as_string());
    EXPECT_EQ(map_as_string(mp), newpq.as_string());
}

TEST(Operator, EqualEqual) {
    map<int, vector<int>> mp;
    vector<int> vals = {15, 16, 17, 6, 7, 8, 9, 2, 1};
    // Note that this uses duplicate priorities!
    vector<int> priorities = {1, 2, 3, 2, 2, 2, 2, 3, 3};
    prqueue<int> pq;
    prqueue<int> pq1;
    for (int i = 0; i < vals.size(); i++) {
        pq.enqueue(vals[i], priorities[i]);
        pq1.enqueue(vals[i], priorities[i]);
        mp[priorities[i]].push_back(vals[i]);
    }
    prqueue<int> pq2;
    pq2.enqueue(5,5);

    EXPECT_TRUE(pq == pq1);
    pq1.dequeue();
    EXPECT_FALSE(pq == pq1);
    EXPECT_FALSE(pq == pq2);
    EXPECT_FALSE(pq1 == pq2);
}

TEST(Operator, EqualEqualLarge) {
    prqueue<int> pq;
    prqueue<int> pq1;

    for (int i = 0; i < 1000; i++) {
        pq.enqueue(i, i+5);
        pq1.enqueue(i, i+5);
    }

    prqueue<int> pq2;
    pq2.enqueue(5,5);

    EXPECT_TRUE(pq == pq1);
    pq1.dequeue();
    EXPECT_FALSE(pq == pq1);
    pq1.enqueue(0, 5);
    EXPECT_FALSE(pq == pq1);
    EXPECT_FALSE(pq == pq2);
    EXPECT_FALSE(pq1 == pq2);
}