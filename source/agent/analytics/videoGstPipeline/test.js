var pipeline = require('./build/Release/videoAnalyzer-pipeline');

var p = new pipeline.Pipeline();

p.createPipeline();
p.emit_ListenTo(8885,8890);
p.emit_ConnectTo(8891);

var port = p.getListeningPort();
console.log(port);

p.play();
