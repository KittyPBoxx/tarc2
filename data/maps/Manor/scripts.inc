Manor_MapScripts::
	.byte 0


OrthyNPC::
# 9 "data/maps/Manor/scripts.pory"
	lock
# 10 "data/maps/Manor/scripts.pory"
	special GetPlayerMonSpecies
# 13 "data/maps/Manor/scripts.pory"
	compare VAR_0x8008, SPECIES_MR_RIME
	goto_if_eq OrthyNPC_2
# 23 "data/maps/Manor/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 24 "data/maps/Manor/scripts.pory"
	msgbox OrthyNPC_Text_1
# 25 "data/maps/Manor/scripts.pory"
	removefieldmugshot
OrthyNPC_1:
# 28 "data/maps/Manor/scripts.pory"
	release
	end

OrthyNPC_2:
# 14 "data/maps/Manor/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 15 "data/maps/Manor/scripts.pory"
	msgbox OrthyNPC_Text_0
# 16 "data/maps/Manor/scripts.pory"
	removefieldmugshot
# 17 "data/maps/Manor/scripts.pory"
	setflag FLAG_HIDE_EXAMINER
# 18 "data/maps/Manor/scripts.pory"
	removeobject 1
# 19 "data/maps/Manor/scripts.pory"
	removeobject 6
	goto OrthyNPC_1


ManorWarpOrb::
# 34 "data/maps/Manor/scripts.pory"
	lock
# 36 "data/maps/Manor/scripts.pory"
	goto_if_unset FLAG_UNLOCK_FOREST_WARP, ManorWarpOrb_2
ManorWarpOrb_1:
# 46 "data/maps/Manor/scripts.pory"
	msgbox ManorWarpOrb_Text_1
# 48 "data/maps/Manor/scripts.pory"
	release
	end

ManorWarpOrb_2:
# 38 "data/maps/Manor/scripts.pory"
	setflag FLAG_UNLOCK_FOREST_WARP
# 39 "data/maps/Manor/scripts.pory"
	special AutoSave
# 40 "data/maps/Manor/scripts.pory"
	playfanfare MUS_OBTAIN_BADGE
# 41 "data/maps/Manor/scripts.pory"
	msgbox ManorWarpOrb_Text_0
# 42 "data/maps/Manor/scripts.pory"
	waitfanfare
	goto ManorWarpOrb_1


CreditsOrb::
# 53 "data/maps/Manor/scripts.pory"
	lock
# 54 "data/maps/Manor/scripts.pory"
	playbgm MUS_BW12_CREDITS, FALSE
# 55 "data/maps/Manor/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 56 "data/maps/Manor/scripts.pory"
	msgbox CreditsOrb_Text_0
# 57 "data/maps/Manor/scripts.pory"
	removefieldmugshot
# 58 "data/maps/Manor/scripts.pory"
	special GameClear
# 59 "data/maps/Manor/scripts.pory"
	release
	end


CuttableCube::
# 65 "data/maps/Manor/scripts.pory"
	lockall
# 67 "data/maps/Manor/scripts.pory"
	msgbox CuttableCube_Text_0
# 69 "data/maps/Manor/scripts.pory"
	checkpartymove MOVE_CUT
# 70 "data/maps/Manor/scripts.pory"
	compare VAR_RESULT, 0
	goto_if_eq CuttableCube_2
CuttableCube_1:
# 77 "data/maps/Manor/scripts.pory"
	releaseall
	end

CuttableCube_2:
# 71 "data/maps/Manor/scripts.pory"
	playse SE_ICE_CRACK
# 72 "data/maps/Manor/scripts.pory"
	setflag FLAG_TREE_CUT
# 73 "data/maps/Manor/scripts.pory"
	removeobject VAR_LAST_TALKED
# 74 "data/maps/Manor/scripts.pory"
	msgbox CuttableCube_Text_1
	goto CuttableCube_1


OrthyNPC_Text_0:
# 15 "data/maps/Manor/scripts.pory"
	.string "What? No I wasn't napping! Go right\n"
	.string "ahead, Berus, Sir.$"

OrthyNPC_Text_1:
# 24 "data/maps/Manor/scripts.pory"
	.string "Successful candidates use the warp orb\n"
	.string "behind me to reach the gods.\p"
	.string "After the cheating last year, I'm not\n"
	.string "supposed to let anyone in unless\l"
	.string "they're escorted by Berus personally.$"

ManorWarpOrb_Text_0:
# 41 "data/maps/Manor/scripts.pory"
	.string "A new warp was unlocked!$"

ManorWarpOrb_Text_1:
# 46 "data/maps/Manor/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"

CreditsOrb_Text_0:
# 56 "data/maps/Manor/scripts.pory"
	.string "Hey Ditto, I know I don't always\n"
	.string "do things the way you're supposed to...\p"
	.string "Sorry about that.\p"
	.string "But I'm really glad you're my partner.\n"
	.string "Let's go meet the gods!$"

CuttableCube_Text_0:
# 67 "data/maps/Manor/scripts.pory"
	.string "The box is weak. A Pokémon might be able\n"
	.string "to cut it.$"

CuttableCube_Text_1:
# 74 "data/maps/Manor/scripts.pory"
	.string "You cut the box revealing a note:\n"
	.string "Use {SELECT_BUTTON} in battle to reorder moves.$"
