#include "../exceptions.h"
#include "../testMacros.h"
#include "../Clan.h"
using namespace mtm;


bool testConstructorAndCopyConstructor() {
	Clan c1("first_clan");
	ASSERT_NO_EXCEPTION(Clan("ok"));
	ASSERT_EXCEPTION(Clan(""), ClanEmptyName);
	ASSERT_TRUE(c1.getSize() == 0);
	Clan c2(c1);
	ASSERT_TRUE(c2.getSize() == 0);
	return true;
}


bool testAddGroupGetGroupDoesContainGetSize() {
	Clan c1("first_clan");
	ASSERT_TRUE(c1.getSize() == 0);
	Group g1("group1", 3, 5);
	Group g2("group2", 1, 1);
	Group g3("group1", 2, 4);
	Group g4("group1", 1, 1);
	Group g5("group2", 10, 10);
	Group g6("group3", 1, 1);
	g6.fight(g1);
	ASSERT_NO_EXCEPTION(c1.addGroup(g1));
	ASSERT_EXCEPTION(c1.addGroup(g6), ClanGroupIsEmpty);
	ASSERT_EXCEPTION(c1.addGroup(g3), ClanGroupNameAlreadyTaken);
	ASSERT_EXCEPTION(c1.addGroup(g6), ClanGroupIsEmpty);
	ASSERT_TRUE(c1.getSize() == 7); //7 after fight
	ASSERT_NO_EXCEPTION(c1.addGroup(g5));
	ASSERT_TRUE(c1.getSize() == 27);
	ASSERT_EXCEPTION(c1.getGroup("groupnotexisted"), ClanGroupNotFound);
	ASSERT_EXCEPTION(c1.getGroup(""), ClanGroupNotFound);
	ASSERT_NO_EXCEPTION(c1.getGroup("group1"));
	GroupPointer group_in_clan = c1.getGroup("group1");
	ASSERT_TRUE(group_in_clan->getClan() == "first_clan");
	ASSERT_TRUE(group_in_clan->getName() == "group1");
	ASSERT_TRUE(group_in_clan->getSize() == 7);
	ASSERT_TRUE(c1.doesContain("group1"));
	ASSERT_TRUE(c1.doesContain("group2"));
	ASSERT_FALSE(c1.doesContain(""));
	ASSERT_FALSE(c1.doesContain("group3"));
	return true;
}


bool testUnitePart1() {

	// define clans&groups
	Clan c1("first_clan");
	Clan c2("second_clan");
	Clan c3("third_clan");
	Group g1("group1", 3, 5);
	Group g2("group2", 10, 10);
	Group g3("group3", 7, 1);
	Group g4("group4", 100, 100);
	Group g5("group1", 23, 43);

	// prepare clans
	ASSERT_NO_EXCEPTION(c1.addGroup(g1));
	ASSERT_NO_EXCEPTION(c1.addGroup(g2));
	ASSERT_NO_EXCEPTION(c2.addGroup(g3));
	ASSERT_NO_EXCEPTION(c2.addGroup(g4));
	ASSERT_NO_EXCEPTION(c3.addGroup(g5));

	// add clans friendships
	Clan c4("friend1_clan");
	Clan c5("friend2_clan");
	ASSERT_NO_EXCEPTION(c1.makeFriend(c4));
	ASSERT_NO_EXCEPTION(c2.makeFriend(c5));

	// try unsuccessufull unite tries
	ASSERT_EXCEPTION(c1.unite(c2, ""), ClanEmptyName);
	ASSERT_EXCEPTION(c1.unite(c1, ""), ClanEmptyName);
	ASSERT_EXCEPTION(c1.unite(c1, "something"), ClanCantUnite);
	ASSERT_EXCEPTION(c1.unite(c3, ""), ClanEmptyName);
	ASSERT_EXCEPTION(c1.unite(c3, "somename"), ClanCantUnite);

	// make sure the clans didnt change
	ASSERT_TRUE(c1.getSize() == 28);
	ASSERT_TRUE(c2.getSize() == 208);
	ASSERT_TRUE(c3.getSize() == 66);
	ASSERT_EXCEPTION(c1.getGroup("group3"), ClanGroupNotFound);
	ASSERT_EXCEPTION(c2.getGroup("group1"), ClanGroupNotFound);

	// try successfull unite
	ASSERT_NO_EXCEPTION(c1.unite(c2, "united_clan"));

	// check if unite is correct
	ASSERT_TRUE(c1.doesContain("group1"));
	ASSERT_TRUE(c1.doesContain("group2"));
	ASSERT_TRUE(c1.doesContain("group3"));
	ASSERT_TRUE(c1.doesContain("group4"));
	ASSERT_FALSE(c2.doesContain("group1"));
	ASSERT_FALSE(c2.doesContain("group2"));
	ASSERT_FALSE(c2.doesContain("group3"));
	ASSERT_FALSE(c2.doesContain("group4"));
	ASSERT_TRUE(c1.isFriend(c4));
	ASSERT_TRUE(c1.isFriend(c5));
	ASSERT_TRUE(c1.doesContain("group2"));
	ASSERT_TRUE(c1.getGroup("group1")->getClan() == "united_clan");
	ASSERT_TRUE(c1.getGroup("group2")->getClan() == "united_clan");
	ASSERT_TRUE(c1.getGroup("group3")->getClan() == "united_clan");
	ASSERT_TRUE(c1.getGroup("group4")->getClan() == "united_clan");
	ASSERT_TRUE(c1.getGroup("group1")->getName() == "group1");
	ASSERT_TRUE(c1.getGroup("group2")->getName() == "group2");
	ASSERT_TRUE(c1.getGroup("group3")->getName() == "group3");
	ASSERT_TRUE(c1.getGroup("group4")->getName() == "group4");
	ASSERT_TRUE(c1.getSize() == 236);
	return true;
}


bool testUnitePart2() {

	// now try another successfull unite
	// -a diffirent unite than part1-
	/// this unite will unite 'this' clan to the given clan in parameter
	// after that, it unite the united clan we got, to an another third clan
	// where the third clan will be the one calling for the unite operation
	// and the unite will be into the third clan
	// for example: if c1,c2,c3 are clans -- first we will do c1.unite(c2,'c2_name')
	// then do c3.unite(c1,'c3_name')

	// define clans&groups
	Clan c1("first_clan");
	Clan c2("second_clan");
	Group g1("group1", 3, 5);
	Group g2("group2", 10, 10);
	Group g3("group3", 7, 1);
	Group g4("group4", 100, 100);

	// prepare clans
	ASSERT_NO_EXCEPTION(c1.addGroup(g1));
	ASSERT_NO_EXCEPTION(c1.addGroup(g2));
	ASSERT_NO_EXCEPTION(c2.addGroup(g3));
	ASSERT_NO_EXCEPTION(c2.addGroup(g4));

	// add clans friendships
	Clan c4("friend1_clan");
	Clan c5("friend2_clan");
	Clan c6("friend3_clan");
	ASSERT_NO_EXCEPTION(c1.makeFriend(c4));
	ASSERT_NO_EXCEPTION(c2.makeFriend(c5));
	ASSERT_NO_EXCEPTION(c1.makeFriend(c6));

	// try successfull unite
	ASSERT_NO_EXCEPTION(c1.unite(c2, "second_clan"));

	// check if unite is correct
	ASSERT_TRUE(c1.doesContain("group1"));
	ASSERT_TRUE(c1.doesContain("group2"));
	ASSERT_TRUE(c1.doesContain("group3"));
	ASSERT_TRUE(c1.doesContain("group4"));
	ASSERT_FALSE(c2.doesContain("group1"));
	ASSERT_FALSE(c2.doesContain("group2"));
	ASSERT_FALSE(c2.doesContain("group3"));
	ASSERT_FALSE(c2.doesContain("group4"));
	ASSERT_TRUE(c1.isFriend(c4));
	ASSERT_TRUE(c1.isFriend(c5));
	ASSERT_TRUE(c1.isFriend(c6));
	ASSERT_TRUE(c1.getGroup("group1")->getClan() == "second_clan");
	ASSERT_TRUE(c1.getGroup("group2")->getClan() == "second_clan");
	ASSERT_TRUE(c1.getGroup("group3")->getClan() == "second_clan");
	ASSERT_TRUE(c1.getGroup("group4")->getClan() == "second_clan");
	ASSERT_TRUE(c1.getGroup("group1")->getName() == "group1");
	ASSERT_TRUE(c1.getGroup("group2")->getName() == "group2");
	ASSERT_TRUE(c1.getGroup("group3")->getName() == "group3");
	ASSERT_TRUE(c1.getGroup("group4")->getName() == "group4");
	ASSERT_TRUE(c1.getSize() == 236);


	Clan c3("third_clan");

	// try successfull unite
	ASSERT_NO_EXCEPTION(c3.unite(c1, "third_clan"));

	// check if unite is correct
	ASSERT_TRUE(c3.doesContain("group1"));
	ASSERT_TRUE(c3.doesContain("group2"));
	ASSERT_TRUE(c3.doesContain("group3"));
	ASSERT_TRUE(c3.doesContain("group4"));
	ASSERT_FALSE(c1.doesContain("group1"));
	ASSERT_FALSE(c1.doesContain("group2"));
	ASSERT_FALSE(c1.doesContain("group3"));
	ASSERT_FALSE(c1.doesContain("group4"));
	ASSERT_TRUE(c3.isFriend(c4));
	ASSERT_TRUE(c3.isFriend(c5));
	ASSERT_TRUE(c3.isFriend(c6));
	ASSERT_TRUE(c3.getGroup("group1")->getClan() == "third_clan");
	ASSERT_TRUE(c3.getGroup("group2")->getClan() == "third_clan");
	ASSERT_TRUE(c3.getGroup("group3")->getClan() == "third_clan");
	ASSERT_TRUE(c3.getGroup("group4")->getClan() == "third_clan");
	ASSERT_TRUE(c3.getGroup("group1")->getName() == "group1");
	ASSERT_TRUE(c3.getGroup("group2")->getName() == "group2");
	ASSERT_TRUE(c3.getGroup("group3")->getName() == "group3");
	ASSERT_TRUE(c3.getGroup("group4")->getName() == "group4");
	ASSERT_TRUE(c3.getSize() == 236);
	return true;
}


bool testIsFriendMakeFriend() {
	Clan c1("first_clan");
	Clan c2("second_clan");
	Clan c3("third_clan");
	ASSERT_NO_EXCEPTION(c1.makeFriend(c2));
	ASSERT_NO_EXCEPTION(c1.makeFriend(c2));
	ASSERT_NO_EXCEPTION(c1.makeFriend(c3));
	ASSERT_TRUE(c1.isFriend(c2));
	ASSERT_TRUE(c2.isFriend(c1));
	ASSERT_TRUE(c1.isFriend(c3));
	ASSERT_TRUE(c3.isFriend(c1));
	ASSERT_FALSE(c2.isFriend(c3));
	ASSERT_FALSE(c3.isFriend(c2));
	return true;
}


bool testClanOutput() {
	Clan c1("first_clan");
	ASSERT_NO_EXCEPTION(c1.addGroup(Group("group1", 1, 1)));
	ASSERT_NO_EXCEPTION(c1.addGroup(Group("group2", 100, 100)));
	Clan c2("second_clan");
	ASSERT_NO_EXCEPTION(c2.addGroup(Group("group3", 100, 100)));
	ostringstream os;
	ASSERT_NO_EXCEPTION(os << c1);
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: first_clan\n"
		"Clan's groups:\n"
		"group2\n"
		"group1\n"));
	ASSERT_NO_EXCEPTION(c1.unite(c2, "first_clan"));
	ostringstream os1;
	ASSERT_NO_EXCEPTION(os1 << c1);
	ASSERT_TRUE(VerifyOutput(os1, "Clan's name: first_clan\n"
		"Clan's groups:\n"
		"group2\n"
		"group3\n"
		"group1\n"));
	return true;
}


bool exampleClan() {
	Clan beta("Beta");
	ASSERT_NO_EXCEPTION(beta.addGroup(Group("Alpha1", 10, 10)));
	ASSERT_NO_EXCEPTION(beta.addGroup(Group("Alpha2", 5, 5)));
	ASSERT_NO_EXCEPTION(beta.addGroup(Group("Alpha3", 15, 15)));
	ASSERT_NO_EXCEPTION(beta.getGroup("Alpha1")->getClan() == "Beta");
	ASSERT_TRUE(beta.doesContain("Alpha2"));
	ASSERT_TRUE(beta.getSize() == 60);
	Clan beta2("Beta2");
	ASSERT_NO_EXCEPTION(beta2.addGroup(Group("Alpha4", 20, 20)));
	ASSERT_NO_EXCEPTION(beta.unite(beta2, "Beta3"));
	ostringstream os;
	ASSERT_TRUE(beta2.getSize() == 0);
	ASSERT_NO_EXCEPTION(os << beta);
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: Beta3\n"
		"Clan's groups:\n"
		"Alpha4\n"
		"Alpha3\n"
		"Alpha1\n"
		"Alpha2\n"));
	Clan gamma1("Gamma 1");
	Clan gamma2("Gamma 2");
	ASSERT_NO_EXCEPTION(gamma1.makeFriend(gamma2));
	ASSERT_TRUE(gamma1.isFriend(gamma2));
	return true;
}


int main()
{
	RUN_TEST(testConstructorAndCopyConstructor);
	RUN_TEST(testAddGroupGetGroupDoesContainGetSize);
	RUN_TEST(testUnitePart1);
	RUN_TEST(testUnitePart2);
	RUN_TEST(testIsFriendMakeFriend);
	RUN_TEST(testClanOutput);
	RUN_TEST(exampleClan);
	return 0;
}

