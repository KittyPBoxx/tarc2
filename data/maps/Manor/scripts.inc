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
# 44 "data/maps/Manor/scripts.pory"
	msgbox ManorWarpOrb_Text_1
# 46 "data/maps/Manor/scripts.pory"
	release
	end

ManorWarpOrb_2:
# 38 "data/maps/Manor/scripts.pory"
	playfanfare MUS_OBTAIN_BADGE
# 39 "data/maps/Manor/scripts.pory"
	msgbox ManorWarpOrb_Text_0
# 40 "data/maps/Manor/scripts.pory"
	setflag FLAG_UNLOCK_FOREST_WARP
	goto ManorWarpOrb_1


CreditsOrb::
# 51 "data/maps/Manor/scripts.pory"
	lock
# 52 "data/maps/Manor/scripts.pory"
	special GameClear
# 53 "data/maps/Manor/scripts.pory"
	release
	end


CuttableCube::
# 59 "data/maps/Manor/scripts.pory"
	lockall
# 61 "data/maps/Manor/scripts.pory"
	msgbox CuttableCube_Text_0
# 63 "data/maps/Manor/scripts.pory"
	checkpartymove MOVE_CUT
# 64 "data/maps/Manor/scripts.pory"
	compare VAR_RESULT, 0
	goto_if_eq CuttableCube_2
CuttableCube_1:
# 71 "data/maps/Manor/scripts.pory"
	releaseall
	end

CuttableCube_2:
# 65 "data/maps/Manor/scripts.pory"
	playse SE_ICE_CRACK
# 66 "data/maps/Manor/scripts.pory"
	setflag FLAG_TREE_CUT
# 67 "data/maps/Manor/scripts.pory"
	removeobject VAR_LAST_TALKED
# 68 "data/maps/Manor/scripts.pory"
	msgbox CuttableCube_Text_1
	goto CuttableCube_1


OrthyNPC_Text_0:
# 15 "data/maps/Manor/scripts.pory"
	.string "What? No I wasn't napping! Go right\n"
	.string "ahead Berus Sir.$"

OrthyNPC_Text_1:
# 24 "data/maps/Manor/scripts.pory"
	.string "Successful candidates use the warp orb\n"
	.string "behind be to reach the gods.\p"
	.string "After the cheating last year I'm not\n"
	.string "supposed to let anyone in unless\l"
	.string "they're escorted by Berus personally.$"

ManorWarpOrb_Text_0:
# 39 "data/maps/Manor/scripts.pory"
	.string "A new warp was unlocked!$"

ManorWarpOrb_Text_1:
# 44 "data/maps/Manor/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"

CuttableCube_Text_0:
# 61 "data/maps/Manor/scripts.pory"
	.string "The box is weak. A Pokémon might be able\n"
	.string "to cut it.$"

CuttableCube_Text_1:
# 68 "data/maps/Manor/scripts.pory"
	.string "You cut the box.$"
