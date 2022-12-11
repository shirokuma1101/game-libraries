# -*- coding: utf-8 -*-

# standard library
import argparse
import glob
import os
import subprocess

# pillow
from PIL import Image

# shutil
import shutil

def main():
    # Parse arguments
    parser = argparse.ArgumentParser(description="Video To Sequential Image")
    parser.add_argument('-i', '--input_file',              type=str, required=True, help='Input file(Required)')
    parser.add_argument('-o', '--output_file',             type=str, required=True, help='Output file(Required)')
    parser.add_argument('-f', '--frame_rate',  default=60, type=int,                help='Frame rate(default:60)')
    parser.add_argument('-w', '--width_count', default=10, type=int,                help='Width count(default:10)')
    args = parser.parse_args()

    # Check arguments
    if args.input_file is None:
        print('Input File is required')
        return
    if args.output_file is None:
        print('Output File is required')
        return
    # Check validity
    if not os.path.exists(args.input_file):
        print('Input file not exist')
        return

    # Create workspace
    os.mkdir('.\workspace')
    # Convert video to image
    subprocess.run(rf'ffmpeg -i {args.input_file} -vcodec png -r {args.frame_rate} workspace\image_%03d.png')
    image_list_len = len(glob.glob(r'.\workspace\*.png'))
    im = Image.open(rf'.\workspace\image_001.png')
    width, height = im.size
    im.close()
    # Create canvas
    canvas_width = width * args.width_count
    canvas_height = height * (image_list_len // args.width_count + 1)
    canvas = Image.new('RGB', (canvas_width, canvas_height), (255, 255, 255))

    # Paste image
    for i in range(image_list_len):
        im = Image.open(rf'.\workspace\image_{i + 1:03d}.png')
        canvas.paste(im, (width * (i % args.width_count), height * (i // args.width_count)))
        im.close()

    canvas.save(args.output_file)
    canvas.close()
    shutil.rmtree('.\workspace')


if __name__ == '__main__':
    main()

