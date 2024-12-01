ℹ️ Versão em Português: [README](README.md)

# calabreso.txt

**Executables (no setup required) are available! See the [README.md](app/README.md) or the `Releases` tab of the repository for more information.**

**Thank you for your support 🌠:**

This is a project that transforms a video into ASCII art, converting each frame into text using any font you want. It offers two output modes:

1. Plays the video in text format directly in the terminal.
2. Saves the ASCII frames as a rendered `.mp4` file in the `output` folder.

**Example** (takes a few seconds to load):  
![Video Example](public/sampletxt.gif)

---

## Requirements

- `make` (GNU)
- OpenCV2 (can be installed with `libopencv-dev` on Ubuntu or `opencv` on Fedora and Arch)
- g++ compiler (for the C++ engine)
- Python3 (for the Python engine)

---

## Setup (UNIX systems)

1. Clone the repository:

```bash
  git clone https://github.com/yantavares/calabreso.txt.git
  cd calabreso.txt
```

2. If needed, make the `play.sh` file executable:

```bash
  chmod +x play.sh
```

3. Install the necessary packages:

```bash
   # For Ubuntu
   sudo apt-get install libopencv-dev
   pip install -r requirements.txt
```

4. **(Optional)** Add your video file to the `videos` folder, or use the provided `SampleVideo.mp4`.

5. **(Optional)** Place your preferred font (.ttf) in the `fonts` folder, or use the default font `ComicMono.ttf`.

---

## How to Run the Project

To run the project, use the `make` command:

```bash
   make
```

You will choose between two execution modes:

1. **Play in terminal**: Displays the video as ASCII art directly in your terminal.
2. **Save as MP4**: Generates a rendered `.mp4` file in the `output` folder.

---

### Steps After Running `make`:

1. **Select the font**:
   - The default is `Comic Mono`.
2. **Choose the font size**:
   - **Must be compatible with your terminal's font size**.
   - Choose between 2 and 20.
   - The default is 11.
3. **Enter the video name**:
   - Choose a video from the `videos` folder.
   - The default is `SampleVideo.mp4`.
4. **Select the mode**:
   - Choose one of the options:
     - **1**: Play in the terminal.
     - **2**: Save as `.mp4` in the `output` folder.

If you don't respond to a prompt, the default values will be used.

---

### Commands

- To **run in the terminal or generate an ASCII mp4**:

```bash
   make
```

**Note**: The video will be automatically resized to fit your terminal window, so don't use a very small window.

- To **play the video in the terminal** after running `make` (without any additional processing):

```bash
   make play
```

---

### Outputs

1. If you choose **Mode 1**, the video will be displayed directly in the terminal.
2. If you choose **Mode 2**, the rendered ASCII video will be saved as `.mp4` in the `output` folder.

Additionally, all the individual ASCII frames of the video will be saved as `.txt` files in the `output` folder.

---

## License

This project is licensed under the MIT license.

---

## Contributions

Contributions are very welcome! Open an issue or send a pull request for any improvement or suggestion. 😄