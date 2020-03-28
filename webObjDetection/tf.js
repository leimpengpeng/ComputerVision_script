function do_ajax() {
    var req = new XMLHttpRequest();
    var result = document.getElementById('result');
    req.onreadystatechange = function()
    {
      if(this.readyState == 4 && this.status == 200) {
        result.innerHTML = this.responseText;
      } else {
        result.innerHTML =  "Work in progress...";
      }
    }
    req.open('POST', '/', true);
    req.setRequestHeader('content-type', 'application/x-www-form-urlencoded;charset=UTF-8');
    //req.send("name=" + document.getElementById('name').value);
    
    req.send("name=" + document.getElementById('name').files[0].name);

  }
  function call_tf() {
    var req = new XMLHttpRequest();
    var Predictresult = document.getElementById('predicted_result');
    req.onreadystatechange = function()
    {
      if(this.readyState == 4 && this.status == 200) {
        predicted_result.innerHTML = this.responseText;
      } else {
        predicted_result.innerHTML = "work in progress...";
      }
    }
    var data = "video_file=" + document.getElementById('file').value +
              "&target_name=" + document.getElementById('target').value
    req.open('POST', '/tf_detection', true);
    req.setRequestHeader('content-type', 'application/x-www-form-urlencoded;charset=UTF-8');
    req.send(data);
  }