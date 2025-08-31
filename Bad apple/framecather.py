import cv2

video = cv2.VideoCapture("badapple.mp4")
success, frame = video.read()
count = 0

while success:
    cv2.imwrite(f"frames/frame_{count:04d}.jpg", frame)
    success, frame = video.read()
    count += 1

video.release()
