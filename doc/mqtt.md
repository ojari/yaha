# Mqtt specific documentation

There are several devices in mqtt bus. Topic structure depends on the device. Below is a table of devices and their topic structure.

| Device      | Topic                         |
|-------------|-------------------------------|
| zigbee2mqtt | zigbee2mqtt/<device_name>/set |
| shelly      | <device_name>/rpc             |

SW must support all these devices.

## Shelly payload
Shelly devices use JSON payload. Below is an example of payload to turn on a shelly device.
 - There are 3 switches in Shelly, use id to select switch.

```json
{
    "id": 1,
    "src": "mytopic",
    "method": "Switch.Set",
    "params": {
        "id": 1,
        "on": true
    }
}
```
## Zigbee2mqtt payload
Zigbee2mqtt devices use JSON payload. Below is an example of payload to turn on a zigbee2mqtt device.

```json
{
    "state": "ON"
}
```