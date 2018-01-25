// ConsoleApplication3.cpp : Defines the entry point for the console application.
//
#include "../testMacros.h"
#include "../Group.h"
#include "../Clan.h"
#include "../Area.h"
#include "../Plain.h"
#include "../Mountain.h"
#include "../River.h"
#include "../exceptions.h"
#include "../World.h"

using namespace mtm;


bool testWorld() {
	World w;

	// add clans
	ASSERT_EXCEPTION(w.addClan(""), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(w.addClan("clan1"));

	ASSERT_EXCEPTION(w.addClan("clan1"), WorldClanNameIsTaken);
	ASSERT_NO_EXCEPTION(w.addClan("clan2"));

	ASSERT_NO_EXCEPTION(w.addClan("clan10"));
	ASSERT_NO_EXCEPTION(w.addClan("clan11"));

	// add areas

	ASSERT_EXCEPTION(w.addArea("", PLAIN), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(w.addArea("plain", PLAIN));

	ASSERT_NO_EXCEPTION(w.addArea("mountain", MOUNTAIN));
	ASSERT_NO_EXCEPTION(w.addArea("river", RIVER));

	ASSERT_EXCEPTION(w.addArea("plain", PLAIN), WorldAreaNameIsTaken);

	// add some object

	ASSERT_NO_EXCEPTION(w.addGroup("group1", "clan1", 5, 5, "plain"));
	ASSERT_EXCEPTION(w.addGroup("", "clan1", 5, 5, "plain"), WorldInvalidArgument);
	ASSERT_EXCEPTION(w.addGroup("group1", "clan1", 5, 5, "plain"), WorldGroupNameIsTaken);
	ASSERT_EXCEPTION(w.addGroup("group3", "clan100", 5, 5, "plain"), WorldClanNotFound);

	ASSERT_EXCEPTION(w.addGroup("group2", "clan1", 5, 5, "rrrr"), WorldAreaNotFound);
	ASSERT_NO_EXCEPTION(w.addGroup("group2", "clan1", 15, 22, "plain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group3", "clan1", 14, 35, "plain"));

	ASSERT_NO_EXCEPTION(w.addGroup("group4", "clan2", 4, 4, "plain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group5", "clan2", 134, 35, "plain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group6", "clan1", 4, 4, "river"));


	ASSERT_NO_EXCEPTION(w.addGroup("group7", "clan1", 12, 344, "river"));
	ASSERT_NO_EXCEPTION(w.addGroup("group8", "clan2", 1222, 35, "river"));
	ASSERT_NO_EXCEPTION(w.addGroup("group9", "clan1", 4, 4, "mountain"));

	ASSERT_NO_EXCEPTION(w.addGroup("group10", "clan2", 155, 35, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group11", "clan2", 12, 35, "mountain"));


	// add some more objects

	ASSERT_NO_EXCEPTION(w.addGroup("group12", "clan11", 1222, 35, "river"));
	ASSERT_NO_EXCEPTION(w.addGroup("group13", "clan11", 4, 4, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group14", "clan11", 155, 35, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group15", "clan11", 12, 35, "mountain"));


	ASSERT_NO_EXCEPTION(w.addGroup("group16", "clan11", 1222, 35, "river"));
	ASSERT_NO_EXCEPTION(w.addGroup("group17", "clan11", 4, 4, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group18", "clan11", 155, 35, "mountain"));

	ASSERT_NO_EXCEPTION(w.addGroup("group19", "clan11", 12, 35, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group20", "clan11", 1222, 35, "river"));
	ASSERT_NO_EXCEPTION(w.addGroup("group21", "clan11", 4, 4, "mountain"));


	ASSERT_NO_EXCEPTION(w.addGroup("group22", "clan11", 155, 35, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group23", "clan11", 12, 35, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group24", "clan11", 1222, 35, "river"));

	ASSERT_NO_EXCEPTION(w.addGroup("group25", "clan11", 4, 4, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group26", "clan11", 155, 35, "mountain"));
	ASSERT_NO_EXCEPTION(w.addGroup("group27", "clan11", 12, 35, "mountain"));

	// make reachable areas
	ASSERT_NO_EXCEPTION(w.makeReachable("plain", "mountain"));
	ASSERT_NO_EXCEPTION(w.makeReachable("mountain", "river"));
	ASSERT_EXCEPTION(w.makeReachable("plain", "mrrrr"), WorldAreaNotFound);

	// move groups
	ASSERT_EXCEPTION(w.moveGroup("group43241", "eea"), WorldGroupNotFound);
	ASSERT_EXCEPTION(w.moveGroup("group1", "eea"), WorldAreaNotFound);
	ASSERT_EXCEPTION(w.moveGroup("group1", "plain"), WorldGroupAlreadyInArea);


	ASSERT_EXCEPTION(w.moveGroup("group1", "river"), WorldAreaNotReachable);
	ASSERT_NO_EXCEPTION(w.moveGroup("group1", "mountain"));
	ASSERT_NO_EXCEPTION(w.moveGroup("group2", "mountain"));
	ASSERT_NO_EXCEPTION(w.moveGroup("group9", "river"));

	// make clans friendships
	ASSERT_EXCEPTION(w.makeFriends("clan1", "clan3"), WorldClanNotFound);
	ASSERT_NO_EXCEPTION(w.makeFriends("clan1", "clan2"));

	// unite clans
	ASSERT_EXCEPTION(w.uniteClans("clan1", "clan2", "clan10"), WorldClanNameIsTaken);
	ASSERT_EXCEPTION(w.uniteClans("clan1", "clan2", "clan11"), WorldClanNameIsTaken);
	ASSERT_EXCEPTION(w.uniteClans("clan1", "clan2", ""), WorldInvalidArgument);
	ASSERT_EXCEPTION(w.uniteClans("clan1", "clan3432234", "united_clan"), WorldClanNotFound);
	ASSERT_NO_EXCEPTION(w.uniteClans("clan1", "clan2", "united_clan"));


	// print
	World w2;
	ASSERT_NO_EXCEPTION(w2.addClan("Beta"));
	ASSERT_NO_EXCEPTION(w2.addArea("Tel-Aviv", PLAIN));
	ASSERT_NO_EXCEPTION(w2.addArea("Carmel", MOUNTAIN));
	ASSERT_NO_EXCEPTION(w2.makeReachable("Carmel", "Tel-Aviv"));

	ASSERT_NO_EXCEPTION(w2.addGroup("Alpha", "Beta", 10, 10, "Carmel"));
	ASSERT_NO_EXCEPTION(w2.moveGroup("Alpha", "Tel-Aviv")); //divides
	ostringstream os;
	ASSERT_NO_EXCEPTION(w2.printGroup(os, "Alpha"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Beta\n"
		"Group's children: 5\n"
		"Group's adults: 5\n"
		"Group's tools: 20\n"
		"Group's food: 25\n"
		"Group's morale: 77\n"
		"Group's current area: Tel-Aviv\n"));
	return true;
}



int main()
{
	RUN_TEST(testWorld);

	return 0;
}

