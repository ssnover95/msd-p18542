import cv2
import numpy as np


def mouse_handler(event, x, y, flags, data):
    if event == cv2.EVENT_LBUTTONDOWN:
        cv2.imshow("Image", data['im'])
        cv2.circle(data['im'], (x, y), 3, (0, 0, 255), 5, 16)
        if len(data['points']) <= 6:
            data['points'].append([y, x])


def get_four_points(im):
    # Set up data to send to mouse handler
    data = {}
    data['im'] = im.copy()
    data['points'] = []
    data['hsv'] = []

    # Set the callback function for any mouse event
    cv2.imshow("Image", im)
    cv2.setMouseCallback("Image", mouse_handler, data)

    cv2.waitKey(0)
    # Convert array to np.array
    points = np.vstack(data['points']).astype(float)

    return points
