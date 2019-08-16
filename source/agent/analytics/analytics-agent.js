'use strict';

const log = require('../logger').logger.getLogger('AnalyticsAgent');
const BaseAgent = require('./base-agent');
//const VideoAnalyzer = require('../videoAnalyzer_sw/build/Release/videoAnalyzer-sw');

const VideoPipeline = require('../videoGstPipeline/build/Release/videoAnalyzer-pipeline');

const MediaFrameMulticaster = require('../mediaFrameMulticaster/build/Release/mediaFrameMulticaster');
const EventEmitter = require('events').EventEmitter;
const { getVideoParameterForAddon } = require('../mediaUtil');

var portInfo = 0; 
var count = 0;

class AddonEngine {
  constructor() {
    var config = {
      'hardware': false,
      'simulcast': false,
      'crop': false,
      'gaccplugin': false,
      'MFE_timeout': 0
    };
    // this.pipeline = new VideoPipeline(config);
  }

  setInput(inputId, codec, inConnection) {
    log.debug('addon setInput', inputId, codec);
    //this.engine.setInput(inputId, codec, inConnection);
  }

  unsetInput(inputId) {
    log.debug('addon unsetInput', inputId);
    //this.engine.unsetInput(inputId);
  }

  // dispatcher is MediaFrameMulticaster(a output streame)
  addOutput(outputId, codec, resolution, framerate, bitrate, kfi,
      algo, pluginName, dispatcher) {
    log.debug('addon addOutput', outputId, codec, resolution, framerate, bitrate, kfi);
    //this.engine.addOutput(outputId, codec, resolution, framerate, bitrate, kfi, algo, pluginName, dispatcher);
  }

  removeOutput(outputId) {
    log.debug('addon removeOutput', outputId);
    //this.engine.removeOutput(outputId);
  }

  close() {
    log.debug('addon close');
    //this.engine.close();
  }
}

class AnalyticsAgent  {
  constructor(config) {
   // super('analytics', config);
    this.algorithms = config.algorithms;
    this.onStatus = config.onStatus;
    this.onStreamGenerated = config.onStreamGenerated;
    this.onStreamDestroyed = config.onStreamDestroyed;

    this.agentId = config.agentId;
    this.rpcId = config.rpcId;
    this.outputs = {};

    var conf = {
      'hardware': false,
      'simulcast': false,
      'crop': false,
      'gaccplugin': false,
      'MFE_timeout': 0
    };
    this.engine = new VideoPipeline(conf);

    this.flag = 0;
   
    //this.pipeline = new VideoPipeline.Pipeline();
  }

// override
createInternalConnection(connectionId, direction, internalOpt) {
    internalOpt.minport = global.config.internal.minport;
    internalOpt.maxport = global.config.internal.maxport;
    log.info('================direction:', direction);
    if(direction == 'in'){
      this.engine.emit_ListenTo(internalOpt.minport,internalOpt.maxport);
      portInfo = this.engine.getListeningPort();
      log.info('portinfo:', portInfo);
    }
    
    // Create internal connection always success
    return Promise.resolve({ip: global.config.internal.ip_address, port: portInfo});
  }

  // override
  publish(connectionId, connectionType, options) {
    log.debug('publish:', connectionId, connectionType, options);
    if (connectionType !== 'analytics') {
      // call BaseAgent's publish
      // return super.publish(connectionId, connectionType, options);
       // this.engine.emit_ConnectTo(options.port);
      return Promise.resolve("ok");
    }
    // should not be reached
    return Promise.reject('no analytics publish');
  }

  // override
  unpublish(connectionId) {
    log.debug('unpublish:', connectionId);
    return Promise.resolve();
    // call BaseAgent's unpublish
    // return super.unpublish(connectionId);
  }

  // override
  subscribe(connectionId, connectionType, options) { 
    log.debug('subscribe:', connectionId, connectionType, JSON.stringify(options));
    if (connectionType !== 'analytics') {
       // return super.subscribe(connectionId, connectionType, options);
       //this.engine.stopLoop();
      if(!this.outputs[connectionId]) {
       this.outputs[connectionId] = count;
       log.debug('====subscribe:', connectionId, count, this.outputs[connectionId]);
       this.engine.addElement();
       this.engine.emit_ConnectTo(count,options.port);
       count++;
       // this.engine.setPlaying();
      }
      return Promise.resolve("ok");
    }

      this.engine.createPipeline();
      const videoFormat = options.connection.video.format;
      const videoParameters = options.connection.video.parameters;
      // this.inputs[connectionId].engine = engine;
      // notify ready, and options.controller is conference.js
      const algo = options.connection.algorithm;
      const status = {type: 'ready', info: {algorithm: algo}};
      this.onStatus(options.controller, connectionId, 'out', status);

      const newStreamId = Math.random() * 1000000000000000000 + '';
      log.info('new stream added', newStreamId);
      const streamInfo = {
          type: 'analytics',
          media: {video: Object.assign({}, videoFormat, videoParameters)},
          analyticsId: connectionId,
          locality: {agent:this.agentId, node:this.rpcId},
        };
      log.info('agent:',this.agentId,'node:',this.rpcId);

      const pluginName = this.algorithms[algo].name;
      let codec = videoFormat.codec;
            if (videoFormat.profile) {
              codec += '_' + videoFormat.profile;
            }
      codec = codec.toLowerCase();
      const {resolution, framerate, keyFrameInterval, bitrate}
              = getVideoParameterForAddon(options.connection.video);

      log.info('resolution:',resolution,'framerate:',framerate,'keyFrameInterval:',
               keyFrameInterval, 'bitrate:',bitrate);
      
      this.engine.setOutputParam(codec,resolution,framerate,bitrate,keyFrameInterval,algo,pluginName);

      streamInfo.media.video.bitrate = bitrate;
            this.onStreamGenerated(options.controller, newStreamId, streamInfo);

      this.engine.addElementMany();
      return Promise.resolve();
  }

  // override
  unsubscribe(connectionId) {
    log.debug('unsubscribe:', connectionId);
    return Promise.resolve();
    // if (this.inputs[connectionId]) {
    //   // destroy generated stream if it has
    //   this.inputs[connectionId].close();
    //   return this.cutoff(connectionId);
    // }
    // return super.unsubscribe(connectionId);
  }

  // override
  linkup(connectionId, audioFrom, videoFrom) {
    log.debug('linkup:', connectionId, audioFrom, videoFrom);
    this.engine.setPlaying();

    return Promise.resolve();
    // var iConn;
    // if (this.inputs[connectionId]) {
    //   // link inputs
    //   iConn = this.connections.getConnection(videoFrom);
    //   if (iConn && iConn.direction === 'in' && !iConn.videoTo) {
    //     this.inputs[connectionId].videoFrom = videoFrom;
    //     iConn.videoTo = connectionId;
    //     let codec = this.inputs[connectionId].inputFormat.codec;
    //     codec = codec.toLowerCase();
    //     this.inputs[connectionId].engine.setInput(videoFrom, codec, iConn.connection);
    //     return Promise.resolve();
    //   }
    // }
    // if (this.outputs[videoFrom]) {
    //   // link outputs
    //   iConn = this.connections.getConnection(connectionId);
    //   if (iConn && iConn.direction === 'out' && !iConn.videoFrom && this.outputs[videoFrom]) {
    //     this.outputs[videoFrom].addDestination('video', iConn.connection.receiver());
    //     iConn.videoFrom = videoFrom;
    //     return Promise.resolve();
    //   }
    // }

    // return Promise.reject('linkup failed');
  }

  // override
  cutoff(connectionId) {
    log.debug('cutoff:', connectionId);
    return Promise.resolve();
    // var iConn = this.connections.getConnection(connectionId);
    // var engine;
    // if (!iConn && this.inputs[connectionId]) {
    //   connectionId = this.inputs[connectionId].videoFrom
    //   iConn = this.connections.getConnection(connectionId);
    // } else if (!iConn && this.outputs[connectionId]) {
    //   this.connections.getIds().forEach((connId) => {
    //     iConn = this.connections.getConnection(connId);
    //     if (iConn.videoFrom === connectionId) {
    //       delete iConn.videoFrom;
    //       this.outputs[connectionId].removeDestination('video', iConn.connection.receiver());
    //     }
    //   });
    //   return Promise.resolve();
    // }

    // if (iConn) {
    //   // cutoff internal connection
    //   if (iConn.direction === 'in') {
    //     // cutoff inputs
    //     if (this.inputs[iConn.videoTo].output) {
    //       // remove generated output first
    //       let generateId = this.inputs[iConn.videoTo].output;
    //       engine = this.inputs[iConn.videoTo].engine;
    //       this.cutoff(generateId).catch((e) => log.error(e));
    //     }
    //     delete this.inputs[iConn.videoTo].videoFrom;
    //     delete iConn.videoTo;
    //     engine.unsetInput(connectionId);
    //     return Promise.resolve();
    //   } else {
    //     // cutoff outputs
    //     if (iConn.videoFrom && this.outputs[iConn.videoFrom]) {
    //       delete iConn.videoFrom;
    //       this.outputs[iConn.videoFrom].removeDestination('video', iConn.connection.receiver());
    //       return Promise.resolve();
    //     }
    //   }
    // }
    // return Promise.reject('cutoff failed');
  }

  cleanup() {
    log.debug('cleanup');
    // var connectionId;
    // var cuts = [];
    // var output;
    // // remove all the inputs, outputs will also be removed
    // for (connectionId in this.inputs) {
    //   this.cutoff(connectionId).catch((e) => log.error(e));
    //   output = this.inputs[connectionId].output;
    //   this.inputs[connectionId].engine.removeOutput(output);
    //   this.inputs[connectionId].engine.close();
    //   this.inputs[connectionId].close();
    // }
    return Promise.resolve();
  }
}

module.exports = AnalyticsAgent;
