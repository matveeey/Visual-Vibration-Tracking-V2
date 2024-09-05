# Vibration detection using OpenCV
The app analyzes vibration on a video and computes frequency and ampltiude.

---
## User input
Click left mouse button for starting tracking chosen point on a video.
Hold right mouse button and drag the mouse to create a ROI for tracking multiple good features (points).

---
## Structure
The project includes modules for:
1. Pin hole camera calibration
2. Distortion removement
3. The main module with Lucas-Kanade tracking

## Documentation
You can check the docs [here](https://matveeey.github.io/Vibration-detection-using-OpenCV/index.html)

## Further development
Probably I would integrate an implementation of Motion Amplifying (or [Motion Magnification](https://github.com/tschnz/Live-Video-Magnification)) technic that I tried in another project.
Here is the result of that algorithm:
![motionmagnification](https://user-images.githubusercontent.com/79339489/179936553-2fefc33e-8f34-4ef4-a9b6-9beea70c7f26.gif)