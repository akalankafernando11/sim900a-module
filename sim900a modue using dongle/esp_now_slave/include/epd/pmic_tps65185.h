#ifndef PMIC_TPS65185_H
#define PMIC_TPS65185_H

int input_mv = 5000;

#define  HVPMIC_Address         0x68

#define  HVPMIC_VERSION         0x65

#define  HVPMIC_DATA_ENABLE     0x00
#define  HVPMIC_DATA_VADJ       0x03
#define  HVPMIC_DATA_VCOM1      0x00
#define  HVPMIC_DATA_VCOM2      0x00
#define  HVPMIC_DATA_INT_EN1    0x00
#define  HVPMIC_DATA_INT_EN2    0x00
#define  HVPMIC_DATA_UPSEQ0     0x78
#define  HVPMIC_DATA_UPSEQ1     0x00
#define  HVPMIC_DATA_DWNSEQ0    0x00
#define  HVPMIC_DATA_DWNSEQ1    0x00
#define  HVPMIC_DATA_TMST1      0x00
#define  HVPMIC_DATA_TMST2      0x78

#define  HVPMIC_REG_TMST_VALUE  0x00
#define  HVPMIC_REG_ENABLE      0x01
#define  HVPMIC_REG_VADJ        0x02
#define  HVPMIC_REG_VCOM1       0x03
#define  HVPMIC_REG_VCOM2       0x04
#define  HVPMIC_REG_INT_EN1     0x05
#define  HVPMIC_REG_INT_EN2     0x06
#define  HVPMIC_REG_INT1        0x07
#define  HVPMIC_REG_INT2        0x08
#define  HVPMIC_REG_UPSEQ0      0x09
#define  HVPMIC_REG_UPSEQ1      0x0A
#define  HVPMIC_REG_DWNSEQ0     0x0B
#define  HVPMIC_REG_DWNSEQ1     0x0C
#define  HVPMIC_REG_TMST1       0x0D
#define  HVPMIC_REG_TMST2       0x0E
#define  HVPMIC_REG_PG_STAT     0x0F
#define  HVPMIC_REG_REV_ID      0x10

#define  HVPMIC_DAC_MAX    ((1 << 8)-1)
#define  HVPMIC_DAC_MIN    0


int16_t dac_x1 = 127, dac_y1 = 4172, dac_x2 = 381, dac_y2 = 12490;
int32_t vgpos_mv = 25080, vgneg_mv = -32300, swing_ideal = 56886;

int32_t swing;
int32_t dac_offset;
int32_t dac_dx;
int32_t dac_dy;
int32_t dac_step_mv;


#endif /* PMIC_TPS65185_H */
