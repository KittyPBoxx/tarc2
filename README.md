# Pokémon Palimpsest – Puzzle Solutions (V1.2)

Welcome, to the acendency exam!  
This page contains solutions for all puzzles in **Pokémon Palimpsest**.  
Each puzzle is hidden behind a collapsible section — click **"Show Solution"** only when you're ready.  


## ⚙️ Debug Menu Guide

Below are optional tools available in the **Debug Menu** that can make testing, troubleshooting or recovery faster.  
Click a section to expand it.

---

<details>
  <summary>Auto Battle Setup (Skip Team Preparation)</summary>

Setting up your Ditto correctly is part of the puzzle experience — but it can be time-consuming.  
If you’d like to skip the setup and have the correct team automatically configured for each battle, you can use the **Debug Menu**.

**How to Access:**
> Press **L + R + B + START** during gameplay to open the Debug Menu.

**How to Auto-Setup Battles:**
1. From the Debug Menu, select **“Battles.”**
2. Choose the trainer you want to fight to start a **Test Battle.**
3. On the **first turn**, retire from the test battle.
4. Your party will now be automatically configured and ready to fight that trainer for real.
</details>

---

<details>
  <summary>Troubleshooting (The Solution Isn't Working or have a Bad Egg)</summary>

If you’re following a solution exactly but it’s not working, your Pokémon’s data may be **corrupted** (incorrect IVs or nature).

You can verify and fix this using the Debug Menu.

**To Check:**
1. Open the Debug Menu (**L + R + B + START**).
2. Go to **“Scripts.”**
3. Run **“Test IV’s.”**

**Expected result:**  
- All IVs = **0**  
- Nature = **Naughty**

If your results differ:

**To Fix:**
1. Go to **“Scripts.”**
2. Run **“Reset Ditto.”**

This will restore your Pokémon’s data to the correct values.
</details>

---

<details>
  <summary>Unlocking Warps (Recover Lost Progress)</summary>

If you’ve lost progress, deleted a save, etc., you can unlock warp flags manually.

**How to Unlock Warps:**
1. Open the Debug Menu (**L + R + B + START**).
2. Navigate to **`Flags & Vars` → `Set Flags`**.
3. Enable any of the following flags:

| Area | Flag Name | Flag ID |
|:--|:--|:--|
| Stairs (base warp, required for others) | `FLAG_UNLOCK_STAIRS_WARP` | `0x63` |
| Lake | `FLAG_UNLOCK_LAKE_WARP` | `0x64` |
| Ruins | `FLAG_UNLOCK_CAVE_WARP` | `0x65` |
| Summit | `FLAG_UNLOCK_SUMMIT_WARP` | `0x66` |
| Flower Field | `FLAG_UNLOCK_FLOWER_WARP` | `0x67` |

> ⚠️ **Note:** The warp menu will only be available if the **Stairs Warp** is unlocked.
</details>

---

## Opponent Info (Available Pokémon, Items, and Abilities by Area)

Below is a list of all the wild and trainer pokemon in each area, with their abilities, items and moves.  
Click a section to expand it.


<details>
  <summary>Start</summary>

| Location | Species | Item         | Ability | Move 1 | Move 2    | Move 3 | Move 4 |
|:--       |:--      |:--           |:--      |:--     |:--        |:--     |:--     |
| Wild     | Pichu   | Room Service | Static  | Tackle |           |        |        |
</details>

<details>
  <summary>Stairs</summary>

| Location | Species   | Item | Ability      | Move 1      | Move 2        | Move 3   | Move 4 |
|:--       |:--        |:--   |:--           |:--          |:--            |:--       |:--     |
| Wild     | Farfetchd | -    | Inner Focus  | Sand Attack | Fury Cutter   | Cut      | Leer   |
</details>

<details>
  <summary>Lake</summary>

| Location       | Species         | Item           | Ability      | Move 1       | Move 2     | Move 3     | Move 4     |
|:--             |:--              |:--             |:--           |:--           |:--         |:--         |:--         |
| Wild           | Forretress      | Zoom Lens      | Sturdy       | Take Down    | Rapid Spin | Heavy Slam | Curse      |
| Wild           | Aron            | Rawst Berry    | Heavy Metal  | Roar         | Metal Claw | Rock Tomb  | Rock Slide |
| Examiner Fionn | Chespin         | -              | Bulletproof  | Worry Seed   | Take Down  | -          | -          |
| Examiner Fionn | Exeggutor       | Coba Berry     | Chlorophyll  | Stored Power | Calm Mind  | Absorb     | -          |
| Examiner Sora  | Chansey         | -              | Natural Cure | Fling        | Covet      | Minimize   | Low Kick   |
| Examiner Sora  | Hawlucha        | -              | Unburden     | Sky Drop     | -          | -          | -          |
| Examiner Rune  | Cottonee        | -              | Prankster    | Stun Spore   | Round      | -          | -          |

</details>

<details>
  <summary>Ruins</summary>

| Location       | Species         | Item             | Ability      | Move 1          | Move 2       | Move 3       | Move 4    |
|:--             |:--              |:--               |:--           |:--              |:--           |:--           |:--        |
| Wild           | Muk             | Ability Shield   | Sticky Hold  | Pound           | Harden       | Earth Power  | Disable   |
| Wild           | Numel           | -                | Oblivious    | Flame Charge    | Incinerate   | Smack Down   | Endure    |
| Examiner Rune  | Ampharos (Mega) | -                | Mold Breaker | Zap Cannon      | Focus Blast  | -            | -         |
| Examiner Rune  | Mr. Mime        | Lagging Tail     | Soundproof   | Dream Eater     | Yawn         | Magic Powder | Role Play |
| Examiner Dante | Starmie         | Terrain Extender | Analytic     | Psychic Terrain | Refresh      | Toxic        | Foresight |
| Examiner Dante | Illumise        | -                | Prankster    | Will-O-Wisp     | Tail Wind    | Last Resort  | Role Play |
| Examiner Dante | Haunter         | Toxic Orb        | Levitate     | Curse           | Destiny Bond | Haze         | -         |
</details>

<details>
  <summary>Plains</summary>

| Location | Species | Item | Ability | Move 1 | Move 2 | Move 3 | Move 4 |
|:--|:--|:--|:--|:--|:--|:--|:--|
| Deity Tenjin | Ninjask | King’s Rock | Speed Boost | Poison Sting | Bullet Punch | - | - |
| Deity Tenjin | Slowpoke | Rocky Helmet | Oblivious | Perish Song | Misty Terrain | Telekinesis | Confusion |
| Deity Tenjin | Zeraora | Air Balloon | Volt Absorb | Plasma Fists | - | - | - |
</details>

<details>
  <summary>End</summary>

| Location | Species      | Item          | Ability     | Move 1     | Move 2  | Move 3 | Move 4 |
|:--       |:--           |:--            |:--          |:--         |:--      |:--     |:--     |
| Berus | Houndoom (Mega) | Bright Powder | Magic Guard | Sheer Cold | Fissure | Guillotine | - |
</details>



## Solutions

Below are optional tools available in the **Debug Menu** that can make testing, troubleshooting or recovery faster.
Click a section to expand it.

### Examiner FIONN 

<details>
  <summary>Show Ditto Config</summary>

| Species | Item | Ability | Move 1 | Move 2 | Move 3 | Move 4 |
|:--|:--|:--|:--|:--|:--|:--|
| Forretress | Zoom Lens | - | Curse | Fury Cutter | - | - |
</details>

<details>
  <summary>Show Setup Solution</summary>

| Type | Name | Description / How to Obtain |
|:--|:--|:--|
| Species | Forretress | Transmute on the Forretress (Lake) |
| Item | Zoom Lens | Use Barter on the Forretress (Lake) |
| Move | Curse | Use Sketch on the Forretress (Lake) |
| Move | Fury Cutter | Use Sketch on the Farfetch'd (Stairs) |
</details>

<details>
  <summary>Show Battle Solution</summary>


| Turn | Action | Target |
|:--|:--|:--|
| 1 | Curse | Chespin |
| 2 | Fury Cutter | Chespin |
| 3 | Fury Cutter | Chespin |
| 4 | Fury Cutter | Exeggutor |
</details>

### Examiner SORA 

<details>
  <summary>Show Ditto Config</summary>

| Species   | Item           | Ability     | Move 1     | Move 2      | Move 3 | Move 4 |
| :-------- | :------------- | :---------- | :--------- | :---------- | :----- | :----- |
| Exeggutor | -              | Heavy Metal | Heavy Slam | -           | -      | -      |

</details>

<details>
  <summary>Show Setup Solution</summary>

| Type    | Name           | Description / How to Obtain     |
| :------ | :------------- | :--------------------------     |
| Species | Exeggutor      | Transmute on FIONNs Exeggutor   |
| Move    | Heavy Slam     | Sketch on Forretress (Lake)     |
| Ability | Heavy Metal    | Transference on Aron (Lake)     |

</details>

<details>
  <summary>Show Battle Solution</summary>

| Turn | Action       | Target        |
| :--- | :----------  | :------------ |
| 1    | Heavy Slam   | Chansey       |
| 2    | Heavy Slam   | Hawlucha      |
| 3    | Heavy Slam   | Hawlucha      |
| 4    | Heavy Slam   | Hawlucha      |
| 5    | Heavy Slam   | Hawlucha      |
| 6    | Heavy Slam   | Hawlucha      |
| 7    | Heavy Slam   | Cottonee      |
| 8    | Heavy Slam   | Cottonee      |

</details>

### Examiner RUNE 

<details>
  <summary>Show Ditto Config</summary>

| Species    | Item           | Ability     | Move 1     | Move 2     | Move 3      | Move 4 |
| :--------- | :----------    | :---------- | :--------- | :--------- | :-----      | :----- |
| Exeggutor  | Ability Shield | Bulletproof | Worry Seed | Heavy Slam | Earth Power | -      |

</details>

<details>
  <summary>Show Setup Solution</summary>

| Type    | Name           | Description / How to Obtain   |
| :------ | :----------    | :--------------------------   |
| Species | Exeggutor      | Transmute on FIONNs Exeggutor |
| Move    | Worry Seed     | Sketch on FIONNs Chespin      |
| Move    | Heavy Slam     | Sketch on Forretress (Lake)   |
| Move    | Earth Power    | Sketch on Muk (Ruins)         |
| Item    | Ability Shield | Covet on Muk (Ruins). Covet must knock out the Muk or sticky hold will proc. You must Barter to get rid of your held items before using Covet or you will not get the item. To get Covet you need to Sketch from Sora's Chansey. Fight Sora with Muk or else Chansey will Low Kick after Minimising and not Covet. |

</details>

<details>
  <summary>Show Battle Solution</summary>

| Turn | Action       | Target        |
| :--- | :----------- | :-------      |
| 1    | Earth Power  | Mega Ampharos |
| 2    | Earth Power  | Mega Ampharos |
| 3    | Earth Power  | Mega Ampharos |
| 4    | Earth Power  | Mega Ampharos |
| 5    | Worry Seed   | Mega Ampharos |
| 6    | Barter       | Mega Ampharos |
| 7    | Earth Power  | Mega Ampharos |
| 8    | Worry Seed   | Mr. Mime      |
| 9    | Transference | Mr. Mime      |
| 10   | Heavy Slam   | Mr. Mime      |
| 11   | Heavy Slam   | Mr. Mime      |
| 12   | Heavy Slam   | Mr. Mime      |

</details>


### Examiner DANTE 

<details>
  <summary>Show Ditto Config</summary>

| Species | Item        | Ability     | Move 1       | Move 2 | Move 3       | Move 4    |
| :------ | :---------- | :---------- | :----------- | :----- | :----------- | :-------- |
| Muk     | Rawst Berry | Prankster   | Magic Powder | Roar   | Flame Charge | -         |

</details>

<details>
  <summary>Show Setup Solution</summary>

| Type    | Name         | Description / How to Obtain |
| :------ | :----------- | :-------------------------- |
| Species | Muk          | Transmute on Muk (Ruins)    |
| Item    | Rawst Berry  | Barter on Aron (Lake)       |
| Ability | Prankster    | Transference on Dantes Illumise |
| Move    | Magic Powder | Sketch on RUNE's Mr.Mime    |
| Move    | Roar         | Sketch on Aron (Lake). In order to Sketch Roar you must have Soundproof. To get this use Transference on RUNE's Mr. Mime (then retire) |
| Move    | Flame Charge | Sketch on Numel (Ruins)    |

</details>

<details>
  <summary>Show Battle Solution</summary>

| Turn | Action                  | Target   |
| :--- | :---------------------- | :------- |
| 1    | Flame Charge            | Illumise |
| 2    | Flame Charge            | Illumise |
| 3    | Roar                    | Illumise |
| 4    | Magic Powder            | Haunter  |
| 5    | Transference            | Haunter  |
| 6    | Transference            | Starmie  |
| 7    | Transference            | Haunter  |
| 8    | Transference            | Starmie  |
| 9    | Transference            | Haunter  |
| 10   | Tera Fire, Flame Charge | Illumise |
| 11   | Flame Charge            | Illumise |
| 12   | Flame Charge            | Starmie  |
| 13   | Flame Charge            | Starmie  |
| 14   | Flame Charge            | Starmie  |
| 15   | Flame Charge            | Starmie  |
| 16   | Flame Charge            | Starmie  |


</details>

### Examiner TENJIN 

<details>
  <summary>Show Ditto Config</summary>

| Species   | Item         | Ability     | Move 1       | Move 2     | Move 3  | Move 4 |
| :-------- | :----------- | :---------- | :----------- | :--------- | :------ | :----- |
| Sandygast | Rocky Helmet | Inner Focus | Plasma Fists | Disable    | Roar    | -    |


</details>

<details>
  <summary>Show Setup Solution</summary>

| Type    | Name         | Description / How to Obtain |
| :------ | :----------- | :-------------------------- |
| Species | Sandygast    | Stone Puzzle on Summit Left |
| Item    | Rocky Helmet | Use Barter on TENJIN's Slowpoke |
| Ability | Inner Focus  | Use Transference on the Farfetch'd (Stairs) |
| Move    | Plasma Fists | Use Sketch on TENJIN's Zeraora (to bring it out you'll need Roar)                           |
| Move    | Disable      | Sketch on Muk (Ruins)        |
| Move    | Roar         | Sketch on Aron (Lake). In order to Sketch Roar you must have Soundproof. To get this use Transference on RUNE's Mr. Mime (then retire)                            |

</details>

<details>
  <summary>Show Battle Solution</summary>

| Turn | Action       | Target   |
| :--- | :----------- | :------- |
| 1    | Roar         | Ninjask  |
| 2    | Transference | Zeraora  |
| 3    | Disable      | Slowpoke |
| 4    | Plasma Fists | Slowpoke |
| 5    | Plasma Fists | Slowpoke |
| 6    | Tera Fire, Anything     | -        |
| 7    | Anything     | -        |
| 8    | Anything     | -        |
| 9    | Anything     | -        |
| 10   | Anything     | -        |


</details>

### Examiner BERUS 

<details>
  <summary>Show Ditto Config</summary>

| Species       | Item         | Ability | Move 1 | Move 2 | Move 3        | Move 4 |
| :-------      | :----------  | :------ | :----- | :----- | :-----        | :----- |
| Houndoom Mega | Room Service | Sturdy  | Curse  | Leer   | Magic Powder  | Fling  |



</details>

<details>
  <summary>Show Setup Solution</summary>

| Type    | Name         | Description / How to Obtain |
| :------ | :----------  | :-------------------------- |
| Species | Sandygast    | From the summit rock puzzle |
| Ability | Sturdy       | From the Forretress (Lake)  |
| Item    | Room Service | From the Pichu (Flower)     |
| Move    | Curse        | Sketch on DANTE's Haunter   |
| Move    | Magic Power  | Sketch on RUNE's Mr Mime    |
| Move    | Leer         | Sketch on Farfetchd         |
| Move    | Fling        | Sketch on SORA's Chansey (after a bater so it has an item) |

</details>

<details>
  <summary>Show Battle Solution</summary>

| Turn | Action       | Target |
| :--- | :-------     | :----- |
| 1    | Curse        | -      |
| 2    | Curse        | -      |
| 3    | Curse        | -      |
| 4    | Curse        | -      |
| 5    | Curse        | -      |
| 6    | Curse        | -      |
| 7    | Barter       | -      |
| 8    | Leer         | -      |
| 9    | Leer         | -      |
| 10   | Leer         | -      |
| 11   | Leer         | -      |
| 12   | Leer         | -      |
| 13   | Magic Powder | -      |
| 14   | Fling        | -      |
| 15   | Barter       | -      |
| 16   | Fling        | -      |

</details>