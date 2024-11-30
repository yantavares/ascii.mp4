# Executables for the calabreso.txt Project

This repository contains two versions of executables to run the calabreso.txt project without requiring additional setup.

Available Executables

	1.	Ubuntu:
	•	Designed for Ubuntu distributions using older OpenCV versions available via APT.
	•	File name: Ubuntu
	2.	Linux (General):
	•	Compatible with Linux distributions using the latest OpenCV version installed manually.
	•	File name: Linux

Note: Both executables are derived from the same source file, LINUX.cpp. Click here to view the source code.

Requirements

1. Ubuntu and derivatives (using APT)

 apt-get install libopencv-dev

This executable is optimized to work with Ubuntu’s default packages.

2. Linux (Distributions with Updated OpenCV)

For Linux distributions with support for newer OpenCV versions, OpenCV must be installed manually:

Arch:

pacman -S opencv

Fedora:

dnf install opencv

How to Use

Run the desired command to start the player:

./Ubuntu <path_to_video>
# or
./Linux <path_to_video>

The video will be automatically adjusted to the terminal size and played directly as an ASCII video player.

If needed, grant execution permission to the file:

chmod +x Ubuntu
# or
chmod +x Linux

Tips for Better Quality

	•	Smaller Font: Reduce the terminal font size to achieve higher definition in characters.
	•	Larger Window: Maximize the terminal window to allow more space for the video.

Notes

This execution mode uses a brightness-based technique to determine the characters for the video, which may result in lower resolution compared to the full mode available in the original project.

To experience the full version, which allows detailed configurations and better visual results, refer to the original README here.

For other Linux versions, you can manually compile the LINUX.cpp file.

A version for Windows is available here, but the executable is not provided in this repository. To use it, compile the Windows.cpp file manually.

Thank you again for your support! :smile: