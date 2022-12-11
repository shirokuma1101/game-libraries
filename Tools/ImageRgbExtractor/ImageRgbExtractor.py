# -*- encoding: utf-8 -*-

# standard library
import argparse
import os

# pillow
from PIL import Image


def main():
    # Parse arguments
    parser = argparse.ArgumentParser(description="Image RGB Extractor")
    parser.add_argument('-i', '--input_file',  type=str, required=True, help='Input file(Required)')
    parser.add_argument('-o', '--output_file', type=str, required=True, help='Output file(Required)')
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

    # Open image
    im = Image.open(args.input_file)
    # Get image size
    width, height = im.size

    with open(args.output_file, 'w') as f:
        f.write(f'[{width},{height}]')
        for y in range(height):
            f.write('\n')
            for x in range(width):
                color = im.getpixel((x, y))
                f.write(f'[{color[0]},{color[1]},{color[2]},{color[3] if len(color) == 4 else 255}]')


if __name__ == "__main__":
    main()

