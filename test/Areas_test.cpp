#include "testMacros.h"
#include "../Group.h"
#include "../Clan.h"
#include "../Area.h"
#include "../River.h"
#include "../Plain.h"
#include "../Mountain.h"
#include "../exceptions.h"

using namespace mtm;

typedef std::shared_ptr<Area> AreaPtr;

bool PrintlessVerifyOutput(ostringstream& output, const string& expected_output){
    if(expected_output != output.str()) {
        output.clear();
        output.str("");
        return false;
    }
    output.clear();
    output.str("");
    return true;
}

std::map<std::string, Clan> makeClanMap(){
    std::map<std::string, Clan> clan_map;
    clan_map.insert(std::pair<std::string, Clan>("Apache", Clan("Apache")));
    clan_map.insert(std::pair<std::string, Clan>("Coyote", Clan("Coyote")));
    clan_map.insert(std::pair<std::string, Clan>("Apache2", Clan("Apache2")));
    clan_map.at("Apache").addGroup(Group("Aragonian", 30, 30));
    clan_map.at("Apache").addGroup(Group("Estonian", "", 30, 23, 40, 0, 75));
    clan_map.at("Apache").addGroup(Group("Bavarian", "Oldie", 2, 3, 13, 37, 100));
    clan_map.at("Coyote").addGroup(Group("Gnomes", 2, 2));
    clan_map.at("Coyote").addGroup(Group("Gnomes2", 2, 2));
    clan_map.at("Coyote").addGroup(Group("Whites", "Oldie", 10, 10, 40, 30, 65));
    clan_map.at("Coyote").addGroup(Group("Wolves", "", 50, 10, 17, 21, 95));
    clan_map.at("Coyote").addGroup(Group("Werewolves", "", 100, 100, 400, 500, 80));
    clan_map.at("Apache2").addGroup(Group("Aragonian2", "Apache2", 10, 10, 1, 2, 70));
    clan_map.at("Apache2").addGroup(Group("Estonian2", "Apache2", 10, 10, 1, 2, 70));
    clan_map.at("Apache2").makeFriend(clan_map.at("Apache"));
    return clan_map;
}

bool testPlain(){
    AreaPtr tel_aviv(new Plain("Tel-Aviv"));

    /* addReachableArea, isReachable */
    AreaPtr nazareth(new Plain("Nazareth"));
    AreaPtr rome(new Plain("Rome"));
    tel_aviv->addReachableArea("Nazareth");
    rome->addReachableArea("Tel-Aviv");
    ASSERT_TRUE(tel_aviv->isReachable("Nazareth"));
    ASSERT_FALSE(tel_aviv->isReachable("Rome"));

    std::map<std::string, Clan> clan_map = makeClanMap();

    /* groupArrive with more than 1/3 of clan, thus is divided */
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Aragonian", "Apache", clan_map));
    ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Aragonian_2"));

    /* groupArrive with more than 1/3 of clan, thus is divided */
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Werewolves", "Coyote", clan_map));
    ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Werewolves_2"));

    /* AreaGroupAlreadyIn exception */
    ASSERT_EXCEPTION(tel_aviv->groupArrive("Werewolves", "Coyote", clan_map),
                     AreaGroupAlreadyIn);

    /* AreaGroupNotInClan exception */
    ASSERT_EXCEPTION(tel_aviv->groupArrive("Firebats", "Coyote", clan_map),
                     AreaGroupNotInClan);

    /* AreaClanNotFoundInMap exception */
    ASSERT_EXCEPTION(tel_aviv->groupArrive("Firebats", "Nomads", clan_map),
                     AreaClanNotFoundInMap);

    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Whites", "Coyote", clan_map));
    /* Whites don't have enough morale to unite */
    ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Whites"));

    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Wolves", "Coyote", clan_map));
    /* They don't unite: if Wolves unites with anyone, they will have more than 1/3 clan */
    ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Wolves"));

    /* groupLeave */
    ASSERT_NO_EXCEPTION(tel_aviv->groupLeave("Wolves"));
    ASSERT_FALSE(tel_aviv->getGroupsNames().contains("Wolves"));
    ASSERT_NO_EXCEPTION(tel_aviv->groupLeave("Werewolves"));
    ASSERT_NO_EXCEPTION(tel_aviv->groupLeave("Whites"));

    /* will be added: only one from this clan */
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Wolves", "Coyote", clan_map));
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Gnomes", "Coyote", clan_map));
    /* They unite: together they do not reach 1/3 clan */

    /* AreaGroupNotFound exception */
    ASSERT_EXCEPTION(tel_aviv->groupLeave("Gnomes"), AreaGroupNotFound);
    ASSERT_EXCEPTION(tel_aviv->groupLeave("Firebats"), AreaGroupNotFound);

    /* groupArrive with division */
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Estonian", "Apache", clan_map));
    ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Estonian_2"));

    const GroupPointer& estonian(clan_map.at("Apache").getGroup("Estonian"));
    const GroupPointer& aragonian(clan_map.at("Apache").getGroup("Aragonian"));
    const GroupPointer& aragonian_2(clan_map.at("Apache").getGroup("Aragonian_2"));

    /* Ensure divisions */
    ostringstream os;
    ASSERT_NO_EXCEPTION(os << *estonian);
    ASSERT_TRUE(VerifyOutput(os, ""
            "Group's name: Estonian\n"
            "Group's clan: Apache\n"
            "Group's children: 15\n"
            "Group's adults: 12\n"
            "Group's tools: 20\n"
            "Group's food: 0\n"
            "Group's morale: 82\n"));

    os.clear();
    ASSERT_NO_EXCEPTION(os << *aragonian);
    ASSERT_TRUE(VerifyOutput(os, ""
            "Group's name: Aragonian\n"
            "Group's clan: Apache\n"
            "Group's children: 15\n"
            "Group's adults: 15\n"
            "Group's tools: 60\n"
            "Group's food: 75\n"
            "Group's morale: 77\n"));

    os.clear();
    MtmSet<string> groups = tel_aviv->getGroupsNames();
    for (const string& group : groups) {
        os << group << endl;
    }
    ASSERT_TRUE(VerifyOutput(os, ""
            "Werewolves_2\n"
            "Aragonian_2\n"
            "Aragonian\n"
            "Wolves\n"
            "Estonian_2\n"
            "Estonian\n"));

    /* groupArrive and unites */
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Bavarian", "Apache", clan_map));
    ASSERT_FALSE(tel_aviv->getGroupsNames().contains("Bavarian"));

    /* Ensure that Bavarian united with Aragonian_2 (the strongest) */
    ASSERT_TRUE(estonian->getSize() == 27); // recall: Estonian divided from 53.

    estonian->changeClan("Coyote");

    ASSERT_NO_EXCEPTION(tel_aviv->groupLeave("Wolves"));
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Gnomes2", "Coyote", clan_map));
    /* Gnomes and Estonian unite, so the group Gnomes2 is not added */
    ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Estonian"));
    ASSERT_FALSE(tel_aviv->getGroupsNames().contains("Gnomes2"));
    ASSERT_TRUE(estonian->getSize() == 31);

    /* Ensure new divided group's name is Werewolves_3 */
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Werewolves", "Coyote", clan_map));
    ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Werewolves_3"));

    return true;
}

bool testMountain(){
    AreaPtr carmel(new Mountain("Carmel"));
    std::map<std::string, Clan> clan_map = makeClanMap();
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Estonian", "Apache", clan_map));
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Aragonian", "Apache", clan_map));
    // they don't fight, and the second becomes the dominant group

    ostringstream os;
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Apache").getGroup("Aragonian"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Aragonian\n"
            "Group's clan: Apache\n"
            "Group's children: 30\n"
            "Group's adults: 30\n"
            "Group's tools: 120\n"
            "Group's food: 150\n"
            "Group's morale: 77\n"));

    ASSERT_NO_EXCEPTION(carmel->groupArrive("Aragonian2", "Apache2", clan_map));
    // the two Aragonian fight

    ASSERT_NO_EXCEPTION(os << *clan_map.at("Apache2").getGroup("Aragonian2"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Aragonian2\n"
            "Group's clan: Apache2\n"
            "Group's children: 6\n"
            "Group's adults: 6\n"
            "Group's tools: 0\n"
            "Group's food: 1\n"
            "Group's morale: 56\n"));

    ASSERT_NO_EXCEPTION(carmel->groupLeave("Aragonian"));
    // Estonian becomes dominant

    ASSERT_NO_EXCEPTION(carmel->groupArrive("Werewolves", "Coyote", clan_map));
    // fights against and wins the Estonian, becoming dominant

    ASSERT_NO_EXCEPTION(os << *clan_map.at("Apache").getGroup("Estonian"));
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Coyote").getGroup("Werewolves"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Estonian\n"
            "Group's clan: Apache\n"
            "Group's children: 20\n"
            "Group's adults: 15\n"
            "Group's tools: 20\n"
            "Group's food: 0\n"
            "Group's morale: 65\n"
            "Group's name: Werewolves\n"
            "Group's clan: Coyote\n"
            "Group's children: 100\n"
            "Group's adults: 75\n"
            "Group's tools: 300\n"
            "Group's food: 500\n"
            "Group's morale: 100\n"));

    ASSERT_NO_EXCEPTION(carmel->groupArrive("Estonian2", "Apache2", clan_map));
    // fights Werewolves and loses

    // make sure werewolves were in a battle
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Coyote").getGroup("Werewolves"));
    ASSERT_FALSE(PrintlessVerifyOutput(os, "Group's name: Werewolves\n"
            "Group's clan: Coyote\n"
            "Group's children: 100\n"
            "Group's adults: 75\n"
            "Group's tools: 300\n"
            "Group's food: 500\n"
            "Group's morale: 100\n"));

    ASSERT_NO_EXCEPTION(carmel->groupLeave("Werewolves"));
    // no more groups from Coyote, so the strongest group becomes dominant
    // therefore, Estonian is dominant

    ASSERT_NO_EXCEPTION(os << *clan_map.at("Apache").getGroup("Estonian"));
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Werewolves", "Coyote", clan_map));
    // Werewolves come again, fight estonian

    std::ostringstream os2;
    ASSERT_NO_EXCEPTION(os2 << *clan_map.at("Apache").getGroup("Estonian"));
    ASSERT_FALSE(PrintlessVerifyOutput(os, os2.str()));

    ASSERT_NO_EXCEPTION(carmel->groupArrive("Gnomes", "Coyote", clan_map));
    // they don't fight, no need to check

    ASSERT_NO_EXCEPTION(carmel->groupLeave("Werewolves"));
    // now Gnomes should become dominant

    os2.clear();
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Coyote").getGroup("Gnomes"));
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Aragonian", "Apache", clan_map));
    // Aragonians come again, fight Gnomes
    ASSERT_NO_EXCEPTION(os2 << *clan_map.at("Coyote").getGroup("Gnomes"));
    ASSERT_FALSE(PrintlessVerifyOutput(os, os2.str()));

    return true;
}

bool testRiver(){
    AreaPtr jordan(new River("Jordan"));
    std::map<std::string, Clan> clan_map = makeClanMap();
    ASSERT_TRUE(clan_map.at("Apache").isFriend(clan_map.at("Apache2")));

    ASSERT_NO_EXCEPTION(jordan->groupArrive("Aragonian", "Apache", clan_map));
    ASSERT_NO_EXCEPTION(jordan->groupArrive("Estonian", "Apache", clan_map));
    // they trade

    ostringstream os;
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Apache").getGroup("Aragonian"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Aragonian\n"
            "Group's clan: Apache\n"
            "Group's children: 30\n"
            "Group's adults: 30\n"
            "Group's tools: 138\n" // initial: 120
            "Group's food: 132\n" // initial: 150
            "Group's morale: 77\n"));

    ASSERT_NO_EXCEPTION(jordan->groupArrive("Wolves", "Coyote", clan_map));
    // no one trades

    os.clear();
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Coyote").getGroup("Wolves"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Wolves\n"
            "Group's clan: Coyote\n"
            "Group's children: 50\n"
            "Group's adults: 10\n"
            "Group's tools: 17\n"
            "Group's food: 21\n"
            "Group's morale: 100\n")); // morale raised from 95 to 100 after clan change

    ASSERT_EXCEPTION(jordan->groupArrive("Wolves", "Coyote", clan_map),
                     AreaGroupAlreadyIn);

    ASSERT_EXCEPTION(jordan->groupLeave("Aragonian2"), AreaGroupNotFound);
    ASSERT_NO_EXCEPTION(jordan->groupLeave("Aragonian"));

    ASSERT_NO_EXCEPTION(jordan->groupArrive("Aragonian2", "Apache2", clan_map));
    // trades with Aragonian

    // Estonian had 22 tools, 18 food
    // Aragonian2 had 1 tool, 2 food
    // ceil((22-18)/2) = 2, ceil((2-1)/2) = 1, ceil(2+1)/2 = 2, min(2,2,22) = 2 --> They trade 2.

    os.clear();
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Apache2").getGroup("Aragonian2"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Aragonian2\n"
            "Group's clan: Apache2\n"
            "Group's children: 10\n"
            "Group's adults: 10\n"
            "Group's tools: 3\n"
            "Group's food: 0\n"
            "Group's morale: 70\n"));

    return true;
}

int main(){
    /* All exceptions are tested in testPlain, so the other two test don't test them. */
    RUN_TEST(testPlain);
    RUN_TEST(testMountain);
    RUN_TEST(testRiver);
    return 0;
}