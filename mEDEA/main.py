#coding: utf-8

""" 
    @brief This script launches a run on the kilombo simulator.
    @params -v windowName  Records a video of the current launch and saves it with the specify windowName. 
                                                     The current date and hour are appended to the given name. Videos are saved in the "./videos/current_date" directory
    @todo replace the loop by a regex in the getWindowPosition (?)
                 adapt this code to other operating systems and simulators

    First, the directory is cleaned from all old compilation files. Then, the current run is compiled.
    If needed, all the recording options are then initialized and directories are created.
    Then, the run is started, window's informations are initialized and the recording is started.
    The recording is stopped after the run's end.
"""

import os 
import sys
import time
import subprocess
import shlex
import datetime
import Tkinter
import json

def getScreenResolution():
    """ 
        @brief returns the width,height of the screen 
        @return the width, height of the screen

         This methods uses Tkinter to find these informatiqons, it should be installed on each python distribution.
         If it's not, please enter the following command line: "pip install tkinter".
         This methods isn't used in this script but was useful in former versions. Nonetheless, it can be useful for other problems.
    """
    root = Tkinter.Tk()
    width = root.winfo_screenwidth()
    height = root.winfo_screenheight()
    return int(width), int(height)

    # Specific for Linux, doesn't require to have Tkinter installed, even though it should be the case on most python distribution.
    # These instructions didn't work on the linux virtual machine
    # cmd = ['xrandr']
    # cmd2 = ['grep', '*']
    # p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    # p2 = subprocess.Popen(cmd2, stdin=p.stdout, stdout=subprocess.PIPE)
    # p.stdout.close()
    # resolution_string, junk = p2.communicate()
    # resolution = resolution_string.split()[0]
    # width, height = resolution.split('x')
    # return int(width), int(height)

def formatPath(defaultwindowName = "recording"):
    """ 
        @brief returns the formatted windowName based on the default windowName. 
        @params the based windowName to create the formatted windowName
        @returns the based windowName with the current date and hour added at the end

         Current date and hour are appended to the windowName. 
          File are stored in the /videos/current_date directory. If this directory doesn't exists, it's created.
    """
    dirwindowName = "./videos/" + str(datetime.date.today())
    if not os.path.exists(dirwindowName):
        os.makedirs(dirwindowName)

    dateInfos = str(datetime.datetime.now())
    dateInfos = "_".join(dateInfos.split(' '))
    return dirwindowName + "/" + defaultwindowName + "_" + dateInfos + ".mp4"


def getWindowPosition(windowName):
    """ 
        @brief Returns the posX, posY of the test window
        @params data the information of the window given by xwininfo.
        @return the absolution position of the window
    """
    windowInfoProcess = subprocess.Popen(["xwininfo", "-name", windowName], stdout=subprocess.PIPE)
    data = windowInfoProcess.communicate()[0]
    posX, posY = 0,0
    print(data)
    arrayLine = data.split("\n")    

    # optimisable, regex ? 
    for line in arrayLine:
        words = line.split(" ")
        if"Absolute" in words:
            if "X:" in words:
                posX = words[-1]
            elif "Y:": 
                posY = words[-1]


    return posX, posY


def getJsonDict(path):
    """ 
        @brief returns the dict of the json struct in the given file
        @params path string identifying the location of the json file
        @returns a dict containing the information stored in the file identified by the given path
    """
    with open(path) as file:
        data = file.read()
        infos= json.loads(data) 

    return infos



## Beginning of the script
## This part should  should be adapted to other simulators and operating systems

os.system("make clean")
os.system("make mEDEA")
# os.system("./main -p start_position.json -b start_position.json");

# The current run is recorded
if  "-v" in sys.argv:

    # Get the screen resolution informations
    # screenWidth, screenHeight = getScreenResolution();

    # get current run informations from the kilombo.json file 
    kilomboInfos = getJsonDict("kilombo.json")
    windowWidth = kilomboInfos["displayWidth"]
    windowHeight = kilomboInfos["displayHeight"] 
    windowName = kilomboInfos["botName"]  if "botName" in kilomboInfos.keys() else "default"

    # Launch the run
    simulator = subprocess.Popen(["./mEDEA"])
    time.sleep(0.1) # wait for the window to be created

    # Get window's information from xwininfo
    posX, posY = getWindowPosition(windowName)

    # get the formated name for the recording file
    index = sys.argv.index("-v")
    if index + 1 < len(sys.argv):
        path = formatPath(sys.argv[index + 1])
    else: 
        path = formatPath()

    # starts the recording
    commandLine = "ffmpeg -f x11grab -s " + str(windowWidth) + "x" + str(windowHeight) + " -r 60 -i :0.0+" + str(posX) + "," + str(posY) + " " + path
    recorder = subprocess.Popen(shlex.split(commandLine))

    # Wait for the simulation to end
    simulator.wait()

    # stop the recording
    recorder.send_signal(2)


else:
    record = False
    recorder = None
    os.system("./mEDEA");
