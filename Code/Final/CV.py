import cv2
import numpy as np
import math
#import serial
import time
from time import sleep
import urllib.request 
#ser = serial.Serial("COM9", 19200, timeout=5)
# time.sleep(2) 

def nothing():
    pass



url0 = 'http://192.168.43.52/turnoff'
url1 = 'http://192.168.43.52/turnon'

nothing()
cv2.namedWindow('frame')
cap = cv2.VideoCapture(0)


#Track bar is for user to customize the colour to his surrounding
cv2.createTrackbar('hl','frame',0,255,nothing)
cv2.createTrackbar('sl','frame',0,255,nothing)
cv2.createTrackbar('vl','frame',0,255,nothing)
cv2.createTrackbar('hu','frame',0,255,nothing)
cv2.createTrackbar('su','frame',0,255,nothing)
cv2.createTrackbar('vu','frame',0,255,nothing)

#This is the default value which usually works pretty good for ordinary lighting
#The user can fine tune the adjustment using the trackbar
cv2.setTrackbarPos('hl', 'frame', 0)
cv2.setTrackbarPos('sl', 'frame', 0)
cv2.setTrackbarPos('vl', 'frame',158)
cv2.setTrackbarPos('hu', 'frame', 101)
cv2.setTrackbarPos('su', 'frame', 255)
cv2.setTrackbarPos('vu', 'frame', 255)

#The algorithm detects the right and left translations
#so to implement the tracking of relative motion rather than absolute position
#queue is implemented 
#History acts as a container(queue) which stores the value of the positions for a timeperiod of 2 seconds
history = []


for i in range(50):

    #We initialize the history with average value((200 + 400)/2)
    history.append(300)

while(1):
    try:  
        ret, frame = cap.read()
        frame = cv2.flip(frame, 1)
        kernel = np.ones((3, 3), np.uint8)

        # define "REGION OF INTEREST"
        #The actions are tracked only inside this rectangle
        roi = frame[200:400, 200:400]

        cv2.rectangle(frame, (200, 200), (400, 400), (255, 0, 0), 1)
        hsv = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)

        h1=cv2.getTrackbarPos('hl','frame')
        s1=cv2.getTrackbarPos('sl','frame')
        v1=cv2.getTrackbarPos('vl','frame')
        h2=cv2.getTrackbarPos('hu','frame')
        v2=cv2.getTrackbarPos('vu','frame')
        s2=cv2.getTrackbarPos('su','frame')

    # define range of skin color in HSV
        lower_skin = np.array([h1, v1, s1], dtype=np.uint8)
        upper_skin = np.array([h2, v2, s2], dtype=np.uint8)

    # extract skin colur image
        mask = cv2.inRange(hsv, lower_skin, upper_skin)

    # extrapolate the hand to fill dark spots within 
        mask = cv2.dilate(mask, kernel, iterations=4)

    #blur the image to smoothen the mask 
        mask = cv2.GaussianBlur(mask, (5, 5), 100)
        
    #find contours
        contours, hierarchy = cv2.findContours(
            mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    # find contour of max area(hand)
    # Works fine when hand is closer to the camera
        cnt = max(contours, key=lambda x: cv2.contourArea(x))
        

    # approx the contour a little
        epsilon = 0.0005*cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, epsilon, True)

    # make convex hull around hand
        hull = cv2.convexHull(cnt)
        cv2.drawContours(roi,contours, contours.index(cnt), color = (0, 255, 255), thickness = 2)
        
    # define area of hull and area of hand
        areahull = cv2.contourArea(hull)
        areacnt = cv2.contourArea(cnt)


    # to locate centroid
        M = cv2.moments(hull)
        #x coordinate of the centroid
        cX = int(M["m10"]/M["m00"])
        #y coordinate of the centroid
        cY = int(M["m01"]/M["m00"])

        #save the centroid coordinates in history
        history.append(cX)

        #checks for left translation
        if(history[len(history) - 1] <= 100 and history[len(history) - 1 - 30] >= 150):
            print("open latch")
            urllib.request.urlopen(url1)

        #checks for right translation
        elif(history[len(history) - 1 - 30] <= 100 and history[len(history) - 1] >= 150):
            print("close latch")
            urllib.request.urlopen(url0)

        #marks the centroid
        cv2.circle(roi, (cX, cY), 3, [255, 0, 0], -1)
        history.pop(0)
        cv2.imshow('mask', mask)
        cv2.imshow('op', frame)

        k = cv2.waitKey(5) & 0xFF
        sleep(0.05)

        if(k==27):
            break
    #error handling
    except:
        print('except')
        pass
    


print('closing')
cv2.destroyAllWindows()
cap.release()
