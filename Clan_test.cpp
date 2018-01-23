#include "Clan.h"
#include "exceptions.h"
#include "testMacros.h"
#include "Group.h"

using namespace mtm;

bool testClan(){

    ASSERT_EXCEPTION(Clan tree(""), ClanEmptyName);

    Clan north("The North");
    Clan vale("The Vale");

    ASSERT_NO_EXCEPTION(north.addGroup(Group("Stark", "", 200, 153, 326, 475, 85)));
    ASSERT_NO_EXCEPTION(north.addGroup(Group("Bolton","", 40, 63, 200, 350, 70)));
    ASSERT_NO_EXCEPTION(north.addGroup(Group("Manderly","The North", 50, 77, 258, 258, 65)));
    ASSERT_NO_EXCEPTION(north.addGroup(Group("Mormont",40, 60)));
    ASSERT_EXCEPTION(north.addGroup(Group("Stark", 25, 78)), ClanGroupNameAlreadyTaken);

    Group hanna1("Hanna" , "Hanna", 1, 1, 5, 7 ,70);
    Group hanna2("Hanna" , "Hanna", 1, 1, 5, 7 ,80);
    ASSERT_TRUE(hanna1.unite(hanna2, 250));
    ASSERT_EXCEPTION(north.addGroup(hanna2), ClanGroupIsEmpty);

    ASSERT_EXCEPTION(north.getGroup("Doesn't exist"), ClanGroupNotFound);
    ASSERT_TRUE(north.getGroup("Stark")->getSize() == 353);
    ASSERT_TRUE(north.doesContain("Bolton"));
    ASSERT_FALSE(north.doesContain("Noor"));
    ostringstream os;
    ASSERT_NO_EXCEPTION(os << north);
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: The North\n"
            "Clan's groups:\n"
            "Stark\n"
            "Manderly\n"
            "Mormont\n"
            "Bolton\n"));

    ASSERT_TRUE(north.getSize() == 683);
    ASSERT_TRUE(vale.getSize() == 0);

    ASSERT_NO_EXCEPTION(vale.addGroup(Group("Arryn", "" ,90, 72, 230, 250, 82)));
    ASSERT_NO_EXCEPTION(vale.addGroup(Group("Baelish", 60, 43)));
    ASSERT_NO_EXCEPTION(vale.addGroup(Group("Waynwood", 54, 71)));
    ASSERT_TRUE(vale.getSize() == 390);

    ASSERT_NO_EXCEPTION(os << vale);
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: The Vale\n"
            "Clan's groups:\n"
            "Arryn\n"
            "Waynwood\n"
            "Baelish\n"));


    //Testing things related to friendships
    ASSERT_TRUE(north.isFriend(north));
    ASSERT_TRUE(vale.isFriend(vale));
    ASSERT_FALSE(north.isFriend(vale));
    ASSERT_FALSE(vale.isFriend(north));
    north.makeFriend(vale);
    ASSERT_TRUE(vale.isFriend(north));
    ASSERT_TRUE(north.isFriend(vale));


    //Testing united stuff

    ASSERT_EXCEPTION(north.unite(vale,""), ClanEmptyName);
    ASSERT_EXCEPTION(north.unite(north,"Dessert"), ClanCantUnite);
    ASSERT_NO_EXCEPTION(os << north);
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: The North\n"
            "Clan's groups:\n"
            "Stark\n"
            "Manderly\n"
            "Mormont\n"
            "Bolton\n"));

    ASSERT_NO_EXCEPTION(os << vale);
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: The Vale\n"
            "Clan's groups:\n"
            "Arryn\n"
            "Waynwood\n"
            "Baelish\n"));


    Clan river("The Riverlands");
    Clan wester("The Westerlands");
    Clan storm("The Stormlands");

    vale.makeFriend(wester);
    north.makeFriend(river);
    north.makeFriend(storm);

    ASSERT_FALSE(vale.isFriend(river));

    ASSERT_NO_EXCEPTION(vale.unite(north, "The Great North Vale"));

    ASSERT_TRUE(vale.isFriend(river));
    ASSERT_TRUE(vale.isFriend(storm));
    ASSERT_TRUE(vale.isFriend(wester));

    ASSERT_TRUE(vale.doesContain("Stark"));
    ASSERT_TRUE(vale.doesContain("Manderly"));
    ASSERT_FALSE(north.doesContain("Stark"));
    ASSERT_NO_EXCEPTION(os << vale);
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: The Great North Vale\n"
            "Clan's groups:\n"
            "Stark\n"
            "Arryn\n"
            "Waynwood\n"
            "Manderly\n"
            "Mormont\n"
            "Bolton\n"
            "Baelish\n"));


    ASSERT_NO_EXCEPTION(river.addGroup(Group("Waynwood", 40, 87)));

    ASSERT_EXCEPTION(river.unite(vale, "Beauty"), ClanCantUnite);

    ASSERT_NO_EXCEPTION(os << vale);
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: The Great North Vale\n"
            "Clan's groups:\n"
            "Stark\n"
            "Arryn\n"
            "Waynwood\n"
            "Manderly\n"
            "Mormont\n"
            "Bolton\n"
            "Baelish\n"));

    ASSERT_NO_EXCEPTION(os << river);
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: The Riverlands\n"
            "Clan's groups:\n"
            "Waynwood\n"));


      return true;
}

int main() {
    RUN_TEST(testClan);
    return 0;
}