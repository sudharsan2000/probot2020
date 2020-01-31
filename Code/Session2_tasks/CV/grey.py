import cv2

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()

    grey = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    
    cv2.imshow('grey image',grey)
    cv2.imshow('Original',frame)

    k = cv2.waitKey(5)
    if k == 27:
        break

