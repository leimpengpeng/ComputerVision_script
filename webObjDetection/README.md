# webObjDetection
  Web application that allow to upload video for tensorflow model object detection processing. 
# Software requirement : 
	1. Python
  2. Tensorflow
  3. Flask
# Step :
	1. Make sure tensorflow object detection applcation is working on system
    $ cd ~/tensorflow
    $ git clone https://github.com/tensorflow/models.git
    $ sudo apt-get install protobuf-compiler python-pil python-lxml python-tk
    $ pip3 install --user Cython
    $ pip3 install --user contextlib2
    $ pip3 install --user pillow
    $ pip3 install --user lxml
    $ pip3 install --user jupyter
    $ pip3 install --user matplotlib
    $ cd ~/tensorflow/models/research
    $ protoc object_detection/protos/*.proto --python_out=. 
    $ export PYTHONPATH=$PYTHONPATH:`pwd`:`pwd`/slim
    $ cd object_detection
  2. Place 'object_detection_webcam.py' in ~/tensorflow/models/research/objectdetection directory 
    $ python3 object_detection_webcam.py
  3. Step to execute the tensorflow object detection with REST api 
    a. Object_detection as parent directory : 
	* Create  folder named 'static' and place 'tf.js' into the folder
  * Create  folder named 'templates' and place 'index_tf.html' into the folder
  * Place the following file  in object_detection folder
	* To execute : 
    $ python web_tf.py 
  * Please add the 'sample video' inside video directory under object_detection parent directory.  
	* curl -d 'video_file='video/video-1539232658.mp4'&target_name='mouse'' 'http://127.0.0.1:4560/tf_detection'




    


  

