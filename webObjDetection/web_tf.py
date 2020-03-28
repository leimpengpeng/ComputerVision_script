

import os

from flask import Flask, render_template, Response, jsonify, request
from camera import VideoCamera
import obj_detection
import cv2
import tensorflow as tf
import string
app = Flask(__name__)
app.debug = True
IMAGE_SIZE = 320
model_name ='ssd_inception_v2_coco_2017_11_17'
net = obj_detection.Net(graph_fp='%s/frozen_inference_graph.pb' % model_name,
                           labels_fp='data/label.pbtxt',
                           num_classes=90,
                           threshold=0.6)

video_camera = None
global_frame = None
@app.route("/", methods=['GET', 'POST'])
def index():
    if request.method == "POST":
        name = request.form["name"]
        print (name + " Hello")
        return name + " Hello"
    return render_template("i.html")
def demo(avi_name):
    cap = cv2.VideoCapture(avi_name)
    print ('test 112 true')
    while True:
        with tf.device('/gpu:0'):
            ret, frame = cap.read()
            in_progress = net.get_status()
            if ret and (not in_progress):
                resize_frame = cv2.resize(frame, (IMAGE_SIZE, IMAGE_SIZE))
                result_json = []
                net.predict(img=resize_frame, display_img=frame, filtered_results=result_json)
                #cv2.imshow('img_frame', frame)
                ret,jpeg = cv2.imencode('.jpg', frame)
                yield (b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + jpeg.tobytes() + b'\r\n\r\n')
                
            else:
                print('[Warning] drop frame or in progress')
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    cap.release()
    cv2.destroyAllWindows()
    
@app.route('/tf_detection', methods=['GET', 'POST'])
def tf_detection():
    #video_name = request.form['name']
    #cap = cv2.VideoCapture('video/DJI_0002.MOV')
    target_object = " false"
    if request.method == "POST":
        video_file = request.form["video_file"]
        target_name = request.form["target_name"]
        video_name = string.replace(video_file,'C:\\fakepath\\', 'video/')
        
        #video_name = 'video/DJI_0002.MOV'
        cap = cv2.VideoCapture(video_name)
        x = 0 

        while True:
            #print ('test  true123')
            #print ('video_file' + video_file)
            with tf.device('/gpu:0'):
                ret, frame = cap.read()
                in_progress = net.get_status()
                if ret and (not in_progress):
                    resize_frame = cv2.resize(frame, (IMAGE_SIZE, IMAGE_SIZE))
                    result_json = []
                    net.predict(img=resize_frame, display_img=frame,target_object=target_name, predicted_results=result_json)
                    filename = str(x) + '.jpg'
                    
                    #cv2.imshow('img_frame', frame)
                    cv2.imwrite(filename,frame)
                    x = x + 1
                else:
                    print('[Warning] drop frame or in progress')
                    break
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
        cap.release()
        cv2.destroyAllWindows()    
                 


    return jsonify(result="Video Processing done")



if __name__ == "__main__":
    app.run(port=4560, debug=True)