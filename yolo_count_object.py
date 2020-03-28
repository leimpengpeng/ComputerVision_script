# -*- coding: utf-8 -*-
from darkflow.net.build import TFNet
import sys
import dlib
import cv2
from collections import Counter
options = {"model": "cfg/tiny-yolo-voc.cfg", "load": "bin/tiny-yolo-voc.weights", "threshold": 0.7, "saveVideo":""}

tfnet = TFNet(options)
objectTrackers = {}
objectNames = {}
frameCounter = 0
currentObjectID = 0
tracker = dlib.correlation_tracker()
cap = cv2.VideoCapture("Video.MOV")
f_width = 320
f_height = 240
out = cv2.VideoWriter('output.avi',cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'), 10.0, (f_width,f_height))

found_object = False
if __name__ == '__main__':
    cv2.namedWindow("image", cv2.WINDOW_AUTOSIZE)

    while(1):
        ret, frame = cap.read()

        if not ret:
            break
        reframe = cv2.resize( frame, ( f_width, f_height))
        for fid in objectTrackers.keys():
            trackingQuality = objectTrackers[ fid ].update( reframe )

        objects_inframe = tfnet.return_predict(reframe)

        for object_inframe in objects_inframe:
                x0 = object_inframe['topleft']['x']
                y0 = object_inframe['topleft']['y']
                x1 = object_inframe['bottomright']['x']
                y1 = object_inframe['bottomright']['y']
                w =  object_inframe['bottomright']['x'] - object_inframe['topleft']['x']
                h =  object_inframe['bottomright']['y'] -object_inframe['topleft']['y']
                label = object_inframe['label']
                #calculate the centerpoint
                x_bar = x0 + (x1 - x0) * 0.5
                y_bar = y0 + (y1 - y0) * 0.5
                matchedFid = None

                #if label in 'person':
###################################Check object Exist ###################################################
                for fid in objectTrackers.keys():
                    tracked_position =  objectTrackers[fid].get_position()

                    t_x = int(tracked_position.left())
                    t_y = int(tracked_position.top())
                    t_w = int(tracked_position.width())
                    t_h = int(tracked_position.height())


                    #calculate the centerpoint
                    t_x_bar = t_x + 0.5 * t_w
                    t_y_bar = t_y + 0.5 * t_h

                    #check if the centerpoint of the object is within the
                    #rectangleof a tracker region. Also, the centerpoint
                    #of the tracker region must be within the region
                    #detected as a object. If both of these conditions hold
                    #we have a match
                    if ( ( t_x <= x_bar   <= (t_x + t_w)) and
                         ( t_y <= y_bar   <= (t_y + t_h)) and
                         ( x0   <= t_x_bar <= (x0   + w  )) and
                         ( y0   <= t_y_bar <= (y0   + h  ))):
                         matchedFid = fid


################################### Add new object  ###################################################
                if matchedFid is None:
                    print("Creating new tracker " + str(currentObjectID))

                    #Create and store the tracker
                    tracker = dlib.correlation_tracker()
                    tracker.start_track(reframe, dlib.rectangle(x0,y0, x1, y1))

                    objectTrackers[ currentObjectID ] = tracker
                    objectNames[ currentObjectID ] = label

                    cv2.rectangle(reframe, (x0,y0), (x1, y1), (0, 255, 0), 3, 1)
                    roi = reframe[y0:y0+h, x0:x0+w]
                    objsave_name = label + '_' + str(currentObjectID) + '.jpg'
                    cv2.imwrite(objsave_name,roi);
                    #Increase the currentObjectID counter
                    currentObjectID += 1
################################### Objects Tracking  ###################################################
        for fid in objectTrackers.keys():
            tracked_position =  objectTrackers[fid].get_position()

            t_x = int(tracked_position.left())
            t_y = int(tracked_position.top())
            t_w = int(tracked_position.width())
            t_h = int(tracked_position.height())
            rectangleColor = (0,165,255)
            cv2.rectangle(reframe, (t_x, t_y),(t_x + t_w , t_y + t_h),rectangleColor ,2)


            if fid in objectNames.keys():

                object_index =str(fid) + '.'+objectNames[fid]
                cv2.putText(reframe, object_index ,
                            (int(t_x + t_w/2), int(t_y)),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            0.5, (255, 255, 255), 2)
            else:
                cv2.putText(reframe, "Detecting..." ,
                            (int(t_x + t_w/2), int(t_y)),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            0.5, (255, 255, 255), 2)

        count_list= list(objectNames.values())
        print(Counter(count_list))
        cv2.putText(reframe, str(Counter(count_list)),(10, 10),cv2.FONT_HERSHEY_SIMPLEX,
                    0.5, (255, 255, 255), 2)


        out.write(reframe)


        cv2.imshow("image", reframe)

        if cv2.waitKey(10) == 27:
            break

    cap.release()
    cv2.destroyAllWindows()
