from __future__ import print_function
import pyzbar.pyzbar as pyzbar
import numpy as np
import cv2
import os
import time
video_path = ''
output_dir = ''
cap = cv2.VideoCapture(0)
output_frames = [ os.path.join(output_dir,'im_proc')]
if not os.path.exists(os.path.join(output_dir,'im_proc')):
  os.makedirs(os.path.join(output_dir,'im_proc'))



def decode(im) :
  # Find barcodes and QR codes
  decodedObjects = pyzbar.decode(im)

  # Print results
  for obj in decodedObjects:
    print('Type : ', obj.type)
    print('Data : ', obj.data,'\n')

  return decodedObjects


# Display barcode and QR code location
def display(im, decodedObjects):

  # Loop over all decoded objects
  im_barcode = im
  for decodedObject in decodedObjects:
    points = decodedObject.polygon

    # If the points do not form a quad, find convex hull
    if len(points) > 4 :
      hull = cv2.convexHull(np.array([point for point in points], dtype=np.float32))
      hull = list(map(tuple, np.squeeze(hull)))
    else :
      hull = points;

    # Number of points in the convex hull
    n = len(hull)

    # Draw the convext hull
    for j in range(0,n):
      im_barcode = cv2.line(im, hull[j], hull[ (j+1) % n], (255,0,0), 3)


  # Display results

  cv2.imshow("Results", cv2.resize(im_barcode,(800,600)));

  cv2.waitKey(3);
  return im_barcode


# Main
if __name__ == '__main__':

  # Read image
  index = 0
  while(cap.isOpened()):
      ret, im = cap.read()
      if ret==True:



          decodedObjects = decode(im)
          im_barcode = display(im, decodedObjects)
          cv2.imshow("Results",im);

          cv2.imwrite(os.path.join(output_dir,'im_proc','im_proc{}.jpg'.format(index)),im_barcode)
          index = index + 1
