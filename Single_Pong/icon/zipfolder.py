import shutil, os


AllFiles = os.listdir()

shutil.copy("Single_Pong.exe", "Single_Pong")

for SingleFile in AllFiles:
    if SingleFile.endswith(".dll"):
        shutil.copy(SingleFile, "Single_Pong")

shutil.make_archive("Single_Pong", 'zip', "Single_Pong")

