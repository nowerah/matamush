#include "testMacros.h"
#include "Group.h"
#include "Clan.h"
#include "Area.h"
#include "Plain.h"
#include "exceptions.h"

using namespace mtm;

typedef std::shared_ptr<Area> AreaPtr;

std::map<std::string, Clan> makeClanMap(){
    std::map<std::string, Clan> clan_map;
    clan_map.insert(std::pair<std::string, Clan>("Apache", Clan("Apache")));
    clan_map.insert(std::pair<std::string, Clan>("Coyote", Clan("Coyote")));
    clan_map.at("Apache").addGroup(Group("Aragonian", 30, 30));
    clan_map.at("Apache").addGroup(Group("Estonian", "", 30, 23, 40, 0, 75));
    clan_map.at("Apache").addGroup(Group("Bavarian", "Oldie", 2, 3, 13, 37, 100));
    clan_map.at("Coyote").addGroup(Group("Gnomes", 2, 2));
    clan_map.at("Coyote").addGroup(Group("Gnomes2", 2, 2));
    clan_map.at("Coyote").addGroup(Group("Whites", "Oldie", 10, 10, 40, 30, 65));
    clan_map.at("Coyote").addGroup(Group("Wolves","", 50, 10, 17, 21, 95));
    clan_map.at("Coyote").addGroup(Group("Werewolves", 100, 100));
    return clan_map;
}

bool examplePlain(){
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

int main(){
    RUN_TEST(examplePlain);
    return 0;
}
