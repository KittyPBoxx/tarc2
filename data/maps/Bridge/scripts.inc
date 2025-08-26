# 1 "data/maps/Bridge/scripts.pory"

# 2 "data/maps/Bridge/scripts.pory"
Bridge_MapScripts::
# 3 "data/maps/Bridge/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, Bridge_OnTransition
# 4 "data/maps/Bridge/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Bridge_OnFrame
# 5 "data/maps/Bridge/scripts.pory"
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Bridge_OnWarp
# 6 "data/maps/Bridge/scripts.pory"
	.byte 0
# 7 "data/maps/Bridge/scripts.pory"

# 8 "data/maps/Bridge/scripts.pory"
Bridge_OnTransition:
# 9 "data/maps/Bridge/scripts.pory"
	end
# 10 "data/maps/Bridge/scripts.pory"

# 11 "data/maps/Bridge/scripts.pory"
Bridge_EventScript_HideMapNamePopup::
# 12 "data/maps/Bridge/scripts.pory"
	setflag FLAG_HIDE_MAP_NAME_POPUP
# 13 "data/maps/Bridge/scripts.pory"
	return
# 14 "data/maps/Bridge/scripts.pory"

# 15 "data/maps/Bridge/scripts.pory"
Bridge_OnFrame:
# 16 "data/maps/Bridge/scripts.pory"
	.2byte 0
# 17 "data/maps/Bridge/scripts.pory"

# 18 "data/maps/Bridge/scripts.pory"
Bridge_OnWarp:
# 19 "data/maps/Bridge/scripts.pory"
	.2byte 0
# 20 "data/maps/Bridge/scripts.pory"

# 21 "data/maps/Bridge/scripts.pory"
Bridge_EventScript_BerusName::
# 22 "data/maps/Bridge/scripts.pory"
.string "Berus$"
# 23 "data/maps/Bridge/scripts.pory"

# 24 "data/maps/Bridge/scripts.pory"

# 25 "data/maps/Bridge/scripts.pory"
Bridge_EventScript_Berus::
# 26 "data/maps/Bridge/scripts.pory"
	@ setspeaker Bridge_EventScript_BerusName
# 27 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 28 "data/maps/Bridge/scripts.pory"
	msgbox Bridge_Text_YoureLate, MSGBOX_NPC
# 29 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 30 "data/maps/Bridge/scripts.pory"
	msgbox Bridge_Text_TellTheTruth, MSGBOX_YESNO
# 31 "data/maps/Bridge/scripts.pory"
	goto_if_eq VAR_RESULT, YES, Bridge_EventScript_Truth
# 32 "data/maps/Bridge/scripts.pory"
	goto_if_eq VAR_RESULT, NO, Bridge_EventScript_Lie
# 33 "data/maps/Bridge/scripts.pory"
	end
# 34 "data/maps/Bridge/scripts.pory"

# 35 "data/maps/Bridge/scripts.pory"
Bridge_EventScript_Lie::
# 36 "data/maps/Bridge/scripts.pory"
	@ setspeaker Bridge_EventScript_BerusName
# 37 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 38 "data/maps/Bridge/scripts.pory"
	msgbox Bridge_Text_Lie, MSGBOX_NPC
# 39 "data/maps/Bridge/scripts.pory"
	end
# 40 "data/maps/Bridge/scripts.pory"

# 41 "data/maps/Bridge/scripts.pory"
Bridge_EventScript_Truth::
# 42 "data/maps/Bridge/scripts.pory"
	@ setspeaker Bridge_EventScript_BerusName
# 43 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 44 "data/maps/Bridge/scripts.pory"
	msgbox Bridge_Text_Truth, MSGBOX_NPC
# 45 "data/maps/Bridge/scripts.pory"
	end
# 46 "data/maps/Bridge/scripts.pory"

# 47 "data/maps/Bridge/scripts.pory"

# 48 "data/maps/Bridge/scripts.pory"
@ TEXT @
# 49 "data/maps/Bridge/scripts.pory"

# 50 "data/maps/Bridge/scripts.pory"
Bridge_Text_YoureLate:
# 51 "data/maps/Bridge/scripts.pory"
	.string "Decided to finally show up, did we?\n"
# 52 "data/maps/Bridge/scripts.pory"
	.string "Is there a good reason you're four\l"
# 53 "data/maps/Bridge/scripts.pory"
	.string "hours late?$"
# 54 "data/maps/Bridge/scripts.pory"

# 55 "data/maps/Bridge/scripts.pory"
Bridge_Text_TellTheTruth:
# 56 "data/maps/Bridge/scripts.pory"
	.string "Tell the truth?$"
# 57 "data/maps/Bridge/scripts.pory"

# 58 "data/maps/Bridge/scripts.pory"
Bridge_Text_Truth:
# 59 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I didn't hear my\n"
# 60 "data/maps/Bridge/scripts.pory"
	.string "alarm clock and-$"
# 61 "data/maps/Bridge/scripts.pory"

# 62 "data/maps/Bridge/scripts.pory"
Bridge_Text_Lie:
# 63 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I had to... er... give\n" 
# 64 "data/maps/Bridge/scripts.pory"
	.string "my train seat to a grandma... who was\n" 
# 65 "data/maps/Bridge/scripts.pory"
	.string "in a wheelchair... and uh... also\n"
# 66 "data/maps/Bridge/scripts.pory"
	.string "pregnant? But I dropped my ticket so-$"
# 67 "data/maps/Bridge/scripts.pory"

# 68 "data/maps/Bridge/scripts.pory"
Bridge_Text_TellMeYourName:
# 69 "data/maps/Bridge/scripts.pory"
	.string "Enough! Tell me your name so I can mark\n"
# 70 "data/maps/Bridge/scripts.pory"
	.string "you down$"
# 71 "data/maps/Bridge/scripts.pory"

# 72 "data/maps/Bridge/scripts.pory"
@ // BERUS: Ah you must be Thya's child. I heard she's sponsored by that trickster god now. How is she?
# 73 "data/maps/Bridge/scripts.pory"

# 74 "data/maps/Bridge/scripts.pory"
@ // PLAYER: I'm not sure, she rarely come home from work
# 75 "data/maps/Bridge/scripts.pory"

# 76 "data/maps/Bridge/scripts.pory"
@ // BERUS: An inspiration to us all. Well they say the apple dosn't fall far from the tree but in my experience fruit that lands in the shade of it's bearer tends to spoil fast.
# 77 "data/maps/Bridge/scripts.pory"
@ // Unfortunately, {PLAYER} you arrived too late to take the test this year but I wish you all the best in future attempts
# 78 "data/maps/Bridge/scripts.pory"

# 79 "data/maps/Bridge/scripts.pory"
@ // CONTROL COMES BACK TO THE PLAYER
# 80 "data/maps/Bridge/scripts.pory"

# 81 "data/maps/Bridge/scripts.pory"
@ // BERUS: I know the exam runs untill sunset but you can't possibly expect to start four hours late and still pass. No one has ever passed it that fast before
# 82 "data/maps/Bridge/scripts.pory"
@ // Did you even prepare? Fine, If you can prove you at least know how to save the game I guess I can let you take the test
# 83 "data/maps/Bridge/scripts.pory"

# 84 "data/maps/Bridge/scripts.pory"
@ // PLAYER MUST SAVE THE GAME:
# 85 "data/maps/Bridge/scripts.pory"
@	- if not Saved 
# 86 "data/maps/Bridge/scripts.pory"
@		If you can prove you at least know how to save the game I guess I can let you take the test
# 87 "data/maps/Bridge/scripts.pory"
@	- if saved
# 88 "data/maps/Bridge/scripts.pory"

# 89 "data/maps/Bridge/scripts.pory"

# 90 "data/maps/Bridge/scripts.pory"
@ // BERUS DROPS BAG LETS YOU CHOSE A MON - THERE IS ONLY ONE 
# 91 "data/maps/Bridge/scripts.pory"

# 92 "data/maps/Bridge/scripts.pory"
@ // PLAYER: there was only one left
# 93 "data/maps/Bridge/scripts.pory"

# 94 "data/maps/Bridge/scripts.pory"
@ // BERUS: A good trainer makes the best choice for their situation. A great trainer makes sure they always have a good choice to make. 
# 95 "data/maps/Bridge/scripts.pory"
@ // You were late so you get the mon that none of the other students wanted. Still, you don't have you're own divine sponsor yet 
# 96 "data/maps/Bridge/scripts.pory"
@ // but you should be able to use the prayers of your mums sponsor untill you get one of your own. Do you know what that means?
# 97 "data/maps/Bridge/scripts.pory"

# 98 "data/maps/Bridge/scripts.pory"
@ - Yes 
# 99 "data/maps/Bridge/scripts.pory"
@ // BERUS: Well then, better hurry you don't have much time
# 100 "data/maps/Bridge/scripts.pory"

# 101 "data/maps/Bridge/scripts.pory"

# 102 "data/maps/Bridge/scripts.pory"
@ - No
# 103 "data/maps/Bridge/scripts.pory"
@ // BERUS: Ha, I knew you didn't prepare. Prayers are like moves 
# 104 "data/maps/Bridge/scripts.pory"

# 105 "data/maps/Bridge/scripts.pory"

# 106 "data/maps/Bridge/scripts.pory"
@ - Talk to berus again
# 107 "data/maps/Bridge/scripts.pory"
@ // BERUS: better hurry you don't have much time
