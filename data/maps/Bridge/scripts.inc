Bridge_MapScripts::
# 4 "data/maps/Bridge/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, BridgeIntro
	.byte 0


BridgeIntro::
	return


# 43 "data/maps/Bridge/scripts.pory"
LookUp:
# 44 "data/maps/Bridge/scripts.pory"
	face_up
	step_end

BerusNPC::
# 51 "data/maps/Bridge/scripts.pory"
	lock
# 52 "data/maps/Bridge/scripts.pory"
	faceplayer
# 54 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, BerusNPC_2
# 60 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_BERUS_FINAL_BATTLE_DONE, BerusNPC_5
# 66 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 67 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_2
# 68 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BerusNPC_1:
# 73 "data/maps/Bridge/scripts.pory"
	release
	end

BerusNPC_2:
# 55 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 56 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_0
# 57 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_5:
# 61 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 62 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_1
# 63 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1


StarterBag::
# 81 "data/maps/Bridge/scripts.pory"
	lock
# 82 "data/maps/Bridge/scripts.pory"
	faceplayer
# 84 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, StarterBag_2
# 96 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_2
StarterBag_1:
# 99 "data/maps/Bridge/scripts.pory"
	release
	end

StarterBag_2:
# 85 "data/maps/Bridge/scripts.pory"
	setflag FLAG_CHOSE_STARTER
# 87 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 88 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_0
# 89 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 91 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 92 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_1
# 93 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto StarterBag_1


CutTree::
# 107 "data/maps/Bridge/scripts.pory"
	lock
# 108 "data/maps/Bridge/scripts.pory"
	faceplayer
# 110 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_TREE_CUT, CutTree_2
# 116 "data/maps/Bridge/scripts.pory"
	msgbox CutTree_Text_1
CutTree_1:
# 119 "data/maps/Bridge/scripts.pory"
	release
	end

CutTree_2:
# 111 "data/maps/Bridge/scripts.pory"
	msgbox CutTree_Text_0
	goto CutTree_1


BerusNPC_Text_0:
# 56 "data/maps/Bridge/scripts.pory"
	.string "Choose a Pokémon from the bag before\n"
	.string "we proceed.$"

BerusNPC_Text_1:
# 62 "data/maps/Bridge/scripts.pory"
	.string "Don't waste time. The exam is already\n"
	.string "underway.$"

BerusNPC_Text_2:
# 67 "data/maps/Bridge/scripts.pory"
	.string "I still can't believe you passed...$"

StarterBag_Text_0:
# 88 "data/maps/Bridge/scripts.pory"
	.string "Wait, there's only one Pokémon in here...$"

StarterBag_Text_1:
# 92 "data/maps/Bridge/scripts.pory"
	.string "A good trainer makes the best of what\n"
	.string "they're given.$"

StarterBag_Text_2:
# 96 "data/maps/Bridge/scripts.pory"
	.string "It's an empty bag with Berus's photo on\n"
	.string "it.$"

CutTree_Text_0:
# 111 "data/maps/Bridge/scripts.pory"
	.string "This tree looks like it could be cut\n"
	.string "down.$"

CutTree_Text_1:
# 116 "data/maps/Bridge/scripts.pory"
	.string "The tree has already been cleared.$"
