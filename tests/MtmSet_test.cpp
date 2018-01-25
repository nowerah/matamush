#include "../testMacros.h"
#include "../MtmSet.h"
#include "../exceptions.h"

using namespace mtm;

bool isEven(int n){
    return !(n%2);
}

bool testSet(){
    MtmSet<int> set;
    ASSERT_NO_EXCEPTION(set.insert(1));
    ASSERT_NO_EXCEPTION(set.insert(2));
    MtmSet<int> set2(set);
    ASSERT_TRUE(set.contains(1));
    ASSERT_NO_EXCEPTION(set.erase(1));
    ASSERT_FALSE(set.contains(1));
    ASSERT_TRUE(set.size() == 1);
    ASSERT_FALSE(set.empty());
    MtmSet<int>::iterator it = set.begin();
    ASSERT_TRUE(*(it++) == 2);
    ASSERT_TRUE(it == set.end());
    ASSERT_TRUE(set.begin() == set.find(2));
    ASSERT_TRUE(set2.isSuperSetOf(set));
    ASSERT_TRUE(set2 != set);
    ASSERT_NO_EXCEPTION(set.unite(set2));
    ASSERT_TRUE(set == set2);
    MtmSet<int> empty_set;
    ASSERT_NO_EXCEPTION(set.intersect(empty_set));
    ASSERT_TRUE(set.empty());
    ASSERT_NO_EXCEPTION(set2.clear());
    ASSERT_TRUE(set2.empty());
    ASSERT_NO_EXCEPTION(set.insert(17));
    ASSERT_NO_EXCEPTION(set.insert(32));
    ASSERT_NO_EXCEPTION(set.insert(1337));
    ASSERT_TRUE(set.getSubSet(isEven).size() == 1);

    ASSERT_TRUE(set.size() == 1);
    ASSERT_NO_EXCEPTION(set.insert(1));
    ASSERT_NO_EXCEPTION(set.insert(2));
    const MtmSet<int> const_set(set);
    ASSERT_TRUE(const_set.size() == 3);
    MtmSet<int> set3 = const_set.getSubSet(isEven);
    ASSERT_TRUE(*(set3.begin()) == 32);
    ASSERT_TRUE(set3.size() == 2);
    ASSERT_TRUE(const_set.size() == 3);
    ASSERT_TRUE(set.size() == 3);
    ASSERT_FALSE(set3.isSuperSetOf(set));
    ASSERT_TRUE(set.isSuperSetOf(set3));
    ASSERT_TRUE(set3.intersect(set).contains(32));
    ASSERT_TRUE(set.size() == 3);
    ASSERT_TRUE(set3.intersect(const_set).contains(32));
    ASSERT_TRUE(set.intersect(set3).contains(32));
    ASSERT_TRUE(set == set3);
    MtmSet<int>::iterator it_find = set.find(7);
    ASSERT_TRUE(it_find == set.end());
    it_find = set.find(32);
    ++it_find;
    ASSERT_TRUE(*it_find == 2);
    MtmSet<int>::const_iterator const_it = const_set.begin();
    ASSERT_TRUE(*(++const_it) == 1);
    ASSERT_TRUE(*((++const_it)++) == 2);
    ASSERT_TRUE(const_it == const_set.end());
    return true;
}

int main(){
    RUN_TEST(testSet);
    return 0;
}
