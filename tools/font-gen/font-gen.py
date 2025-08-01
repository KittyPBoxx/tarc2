from PIL import Image, ImageDraw, ImageFont

# Config (same as before)
FONT_PATH = "font.ttf"
IMAGE_WIDTH, IMAGE_HEIGHT = 256, 512
TILE_SIZE = 16
RENDER_HEIGHT = 15  # Glyph height used in mask
TILES_PER_ROW = 16
START_Y = 160
# Updated colors to match the indexed PNG palette
TRANSPARENT_COLOR = (144, 200, 255)  # For reference if needed
TEXT_COLOR = (56, 56, 56)             # Black text
SHADOW_COLOR = (216, 216, 216)        # Light gray shadow
BG_COLOR = (255, 255, 255)            # White background box
SHADOW_OFFSET = (1, 1)
VERTICAL_SHIFT = -2
FONT_SIZE = 14

CHAR_ORDER = [
    "ʳ", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "!", "?", ".", "-", "•",
    "…", "“", "”", "‘", "’", "♂", "♀", "$", ",", "×", "/", "A", "B", "C", "D", "E", "F",
    "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
    "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "▲", ":", "Ä", "Ö", "Ü",
    "ä", "ö", "ü"
]

CHAR_WIDTHS = [
    3,  6,  6,  6,  6,  6,  6,  6,  6,  6,  3,  6,  6,  6,  6,  6,
    8,  6,  6,  6,  6,  6,  6,  6,  3,  6,  6,  6,  6,  6,  6,  3,
    6,  6,  6,  6,  6,  8,  6,  6,  6,  6,  6,  6,  9,  7,  6,  3,
    3,  3,  3,  3, 10,  8,  3,  3,  7,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    6,  6,  4,  8,  8,  8,  7,  8,  8,  4,  6,  6,  4,  4,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  6,  3,  3,  3,  3,  3,  3,  6,
    3,  3,  3,  3,  3,  3,  3,  6,  3,  7,  7,  7,  7,  1,  2,  3,
    4,  5,  6,  7,  6,  6,  6,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    8,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  4,  6,  3,  6,  3,
    6,  6,  6,  3,  3,  6,  6,  6,  3,  7,  6,  6,  6,  6,  6,  6,
    6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
    6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  4,  5,  6,
    4,  6,  6,  6,  6,  6,  5,  6,  6,  6,  6,  6,  6,  6,  6,  8,
    3,  6,  6,  6,  6,  6,  6,  3,  3,  3,  3,  8,  3,  3,  3,  3,
    10, 10, 10, 10,  8, 10, 10,  8, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  3,
]

font = ImageFont.truetype(FONT_PATH, FONT_SIZE)

# Prepare fallback glyph for missing char detection
missing_char = "\uFFFF"
fallback_img = Image.new("1", (TILE_SIZE * 2, RENDER_HEIGHT), 0)
fallback_draw = ImageDraw.Draw(fallback_img)
fallback_draw.text((0, VERTICAL_SHIFT), missing_char, font=font, fill=1)
fallback_bbox = fallback_img.getbbox()
fallback_data = None
if fallback_bbox:
    fallback_cropped = fallback_img.crop((fallback_bbox[0], 0, fallback_bbox[2], RENDER_HEIGHT))
    fallback_data = list(fallback_cropped.getdata())

# Step 1: Create image with *only* valid glyphs rendered
new_glyphs_img = Image.new("RGBA", (IMAGE_WIDTH, IMAGE_HEIGHT), TRANSPARENT_COLOR)

valid_glyph_positions = []  # Will hold (tile_x, tile_y, width, mask) for each valid glyph

for glyph_count, char in enumerate(CHAR_ORDER):
    temp_img = Image.new("1", (TILE_SIZE * 2, RENDER_HEIGHT), 0)
    temp_draw = ImageDraw.Draw(temp_img)
    temp_draw.text((0, VERTICAL_SHIFT), char, font=font, fill=1)

    bbox = temp_img.getbbox()
    if not bbox:
        continue

    left, _, right, _ = bbox
    width = right - left
    cropped = temp_img.crop((left, 0, right, RENDER_HEIGHT))
    glyph_data = list(cropped.getdata())
    if fallback_data and glyph_data == fallback_data:
        # Skip fallback glyph
        continue

    mask = cropped.convert("L").point(lambda p: 255 if p > 0 else 0)

    # Shadow and text layers
    shadow_layer = Image.new("RGBA", (width, RENDER_HEIGHT), SHADOW_COLOR + (255,))
    text_layer = Image.new("RGBA", (width, RENDER_HEIGHT), TEXT_COLOR + (255,))

    col = glyph_count % TILES_PER_ROW
    row = glyph_count // TILES_PER_ROW
    tile_x = col * TILE_SIZE
    tile_y = START_Y + row * TILE_SIZE

    # Background box
    bg_width = width + SHADOW_OFFSET[0]
    bg_box = Image.new("RGBA", (bg_width, RENDER_HEIGHT), BG_COLOR + (255,))
    new_glyphs_img.paste(bg_box, (tile_x, tile_y))

    CHAR_WIDTHS[glyph_count + (10 * 16)] = bg_width

    new_glyphs_img.paste(shadow_layer, (tile_x + SHADOW_OFFSET[0], tile_y + SHADOW_OFFSET[1]), mask)
    new_glyphs_img.paste(text_layer, (tile_x, tile_y), mask)

    # Store valid glyph info for replacement step
    valid_glyph_positions.append((tile_x, tile_y, bg_width, RENDER_HEIGHT))

# Save the new glyph image (optional)
new_glyphs_img.save("new_glyphs_only.png")

# Step 2: Load existing template and replace glyph tiles with new ones
template_img = Image.open("default_template.png").convert("RGBA")

for tile_x, tile_y, width, height in valid_glyph_positions:
    # Clear original glyph area: fill with transparent or white
    clear_box = Image.new("RGBA", (TILE_SIZE, TILE_SIZE), TRANSPARENT_COLOR)
    template_img.paste(clear_box, (tile_x, tile_y))

    # Paste the new glyph area from new_glyphs_img (crop width × height)
    glyph_area = new_glyphs_img.crop((tile_x, tile_y, tile_x + width, tile_y + height))
    template_img.paste(glyph_area, (tile_x, tile_y), glyph_area)

# Step 3: Save combined new font image
template_img.save("new_font.png")
print("Combined new font image saved as new_font.png")

output_code_file = "glyph_widths.c"
with open(output_code_file, "w") as f:
    f.write("ALIGNED(4) const u8 gFontNormalLatinGlyphWidths[] = {\n")

    for i in range(0, len(CHAR_WIDTHS), 16):
        row = CHAR_WIDTHS[i:i+16]
        row_str = ", ".join(f"{num:2}" for num in row)
        f.write(f"    {row_str},\n")

    f.write("};\n")
