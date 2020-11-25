/*
 Author: Leslie Nix
 Date Created: 14 November 2020
 Date Updated: 18 November 2020
 Status: Probably Fine
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "IMU.h"
#include "lpfilter.h"
#include "uartManager.h"
#include "customMath.h"

static float calculateHeading(gyroAngle_t *angle, sensor_t *mag);

// *** Convert to NED Convention ***
void boardOrientation(sensorRaw_t *sensorRaw)
{

    sensorRaw_t temp = *sensorRaw;
    sensorRaw->axis.Y = -temp.axis.Y;
    sensorRaw->axis.Z = -temp.axis.Z;

}

// *** Convert to SI Units and Filter ***
// THIS DT PROBABLY NEEDS TO BE EQUIVALENT TO ONE SECOND
void convert_for_cntrl(imuData *data, gyroAngle_t * angle, sensor_t *mag,
                       float dt)
{
    // apply board orientation
    boardOrientation(&data->accel);
    boardOrientation(&data->gyro);


    uint8_t axis = 0;
    for (axis = 0; axis < 3; axis++)
    {
        // subtract zero values
        data->accel.data[axis] -= _CFG.accel.data[axis];
        data->gyro.data[axis] -= _CFG.gyro.data[axis];

        // move scaled gyro data to gyroRate
        data->gyroRate.data[axis] = (float) (data->gyro.data[axis]);

    }


    static const float gyro_cf_factor = 600.0f;
    static const float inv_gyro_cf_factor = (1.0f / (600.0f + 1.0f));
    static low_pass_t accel_lp[3] = {  // cutoff fr in Hz
            { .Fc = 53.05f }, { .Fc = 53.05f }, { .Fc = 53.05f }, };


    sensor_t gyroRAD;  // gyro readings in rad/s
    sensor_t accLP; //accel readings after lp filtering
    float accelMagSq = 0; //accel magnitude squared


    axis = 0;
    for (axis = 0; axis < 3; axis++)
    {
        gyroRAD.data[axis] = data->gyroRate.data[axis] * DEG_TO_RAD;

        accLP.data[axis] = applyLowPass(&accel_lp[axis], data->accel.data[axis],
                                        dt);
        data->accelBody.data[axis] = accLP.data[axis];
        accelMagSq += accLP.data[axis] * accLP.data[axis];

    }


    // find angle by which to rotate body frame
    // mult gyroRAD by dt to convert to seconds
    gyroAngle_t deltaAngle = { .data =
      { gyroRAD.axis.X * dt,      // set deltaAngle.data.roll to gyroRAD.axis.X
        gyroRAD.axis.Y * dt,      // pitch to Y
        gyroRAD.axis.Z * dt } };  // yaw to Z


    rotateV(&(data->accelBody), &deltaAngle); // rotate body frame


    // if <0.85G or >1.15, skip new accel readings
    // apply complimentary filter
    if (0.72f < accelMagSq && accelMagSq < 1.32f)
    {
        axis = 0;
        for (axis = 0; axis < 3; axis++)
        {
            data->accelBody.data[axis] = (data->accelBody.data[axis]
                    * gyro_cf_factor + accLP.data[axis]) * inv_gyro_cf_factor;
        }
    }

    // calculate roll and pitch angles
    // restrict pitch to +- 90 degrees
    angle->axis.roll = atan2f(data->accelBody.axis.Y, (data->accelBody.axis.Z));

    // more numerically stable version eliminates roll singularity when x axis pointed directly upward but honesetly if that happens we have bigger issues than numerical stability; see nxp application note AN3461, eq. 38

    angle->axis.pitch = atan2f(-(data->accelBody.axis.X), sqrtf((data->accelBody.axis.Y * data->accelBody.axis.Y) + (data->accelBody.axis.Z * data->accelBody.axis.Z)));

    // more numerically stable version eliminates roll singularity when x axis pointed directly upward but honesetly if that happens we have bigger issues than numerical stability



    // complimentary filter factors for mag data
    // Leslie don't do that again. Or change to a define statement
    static const float gyro_cf_mag_factor = 250.0f;
    static const float inv_gyro_cf_mag_factor = (1.0f / (250.0f + 1.0f));

    // magnitude of earth's magnetic field in the body frame
    static sensor_t magBodyFrame;

    // rotate body frame
    rotateV(&magBodyFrame, &deltaAngle);

    // apply complimentary filter
    axis = 0;
    for (axis = 0; axis < 3; axis++)
    {
        magBodyFrame.data[axis] = (magBodyFrame.data[axis] * gyro_cf_mag_factor
                + mag->data[axis]) * inv_gyro_cf_mag_factor;
    }

    // get heading in degrees
    angle->axis.yaw = calculateHeading(angle, &magBodyFrame);

    // convert to degrees
    angle->axis.roll *= RAD_TO_DEG;
    angle->axis.pitch *= RAD_TO_DEG;

//    UART2PCString("End of function...: ");
//    UART2PCFloat(data->gyroRate.data[1]);
//    UART2PCNewLine();
//    UART2PCNewLine();
}

// rotate accelerometer coordinate frame by delta angle given from gyroscope
// rotates from NED as follows:
void rotateV(sensor_t *v, const gyroAngle_t *angle)
{
    sensor_t v_tmp = *v;



    // angles for rotation matrix
    float cx = cosf(angle->axis.roll);
    float sx = sinf(angle->axis.roll);
    float cy = cosf(angle->axis.pitch);
    float sy = sinf(angle->axis.pitch);
    float cz = cosf(angle->axis.yaw);
    float sz = sinf(angle->axis.yaw);

    float czcx = cz * cx;
    float szcx = sz * cx;
    float czsx = cz * sx;
    float szsx = sz * sx;

    // Calculate rotation matrix as follows:
    //
    float rmat[3][3];
    rmat[0][0] = cz * cy;
    rmat[0][1] = cy * sz;
    rmat[0][2] = -sy;
    rmat[1][0] = -szcx + (czsx * sy);
    rmat[1][1] = czcx - (szsx * sy);
    rmat[1][2] = sx * cy;
    rmat[2][0] = szsx - (czcx * sy);
    rmat[2][1] = -czsx + (szcx * sy);
    rmat[2][2] = cy * cx;

    // Apply rotation

    v->axis.X = v_tmp.axis.X * rmat[0][0] + v_tmp.axis.Y * rmat[1][0]
            + v_tmp.axis.Z * rmat[2][0];

    v->axis.Y = v_tmp.axis.X * rmat[0][1] + v_tmp.axis.Y * rmat[1][1]
            + v_tmp.axis.Z * rmat[2][1];

    v->axis.Z = v_tmp.axis.X * rmat[0][2] + v_tmp.axis.Y * rmat[1][2]
            + v_tmp.axis.Z * rmat[2][2];
}

static float calculateHeading(gyroAngle_t *angle, sensor_t *mag)
{
    // calculate magnetic declination
    // +2 deg 12' for Houston, Tx on 18 Nov 2020
    static const int16_t magDeclinationCfg = -212;
    static const int16_t deg = magDeclinationCfg / 100;
    static const int16_t min = magDeclinationCfg % 100;
    static const float magDeclin = (deg + ((float) min * (1.0f / 60.0f)));

    float cx = cosf(angle->axis.roll);
    float sx = sinf(angle->axis.roll);
    float cy = cosf(angle->axis.pitch);
    float sy = sinf(angle->axis.pitch);

    float Bfy = mag->axis.Z * sx - mag->axis.Y * cx;
    float Bfx = mag->axis.X * cy + mag->axis.Y * sy * sx
            + mag->axis.Z * sy * cx;

    // calculate heading
    float heading = atan2f(-Bfy, Bfx) * RAD_TO_DEG + magDeclin;

    // keep heading in 0 to 360 range
    if (heading < 0)
        heading += 360.0f;
    return heading;
}
