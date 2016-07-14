# hand_tracking
handtracking project for music group

#demo
https://www.youtube.com/watch?v=XNB4vRzdzkU

#requirements
opencv 2.4.13
Qt creator 5.5

#background
this program tracks a hand from the default webcam, counting the fingers and provides simple gesture recognition (open hand and peace sign)

#algorithm
the program converts the image to the YCrCb colour space to improve skin tracking.
the user places their hand over the on-screen square, the average of the pixel values is calculated alogn with the standard deviation.
These values are used to threshold the image.

The contours of the image are calculated with the largest contour assuemd to be the hand. the convex hull is taken and
The centre of the hand is calculate.
the points that lie on the hull and the contour and are suffciently far from the centre are assumed to be finger tips.

gesture recognition is simply done by counting the available fingers, with improvments to more complex gestures to follow

#limitations
noise free background (white)
one hand only
due to calibration method, the program may work in a noisy background if a glove is worn

