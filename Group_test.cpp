#include "Group.h" /* includes string and ostream */
#include "exceptions.h"
#include "testMacros.h"

#define MAX_UNITE1 250
#define MAX_UNITE2 400

using namespace mtm;

bool testGroupGeneral() {

    //Testing constructors

    ASSERT_EXCEPTION(Group("Dragon", "heart", 5, 7, 122, 30, 120), GroupInvalidArgs);
    ASSERT_EXCEPTION(Group("Dragon", "heart", 0, 0, 122, 30, 20), GroupInvalidArgs);
    ASSERT_EXCEPTION(Group("Dragon", "heart", 50, 17, -5, 30, 78), GroupInvalidArgs);
    ASSERT_EXCEPTION(Group("", "heart", 50, 17, 5, 30, 78), GroupInvalidArgs);
    ASSERT_NO_EXCEPTION(Group("Lovely", "Dark", 15, 17, 24, 224, 100));

    Group hawari("Hawari", "Hawarah", 40, 70, 200, 350, 74);
    Group abunassar("Abu Nassar", "Nassar", 0, 25, 40, 30, 80);
    Group athamnah("Athamnah", 40, 60);
    Group farah(hawari);

    //Testing get functions
    ASSERT_TRUE(farah.getName() == "Hawari");
    ASSERT_TRUE(abunassar.getSize() == 25);
    ASSERT_TRUE(athamnah.getSize() == 100);
    ASSERT_TRUE(athamnah.getClan().empty());
    ASSERT_TRUE(farah.getClan() == "Hawarah");

    //Testing operators
    ASSERT_TRUE(hawari > athamnah);
    ASSERT_FALSE(hawari <= athamnah);
    ASSERT_NO_EXCEPTION(athamnah.changeClan("Othman"));
    ASSERT_TRUE(athamnah.getClan() == "Othman");
    ASSERT_TRUE(hawari < athamnah);
    ASSERT_FALSE(hawari == athamnah);
    ASSERT_FALSE(hawari > athamnah);
    ASSERT_FALSE(hawari >= athamnah);
    ASSERT_TRUE(farah == hawari);

    return true;
}

bool testUnite() {

    //Testing unite
    Group hawari("Hawari", "Hawarah", 40, 70, 200, 350, 74);
    Group athamnah("Athamnah", 40, 60);
    ASSERT_NO_EXCEPTION(athamnah.changeClan("Othman"));

    ASSERT_FALSE(hawari.unite(hawari, MAX_UNITE1));
    ASSERT_FALSE(hawari.unite(athamnah, MAX_UNITE1));
    Group hanna("Hanna", "Jaafar", 53, 74, 200, 400, 80);
    Group bathish("Bathish", "Jaafar", 50, 86, 174, 263, 80);
    ASSERT_FALSE(hanna.unite(bathish, MAX_UNITE1));
    ostringstream os;
    ASSERT_NO_EXCEPTION(os << hanna);
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Hanna\n"
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
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Hanna\n"
            "Group's clan: Jaafar\n"
            "Group's children: 103\n"
            "Group's adults: 160\n"
            "Group's tools: 374\n"
            "Group's food: 663\n"
            "Group's morale: 80\n"));
    ASSERT_NO_EXCEPTION(os << hanna);
    ASSERT_TRUE(VerifyOutput(os, "Group's name: \n"
            "Group's clan: \n"
            "Group's children: 0\n"
            "Group's adults: 0\n"
            "Group's tools: 0\n"
            "Group's food: 0\n"
            "Group's morale: 0\n"));

    return true;
}

bool testDivide() {
    ostringstream os;
    Group nseir("Nuseir", "Naser", 243, 300, 640, 1520, 65);
    Group salem = nseir.divide("Salem");

    ASSERT_NO_EXCEPTION(os << nseir);
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Nuseir\n"
            "Group's clan: Naser\n"
            "Group's children: 122\n"
            "Group's adults: 150\n"
            "Group's tools: 320\n"
            "Group's food: 760\n"
            "Group's morale: 65\n"));

    ASSERT_NO_EXCEPTION(os << salem);
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Salem\n"
            "Group's clan: Naser\n"
            "Group's children: 121\n"
            "Group's adults: 150\n"
            "Group's tools: 320\n"
            "Group's food: 760\n"
            "Group's morale: 65\n"));

    Group blue("Blue", "Berry", 1, 1, 640, 1520, 65);
    ASSERT_EXCEPTION(blue.divide("Red"), GroupCantDivide);
    Group red("Red", "Cherry", 10, 15, 640, 1520, 65);
    ASSERT_EXCEPTION(red.divide(""), GroupInvalidArgs);

    return true;
}

bool testFight() {
    ostringstream os;
    Group red("Red", "Cherry", 10, 15, 640, 1520, 65);
    Group nseir("Nuseir", "Naser", 122, 150, 320, 760, 65);
    Group salem("Salem", "Naser", 121, 150, 320, 760, 65);
    Group hanna1("Hanna" , "Hanna", 1, 1, 5, 7 ,70);
    Group hanna2("Hanna" , "Hanna", 1, 1, 5, 7 ,80);
    ASSERT_TRUE(hanna1.unite(hanna2, MAX_UNITE1));

    ASSERT_EXCEPTION(red.fight(red), GroupCantFightWithItself);
    ASSERT_EXCEPTION(red.fight(hanna2), GroupCantFightEmptyGroup);
    ASSERT_TRUE(nseir.fight(salem) == WON);

    ASSERT_NO_EXCEPTION(os << nseir);
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Nuseir\n"
            "Group's clan: Naser\n"
            "Group's children: 122\n"
            "Group's adults: 113\n"
            "Group's tools: 240\n"
            "Group's food: 950\n"
            "Group's morale: 78\n"));

    ASSERT_NO_EXCEPTION(os << salem);
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Salem\n"
            "Group's clan: Naser\n"
            "Group's children: 80\n"
            "Group's adults: 100\n"
            "Group's tools: 160\n"
            "Group's food: 380\n"
            "Group's morale: 52\n"));

    Group yellow("Yellow", "Banana", 50, 60, 640, 1520, 65);
    Group green("Green", "Apple", 50, 60, 640, 1520, 65);
    Group yellow2("Yellow", "Sun", 50, 60, 640, 1520, 65);
    Group yellow3("Yellow", "Butterfly", 50, 60, 640, 1520, 65);
    ASSERT_TRUE(yellow2.fight(yellow3) == DRAW);
    ASSERT_TRUE(green.fight(yellow) == LOST);

    return true;
}

bool testTrade() {
    ostringstream os;
    Group orange1("Orange", "Orange", 50, 61, 640, 1520, 65);
    Group orange2("Orange", "Orange", 40, 60, 120, 300, 65);
    Group orange3("Orange", "Orange", 200, 153, 258, 258, 65);
    Group orange4("Orange", "Orange", 100, 60, 10, 3, 92);

    ASSERT_FALSE(orange1.trade(orange2));
    ASSERT_FALSE(orange1.trade(orange3));

    ASSERT_NO_EXCEPTION(os << orange1);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Orange\n"
            "Group's clan: Orange\n"
            "Group's children: 50\n"
            "Group's adults: 61\n"
            "Group's tools: 640\n"
            "Group's food: 1520\n"
            "Group's morale: 65\n"));

    ASSERT_TRUE(orange1.trade(orange4));

    ASSERT_NO_EXCEPTION(os << orange1);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Orange\n"
            "Group's clan: Orange\n"
            "Group's children: 50\n"
            "Group's adults: 61\n"
            "Group's tools: 650\n"
            "Group's food: 1510\n"
            "Group's morale: 65\n"));

    ASSERT_NO_EXCEPTION(os << orange4);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Orange\n"
            "Group's clan: Orange\n"
            "Group's children: 100\n"
            "Group's adults: 60\n"
            "Group's tools: 0\n"
            "Group's food: 13\n"
            "Group's morale: 92\n"));

    Group orange5("Orange", "Orange", 123, 740, 45, 60, 74);
    Group orange6("Orange", "Orange", 220, 423, 128, 100, 82);

    ASSERT_TRUE(orange5.trade(orange6));

    ASSERT_NO_EXCEPTION(os << orange5);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Orange\n"
            "Group's clan: Orange\n"
            "Group's children: 123\n"
            "Group's adults: 740\n"
            "Group's tools: 56\n"
            "Group's food: 49\n"
            "Group's morale: 74\n"));

    ASSERT_NO_EXCEPTION(os << orange6);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Orange\n"
            "Group's clan: Orange\n"
            "Group's children: 220\n"
            "Group's adults: 423\n"
            "Group's tools: 117\n"
            "Group's food: 111\n"
            "Group's morale: 82\n"));

    Group orange7("Orange", "Orange", 123, 740, 45, 60, 74);
    Group orange8("Orange", "Orange", 220, 423, 146, 100, 82);

    ASSERT_TRUE(orange7.trade(orange8));

    ASSERT_NO_EXCEPTION(os << orange7);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Orange\n"
            "Group's clan: Orange\n"
            "Group's children: 123\n"
            "Group's adults: 740\n"
            "Group's tools: 61\n"
            "Group's food: 44\n"
            "Group's morale: 74\n"));

    ASSERT_NO_EXCEPTION(os << orange8);
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Orange\n"
            "Group's clan: Orange\n"
            "Group's children: 220\n"
            "Group's adults: 423\n"
            "Group's tools: 130\n"
            "Group's food: 116\n"
            "Group's morale: 82\n"));
    return true;
}

int main(){
    RUN_TEST(testGroupGeneral);
    RUN_TEST(testDivide);
    RUN_TEST(testUnite);
    RUN_TEST(testFight);
    RUN_TEST(testTrade);
    return 0;
}