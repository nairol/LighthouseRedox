## HMD and controller calibration data

**Warning:** Do not load the files in this folder into your own Vive or controllers! Every device has slightly different calibration data and will not work as intended with a different calibration.

To get this from your devices:

* Open the folder *...Steam\steamapps\common\SteamVR\tools\lighthouse\bin\win32*
* Shift + right click to get the extended context menu
* Click *open command window here*
* Type `lighthouse_console.exe serial` and press enter
* For each of the listed serial numbers that start with "LHR-" do the following:
* Type `lighthouse_console.exe serial <SerialNumber>` and press enter
* Type `downloadconfig <OutputFileName>` and press enter

To identify the HMD open the json files in a text editor and look for a line starting with `"device_class": "hmd"`. For the HMD you can get the mura correction data:

* Type `lighthouse_console.exe serial <SerialNumberOfTheHMD>` and press enter
* Type `userdata` and press enter
* For each listed files do the following:
* Type `userdatadownload <FileName>` and press enter. (This takes about 25 seconds)
* The mura correction files are PNGs with the (wrong) file extension ".mc". Change it back to ".png" if you want.

When finished type `quit`, press enter and close the window.