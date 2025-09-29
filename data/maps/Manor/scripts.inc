Manor_MapScripts::
	.byte 0


OrthyNPC::
# 6 "data/maps/Manor/scripts.pory"
	lock
# 7 "data/maps/Manor/scripts.pory"
	special GetPlayerMonSpecies
# 10 "data/maps/Manor/scripts.pory"
	compare VAR_0x8008, SPECIES_MR_RIME
	goto_if_eq OrthyNPC_2
# 18 "data/maps/Manor/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 19 "data/maps/Manor/scripts.pory"
	msgbox OrthyNPC_Text_1
# 20 "data/maps/Manor/scripts.pory"
	removefieldmugshot
OrthyNPC_1:
# 23 "data/maps/Manor/scripts.pory"
	release
	end

OrthyNPC_2:
# 11 "data/maps/Manor/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 12 "data/maps/Manor/scripts.pory"
	msgbox OrthyNPC_Text_0
# 13 "data/maps/Manor/scripts.pory"
	removefieldmugshot
# 14 "data/maps/Manor/scripts.pory"
	applymovement 1, MoveOutTheWay
	goto OrthyNPC_1


# 28 "data/maps/Manor/scripts.pory"
MoveOutTheWay:
# 29 "data/maps/Manor/scripts.pory"
	face_left
# 30 "data/maps/Manor/scripts.pory"
	walk_left
# 30 "data/maps/Manor/scripts.pory"
	walk_left
	step_end

ManorWarpOrb::
# 34 "data/maps/Manor/scripts.pory"
	lock
# 36 "data/maps/Manor/scripts.pory"
	goto_if_unset FLAG_UNLOCK_FOREST_WARP, ManorWarpOrb_2
ManorWarpOrb_1:
# 43 "data/maps/Manor/scripts.pory"
	msgbox ManorWarpOrb_Text_1
# 45 "data/maps/Manor/scripts.pory"
	release
	end

ManorWarpOrb_2:
# 38 "data/maps/Manor/scripts.pory"
	msgbox ManorWarpOrb_Text_0
# 39 "data/maps/Manor/scripts.pory"
	setflag FLAG_UNLOCK_FOREST_WARP
	goto ManorWarpOrb_1


CuttableCube::
# 51 "data/maps/Manor/scripts.pory"
	lockall
# 53 "data/maps/Manor/scripts.pory"
	msgbox CuttableCube_Text_0
# 55 "data/maps/Manor/scripts.pory"
	checkpartymove MOVE_CUT
# 56 "data/maps/Manor/scripts.pory"
	compare VAR_RESULT, 0
	goto_if_eq CuttableCube_2
CuttableCube_1:
# 62 "data/maps/Manor/scripts.pory"
	releaseall
	end

CuttableCube_2:
# 57 "data/maps/Manor/scripts.pory"
	setflag FLAG_TREE_CUT
# 58 "data/maps/Manor/scripts.pory"
	removeobject VAR_LAST_TALKED
# 59 "data/maps/Manor/scripts.pory"
	msgbox CuttableCube_Text_1
	goto CuttableCube_1


OrthyNPC_Text_0:
# 12 "data/maps/Manor/scripts.pory"
	.string "What? No I wasn't napping! Go right in\n"
	.string "Berus Sir.$"

OrthyNPC_Text_1:
# 19 "data/maps/Manor/scripts.pory"
	.string "Successful candidates use the warp orb\n"
	.string "behind be to reach the gods.\p"
	.string "After the cheating last year I'm not\n"
	.string "supposed to let anyone in unless\l"
	.string "they're escorted by Berus personally.$"

ManorWarpOrb_Text_0:
# 38 "data/maps/Manor/scripts.pory"
	.string "A new warp was unlocked!$"

ManorWarpOrb_Text_1:
# 43 "data/maps/Manor/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"

CuttableCube_Text_0:
# 53 "data/maps/Manor/scripts.pory"
	.string "The box is weak. A Pokémon might be able\n"
	.string "to cut it.$"

CuttableCube_Text_1:
# 59 "data/maps/Manor/scripts.pory"
	.string "You cut the box.$"
