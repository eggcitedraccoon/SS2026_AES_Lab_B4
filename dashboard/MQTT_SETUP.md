# MQTT Broker Setup for Device Discovery

To ensure the dashboard can discover and communicate with the watering stations, the following MQTT topic structure is used:

## Topic Structure
- **Responses (from Station to Dashboard)**: `/<device-id>/response`
- **Requests (from Dashboard to Station)**: `/<device-id>/request`

## Device IDs
The dashboard specifically looks for devices with IDs starting with `WS-` (e.g., `WS-1`, `WS-100`).

## Message Format
### Responses (`/<device-id>/response`)
The station should publish a **valid** JSON object to this topic. Ensure keys and string values are enclosed in **double quotes**.

**Correct Format:**
```json
{
  "moisture": 45,
  "water_low": false
}
```

**Incorrect Format (will fail or cause warnings):**
```
{moisture: 45, water_low: false}
```

- `moisture`: Integer/Float representing soil moisture percentage.
- `water_low`: Boolean (`true` if water level is low/empty, `false` if OK).

### Requests (`/<device-id>/request`)
The dashboard sends simple string commands.
- `WATER`: Trigger watering.

## Broker Configuration
The dashboard uses dynamic discovery by subscribing to `+/response`.
- Ensure your broker allows wildcards.
- It is recommended that the stations publish their status with the **retain** flag set to `true`. This allows the dashboard to see the latest state of all stations immediately upon connection, even if the stations only publish updates occasionally.
