//
// Created by Noor Athamnah on 1/17/2018.
//

#include "Group.h" /* includes string and ostream */
#include "exceptions.h"
#include "testMacros.h"

#define MAX_UNITE1 250
#define MAX_UNITE2 400

using namespace mtm;

bool testGroup() {
    ASSERT_EXCEPTION(Group("Dragon", "heart", 5, 7 , 122, 30, 120), GroupInvalidArgs);
    ASSERT_EXCEPTION(Group("Dragon", "heart", 0, 0 , 122, 30, 20), GroupInvalidArgs);
    ASSERT_EXCEPTION(Group("Dragon", "heart", 50, 17 , -5, 30, 78), GroupInvalidArgs);
    ASSERT_EXCEPTION(Group("", "heart", 50, 17 , 5, 30, 78), GroupInvalidArgs);
    ASSERT_NO_EXCEPTION(Group("Lovely","Dark", 15, 17, 24, 224, 100));
//    Group farah;

    Group hawari("Hawari", "Hawarah", 40, 70, 200, 350, 74);
    Group abunassar("Abu Nassar", "Nassar", 0, 25, 40, 30, 80);
    Group athamnah("Athamnah", 40, 60);
    Group farah(hawari);


    ASSERT_TRUE(farah.getName() == "Hawari");
    ASSERT_TRUE(abunassar.getSize() == 25);
    ASSERT_TRUE(athamnah.getSize() == 100);
    ASSERT_TRUE(athamnah.getClan().empty());
    ASSERT_TRUE(farah.getClan() == "Hawarah");
    ASSERT_TRUE(hawari > athamnah);
    ASSERT_FALSE(hawari <= athamnah);
    ASSERT_NO_EXCEPTION(athamnah.changeClan("Othman"));
    ASSERT_TRUE(athamnah.getClan() == "Othman");
    ASSERT_TRUE(hawari < athamnah);
    ASSERT_FALSE(hawari == athamnah);
    ASSERT_FALSE(hawari > athamnah);
    ASSERT_FALSE(hawari >= athamnah);
    ASSERT_TRUE(farah == hawari);

    ASSERT_FALSE(hawari.unite(hawari, MAX_UNITE1));
    ASSERT_FALSE(hawari.unite(athamnah, MAX_UNITE1));
    Group hanna("Hanna", "Jaafar", 53, 74, 200, 400, 80);
    Group bathish("Bathish", "Jaafar", 50, 86, 174, 263, 80);
    ASSERT_FALSE(hanna.unite(bathish, MAX_UNITE1));
    ostringstream os;
    ASSERT_NO_EXCEPTION(os << hanna);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Hanna\n"
            "Group's clan: Jaafar\n"
            "Group's children: 53\n"
            "Group's adults: 74\n"
            "Group's tools: 200\n"
            "Group's food: 400\n"
            "Group's morale: 80\n"));
    Group khoury("Khoury", "Jaafar", 53, 74, 200, 400, 65);
    Group abuleil("Abu Leil", "Jaafar", 50, 52, 174, 263, 90);
    ASSERT_FALSE(khoury.unite(abuleil, MAX_UNITE1));

    ASSERT_TRUE(hanna > bathish);
    ASSERT_TRUE(bathish.unite(hanna, MAX_UNITE2));
    ASSERT_NO_EXCEPTION(os << bathish);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Hanna\n"
            "Group's clan: Jaafar\n"
            "Group's children: 103\n"
            "Group's adults: 160\n"
            "Group's tools: 374\n"
            "Group's food: 663\n"
            "Group's morale: 80\n"));
    ASSERT_NO_EXCEPTION(os << hanna);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: \n"
            "Group's clan: \n"
            "Group's children: 0\n"
            "Group's adults: 0\n"
            "Group's tools: 0\n"
            "Group's food: 0\n"
            "Group's morale: 0\n"));














    return true;
}

int main(){
    RUN_TEST(testGroup);
    return 0;
}