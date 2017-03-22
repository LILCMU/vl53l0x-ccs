#ifndef VL53L0X_h
#define VL53L0X_h

//#include <Arduino.h>


    // register addresses from API vl53l0x_device.h (ordered as listed there)
    enum regAddr
    {
      SYSRANGE_START                              = 0x00,

      SYSTEM_THRESH_HIGH                          = 0x0C,
      SYSTEM_THRESH_LOW                           = 0x0E,

      SYSTEM_SEQUENCE_CONFIG                      = 0x01,
      SYSTEM_RANGE_CONFIG                         = 0x09,
      SYSTEM_INTERMEASUREMENT_PERIOD              = 0x04,

      SYSTEM_INTERRUPT_CONFIG_GPIO                = 0x0A,

      GPIO_HV_MUX_ACTIVE_HIGH                     = 0x84,

      SYSTEM_INTERRUPT_CLEAR                      = 0x0B,

      RESULT_INTERRUPT_STATUS                     = 0x13,
      RESULT_RANGE_STATUS                         = 0x14,

      RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN       = 0xBC,
      RESULT_CORE_RANGING_TOTAL_EVENTS_RTN        = 0xC0,
      RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF       = 0xD0,
      RESULT_CORE_RANGING_TOTAL_EVENTS_REF        = 0xD4,
      RESULT_PEAK_SIGNAL_RATE_REF                 = 0xB6,

      ALGO_PART_TO_PART_RANGE_OFFSET_MM           = 0x28,

      I2C_SLAVE_DEVICE_ADDRESS                    = 0x8A,

      MSRC_CONFIG_CONTROL                         = 0x60,

      PRE_RANGE_CONFIG_MIN_SNR                    = 0x27,
      PRE_RANGE_CONFIG_VALID_PHASE_LOW            = 0x56,
      PRE_RANGE_CONFIG_VALID_PHASE_HIGH           = 0x57,
      PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT          = 0x64,

      FINAL_RANGE_CONFIG_MIN_SNR                  = 0x67,
      FINAL_RANGE_CONFIG_VALID_PHASE_LOW          = 0x47,
      FINAL_RANGE_CONFIG_VALID_PHASE_HIGH         = 0x48,
      FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT = 0x44,

      PRE_RANGE_CONFIG_SIGMA_THRESH_HI            = 0x61,
      PRE_RANGE_CONFIG_SIGMA_THRESH_LO            = 0x62,

      PRE_RANGE_CONFIG_VCSEL_PERIOD               = 0x50,
      PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI          = 0x51,
      PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO          = 0x52,

      SYSTEM_HISTOGRAM_BIN                        = 0x81,
      HISTOGRAM_CONFIG_INITIAL_PHASE_SELECT       = 0x33,
      HISTOGRAM_CONFIG_READOUT_CTRL               = 0x55,

      FINAL_RANGE_CONFIG_VCSEL_PERIOD             = 0x70,
      FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI        = 0x71,
      FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO        = 0x72,
      CROSSTALK_COMPENSATION_PEAK_RATE_MCPS       = 0x20,

      MSRC_CONFIG_TIMEOUT_MACROP                  = 0x46,

      SOFT_RESET_GO2_SOFT_RESET_N                 = 0xBF,
      IDENTIFICATION_MODEL_ID                     = 0xC0,
      IDENTIFICATION_REVISION_ID                  = 0xC2,

      OSC_CALIBRATE_VAL                           = 0xF8,

      GLOBAL_CONFIG_VCSEL_WIDTH                   = 0x32,
      GLOBAL_CONFIG_SPAD_ENABLES_REF_0            = 0xB0,
      GLOBAL_CONFIG_SPAD_ENABLES_REF_1            = 0xB1,
      GLOBAL_CONFIG_SPAD_ENABLES_REF_2            = 0xB2,
      GLOBAL_CONFIG_SPAD_ENABLES_REF_3            = 0xB3,
      GLOBAL_CONFIG_SPAD_ENABLES_REF_4            = 0xB4,
      GLOBAL_CONFIG_SPAD_ENABLES_REF_5            = 0xB5,

      GLOBAL_CONFIG_REF_EN_START_SELECT           = 0xB6,
      DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD         = 0x4E,
      DYNAMIC_SPAD_REF_EN_START_OFFSET            = 0x4F,
      POWER_MANAGEMENT_GO1_POWER_FORCE            = 0x80,

      VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV           = 0x89,

      ALGO_PHASECAL_LIM                           = 0x30,
      ALGO_PHASECAL_CONFIG_TIMEOUT                = 0x30,
    };

    // private variables

    typedef struct 
    {
      boolean tcc, msrc, dss, pre_range, final_range;
    } SequenceStepEnables;



    typedef struct 
    {
      int16 pre_range_vcsel_period_pclks, final_range_vcsel_period_pclks;

      int16 msrc_dss_tcc_mclks, pre_range_mclks, final_range_mclks;
      int32 msrc_dss_tcc_us,    pre_range_us,    final_range_us;
    } SequenceStepTimeouts;

    void getSequenceStepEnables(SequenceStepEnables * enables);
    void getSequenceStepTimeouts(SequenceStepEnables * enables, SequenceStepTimeouts * timeouts);
    
    
    enum vcselPeriodType { VcselPeriodPreRange, VcselPeriodFinalRange };

    int8 address;
    int16 io_timeout;
    boolean did_timeout;
    int16 timeout_start_ms;

    int8 stop_variable; // read by init and used when starting measurement; is StopVariable field of VL53L0X_DevData_t structure in API
    int32 measurement_timing_budget_us;

    boolean getSpadInfo(int8 * count, boolean * type_is_aperture);

    int8 last_status; // status of last I2C transmission

    VL53L0X(void);

    void setAddress(int8 new_addr);
    int8 getAddress(void) { return address; }

    boolean init(boolean io_2v8 = TRUE);
    #separate boolean init2();

    void writeReg(int8 reg, int8 value);
    void writeReg16Bit(int8 reg, int16 value);
    void writeReg32Bit(int8 reg, int32 value);
    int8 readReg(int8 reg);
    int16 readReg16Bit(int8 reg);
    int32 readReg32Bit(int8 reg);

    void writeMulti(int8 reg, int8  * src, int8 count);
    void readMulti(int8 reg, int8 * dst, int8 count);

    boolean setSignalRateLimit(float limit_Mcps);
    float getSignalRateLimit(void);

    boolean setMeasurementTimingBudget(int32 budget_us);
    int32 getMeasurementTimingBudget(void);

    boolean setVcselPulsePeriod(vcselPeriodType type, int8 period_pclks);
    int8 getVcselPulsePeriod(vcselPeriodType type);

    void startContinuous(int32 period_ms = 0);
    void stopContinuous(void);
    int16 readRangeContinuousMillimeters(void);
    int16 readRangeSingleMillimeters(void);

    void setTimeout(int16 timeout) { io_timeout = timeout; }
    int16 getTimeout(void) { return io_timeout; }
    boolean timeoutOccurred(void);



    boolean performSingleRefCalibration(int8 vhv_init_byte);

    static int16 decodeTimeout(int16 value);
    static int16 encodeTimeout(int16 timeout_mclks);
    static int32 timeoutMclksToMicroseconds(int16 timeout_period_mclks, int8 vcsel_period_pclks);
    static int32 timeoutMicrosecondsToMclks(int32 timeout_period_us, int8 vcsel_period_pclks);


#endif



