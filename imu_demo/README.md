# IMU Demo

An example of using the _QMI8658C 6D MEMS IMU_ on the Wemos S3 Mini Pro.

Outputs the current values of the accelerometer, gyroscope, and temperature sensors to the serial port. Also changes the color of the onboard RGB LED based on the orientation of the board (spin it around the USB connector axis to see the colors change).

## Issues with `Qmi8658c`

Many example used the `Qmi8658c` library, however for me on the v1.1.0 board it would never update unless I reopened the device each time. The `lewisxhe/SensorLib` library worked fine for me as per this example.