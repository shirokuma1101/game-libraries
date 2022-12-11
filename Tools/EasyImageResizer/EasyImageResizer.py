# -*- coding: utf-8 -*-

# standard library
import argparse
import os
import subprocess


def main():
    # Parse arguments
    parser = argparse.ArgumentParser(description="Easy Image Resizer")
    parser.add_argument('-i', '--input_file',             type=str, required=True, help='Input file(Required)')
    parser.add_argument('-r', '--resolution',             type=int, required=True, help='Resolution(Required)')
    parser.add_argument('-c', '--compression', default=1, type=int, required=True, help='Compression ratio(1~9 default:1)')
    args = parser.parse_args()

    # Check arguments
    if args.input_file is None:
        print('Input File is required')
        return
    if args.resolution is None:
        print('Resolution is required')
        return
    # Check validity
    if not os.path.exists(args.input_file):
        print('Input file not exist')
        return
    if args.compression < 1 or args.compression > 9:
        print('Compression ratio must be between 1 and 9')
        return

    # Backup file
    splitted_file_name = os.path.splitext(args.input_file)
    backup_file_name = f'{splitted_file_name[0]}_backup.{splitted_file_name[1][1:]}'
    os.rename(args.input_file, backup_file_name)
    # Resize
    if args.compression == 1:
        subprocess.run(f'ffmpeg -y -i "{backup_file_name}" -vf "scale={args.resolution}:{args.resolution}" -qmin 1 -q {args.compression} "{args.input_file}"')
    else:
        subprocess.run(f'ffmpeg -y -i "{backup_file_name}" -vf "scale={args.resolution}:{args.resolution}" -q {args.compression} "{args.input_file}"')
    # Delete backup file
    os.remove(backup_file_name)


if __name__ == '__main__':
    main()

