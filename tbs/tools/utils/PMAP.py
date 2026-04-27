# PMAP
# A simple image format that is human readable and editable
# Licence: GNU General Public License v3.0
# By: 0x4248

try:
    from PIL import Image
except ImportError:
    print(
        "PIL is not installed. Please install it using 'pip install pillow' or use the requirements.txt file"
    )
    exit(1)
import os
import sys


def get_meta_data(file):
    for line in file:
        line = line.replace("\n", "")
        if line.startswith("s:"):
            res = line.split(":")[1].split("x")
        if line.startswith("f:"):
            fill = line.split(":")[1].split(",")
    return res, fill


def generate_base_image(res, fill):
    image = []
    for i in range(int(res[0])):
        row = []
        for j in range(int(res[1])):
            row.append(fill)
        image.append(row)
    return image


def replace_pixel(image, x, y, color):
    image[x][y] = color
    return image


def generate_image(file):
    res, fill = get_meta_data(file)
    image = generate_base_image(res, fill)
    pixels = False
    for line in file:
        line = line.replace("\n", "")
        if line.startswith("--PIXELS--"):
            pixels = True
            continue
        if line.startswith("--END--"):
            pixels = False
            continue
        if pixels:
            x, y = line.split(":")[0].split(",")
            color = line.split(":")[1].split(",")
            image = replace_pixel(image, int(x), int(y), color)
    return image


def pmap_to_img(file, output):
    image = generate_image(file)
    img = Image.new("RGB", (len(image), len(image[0])))
    pixels = img.load()
    for i in range(len(image)):
        for j in range(len(image[i])):
            pixels[i, j] = tuple(map(int, image[i][j]))
    img.show()
    img.save(output)


def img_to_pmap(image, output):
    img = Image.open(image)
    pixels = img.load()
    res = img.size
    fill = pixels[0, 0]
    pmap = f"s:{res[0]}x{res[1]}\nf:{fill[0]},{fill[1]},{fill[2]}\n--PIXELS--\n"
    for i in range(res[0]):
        for j in range(res[1]):
            pmap += f"{i},{j}:{pixels[i, j][0]},{pixels[i, j][1]},{pixels[i, j][2]}\n"
    pmap += "--END--"

    with open(output, "w") as file:
        file.write(pmap)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        pass
    else:
        if sys.argv[1] == "-a":
            img_to_pmap(sys.argv[2], sys.argv[3])
            sys.exit(0)
        elif sys.argv[1] == "-b":
            with open(sys.argv[2], "r") as file:
                lines = file.readlines()
            pmap_to_img(file, sys.argv[3])
            sys.exit(0)
        elif sys.argv[1] == "-v":
            file = open(sys.argv[2], "r").readlines()
            pmap_to_img(file, "temp.png")
            os.remove("temp.png")
            sys.exit(0)

    print("1. Convert image to pmap")
    print("2. Convert pmap to image")
    print("3. View image from pmap")
    choice = input("Enter your choice: ")
    if choice == "1":
        image = input("Enter the image path: ")
        output = input("Enter the output path: ")
        img_to_pmap(image, output)
    elif choice == "2":
        pmap = input("Enter the pmap path: ")
        output = input("Enter the output path: ")
        file = open(pmap, "r").readlines()
        pmap_to_img(file, output)
    elif choice == "3":
        pmap = input("Enter the pmap path: ")
        file = open(pmap, "r").readlines()
        pmap_to_img(file, "temp.png")
        os.remove("temp.png")
    else:
        print("Invalid choice")
        sys.exit(1)
