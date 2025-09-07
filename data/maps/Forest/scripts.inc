# 1 "data/maps/Forest/scripts.pory"

# 2 "data/maps/Forest/scripts.pory"
Forest_MapScripts::
# 3 "data/maps/Forest/scripts.pory"
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Forest_MapScripts_CreateTiles
# 4 "data/maps/Forest/scripts.pory"
	map_script MAP_SCRIPT_ON_RESUME, Forest_MapScripts_CreateTiles
# 5 "data/maps/Forest/scripts.pory"
	.byte 0
# 6 "data/maps/Forest/scripts.pory"

# 7 "data/maps/Forest/scripts.pory"

# 8 "data/maps/Forest/scripts.pory"
Forest_MapScripts_CreateTiles::
# 9 "data/maps/Forest/scripts.pory"
	createvobject OBJ_EVENT_GFX_TRUCK, 1, 20, 0
# 10 "data/maps/Forest/scripts.pory"
	createvobject OBJ_EVENT_GFX_TRUCK, 1, 15, 4
# 11 "data/maps/Forest/scripts.pory"
	return
