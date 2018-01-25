#include "../testMacros.h"
#include "../Group.h"
#include "../Clan.h"
#include "../Area.h"
#include "../World.h"
#include "../exceptions.h"

using namespace mtm;

void fillWorld(World& w) {
    /* Set up areas */
    w.addArea("Varrock", PLAIN);
    w.addArea("Lumbridge", RIVER);
    w.addArea("Falador", RIVER);
    w.addArea("Taverley", MOUNTAIN);
    w.addArea("Wilderness", MOUNTAIN);
    w.makeReachable("Varrock", "Lumbridge");
    w.makeReachable("Lumbridge", "Varrock");
    w.makeReachable("Varrock", "Falador");
    w.makeReachable("Falador", "Varrock");
    w.makeReachable("Varrock", "Wilderness");
    w.makeReachable("Taverley", "Falador");
    
    /* Set up clans */
    for (string clan : {"Misthalin", "Asgarnia", "Entrana", "Crandor",
                        "Al-Kharid", "Morytania", "Newbie"}) {
        w.addClan(clan);
    }
    w.makeFriends("Misthalin", "Asgarnia");
    w.makeFriends("Al-Kharid", "Misthalin");
    w.makeFriends("Entrana", "Crandor");
    w.makeFriends("Newbie", "Misthalin");
}

bool testWorld(){
    World w;
    fillWorld(w);
    
    ASSERT_EXCEPTION(w.makeReachable("Falador", "Camelot"), WorldAreaNotFound);
    ASSERT_EXCEPTION(w.makeReachable("Camelot", "Falador"), WorldAreaNotFound);
    
    ASSERT_EXCEPTION(w.makeFriends("Misthalin", "Lunars"), WorldClanNotFound);
    ASSERT_EXCEPTION(w.makeFriends("Lunars", "Misthalin"), WorldClanNotFound);
    
    ASSERT_EXCEPTION(w.uniteClans("Lunars", "Misthalin", ""),
                     WorldInvalidArgument);
    ASSERT_EXCEPTION(w.uniteClans("Lunars", "Misthalin", "Al-Kharid"),
                     WorldClanNameIsTaken);
    ASSERT_EXCEPTION(w.uniteClans("Lunars", "Misthalin", "Lunars"),
                     WorldClanNotFound);
    
    ASSERT_NO_EXCEPTION(w.uniteClans("Newbie", "Misthalin", "Misthalin"));
    // Misthalin's old friends are still friends of the united clan
    // The new clan is stored in the memory of Newbie
    
    ASSERT_EXCEPTION(w.addClan(""), WorldInvalidArgument);
    ASSERT_EXCEPTION(w.addClan("Misthalin"), WorldClanNameIsTaken);
    
    ASSERT_EXCEPTION(w.addGroup("", "", 10, 10, "Varrock"),
                     WorldInvalidArgument);
    ASSERT_EXCEPTION(w.addGroup("Smiths", "", -1, 10, "Varrock"),
                     WorldInvalidArgument);
    ASSERT_EXCEPTION(w.addGroup("Smiths", "", 0, 0, "Varrock"),
                     WorldInvalidArgument);
    ASSERT_EXCEPTION(w.addGroup("Smiths", "Lunars", 10, 10, "Karamja"),
                     WorldClanNotFound);
    ASSERT_EXCEPTION(w.addGroup("Smiths", "Misthalin", 10, 10, "Karamja"),
                     WorldAreaNotFound);
    ASSERT_NO_EXCEPTION(w.addGroup("Smiths", "Misthalin", 10, 10, "Lumbridge"));
    // Smiths of Misthalin have arrived to Lumbridge, where a river passes
    ASSERT_EXCEPTION(w.addGroup("Smiths", "Lunars", 15, 15, "Wilderness"),
                     WorldGroupNameIsTaken);
    
    ASSERT_NO_EXCEPTION(w.addGroup("Goblins","Misthalin",10,200,"Lumbridge"));
    // Goblins of Misthalin joined their fellow Smiths in Lumbridge, and traded
    
    ostringstream os;
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Goblins"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Goblins\n"
        "Group's clan: Misthalin\n"
        "Group's children: 10\n"
        "Group's adults: 200\n"
        "Group's tools: 752\n"
        "Group's food: 668\n"
        "Group's morale: 77\n"
        "Group's current area: Lumbridge\n"));
    // Smiths status: 88 tools, 2 food
    
    ASSERT_NO_EXCEPTION(w.printClan(os, "Misthalin"));
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: Misthalin\n"
        "Clan's groups:\n"
        "Goblins\n"
        "Smiths\n"));
    
    ASSERT_EXCEPTION(w.printGroup(os, "Seers"), WorldGroupNotFound);
    ASSERT_EXCEPTION(w.printClan(os, "Newbie"), WorldClanNotFound);
    
    ASSERT_EXCEPTION(w.moveGroup("Rats", "Draynor"), WorldGroupNotFound);
    ASSERT_EXCEPTION(w.moveGroup("Goblins", "Draynor"), WorldAreaNotFound);
    ASSERT_EXCEPTION(w.moveGroup("Goblins", "Taverley"), WorldAreaNotReachable);
    ASSERT_NO_EXCEPTION(w.moveGroup("Goblins", "Varrock"));
    // Goblins move north to Varrock, a plain
    // They are divided (they are currently all of the clan, thus > 1/3 of clan)
    ASSERT_EXCEPTION(w.moveGroup("Goblins","Varrock"), WorldGroupAlreadyInArea);
    
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Goblins_2"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Goblins_2\n"
        "Group's clan: Misthalin\n"
        "Group's children: 5\n"
        "Group's adults: 100\n"
        "Group's tools: 376\n"
        "Group's food: 334\n"
        "Group's morale: 77\n"
        "Group's current area: Varrock\n"));
    
    ASSERT_NO_EXCEPTION(w.addGroup("Dwarves", "Asgarnia", 120, 5, "Varrock"));
    // Friendly Dwarves of Asgarnia join the Goblins in Varrock, and are divided
    
    ASSERT_NO_EXCEPTION(w.moveGroup("Dwarves_2", "Wilderness"));
    // Dwarves_2 head to the mountainous wild, becoming its dominant group
    
    ASSERT_NO_EXCEPTION(w.addGroup("Barbarians","Asgarnia",0,150,"Wilderness"));
    // Fellow aggressive Barbarians of Asgarnia dominate the wild instead
    
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Barbarians"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Barbarians\n"
        "Group's clan: Asgarnia\n"
        "Group's children: 0\n"
        "Group's adults: 150\n"
        "Group's tools: 600\n"
        "Group's food: 450\n"
        "Group's morale: 77\n"
        "Group's current area: Wilderness\n"));
    
    ASSERT_NO_EXCEPTION(w.addGroup("Merchants","Al-Kharid",70,40,"Lumbridge"));
    // Meanwhile, Merchants of Al-Kharid, a friendly clan, trade with the Smiths
    
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Merchants"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Merchants\n"
        "Group's clan: Al-Kharid\n"
        "Group's children: 70\n"
        "Group's adults: 40\n"
        "Group's tools: 207\n"
        "Group's food: 213\n"
        "Group's morale: 77\n"
        "Group's current area: Lumbridge\n"));
    // Smiths status: 41 tools, 49 food
    
    ASSERT_NO_EXCEPTION(w.moveGroup("Goblins", "Wilderness"));
    // Goblins head over to the wild, fight with the Barbarians and lose
    
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Goblins"));
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Barbarians"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Goblins\n"
        "Group's clan: Misthalin\n"
        "Group's children: 3\n"
        "Group's adults: 66\n"
        "Group's tools: 188\n"
        "Group's food: 167\n"
        "Group's morale: 61\n"
        "Group's current area: Wilderness\n"
        "Group's name: Barbarians\n"
        "Group's clan: Asgarnia\n"
        "Group's children: 0\n"
        "Group's adults: 113\n"
        "Group's tools: 450\n"
        "Group's food: 533\n"
        "Group's morale: 93\n"
        "Group's current area: Wilderness\n"));
    
    ASSERT_NO_EXCEPTION(w.addGroup("Miners", "Misthalin", 4, 10, "Falador"));
    // Miners of Misthalin find a good spot in Falador, where one sees a river
    
    ASSERT_EXCEPTION(w.moveGroup("Goblins", "Varrock"), WorldAreaNotReachable);
    // Goblins wish they could get out of the wild, but it is too late
    
    ASSERT_NO_EXCEPTION(w.moveGroup("Smiths", "Varrock"));
    // They are less than a third, but together with Goblins_2, they are more
    // Nothing interesting happens
    
    ASSERT_NO_EXCEPTION(w.moveGroup("Miners", "Varrock"));
    // The miners had enough ores, so are united with the Smiths to smith armor
    
    ASSERT_EXCEPTION(w.printGroup(os, "Miners"), WorldGroupNotFound);
    
    ASSERT_NO_EXCEPTION(w.addGroup("Wizards", "Entrana", 8, 80, "Taverley"));
    // Wizards of Entrana reach the mountain of Taverley, dominating the top
    
    ASSERT_NO_EXCEPTION(w.uniteClans("Entrana", "Crandor", "Crandor"));
    // Wizards have seen Slayers coming from afar, and decided to be friendly
    
    ASSERT_NO_EXCEPTION(w.addGroup("Slayers", "Crandor", 0, 3, "Taverley"));
    // The Slayers of Crandor join them on the mountain, but do not dominate
    
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Slayers"));
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Wizards"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Slayers\n"
        "Group's clan: Crandor\n"
        "Group's children: 0\n"
        "Group's adults: 3\n"
        "Group's tools: 12\n"
        "Group's food: 9\n"
        "Group's morale: 77\n"
        "Group's current area: Taverley\n"
        "Group's name: Wizards\n"
        "Group's clan: Crandor\n"
        "Group's children: 8\n"
        "Group's adults: 80\n"
        "Group's tools: 320\n"
        "Group's food: 256\n"
        "Group's morale: 69\n" // changed clans
        "Group's current area: Taverley\n"));
    
    ASSERT_NO_EXCEPTION(w.moveGroup("Wizards", "Falador"));
    // Wizards hear of the Wizards' Guild, and start their journey to there
    // Slayers are left alone in the mountain, showing weak dominance
    
    ASSERT_NO_EXCEPTION(w.addGroup("Snails", "Morytania", 300, 0, "Taverley"));
    // Snails of Morytania brought forth the plague to the Slayers' reach
    // They finish one of them off, and win the dominance, knowing no mercy
    
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Slayers"));
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Snails"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Slayers\n"
        "Group's clan: Crandor\n"
        "Group's children: 0\n"
        "Group's adults: 2\n"
        "Group's tools: 6\n"
        "Group's food: 4\n"
        "Group's morale: 61\n"
        "Group's current area: Taverley\n"
        "Group's name: Snails\n"
        "Group's clan: Morytania\n"
        "Group's children: 300\n"
        "Group's adults: 0\n"
        "Group's tools: 0\n"
        "Group's food: 602\n"
        "Group's morale: 93\n"
        "Group's current area: Taverley\n"));
    
    // If you are interested in a continuation of the story, please talk to
    //  Perdu, the travelling dwarven merchant in Edgeville.
    // He is normally found at the first floor of the general store.
    // Tell him I sent you, and he shall tell you the rest of the details of
    //  this fascinating story.
    
    // Thank you for listening!
    
    return true;
}

int main(){
    RUN_TEST(testWorld);
    return 0;
}