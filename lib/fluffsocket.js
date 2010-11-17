/*
  
  Fluff.js
  Copyright (c) 2010 Ivo Wetzel.
  
  All rights reserved.
  
  Fluff.js is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Fluff.js is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  Fluff.js. If not, see <http://www.gnu.org/licenses/>.
  
*/


// Fluff.js Simple Sockets -----------------------------------------------------
// -----------------------------------------------------------------------------
function FluffSocket() {
    this.socket = new fluff.network.Socket();
    this.connected = false;
    this.dataBuffer = '';
    this.dataSize = -1;
    this.init();
};

fluff.network.FluffSocket = FluffSocket;


// Prototype -------------------------------------------------------------------
FluffSocket.prototype = {
    onMessage: function(msg) {},
    onConnect: function() {},
    onClose: function() {},
    onError: function() {},
    
    init: function() {
        var that = this;
        this.socket.onConnect = function() {
            that.socket.send('GET / HTTP/1.1\r\n'
                             + 'Connection: Upgrade\r\n'
                             + 'Upgrade: FluffSocket\r\n\r\n'); 
            
            that.connected = true;
            that.onConnect();
        };
        
        this.socket.onData = function(data) {that.read(data);};
        this.socket.onError = function() {that.onError();};
        this.socket.onClose = function() {that.close();};
    },
    
    connect: function(host, port) {
        this.socket.connect(host, port);        
    },
    
    send: function(msg) {
        var size = msg.length;
        this.socket.send(String.fromCharCode((size >> 16) & 0xffff)
                    + String.fromCharCode(size & 0xffff) + msg);
    },
    
    read: function(data) {
        if (!this.connected) {
            return false;
        }
        this.dataBuffer += data;
        
        var more = true;
        while (more) {
            more = false;
            var len = this.dataBuffer.length;
            if (this.dataSize === -1) {
                if (len >= 2) {
                    this.dataSize = (this.dataBuffer.charCodeAt(0) << 16)
                                     + this.dataBuffer.charCodeAt(1);
                    
                    this.dataBuffer = this.dataBuffer.substr(2);
                }
            }
            if (this.dataSize !== -1 && len >= this.dataSize) {
                this.message(this.dataBuffer.substr(0, this.dataSize));
                this.dataBuffer = this.dataBuffer.substr(this.dataSize);
                this.dataSize = -1;
                more = true;
            }
        }
    },
    
    message: function(msg) {
        this.onMessage(msg);
    },
    
    close: function() {
        this.dataBuffer = '';
        this.dataSize = -1;
        if (this.connected) {
            this.onClose();
        }
        this.connected = false;
        this.socket.close();
    }
};

