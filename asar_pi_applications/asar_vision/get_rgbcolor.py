
import cv2
import numpy as np
from math import sqrt
import json
from scipy.spatial import distance as dist

def get_rgbcolor(average_hsv):
    distances = []
    red = "red"
    blue = "blu"
    green = "grn"
    gray = "gry"
    purple = "prp"
    yellow = "yel"
    orange = "org"
    pink = "pnk"
    black = "blk"
    colors = [red, blue, green, gray, orange, purple, pink]
    parameters = json.load(open('parameters.txt'))
    if len(parameters['value'][0]) < 10:
        preset_colors = ([174, 224],
                         [102, 148],
                         [36, 135],
                         [13, 46],
                         [13, 160],
                         [148.5, 117],
                         [158, 252])
        for i in range(0, len(preset_colors)):
            # de = dist.euclidean(average_hsv[0][0], preset_colors[i])
            d = sqrt((average_hsv[0][0][0] - preset_colors[i][0]) ** 2 +
                        (average_hsv[0][0][1] - preset_colors[i][1]) ** 2)
            distances += [d]
        index_min = np.argmin(distances)
        color = colors[index_min]
        return color

