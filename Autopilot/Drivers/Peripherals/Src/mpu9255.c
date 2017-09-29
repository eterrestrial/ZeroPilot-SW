#include "mpu9255.h"
#include "debug.h"

#define MPU9255_ADDR 0xD0
#define AK8963_ADDR (0x0C << 1)
#define MPU9255_I2C I2C4


/* Magnetometer */
#define WHO_AM_I_AK8963     0x00 // Result = 0x48
#define INFO                0x01
#define AK8963_ST1          0x02
#define AK8963_XOUT_L	    0x03
#define AK8963_XOUT_H	    0x04
#define AK8963_YOUT_L	    0x05
#define AK8963_YOUT_H	    0x06
#define AK8963_ZOUT_L	    0x07
#define AK8963_ZOUT_H	    0x08
#define AK8963_ST2          0x09
#define AK8963_CNTL         0x0A
#define AK8963_ASTC         0x0C
#define AK8963_I2CDIS       0x0F
#define AK8963_ASAX         0x10
#define AK8963_ASAY         0x11
#define AK8963_ASAZ         0x12

/* MPU9250 data */
#define SELF_TEST_X_GYRO    0x00
#define SELF_TEST_Y_GYRO    0x01
#define SELF_TEST_Z_GYRO    0x02

#define SELF_TEST_X_ACCEL   0x0D
#define SELF_TEST_Y_ACCEL   0x0E
#define SELF_TEST_Z_ACCEL   0x0F

// #define SELF_TEST_A         0x10

#define XG_OFFSET_H         0x13
#define XG_OFFSET_L         0x14
#define YG_OFFSET_H         0x15
#define YG_OFFSET_L         0x16
#define ZG_OFFSET_H         0x17
#define ZG_OFFSET_L         0x18
#define SMPLRT_DIV          0x19
#define CONFIG              0x1A
#define GYRO_CONFIG         0x1B
#define ACCEL_CONFIG        0x1C
#define ACCEL_CONFIG2       0x1D
#define LP_ACCEL_ODR        0x1E
#define WOM_THR             0x1F

// #define MOT_DUR             0x20
// #define ZMOT_THR            0x21
// #define ZRMOT_DUR           0x22

#define FIFO_EN             0x23
#define I2C_MST_CTRL        0x24
#define I2C_SLV0_ADDR       0x25
#define I2C_SLV0_REG        0x26
#define I2C_SLV0_CTRL       0x27
#define I2C_SLV1_ADDR       0x28
#define I2C_SLV1_REG        0x29
#define I2C_SLV1_CTRL       0x2A
#define I2C_SLV2_ADDR       0x2B
#define I2C_SLV2_REG        0x2C
#define I2C_SLV2_CTRL       0x2D
#define I2C_SLV3_ADDR       0x2E
#define I2C_SLV3_REG        0x2F
#define I2C_SLV3_CTRL       0x30
#define I2C_SLV4_ADDR       0x31
#define I2C_SLV4_REG        0x32
#define I2C_SLV4_DO         0x33
#define I2C_SLV4_CTRL       0x34
#define I2C_SLV4_DI         0x35
#define I2C_MST_STATUS      0x36
#define INT_PIN_CFG         0x37
#define INT_ENABLE          0x38
// #define DMP_INT_STATUS      0x39
#define INT_STATUS          0x3A
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
#define TEMP_OUT_H          0x41
#define TEMP_OUT_L          0x42
#define GYRO_XOUT_H         0x43
#define GYRO_XOUT_L         0x44
#define GYRO_YOUT_H         0x45
#define GYRO_YOUT_L         0x46
#define GYRO_ZOUT_H         0x47
#define GYRO_ZOUT_L         0x48
#define EXT_SENS_DATA_00    0x49
#define EXT_SENS_DATA_01    0x4A
#define EXT_SENS_DATA_02    0x4B
#define EXT_SENS_DATA_03    0x4C
#define EXT_SENS_DATA_04    0x4D
#define EXT_SENS_DATA_05    0x4E
#define EXT_SENS_DATA_06    0x4F
#define EXT_SENS_DATA_07    0x50
#define EXT_SENS_DATA_08    0x51
#define EXT_SENS_DATA_09    0x52
#define EXT_SENS_DATA_10    0x53
#define EXT_SENS_DATA_11    0x54
#define EXT_SENS_DATA_12    0x55
#define EXT_SENS_DATA_13    0x56
#define EXT_SENS_DATA_14    0x57
#define EXT_SENS_DATA_15    0x58
#define EXT_SENS_DATA_16    0x59
#define EXT_SENS_DATA_17    0x5A
#define EXT_SENS_DATA_18    0x5B
#define EXT_SENS_DATA_19    0x5C
#define EXT_SENS_DATA_20    0x5D
#define EXT_SENS_DATA_21    0x5E
#define EXT_SENS_DATA_22    0x5F
#define EXT_SENS_DATA_23    0x60
// #define MOT_DETECT_STATUS   0x61
#define I2C_SLV0_DO         0x63
#define I2C_SLV1_DO         0x64
#define I2C_SLV2_DO         0x65
#define I2C_SLV3_DO         0x66
#define I2C_MST_DELAY_CTRL  0x67
#define SIGNAL_PATH_RESET   0x68
#define MOT_DETECT_CTRL     0x69
#define USER_CTRL           0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1          0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2          0x6C
// #define DMP_BANK            0x6D  // Activates a specific bank in the DMP
// #define DMP_RW_PNT          0x6E  // Set read/write pointer to a specific start address in specified DMP bank
// #define DMP_REG             0x6F  // Register in DMP from which to read or to which to write
// #define DMP_REG_1           0x70
// #define DMP_REG_2           0x71
#define FIFO_COUNTH         0x72
#define FIFO_COUNTL         0x73
#define FIFO_R_W            0x74
#define WHO_AM_I_MPU9255    0x75 // Should return 0x73
#define XA_OFFSET_H         0x77
#define XA_OFFSET_L         0x78
#define YA_OFFSET_H         0x7A
#define YA_OFFSET_L         0x7B
#define ZA_OFFSET_H         0x7D
#define ZA_OFFSET_L         0x7E

// default -> MSB (high) first
#define BYTES2WORD_BE(bytes) (((int16_t)(bytes)[0] << 8) | (bytes)[1])
#define BYTES2WORD_LE(bytes) (((int16_t)(bytes)[1] << 8) | (bytes)[0])

static I2C_HandleTypeDef* hi2c;

HAL_StatusTypeDef MPU9255_Init(MPU9255_t *mpu) {
    hi2c = I2C_GetHandle(MPU9255_I2C);

    // is connected
    if (HAL_I2C_IsDeviceReady(hi2c, MPU9255_ADDR, 2, 5) != HAL_OK) {
        debug("no mpu");
        // error
    }

    // who am i
    if (I2C_ReadByte(hi2c, MPU9255_ADDR, WHO_AM_I_MPU9255) != 0x73) {
        debug("mpu who am i failed");
        // error
    }

    // power
    I2C_WriteByte(hi2c, MPU9255_ADDR, PWR_MGMT_1, 0x01);
    // wait for a bit
    HAL_Delay(200);

    // config gyro, accel, thermo
    // low pass for temp & 1 KHz output
    I2C_WriteByte(hi2c, MPU9255_ADDR, CONFIG, 0x03);

    // configure sensors for 250 Hz output
    I2C_WriteByte(hi2c, MPU9255_ADDR, SMPLRT_DIV, 0x03);

   // set full scale range for accel and gyro
    I2C_WriteByte(hi2c, MPU9255_ADDR, GYRO_CONFIG, 0x03 << 3);
    I2C_WriteByte(hi2c, MPU9255_ADDR, ACCEL_CONFIG, 0x03 << 3);

    // low pass for accel & 1KHz outpout
    I2C_WriteByte(hi2c, MPU9255_ADDR, ACCEL_CONFIG2, 0x03);

    // config slave port for pass-through
    I2C_WriteByte(hi2c, MPU9255_ADDR, INT_PIN_CFG, 0x02);
    // disable interrupts
    I2C_WriteByte(hi2c, MPU9255_ADDR, INT_ENABLE, 0x00);
    HAL_Delay(100);

    // is mag connected
    if (HAL_I2C_IsDeviceReady(hi2c, AK8963_ADDR, 2, 5) != HAL_OK) {
        debug("no mag");
        // error
    }

    // who am i mag
    if (I2C_ReadByte(hi2c, AK8963_ADDR, WHO_AM_I_AK8963) != 0x48) {
        debug("mag who am i failed");
        // error
    }

    // config mag
    // Power down
    I2C_WriteByte(hi2c, AK8963_ADDR, AK8963_CNTL, 0x00);
    HAL_Delay(10);

    // ROM access
    I2C_WriteByte(hi2c, AK8963_ADDR, AK8963_CNTL, 0x0F);
    HAL_Delay(10);
    uint8_t rawData[3];
    I2C_ReadBytes(hi2c, AK8963_ADDR, AK8963_ASAX, rawData, 3);

    mpu->Mx_adj = ((float)(rawData[0] - 128) / 256.f) + 1;
    mpu->My_adj = ((float)(rawData[1] - 128) / 256.f) + 1;
    mpu->Mz_adj = ((float)(rawData[2] - 128) / 256.f) + 1;

    // Power down again
    I2C_WriteByte(hi2c, AK8963_ADDR, AK8963_CNTL, 0x00);
    HAL_Delay(10);

    // full resolution, 100 Hz
    I2C_WriteByte(hi2c, AK8963_ADDR, AK8963_CNTL, 0x16);
    HAL_Delay(10);

    mpu->A_res = 16.f / 32768.f; // Convert ADC val to g's
    mpu->G_res = 0.01745329251f * 2000.f / 32768.f; // rads/s
    mpu->M_res = 0.15f;// 4912.f / 32768.f; // microTesla

    return HAL_OK;
}

void calibrateMPU(MPU9255_t* mpu) {
  //https://github.com/sparkfun/MPU-9250_Breakout/blob/master/Libraries/Arduino/src/MPU9250.cpp

  uint8_t data[12]; //accelerometer and gyro x, y, z data
  unint_t ii, packetCountn, fifoCount;
  int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

  //reset
  I2C_WriteByte(MPU9255_ADDR, PWR_MGMT_1, 0x80);
  HAL_Delay(100);

  //get time source
  I2C_WriteByte(MPU9255_ADDR, PWR_MGMT_1, 0x01);
  I2C_WriteByte(MPU9255_ADDR, PWR_MGMT_2, 0x00);
  HAL_Delay(200);

  //configure device for bias calculation
  I2C_WriteByte(MPU9255_ADDR, INT_ENABLE, 0x00);
  I2C_WriteByte(MPU9255_ADDR, FIFO_EN, 0x00);
  I2C_WriteByte(MPU9255_ADDR, PWR_MGMT_1, 0x00);
  I2C_WriteByte(MPU9255_ADDR, I2C_MST_CTRL, 0x00);
  I2C_WriteByte(MPU9255_ADDR, USER_CTRL, 0x00);
  I2C_WriteByte(MPU9255_ADDR, USER_CTRL, 0x0C);
  HAL_Delay(15);

  //config gyro and accelerometer for bias calibration
  I2C_WriteByte(MPU9255_ADDR, CONFIG, 0x01); //low pass filter set to 188Hz
  I2C_WriteByte(MPU9255_ADDR, SMPLRT_DIV, 0x00); //sample rate set to 1kHz
  I2C_WriteByte(MPU9255_ADDR, GYRO_CONFIG, 0x00); //gyro set to fullscale 250 degrees per sec, max sensitivity
  I2C_WriteByte(MPU9255_ADDR, ACCEL_CONFIG, 0x00); //set accelerometer to fullscale to 2 g, max sensitivity

  uint16_t gyroSensitivity = 16; // 131 LSB/degrees/sec
  uint16_t accelSensitivity = 2048; // 16384 LSB/g

  //config fifo to capture accelerometer and gyro data
  I2C_WriteByte(MPU9255_ADDR, USER_CTRL, 0x40);
  I2C_WriteByte(MPU9255_ADDR, FIFO_EN, 0x78);
  HAL_Delay(40); //get 40 samples in 40 ms = 480 bytes

  //turn off fifo sensor read
  I2C_WriteByte(MPU9255_ADDR, FIFO_EN, 0x00);
  I2C_ReadBytes(MPU9255_ADDR, FIFO_COUNTH, 2, data[1]);
  fifoCount = ((uint16_t)data[0] << 8 | data[1]);
  packetCount = fifoCount/12;

  for (ii = 0; ii < packetCount; ii++){
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    accel_temp[0] = (int16_t)(((int16_t)data[0] << 8) | data[1]);
    accel_temp[1] = (int16_t)(((int16_t)data[2] << 8) | data[3]);
    accel_temp[2] = (int16_t)(((int16_t)data[4] << 8) | data[5]);
    gyro_temp[0] = (int16_t)(((int16_t)data[6] << 8) | data[8]);
    gyro_temp[1] = (int16_t)(((int16_t)data[8] << 8) | data[9]);
    gyro_temp[2] = (int16_t)(((int16_t)data[10] << 8) | data[11]);

    accel_bias[0] += (int32_t) accel_temp[0];
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0] += (int32_t) gyro_temp[0];
    gyro_bias[1] += (int32_t) gyro_temp[1];
    gyro_bias[2] += (int32_t) gyro_temp[2];
  }
  accel_bias[0] /= (int32_t) packetCount;
  accel_bias[1] /= (int32_t) packetCount;
  accel_bias[2] /= (int32_t) packetCount;
  gyro_bias[0] /= (int32_t) packetCount;
  gyro_bias[1] /= (int32_t) packetCount;
  gyro_bias[2] /= (int32_t) packetCount;

  if(accel_bias[2] > 0L) //remove gravity from accelerometer z axis calibration
    accel_bias[2] -= (int32_t) accelSensitivity;
  else
    accel_bias[2] += (int32_t) accelSensitivity;

  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

  I2C_WriteByte(MPU9255_ADDR, XG_OFFSET_H, data[0]);
  I2C_WriteByte(MPU9255_ADDR, XG_OFFSET_L, data[1]);
  I2C_WriteByte(MPU9255_ADDR, YG_OFFSET_H, data[2]);
  I2C_WriteByte(MPU9255_ADDR, YG_OFFSET_L, data[3]);
  I2C_WriteByte(MPU9255_ADDR, ZG_OFFSET_H, data[4]);
  I2C_WriteByte(MPU9255_ADDR, ZG_OFFSET_L, data[5]);

  gyroBias[0] = (float) gyro_bias[0] / (float) gyroSensitivity;
  gyroBias[1] = (float) gyro_bias[1] / (float) gyroSensitivity;
  gyroBias[2] = (float) gyro_bias[2] / (float) gyroSensitivity;

  int32_t accel_bias_reg[3] = {0, 0, 0};
  I2C_ReadBytes(MPU9255_ADDR, XA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  I2C_ReadBytes(MPU9255_ADDR, YA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  I2C_ReadBytes(MPU9255_ADDR, ZA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

  uint32_t mask = 1ul;
  uint8_t mask_bit[3] = {0, 0, 0};

  for(ii = 0; ii < 3; ii++){
      if((accel_bias_reg[ii] & mask))
        mask_bit[ii] = 0x01;
  }

  // Construct total accelerometer bias, including calculated average accelerometer bias from above
 accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
 accel_bias_reg[1] -= (accel_bias[1]/8);
 accel_bias_reg[2] -= (accel_bias[2]/8);

 data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
 data[1] = (accel_bias_reg[0])      & 0xFF;
 data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
 data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
 data[3] = (accel_bias_reg[1])      & 0xFF;
 data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
 data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
 data[5] = (accel_bias_reg[2])      & 0xFF;
 data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

 I2C_WriteByte(MPU9255_ADDR, XA_OFFSET_H, data[0]);
 I2C_WriteByte(MPU9255_ADDR, XA_OFFSET_L, data[1]);
 I2C_WriteByte(MPU9255_ADDR, YA_OFFSET_H, data[2]);
 I2C_WriteByte(MPU9255_ADDR, YA_OFFSET_L, data[3]);
 I2C_WriteByte(MPU9255_ADDR, ZA_OFFSET_H, data[4]);
 I2C_WriteByte(MPU9255_ADDR, ZA_OFFSET_L, data[5]);

 accelBias[0] = (float)accel_bias[0]/(float)accelsensitivity;
 accelBias[1] = (float)accel_bias[1]/(float)accelsensitivity;
 accelBias[2] = (float)accel_bias[2]/(float)accelsensitivity;

}

void MPU9255_ReadAccel(MPU9255_t* mpu) {
    uint8_t data[6];

    I2C_ReadBytes(hi2c, MPU9255_ADDR, ACCEL_XOUT_H, data, 6);

    /* DO NOT CHANGE THESE. MPU9255 REFERECE FRAME IS WEIRD */
    mpu->Ax_raw = -BYTES2WORD_BE(data);
    mpu->Ay_raw = BYTES2WORD_BE(data + 2);
    mpu->Az_raw = BYTES2WORD_BE(data + 4);

    mpu->Ax = mpu->Ax_raw * mpu->A_res;
    mpu->Ay = mpu->Ay_raw * mpu->A_res;
    mpu->Az = mpu->Az_raw * mpu->A_res;
}

void MPU9255_ReadGyro(MPU9255_t* mpu) {
    uint8_t data[6];

    I2C_ReadBytes(hi2c, MPU9255_ADDR, GYRO_XOUT_H, data, 6);

    /* DO NOT CHANGE THESE. MPU9255 REFERECE FRAME IS WEIRD */
    mpu->Gx_raw = BYTES2WORD_BE(data);
    mpu->Gy_raw = -BYTES2WORD_BE(data + 2);
    mpu->Gz_raw = -BYTES2WORD_BE(data + 4);

    mpu->Gx = mpu->Gx_raw * mpu->G_res;
    mpu->Gy = mpu->Gy_raw * mpu->G_res;
    mpu->Gz = mpu->Gz_raw * mpu->G_res;
}

void MPU9255_ReadMag(MPU9255_t* mpu) {
    uint8_t data[7];

    if (I2C_ReadByte(hi2c, AK8963_ADDR, AK8963_ST1) & 0x01) {
        // read all data and status byte
        I2C_ReadBytes(hi2c, AK8963_ADDR, AK8963_XOUT_L, data, 7);

        if (!(data[6] & 0x08)) {
            /* DO NOT CHANGE THESE. MPU9255 REFERECE FRAME IS WEIRD */
            mpu->My_raw = -BYTES2WORD_LE(data);
            mpu->Mx_raw = BYTES2WORD_LE(data + 2);
            mpu->Mz_raw = -BYTES2WORD_LE(data + 4);
            // yes x and y are swapped. yes that's on purpose.

            mpu->Mx = mpu->Mx_raw * mpu->Mx_adj * mpu->M_res;
            mpu->My = mpu->My_raw * mpu->My_adj * mpu->M_res;
            mpu->Mz = mpu->Mz_raw * mpu->Mz_adj * mpu->M_res;
        }
    }
}

int16_t MPU9255_ReadTemp() {
    uint8_t data[2];
    I2C_ReadBytes(hi2c, MPU9255_ADDR, TEMP_OUT_H, data, 2);
    return BYTES2WORD_BE(data);
}
