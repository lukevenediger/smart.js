print('\r\nThis is Smart.JS ' + Sys.ro_vars.fw_version + ' (' + Sys.ro_vars.arch + ')\r\n');
print('Below you should see a JS prompt, feel free to play with it.');
print('sendRandomData() will send some random data to the cloud.');

function sendRandomData() {
  function send() {
    var value  = 20 + Math.random() * 20;  // Simulate sensor data
    clubby.call('//api.cesanta.com',
                {cmd: '/v1/Metrics.Publish',
                    args: {vars: [[{__name__: 'value'}, value]]}},
                function(res) {
                    print("demoSendRandomData:", res);
                    setTimeout(send_on_ready, 2000);  // Call us in 2 seconds
                })
  }

  function send_on_ready() {
    clubby.ready(send);
  }

  send_on_ready();
}
