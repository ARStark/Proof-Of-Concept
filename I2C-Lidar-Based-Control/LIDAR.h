#define decodeVcselPeriod(reg_val)      (((reg_val) + 1) << 1)
#define calcMacroPeriod(vcsel_period_pclks) ((((int)2304 * (vcsel_period_pclks) * 1655) + 500) / 1000)

static int measurement_timing_budget_us;

char stopFlag;

char tcc, msrc, dss, pre_range, final_range;

short pre_range_vcsel_period_pclks, final_range_vcsel_period_pclks;

short msrc_dss_tcc_mclks, pre_range_mclks, final_range_mclks;

int msrc_dss_tcc_us,    pre_range_us,    final_range_us;

int setSignalRateLimit(float limit_mcps)
{
  if (limit_mcps < 0 || limit_mcps > 511.99) { return 0; }

  // Q9.7 fixed point format (9 integer bits, 7 fractional bits)
  i2c_write_16( 0x44 ,limit_mcps * (1 << 7), 2);
  //writeReg16Bit(FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT, limit_Mcps * (1 << 7));
  return 1;

}

void getSequenceStepEnables()
{
   char sequence_config = i2c_read(0x01);

  tcc          = (sequence_config >> 4) & 0x1;
  dss          = (sequence_config >> 3) & 0x1;
  msrc         = (sequence_config >> 2) & 0x1;
  pre_range    = (sequence_config >> 6) & 0x1;
  final_range  = (sequence_config >> 7) & 0x1;
}
char getVcselPulsePeriod(int type)
{
  if (type == 0)
  {
    return decodeVcselPeriod(i2c_read(0x50));
  }
  else if (type == 1)
  {
    return decodeVcselPeriod(i2c_read(0x70));
  }
  else { return 255; }
}

short decodeTimeout(short reg_val)
{
  // format: "(LSByte * 2^MSByte) + 1"
  return (short)((reg_val & 0x00FF) <<
         (short)((reg_val & 0xFF00) >> 8)) + 1;
}


int timeoutMclksToMicroseconds(short timeout_period_mclks, char vcsel_period_pclks)
{
  int macro_period_ns = calcMacroPeriod(vcsel_period_pclks);

  return ((timeout_period_mclks * macro_period_ns) + (macro_period_ns / 2)) / 1000;
}

void getSequenceStepTimeouts()
{
  pre_range_vcsel_period_pclks = getVcselPulsePeriod(0);

  msrc_dss_tcc_mclks = i2c_read(0x46) + 1;
  msrc_dss_tcc_us =
    timeoutMclksToMicroseconds(msrc_dss_tcc_mclks,
                               pre_range_vcsel_period_pclks);

  pre_range_mclks = decodeTimeout(i2c_read_16(0x51));
  pre_range_us =
    timeoutMclksToMicroseconds(pre_range_mclks,
                               pre_range_vcsel_period_pclks);

  final_range_vcsel_period_pclks = getVcselPulsePeriod(1);

  final_range_mclks = decodeTimeout(i2c_read_16(0x71));

  if (pre_range)
  {
    final_range_mclks -= pre_range_mclks;
  }

  final_range_us =
    timeoutMclksToMicroseconds(final_range_mclks,
                               final_range_vcsel_period_pclks);
}

void getSpadInfo(char * count, char * type_is_aperture)
{
    char tmp;
    
    i2c_write(0x80, 0x01,1);
    i2c_write(0xFF, 0x01,1);
    i2c_write(0x00, 0x00,1);

    i2c_write(0xFF, 0x06,1);
    i2c_write(0x83, i2c_read(0x83) | 0x04,1);
    i2c_write(0xFF, 0x07,1);
    i2c_write(0x81, 0x01,1);

    i2c_write(0x80, 0x01,1);

    i2c_write(0x94, 0x6b,1);
    i2c_write(0x83, 0x00,1);
    
    i2c_read(0x83); // may not need
    i2c_write(0x83, 0x01, 1);
    
    tmp = i2c_read(0x92);
    *count = tmp & 0x7f;
    *type_is_aperture = (tmp >> 7) & 0x01; // 1 true; 0 false;
    
    i2c_write(0x81, 0x00,1);
    i2c_write(0xFF, 0x06,1);
    i2c_write(0x83, i2c_read(0x83)  & ~0x04,1);
    i2c_write(0xFF, 0x01,1);
    i2c_write(0x00, 0x01,1);

    i2c_write(0xFF, 0x00,1);
    i2c_write(0x80, 0x00,1);
}//end getSpadInfo())

int timeoutMicrosecondsToMclks(int timeout_period_us, char vcsel_period_pclks)
{
  int macro_period_ns = calcMacroPeriod(vcsel_period_pclks);

  return (((timeout_period_us * 1000) + (macro_period_ns / 2)) / macro_period_ns);
}

short encodeTimeout(short timeout_mclks)
{
  // format: "(LSByte * 2^MSByte) + 1"

  int ls_byte = 0;
  short ms_byte = 0;

  if (timeout_mclks > 0)
  {
    ls_byte = timeout_mclks - 1;

    while ((ls_byte & 0xFFFFFF00) > 0)
    {
      ls_byte >>= 1;
      ms_byte++;
    }

    return (ms_byte << 8) | (ls_byte & 0xFF);
  }
  else { return 0; }
}


int getMeasurementTimingBudget(void)
{
  //SequenceStepEnables enables;
//  SequenceStepTimeouts timeouts;

  short const StartOverhead     = 1910; // note that this is different than the value in set_
  short const EndOverhead        = 960;
  short const MsrcOverhead       = 660;
  short const TccOverhead        = 590;
  short const DssOverhead        = 690;
  short const PreRangeOverhead   = 660;
  short const FinalRangeOverhead = 550;

  // "Start and end overhead times always present"
  int budget_us = StartOverhead + EndOverhead;

  getSequenceStepEnables();
  getSequenceStepTimeouts();

  if (tcc)
  {
    budget_us += (msrc_dss_tcc_us + TccOverhead);
  }

  if (dss)
  {
    budget_us += 2 * (msrc_dss_tcc_us + DssOverhead);
  }
  else if (msrc)
  {
    budget_us += (msrc_dss_tcc_us + MsrcOverhead);
  }

  if (pre_range)
  {
    budget_us += (pre_range_us + PreRangeOverhead);
  }

  if (final_range)
  {
    budget_us += (final_range_us + FinalRangeOverhead);
  }

  measurement_timing_budget_us = budget_us; // store for internal reuse
  return budget_us;
}

char setMeasurementTimingBudget(int budget_us)
{
//  SequenceStepEnables enables;
  //SequenceStepTimeouts timeouts;

  short const StartOverhead      = 1320; // note that this is different than the value in get_
  short const EndOverhead        = 960;
  short const MsrcOverhead       = 660;
  short const TccOverhead        = 590;
  short const DssOverhead        = 690;
  short const PreRangeOverhead   = 660;
  short const FinalRangeOverhead = 550;

  int const MinTimingBudget = 20000;

  if (budget_us < MinTimingBudget) { return 0; }

  int used_budget_us = StartOverhead + EndOverhead;

  getSequenceStepEnables();
  getSequenceStepTimeouts();

  if (tcc)
  {
    used_budget_us += (msrc_dss_tcc_us + TccOverhead);
  }

  if (dss)
  {
    used_budget_us += 2 * (msrc_dss_tcc_us + DssOverhead);
  }
  else if (msrc)
  {
    used_budget_us += (msrc_dss_tcc_us + MsrcOverhead);
  }

  if (pre_range)
  {
    used_budget_us += (pre_range_us + PreRangeOverhead);
  }

  if (final_range)
  {
    used_budget_us += FinalRangeOverhead;

    // "Note that the final range timeout is determined by the timing
    // budget and the sum of all other timeouts within the sequence.
    // If there is no room for the final range timeout, then an error
    // will be set. Otherwise the remaining time will be applied to
    // the final range."

    if (used_budget_us > budget_us)
    {
      // "Requested timeout too big."
      return 0;
    }

    int final_range_timeout_us = budget_us - used_budget_us;

    // set_sequence_step_timeout() begin
    // (SequenceStepId == VL53L0X_SEQUENCESTEP_FINAL_RANGE)

    // "For the final range timeout, the pre-range timeout
    //  must be added. To do this both final and pre-range
    //  timeouts must be expressed in macro periods MClks
    //  because they have different vcsel periods."

    short final_range_timeout_mclks =
      timeoutMicrosecondsToMclks(final_range_timeout_us,
                                 final_range_vcsel_period_pclks);

    if (pre_range)
    {
      final_range_timeout_mclks += pre_range_mclks;
    }

    i2c_write_16(0x71,
      encodeTimeout(final_range_timeout_mclks),2);

    // set_sequence_step_timeout() end

    measurement_timing_budget_us = budget_us; // store for internal reuse
  }
  return 1;
}

void performSingleRefCalibration(uint8_t vhv_init_byte)
{
  i2c_write(0x00, 0x01 | vhv_init_byte,1); // VL53L0X_REG_SYSRANGE_MODE_START_STOP

  
  i2c_write(0x0B, 0x01,1);

  i2c_write(0x00, 0x00,1);

}


void startContinousReadings(int period_ms)
{
    i2c_write(0x80, 0x01,1);
    i2c_write(0xFF, 0x01,1);
    i2c_write(0x00, 0x00,1);
    i2c_write(0x91, stopFlag,1);
    i2c_write(0x00, 0x01,1);
    i2c_write(0xFF, 0x00,1);
    i2c_write(0x80, 0x00,1);
    
    if (period_ms != 0)
  {
    // continuous timed mode

    // VL53L0X_SetInterMeasurementPeriodMilliSeconds() begin

    short osc_calibrate_val = i2c_read_16(0xF8);

    if (osc_calibrate_val != 0)
    {
      period_ms *= osc_calibrate_val;
    }

    i2c_write_32(0x04, period_ms,4);

    // VL53L0X_SetInterMeasurementPeriodMilliSeconds() end

    i2c_write(0x00, 0x04,1); // VL53L0X_REG_SYSRANGE_MODE_TIMED
  }
  else
  {
    // continuous back-to-back mode
    i2c_write(0x00, 0x02,1); // VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK
  }
}

short readRangeContinuousMillimeters(void)
{
 
    while ((i2c_read(0x13) & 0x07) == 0)
  {}
  // assumptions: Linearity Corrective Gain is 1000 (default);
  // fractional ranging is not enabled
  short range = i2c_read_16(0x14);

  i2c_write(0x0B, 0x01,1);
  return range;
}

void sensor_initialize()
{
    //begin dataInit())
    i2c_write(0x88, 0x00,1);
    
    i2c_write(0x80, 0x01,1 );
    i2c_write(0xFF, 0x01,1);
    i2c_write(0x00, 0x00,1);
    stopFlag=i2c_read(0x91);
    i2c_write(0x00, 0x01,1);
    i2c_write(0xFF, 0x00,1);
    i2c_write(0x80, 0x00,1);
    
    i2c_write(0x60, i2c_read(0x60) | 0x12,1);
    
    setSignalRateLimit(0.25);
    
    i2c_write(0x01,0xFF,1);
    //end dataInit())
    
    
    
    
    
    // start staticInit()
    char spad_count;
    char spad_type_is_aperture;
    getSpadInfo(&spad_count, &spad_type_is_aperture);
    
    char ref_spad_map[6];
    i2c_read_multi_reg(0xB0,ref_spad_map,6);
    i2c_write(0xFF, 0x01,1);
    i2c_write(0x4F, 0x00,1);
    i2c_write(0x4E, 0x2C,1);
    i2c_write(0xFF, 0x00,1);
    i2c_write(0xB6, 0xB4,1);
    
    char first_spad_to_enable = 12;
    char spads_enabled=0;
    char i;
    for (i =0; i< 48; i++)
    {
        if (i < first_spad_to_enable || spads_enabled == spad_count)
        {
            ref_spad_map[i/8] &= (1 << (i%8));
        }
         else if ((ref_spad_map[i / 8] >> (i % 8)) & 0x1)
        {
            spads_enabled++;
        }
    }
    
    i2c_write_multi_reg(0xB0, ref_spad_map, 6);
    // set_reference_spads() ends
    
    // load tuning begins
      i2c_write(0xFF, 0x01,1);
      i2c_write(0x00, 0x00,1);

      i2c_write(0xFF, 0x00,1);
  i2c_write(0x09, 0x00,1);
  i2c_write(0x10, 0x00,1);
  i2c_write(0x11, 0x00,1);

  i2c_write(0x24, 0x01,1);
  i2c_write(0x25, 0xFF,1);
  i2c_write(0x75, 0x00,1);

  i2c_write(0xFF, 0x01,1);
  i2c_write(0x4E, 0x2C,1);
  i2c_write(0x48, 0x00,1);
  i2c_write(0x30, 0x20,1);

  i2c_write(0xFF, 0x00,1);
  i2c_write(0x30, 0x09,1);
  i2c_write(0x54, 0x00,1);
  i2c_write(0x31, 0x04,1);
  i2c_write(0x32, 0x03,1);
  i2c_write(0x40, 0x83,1);
  i2c_write(0x46, 0x25,1);
  i2c_write(0x60, 0x00,1);
  i2c_write(0x27, 0x00,1);
  i2c_write(0x50, 0x06,1);
  i2c_write(0x51, 0x00,1);
  i2c_write(0x52, 0x96,1);
  i2c_write(0x56, 0x08,1);
  i2c_write(0x57, 0x30,1);
  i2c_write(0x61, 0x00,1);
  i2c_write(0x62, 0x00,1);
  i2c_write(0x64, 0x00,1);
  i2c_write(0x65, 0x00,1);
  i2c_write(0x66, 0xA0,1);

  i2c_write(0xFF, 0x01,1);
  i2c_write(0x22, 0x32,1);
  i2c_write(0x47, 0x14,1);
  i2c_write(0x49, 0xFF,1);
  i2c_write(0x4A, 0x00,1);

  i2c_write(0xFF, 0x00,1);
  i2c_write(0x7A, 0x0A,1);
  i2c_write(0x7B, 0x00,1);
  i2c_write(0x78, 0x21,1);

  i2c_write(0xFF, 0x01,1);
  i2c_write(0x23, 0x34,1);
  i2c_write(0x42, 0x00,1);
  i2c_write(0x44, 0xFF,1);
  i2c_write(0x45, 0x26,1);
  i2c_write(0x46, 0x05,1);
  i2c_write(0x40, 0x40,1);
  i2c_write(0x0E, 0x06,1);
  i2c_write(0x20, 0x1A,1);
  i2c_write(0x43, 0x40,1);

  i2c_write(0xFF, 0x00,1);
  i2c_write(0x34, 0x03,1);
  i2c_write(0x35, 0x44,1);

  i2c_write(0xFF, 0x01,1);
  i2c_write(0x31, 0x04,1);
  i2c_write(0x4B, 0x09,1);
  i2c_write(0x4C, 0x05,1);
  i2c_write(0x4D, 0x04,1);

  i2c_write(0xFF, 0x00,1);
  i2c_write(0x44, 0x00,1);
  i2c_write(0x45, 0x20,1);
  i2c_write(0x47, 0x08,1);
  i2c_write(0x48, 0x28,1);
  i2c_write(0x67, 0x00,1);
  i2c_write(0x70, 0x04,1);
  i2c_write(0x71, 0x01,1);
  i2c_write(0x72, 0xFE,1);
  i2c_write(0x76, 0x00,1);
  i2c_write(0x77, 0x00,1);

  i2c_write(0xFF, 0x01,1);
  i2c_write(0x0D, 0x01,1);

  i2c_write(0xFF, 0x00,1);
  i2c_write(0x80, 0x01,1);
  i2c_write(0x01, 0xF8,1);

  i2c_write(0xFF, 0x01,1);
  i2c_write(0x8E, 0x01,1);
  i2c_write(0x00, 0x01,1);
  i2c_write(0xFF, 0x00,1);
  i2c_write(0x80, 0x00,1);

    // load tuning ends
  //set GPIO begin
  i2c_write(0x0A, 0x04,1);
  i2c_write(0x84, i2c_read(0x84) & ~0x10,1); // active low
  i2c_write(0x0B, 0x01,1);
  //set GPIO end
  
  measurement_timing_budget_us = getMeasurementTimingBudget();
  
  i2c_write(0x01, 0xE8,1);
  setMeasurementTimingBudget(measurement_timing_budget_us);
  i2c_write(0x01, 0x01,1);
  performSingleRefCalibration(0x40);
  i2c_write(0x01, 0x02, 1);
  performSingleRefCalibration(0x00);
  i2c_write(0x01, 0xE8,1);
}

