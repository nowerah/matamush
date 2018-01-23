//
// Created by Noor Athamnah on 1/17/2018.
//

#include "Group.h" /* includes string and ostream */
#include "exceptions.h"
#include "testMacros.h"

#define MAX_UNITE 250

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

    ASSERT_FALSE(hawari.unite(hawari, MAX_UNITE));
    ASSERT_FALSE(hawari.unite(athamnah, MAX_UNITE));






    return true;
}

int main(){
    RUN_TEST(testGroup);
    return 0;
}