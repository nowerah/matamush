#include "testMacros.h"
#include "MtmSet.h"
#include "exceptions.h"

using namespace mtm;

bool isEven(int n) {
	return !(n % 2);
}

bool startsWithRandom(std::string string) {
	std::string element("random");
	return string.compare(0, element.length(), element) == 0;
}

//this is code duplication, however it's needed and it's only a test function.
bool startsWithElement(std::string string) {
	std::string element("element");
	return string.compare(0, element.length(), element) == 0;
}


bool setTestOverloaded() {
	MtmSet<std::string> set1;
	ASSERT_NO_EXCEPTION(set1.insert("element1"));
	ASSERT_NO_EXCEPTION(set1.insert("element2"));
	ASSERT_NO_EXCEPTION(set1.insert("element3"));
	MtmSet<std::string> set2;
	ASSERT_NO_EXCEPTION(set2.insert("random1"));
	ASSERT_NO_EXCEPTION(set2.insert("random2"));
	MtmSet<std::string>::iterator it1 = set1.begin();
	MtmSet<std::string>::iterator it2 = set2.begin();
	ASSERT_TRUE(startsWithElement(*it1));
	ASSERT_TRUE(startsWithRandom(*it2));
	ASSERT_TRUE(it1 != it2);
	it2 = it1;
	ASSERT_TRUE(startsWithElement(*it2));
	ASSERT_TRUE(it1 == it2);


	return true;
}
bool setTestBasicOperations() {
	MtmSet<std::string> set1;
	ASSERT_NO_EXCEPTION(set1.insert("element1"));
	MtmSet<std::string> set2;
	ASSERT_NO_EXCEPTION(set2.insert("element2"));
	MtmSet<std::string> set3;
	ASSERT_NO_EXCEPTION(set3.insert("element2"));
	set1.unite(set2).unite(set3);
	ASSERT_TRUE(set1.size() == 2);
	set1.clear();
	set2.clear();
	set3.clear();
	set1.unite(set2).unite(set3);
	ASSERT_TRUE(set1.size() == 0);
	ASSERT_TRUE(set2.size() == 0);
	ASSERT_TRUE(set3.size() == 0);
	ASSERT_NO_EXCEPTION(set1.insert("element1"));
	ASSERT_NO_EXCEPTION(set2.insert("element2"));
	set1.intersect(set2);
	ASSERT_TRUE(set1.empty());
	ASSERT_NO_EXCEPTION(set1.insert("element3"));
	ASSERT_NO_EXCEPTION(set1.insert("element4"));
	ASSERT_NO_EXCEPTION(set2.insert("element5"));
	ASSERT_NO_EXCEPTION(set2.insert("element6"));
	ASSERT_NO_EXCEPTION(set1.insert("element2"));
	set1.intersect(set2);
	ASSERT_TRUE(set2.size() == 3);
	set2.intersect(set2);
	ASSERT_TRUE(set2.size() == 3);
	ASSERT_NO_EXCEPTION(set2.insert("ele7"));
	ASSERT_NO_EXCEPTION(set2.insert("gt2d"));
	ASSERT_NO_EXCEPTION(set2.insert("asw"));
	ASSERT_TRUE(set2.size() == 6);
	ASSERT_TRUE(set2.getSubSet(startsWithElement).size() == 3);
	ASSERT_TRUE(set2.size() == 3);
	return true;
}

bool setTestInsertionDeletion() {
	MtmSet<std::string> set;
	ASSERT_NO_EXCEPTION(set.insert("element1"));
	ASSERT_NO_EXCEPTION(set.insert("element2"));
	ASSERT_NO_EXCEPTION(set.insert("element3"));
	ASSERT_NO_EXCEPTION(set.insert("element3"));
	ASSERT_TRUE(set.size() == 3);
	ASSERT_TRUE(set.contains("element1"));
	ASSERT_TRUE(set.contains("element2"));
	ASSERT_FALSE(set.empty());
	MtmSet<std::string>::iterator it = set.find("element1");
	ASSERT_TRUE(*it == "element1");
	set.erase(it);
	ASSERT_TRUE(set.size() == 2);
	set.clear();
	ASSERT_TRUE(set.empty());
	ASSERT_TRUE(set.size() == 0);
	set.insert("2");
	ASSERT_TRUE(set.size() == 1);
	ASSERT_TRUE(set.contains("2"));
	ASSERT_FALSE(set.contains("element1"));
	ASSERT_FALSE(set.empty());
	return true;
}

bool exampleSet() {
	MtmSet<int> set;
	ASSERT_NO_EXCEPTION(set.insert(1)); //1
	ASSERT_NO_EXCEPTION(set.insert(2)); //1,2
	MtmSet<int> set2(set);
	ASSERT_TRUE(set.contains(1)); //1
	ASSERT_NO_EXCEPTION(set.erase(1)); //2
	ASSERT_FALSE(set.contains(1));// 2
	ASSERT_TRUE(set.size() == 1); //2
	ASSERT_FALSE(set.empty()); //2
	MtmSet<int>::iterator it = set.begin(); //
	ASSERT_TRUE(*(it++) == 2); //3
	ASSERT_TRUE(it == set.end());
	ASSERT_TRUE(set.begin() == set.find(2));
	ASSERT_TRUE(set2.isSuperSetOf(set));
	ASSERT_TRUE(set2 != set);
	ASSERT_NO_EXCEPTION(set.unite(set2));
	ASSERT_TRUE(set == set2);
	MtmSet<int> set3;
	ASSERT_NO_EXCEPTION(set.intersect(set3));
	ASSERT_TRUE(set.empty());
	ASSERT_NO_EXCEPTION(set2.clear());
	ASSERT_TRUE(set2.empty());
	ASSERT_NO_EXCEPTION(set.insert(1));
	ASSERT_NO_EXCEPTION(set.insert(2));
	ASSERT_NO_EXCEPTION(set.insert(3));
	ASSERT_TRUE(set.getSubSet(isEven).size() == 1);
	return true;
}


int main() {
	RUN_TEST(exampleSet);
	RUN_TEST(setTestInsertionDeletion);
	RUN_TEST(setTestBasicOperations);
	RUN_TEST(setTestOverloaded);
	return 0;
}
