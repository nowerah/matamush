
#include "../testMacros.h"
#include "../Group.h"
#include "../Clan.h"
#include "../Area.h"
#include "../Plain.h"
#include "../Mountain.h"
#include "../River.h"
#include "../exceptions.h"

using namespace mtm;


bool testPlainMountainRiverConstructor() {
	ASSERT_NO_EXCEPTION(mtm::Plain("plain"));
	ASSERT_NO_EXCEPTION(mtm::Mountain("mountain"));
	ASSERT_NO_EXCEPTION(mtm::River("river"));
	ASSERT_EXCEPTION(mtm::Plain(""), AreaInvalidArguments);
	ASSERT_EXCEPTION(mtm::Mountain(""), AreaInvalidArguments);
	ASSERT_EXCEPTION(mtm::River(""), AreaInvalidArguments);
	mtm::Plain p("plain");
	mtm::Mountain m("mountain");
	mtm::River r("river");
	ASSERT_TRUE(p.getGroupsNames().size() == 0);
	ASSERT_TRUE(m.getGroupsNames().size() == 0);
	ASSERT_TRUE(r.getGroupsNames().size() == 0);
	return true;
}

std::map<std::string, Clan> makeClanMap() {
	std::map<std::string, Clan> clan_map;
	clan_map.insert(std::pair<std::string, Clan>("clan1", Clan("clan1")));
	clan_map.insert(std::pair<std::string, Clan>("clan2", Clan("clan2")));
	clan_map.at("clan1").addGroup(Group("group1_1", 10, 10));
	clan_map.at("clan1").addGroup(Group("group1_2", 5, 5));
	clan_map.at("clan1").addGroup(Group("group1_3", 100, 100));
	clan_map.at("clan2").addGroup(Group("group2_1", 5, 4));
	clan_map.at("clan2").addGroup(Group("group2_2", "", 1, 1, 1, 0, 70));
	return clan_map;
}
bool testPlain() {
	shared_ptr<Area> plain(new Plain("plain"));
	std::map<std::string, Clan> clan_map = makeClanMap();
	ASSERT_EXCEPTION(plain->groupArrive("group1_1", "clanno", clan_map), AreaClanNotFoundInMap);
	ASSERT_EXCEPTION(plain->groupArrive("group343", "clan1", clan_map), AreaGroupNotInClan);
	ASSERT_NO_EXCEPTION(plain->groupArrive("group1_1", "clan1", clan_map));
	ASSERT_EXCEPTION(plain->groupArrive("group1_1", "clan1", clan_map), AreaGroupAlreadyIn);
	ASSERT_TRUE(plain->getGroupsNames().contains("group1_1"));

	// unite arrival
	ASSERT_NO_EXCEPTION(plain->groupArrive("group1_2", "clan1", clan_map));
	ASSERT_TRUE(plain->getGroupsNames().contains("group1_1"));

	// divide
	ASSERT_NO_EXCEPTION(plain->groupArrive("group1_3", "clan1", clan_map));
	ASSERT_TRUE(plain->getGroupsNames().contains("group1_3"));
	ASSERT_TRUE(plain->getGroupsNames().contains("group1_3_2"));

	// regular arrival (no unite no divide)
	ASSERT_NO_EXCEPTION(plain->groupArrive("group2_1", "clan2", clan_map));
	ASSERT_TRUE(plain->getGroupsNames().contains("group2_1"));
	ASSERT_FALSE(plain->getGroupsNames().contains("group2_1_2"));
	return true;
}

bool testMountain() {
	shared_ptr<Area> mountain(new Mountain("mountain"));
	std::map<std::string, Clan> clan_map = makeClanMap();

	// arrival of groups
	ASSERT_NO_EXCEPTION(mountain->groupArrive("group1_2", "clan1", clan_map));
	ASSERT_NO_EXCEPTION(mountain->groupArrive("group1_1", "clan1", clan_map));
	ASSERT_NO_EXCEPTION(mountain->groupArrive("group2_1", "clan2", clan_map));
	ASSERT_NO_EXCEPTION(mountain->groupArrive("group1_3", "clan1", clan_map));
	ASSERT_EXCEPTION(mountain->groupArrive("group2_2", "clan2", clan_map), AreaGroupNotInClan); //group2_2 dies cause it fights with group1_1

																								// check if its ok
	ASSERT_TRUE(mountain->getGroupsNames().contains("group1_1"));
	ASSERT_TRUE(mountain->getGroupsNames().contains("group1_2"));
	ASSERT_TRUE(mountain->getGroupsNames().contains("group1_3"));
	ASSERT_TRUE(mountain->getGroupsNames().contains("group2_1"));
	ASSERT_FALSE(mountain->getGroupsNames().contains("group2_2"));

	// check group leaving operator
	ASSERT_NO_EXCEPTION(mountain->groupLeave("group1_1"));
	ASSERT_FALSE(mountain->getGroupsNames().contains("group1_1"));
	ASSERT_NO_EXCEPTION(mountain->groupLeave("group1_2"));
	ASSERT_FALSE(mountain->getGroupsNames().contains("group1_2"));
	ASSERT_NO_EXCEPTION(mountain->groupLeave("group1_3"));
	ASSERT_FALSE(mountain->getGroupsNames().contains("group1_3"));
	ASSERT_NO_EXCEPTION(mountain->groupLeave("group2_1"));
	ASSERT_FALSE(mountain->getGroupsNames().contains("group2_1"));
	ASSERT_FALSE(mountain->getGroupsNames().contains("group2_2"));

	return true;
}

bool testRiver() {
	shared_ptr<Area> river(new River("river"));
	std::map<std::string, Clan> clan_map = makeClanMap();

	// arrival of groups
	ASSERT_NO_EXCEPTION(river->groupArrive("group1_1", "clan1", clan_map));
	ASSERT_NO_EXCEPTION(river->groupArrive("group1_2", "clan1", clan_map));
	ASSERT_NO_EXCEPTION(river->groupArrive("group1_3", "clan1", clan_map));

	// arrival of groups from other clan
	ASSERT_NO_EXCEPTION(river->groupArrive("group2_1", "clan2", clan_map));
	ASSERT_NO_EXCEPTION(river->groupArrive("group2_2", "clan2", clan_map));

	// checking everthing is ok
	ASSERT_TRUE(river->getGroupsNames().contains("group1_1"));
	ASSERT_TRUE(river->getGroupsNames().contains("group1_2"));
	ASSERT_TRUE(river->getGroupsNames().contains("group1_3"));
	ASSERT_TRUE(river->getGroupsNames().contains("group2_1"));
	ASSERT_TRUE(river->getGroupsNames().contains("group2_2"));
	return true;
}


std::map<std::string, Clan> makeClanMap2() {
	std::map<std::string, Clan> clan_map;
	clan_map.insert(std::pair<std::string, Clan>("Beta", Clan("Beta")));
	clan_map.insert(std::pair<std::string, Clan>("Gamma", Clan("Gamma")));
	clan_map.at("Beta").addGroup(Group("Alpha1", 10, 10));
	clan_map.at("Beta").addGroup(Group("Alpha2", "", 10, 10, 40, 0, 70));
	clan_map.at("Gamma").addGroup(Group("Lambda1", 10, 10));
	clan_map.at("Gamma").addGroup(Group("Lambda2", 5, 5));
	clan_map.at("Gamma").addGroup(Group("Lambda3", 100, 100));
	return clan_map;
}

bool examplePlain() {
	shared_ptr<Area> tel_aviv(new Plain("Tel-Aviv"));
	std::map<std::string, Clan> clan_map = makeClanMap2();
	ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Alpha1", "Beta", clan_map));
	// divide
	ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Alpha1_2"));
	ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Lambda1", "Gamma", clan_map));
	ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Lambda2", "Gamma", clan_map));
	//unite with lambda1

	ASSERT_FALSE(tel_aviv->getGroupsNames().contains("Lambda2"));

	return true;
}

bool exampleMountain() {
	shared_ptr<Area> carmel(new Mountain("Carmel"));
	std::map<std::string, Clan> clan_map = makeClanMap2();
	ASSERT_NO_EXCEPTION(carmel->groupArrive("Alpha1", "Beta", clan_map));
	ASSERT_NO_EXCEPTION(carmel->groupArrive("Lambda2", "Gamma", clan_map));
	ostringstream os;
	ASSERT_NO_EXCEPTION(os << *clan_map.at("Gamma").getGroup("Lambda2"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Lambda2\n"
		"Group's clan: Gamma\n"
		"Group's children: 3\n"
		"Group's adults: 3\n"
		"Group's tools: 10\n"
		"Group's food: 12\n"
		"Group's morale: 61\n"));
	return true;
}

bool exampleRiver() {
	shared_ptr<Area> jordan(new River("Jordan"));
	std::map<std::string, Clan> clan_map = makeClanMap2();
	ASSERT_NO_EXCEPTION(jordan->groupArrive("Alpha1", "Beta", clan_map));
	ASSERT_NO_EXCEPTION(jordan->groupArrive("Alpha2", "Beta", clan_map));
	ostringstream os;
	ASSERT_NO_EXCEPTION(os << *clan_map.at("Beta").getGroup("Alpha1"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha1\n"
		"Group's clan: Beta\n"
		"Group's children: 10\n"
		"Group's adults: 10\n"
		"Group's tools: 53\n"
		"Group's food: 37\n"
		"Group's morale: 77\n"));
	return true;
}

int main()
{
	RUN_TEST(testPlainMountainRiverConstructor);
	RUN_TEST(testPlain);
	RUN_TEST(testMountain);
	RUN_TEST(testRiver);
	RUN_TEST(examplePlain);
	RUN_TEST(exampleMountain);
	RUN_TEST(exampleRiver);
	return 0;
}

