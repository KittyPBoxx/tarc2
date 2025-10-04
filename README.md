# Pokémon Palimpsest – Puzzle Solutions

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
  <summary>Troubleshooting (The Solution Isn't Working)</summary>

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
| Cave | `FLAG_UNLOCK_CAVE_WARP` | `0x65` |
| Summit | `FLAG_UNLOCK_SUMMIT_WARP` | `0x66` |
| Flower Field | `FLAG_UNLOCK_FLOWER_WARP` | `0x67` |

> ⚠️ **Note:** The warp menu will only be available if the **Stairs Warp** is unlocked.
</details>

---

## Solutions

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
| Exeggutor | Ability Shield | Bulletproof | Heavy Slam | Earth Power | -      | -      |

</details>

<details>
  <summary>Show Setup Solution</summary>

| Type    | Name           | Description / How to Obtain |
| :------ | :------------- | :-------------------------- |
| Species | Exeggutor      | Transmute on FIONNs Exeggutor          |
| Item    | Ability Shield | Covet on Muk (Lake). Covet must knock out the Muk or sticky hold will proc. You must Barter to get rid of your held items before using Covet or you will not get the item. To get Covet you need to Sketch from Sora's Chansey. Fight Sora with Muk or else Chansey will Low Kick after Minimising and not Covet. |
| Ability | Bulletproof    | Transference on FIONNs Chespin |
| Move    | Heavy Slam     | Sketch on FIONNs Chespin or Forretress (Lake) |
| Move    | Earth Power    | Sketch on Muk (Lake)                          |

</details>

<details>
  <summary>Show Battle Solution</summary>

| Turn | Action      | Target        |
| :--- | :---------- | :------------ |
| 1    | Heavy Slam  | Chansey       |
| 2    | Heavy Slam  | Chansey       |
| 3    | Earth Power | Mega Ampharos |
| 4    | Earth Power | Mega Ampharos |
| 5    | Earth Power | Mega Ampharos |
</details>

### Examiner RUNE 

<details>
  <summary>Show Ditto Config</summary>

| Species    | Item        | Ability     | Move 1     | Move 2     | Move 3 | Move 4 |
| :--------- | :---------- | :---------- | :--------- | :--------- | :----- | :----- |
| Forretress | -           | Heavy Metal | Worry Seed | Heavy Slam | -      | -      |

</details>

<details>
  <summary>Show Setup Solution</summary>

| Type    | Name        | Description / How to Obtain |
| :------ | :---------- | :-------------------------- |
| Species | Forretress  | Transmute Forretress (Lake) |
| Ability | Heavy Metal | Transference on Aron (Cave) |
| Move    | Worry Seed  | Sketch on FIONNs Chespin    |
| Move    | Heavy Slam  | Sketch on FIONNs Chespin or Forretress (Lake) |

</details>

<details>
  <summary>Show Battle Solution</summary>

| Turn | Action       | Target   |
| :--- | :----------- | :------- |
| 1    | Worry Seed   | Hawlucha |
| 2    | Worry Seed   | Hawlucha |
| 3    | Heavy Slam   | Hawlucha |
| 4    | Heavy Slam   | Hawlucha |
| 5    | Heavy Slam   | Hawlucha |
| 6    | Worry Seed   | Mr. Mime |
| 7    | Transference | Mr. Mime |
| 8    | Heavy Slam   | Mr. Mime |
| 9    | Heavy Slam   | Mr. Mime |

</details>