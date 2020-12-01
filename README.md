# `broadcast-interfaces`

A simple native Node module to list the IPv4 broadcast addresses of all network
interfaces.

```javascript
> const broadcastInterfaces = require('broadcast-interfaces');
> broadcastInterfaces();
[
  {
    name: 'wlp2s0',
    running: true,
    internal: false,
    address: '192.168.1.34',
    broadcast: '192.168.1.255'
  },
  {
    name: 'docker0',
    running: false,
    internal: false,
    address: '172.17.0.1',
    broadcast: '172.17.255.255'
  },
  {
    name: 'br-aad22280aa11',
    running: false,
    internal: false,
    address: '172.18.0.1',
    broadcast: '172.18.255.255'
  }
]
```

## What about `os.networkInterfaces()`?

Node's built-in `os.networkInterfaces()` function is missing some fields, like
[the broadcast address](https://github.com/nodejs/node/issues/23437) and whether
the interface is currently running. This module is an alternative that lists
these missing fields.

Only IPv4 network interfaces with a broadcast address are included.

## License

Copyright &copy; 2020 Adam Nelson

`broadcast-interfaces` is distributed under the [Blue Oak Model
License][blue-oak]. It is a MIT/BSD-style license, but with [some clarifying
improvements][why-blue-oak] around patents, attribution, and multiple
contributors.

[blue-oak]: https://blueoakcouncil.org/license/1.0.0
[why-blue-oak]: https://writing.kemitchell.com/2019/03/09/Deprecation-Notice.html


