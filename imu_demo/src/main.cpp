#include <Arduino.h>
#include "SensorQMI8658.hpp"

#define QMI_ADDRESS 0x6B             // Define the QMI8658C I2C address
#define QMI8658C_I2C_FREQUENCY 80000 // Define the I2C frequency

#define ORIENTATION_THRESHOLD 0.6 // threshold for orientation detection

SensorQMI8658 qmi;

IMUdata acc;
IMUdata gyr;

enum Orientation
{
  UNKNOWN,
  RIGHTSIDE_UP,
  UPSIDE_DOWN,
  ON_SIDE
};

Orientation determineOrientation(float x, float y, float z)
{
  // Check primary orientations using z-axis
  if (z > ORIENTATION_THRESHOLD)
  {
    return RIGHTSIDE_UP;
  }
  else if (z < -ORIENTATION_THRESHOLD)
  {
    return UPSIDE_DOWN;
  }
  else if (abs(x) > ORIENTATION_THRESHOLD || abs(y) > ORIENTATION_THRESHOLD)
  {
    return ON_SIDE;
  }

  return UNKNOWN;
}

void setup()
{
  Serial.begin(460800);

#ifdef RGB_POWER
  // Turn on power the RGB LED
  pinMode(RGB_POWER, OUTPUT);
  digitalWrite(RGB_POWER, HIGH);
  rgbLedWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, RGB_BRIGHTNESS); // Red
  delay(3000);
#endif

  bool ret = false;
  ret = qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SDA, SCL);

  if (!ret)
  {
    Serial.println("Failed to find QMI8658 - check your wiring!");
    while (1)
    {
      delay(1000);
    }
  }

  /* Get chip id*/
  Serial.print("Device ID:");
  Serial.println(qmi.getChipID(), HEX);

  if (qmi.selfTestAccel())
  {
    Serial.println("Accelerometer self-test successful");
  }
  else
  {
    Serial.println("Accelerometer self-test failed!");
  }

  if (qmi.selfTestGyro())
  {
    Serial.println("Gyroscope self-test successful");
  }
  else
  {
    Serial.println("Gyroscope self-test failed!");
  }

  qmi.configAccelerometer(
      /*
       * ACC_RANGE_2G
       * ACC_RANGE_4G
       * ACC_RANGE_8G
       * ACC_RANGE_16G
       * */
      SensorQMI8658::ACC_RANGE_4G,
      /*
       * ACC_ODR_1000H
       * ACC_ODR_500Hz
       * ACC_ODR_250Hz
       * ACC_ODR_125Hz
       * ACC_ODR_62_5Hz
       * ACC_ODR_31_25Hz
       * ACC_ODR_LOWPOWER_128Hz
       * ACC_ODR_LOWPOWER_21Hz
       * ACC_ODR_LOWPOWER_11Hz
       * ACC_ODR_LOWPOWER_3H
       * */
      SensorQMI8658::ACC_ODR_250Hz,
      /*
       *  LPF_MODE_0     //2.66% of ODR
       *  LPF_MODE_1     //3.63% of ODR
       *  LPF_MODE_2     //5.39% of ODR
       *  LPF_MODE_3     //13.37% of ODR
       *  LPF_OFF        // OFF Low-Pass Fitter
       * */
      SensorQMI8658::LPF_MODE_2);

  qmi.configGyroscope(
      /*
       * GYR_RANGE_16DPS
       * GYR_RANGE_32DPS
       * GYR_RANGE_64DPS
       * GYR_RANGE_128DPS
       * GYR_RANGE_256DPS
       * GYR_RANGE_512DPS
       * GYR_RANGE_1024DPS
       * */
      // SensorQMI8658::GYR_RANGE_64DPS,
      SensorQMI8658::GYR_RANGE_1024DPS,
      /*
       * GYR_ODR_7174_4Hz
       * GYR_ODR_3587_2Hz
       * GYR_ODR_1793_6Hz
       * GYR_ODR_896_8Hz
       * GYR_ODR_448_4Hz
       * GYR_ODR_224_2Hz
       * GYR_ODR_112_1Hz
       * GYR_ODR_56_05Hz
       * GYR_ODR_28_025H
       * */
      // SensorQMI8658::GYR_ODR_896_8Hz,
      SensorQMI8658::GYR_ODR_56_05Hz,
      /*
       *  LPF_MODE_0     //2.66% of ODR
       *  LPF_MODE_1     //3.63% of ODR
       *  LPF_MODE_2     //5.39% of ODR
       *  LPF_MODE_3     //13.37% of ODR
       *  LPF_OFF        // OFF Low-Pass Fitter
       * */
      SensorQMI8658::LPF_MODE_3);

  /*
   * If both the accelerometer and gyroscope sensors are turned on at the same time,
   * the output frequency will be based on the gyroscope output frequency.
   * The example configuration is 896.8HZ output frequency,
   * so the acceleration output frequency is also limited to 896.8HZ
   * */
  qmi.enableGyroscope();
  qmi.enableAccelerometer();

  // Print register configuration information
  qmi.dumpCtrlRegister();

#if IMU_IRQ > 0
  // If you want to enable interrupts, then turn on the interrupt enable
  qmi.enableINT(SensorQMI8658::INTERRUPT_PIN_1, true);
  qmi.enableINT(SensorQMI8658::INTERRUPT_PIN_2, false);
#endif

  Serial.println("Read data now...");
}

void loop()
{
  // When the interrupt pin is passed in through setPin,
  // the GPIO will be read to see if the data is ready.
  if (qmi.getDataReady())
  {
    static Orientation lastOrientation = UNKNOWN;
    // rgbLedWrite(RGB_BUILTIN, 0, 0, 0); // Off
    if (qmi.getAccelerometer(acc.x, acc.y, acc.z))
    {
      Orientation currentOrientation = determineOrientation(acc.x, acc.y, acc.z);

      // Only update if orientation has changed
      if (currentOrientation != lastOrientation)
      {
        switch (currentOrientation)
        {
        case RIGHTSIDE_UP:
          Serial.println("Device is right side up");
          rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // Green
          break;
        case UPSIDE_DOWN:
          Serial.println("Device is upside down");
          rgbLedWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS); // Blue
          break;
        case ON_SIDE:
          Serial.println("Device is on its side");
          rgbLedWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // Red
          break;
        default:
          Serial.println("Orientation unknown");
          // rgbLedWrite(RGB_BUILTIN, 0, 0, 0); // Off
        }
        lastOrientation = currentOrientation;
      }

      // Print accelerometer data
      Serial.printf("ACC(g) x:%.2f y:%.2f z:%.2f\n",
                    acc.x / 9.81, acc.y / 9.81, acc.z / 9.81);
    }

    if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z))
    {
      // Print to serial plotter
      Serial.print("GYRO.x:");
      Serial.print(gyr.x);
      Serial.print(",");
      Serial.print("GYRO.y:");
      Serial.print(gyr.y);
      Serial.print(",");
      Serial.print("GYRO.z:");
      Serial.print(gyr.z);
      Serial.println();
    }

    Serial.print("Temperature:");
    Serial.print(qmi.getTemperature_C());
    Serial.println(" degrees C");
    Serial.println();
  }
  delay(100);
}