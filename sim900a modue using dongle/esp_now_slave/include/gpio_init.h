#ifndef GPIO_INIT_H
#define GPIO_INIT_H

#define EPD_VCOM        2
#define EPD_HVEN        12
#define EPD_RST         13
#define EPD_POK         14
#define EPD_BB_SLEEP    27
#define EPD_HDC         25
#define EPD_VCC_EN      26

#define SPI_CLK         18
#define SPI_MISO        19
#define SPI_CS          33
#define SPI_MOSI        23

#define I2C_SCL_1       22   
#define I2C_SDA_1       21   

#define I2C_SCL_2       5      
#define I2C_SDA_2       4      

#define ON_BTN_SIG	 	34
#define ON_BTN_TRIG     GPIO_NUM_34
#define	BC_CHG_SIG		35 
#define BC_CHG_TRIG     GPIO_NUM_35
#define USB_DETECT_SIG	38
#define USB_DETECT_TRIG GPIO_NUM_38

#define LED_EN          0

#define SOFT_PWR_OFF 	32

#define RTC_INT			36
#define RTC_INT_TRIG    GPIO_NUM_36

#define BM_ALARM		39

#define TEMP_ALERT_1	15
#define TEMP_ALERT_2	37

#endif  /* GPIO_INIT_H */
