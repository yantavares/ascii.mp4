import cv2
import os


def create_video_from_frames(frame_dir, output_video_path, fps=24):

    frame_files = [os.path.join(frame_dir, f) for f in sorted(
        os.listdir(frame_dir)) if f.endswith('.png')]

    frame = cv2.imread(frame_files[0])
    height, width, layers = frame.shape

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video = cv2.VideoWriter(output_video_path, fourcc, fps, (width, height))

    for file in frame_files:
        frame = cv2.imread(file)
        video.write(frame)

    video.release()


frame_dir = 'output/frames'

output_video_path = 'output/text.mp4'
fps = 24

create_video_from_frames(frame_dir, output_video_path, fps)
