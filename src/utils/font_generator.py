from PIL import Image, ImageDraw, ImageFont
import os
import sys
import shutil


def textsize(text, font):
    im = Image.new(mode="L", size=(0, 0))
    draw = ImageDraw.Draw(im)
    _, _, width, height = draw.textbbox((0, 0), text=text, font=font)
    return width, height


def create_images_from_font(font_path, output_dir, characters, image_size=(10, 10), font_size=10):
    font = ImageFont.truetype(font_path, font_size)

    if os.path.exists(output_dir):
        print(f"Removing existing directory: {output_dir}")
        shutil.rmtree(output_dir)

    os.makedirs(output_dir)

    for char in characters:
        img = Image.new('L', image_size, color=255)
        d = ImageDraw.Draw(img)
        width, height = textsize(char, font)
        position = ((image_size[0] - width) / 2, (image_size[1] - height) / 2)
        d.text(position, char, font=font, fill=0)  # black text

        img.save(os.path.join(output_dir, f"{ord(char)}.png"))


font = 'ComicMono'

if len(sys.argv) > 1:
    font = sys.argv[1]

if len(sys.argv) > 2:
    font_size = int(sys.argv[2].replace(' ', ''))

font_path = f'fonts/{font}.ttf'
output_dir = f'fonts/{font}_chars'

characters = '@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,^` '

create_images_from_font(font_path, output_dir, characters,
                        (font_size, font_size), font_size)
