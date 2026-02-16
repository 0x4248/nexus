#made with Sparky's own 2 paws with a little help from Claude AI
from PIL import Image
from escpos import *
p = printer.Usb(0x08a6, 0x003d, profile ="default")

while True:
    usertext = input("Enter text to print, enter 'cut' to cut the paper, enter 'exit' to quit: ")
    if usertext.lower() == 'exit':
        break
    elif usertext.lower() == 'cut':
        for i in range(3):
            p.text("\n")
        p.cut()
    elif usertext.lower() == 'image':
        p.profile.profile_data['media']['width']['pixels'] = 576  # or 384
        img = Image.open(input("Enter the path to the image you want to print: "))
        printer_width = 576
        aspect_ratio = img.height / img.width
        new_height = int(printer_width * aspect_ratio)
        img = img.resize((printer_width, new_height))
        img = img.convert("1")
        p.image(img, impl="bitImageColumn")
        for i in range(3):
            p.text("\n")
        p.cut()
    else:
        p.text(usertext + "\n")