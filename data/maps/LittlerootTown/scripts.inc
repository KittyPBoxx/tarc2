LittlerootTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LittlerootTown_OnWarp
	.byte 0

LittlerootTown_OnTransition:
	setflag FLAG_VISITED_LITTLEROOT_TOWN
	end

LittlerootTown_EventScript_HideMapNamePopup::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	return

LittlerootTown_OnFrame:
	.2byte 0

LittlerootTown_OnWarp:
	.2byte 0

LittlerootTown_EventScript_ManName::
.string "MAN$"

LittlerootTown_EventScript_FatMan::
	setspeaker LittlerootTown_EventScript_ManName
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
	msgbox LittlerootTown_Text_CanUsePCToStoreItems, MSGBOX_NPC
	removefieldmugshot
	end

LittlerootTown_EventScript_BoyName::
.string "BOY$"

LittlerootTown_EventScript_Boy::
	setspeaker LittlerootTown_EventScript_BoyName
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
	msgbox LittlerootTown_Text_BirchSpendsDaysInLab, MSGBOX_NPC
	removefieldmugshot
	end

LittlerootTown_EventScript_GirlName::
.string "GIRL$"

LittlerootTown_EventScript_Twin::
	setspeaker LittlerootTown_EventScript_GirlName
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
	msgbox LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut, MSGBOX_DEFAULT
	release
	end

LittlerootTown_Text_CanUsePCToStoreItems:
	.string "If you use a PC, you can store items\n"
	.string "and POKéMON.\p"
	.string "The power of science is staggering!$"

LittlerootTown_Text_BirchSpendsDaysInLab:
	.string "PROF. BIRCH spends days in his LAB\n"
	.string "studying, then he'll suddenly go out in\l"
	.string "the wild to do more research…\p"
	.string "When does PROF. BIRCH spend time\n"
	.string "at home?$"

LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut:
	.string "Um, um, um!\p"
	.string "If you go outside and go in the grass,\n"
	.string "wild POKéMON will jump out!$"
