# Compiled C
# A collection of C programs that are compiled to assembly 
# code for understanding how C and assembly work.
# GitHub: https://wwww.github.com/0x4248/Compiled_C
# Licence: GNU General Public Licence v3.0
# By: 0x4248

import os

SOURCES_FILE = "sources.conf"
COLOUR_BLUE = "\033[94m"
COLOUR_RESET = "\033[0m"

def calculate_percentage_done(current_build, total_builds):
    try:
        current_build = total_builds.index(current_build)
    except:
        current_build = total_builds.index(current_build+"\n")
    total_builds = len(total_builds)
    return round((current_build / total_builds) * 100)

if __name__ == "__main__":
    f = open("sources.conf", "r")
    sources = f.readlines()
    f.close()
    # The sources file contains where the makefiles are to run
    for source in sources:
        source = source.strip()
        # Strip the new line character from the end of the line
        if source == "" or source.startswith("#"):
            # If the line is blank or starts with a hash, skip it
            continue
        # Print [BLUE+Percentage done+RESET] [source]
        print("[" + COLOUR_BLUE + str(calculate_percentage_done(source, sources)) + "%\t" + COLOUR_RESET + "] " + source)
        os.chdir(source)
        os.system("make")
        while True:
            os.chdir("..")
            files = os.listdir()
            if "sources.conf" in files:
                break