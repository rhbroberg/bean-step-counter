uint32_t steps;
uint8_t clearit[2];
uint8_t override = 0x47;
uint8_t blink = 0;

void setup() 
{
  steps = 0;
  clearit[0] = ' ';
  clearit[1] = ' ';
  
  Bean.setAccelerationRange(2);
  Bean.enableMotionEvent(ANY_MOTION_EVENT);
  Bean.setAccelerometerPowerMode(0x0);
  Bean.accelRegisterWrite(0x27, 0x00); // 0x0b is default; 0x01 and 0x10 more sensitive
  Bean.accelRegisterWrite(0x28, 0x14);
}

void
readScratchWriteReg(const uint8_t scratch, const uint8_t reg)
{
    ScratchData s = Bean.readScratchData(scratch);
    if ((s.length > 0) && (s.data[0] != ' '))
    {
      // little blink
      Bean.setLed(255, 0, 0);
      Bean.sleep(1000);
      Bean.setLed(0, 0, 0);
  
      Bean.accelRegisterWrite(reg, s.data[0]);
      Bean.setScratchData(scratch, clearit, 2);
    }
}

void
setAccelerometer()
{
  if (Bean.getConnectionState() == true)
  {
    readScratchWriteReg(3, 0x27);
    readScratchWriteReg(4, 0x28); 
    readScratchWriteReg(5, 0x16);
  }
}

void
resetStepCount()
{
  // if scratchData[2] == 'x', reset count
  ScratchData s = Bean.readScratchData(2);
  if ((s.length > 0) && s.data[0] == 'x')
  {
    steps = 0;
    Bean.setScratchData(2, clearit, 2);
    Bean.setLed(0, 255, 0);
    Bean.sleep(5000);
    Bean.setLed(0, 0, 0); 
  }
}

void
maybeBlink()
{
  if (blink)
  {
    // little blink
    Bean.setLed(0, 0, 255);
    Bean.sleep(10);
    Bean.setLed(0, 0, 0); 
  }
}

void
loop() {
  Bean.sleep(5000); // 0xFFFFffff);

  if (Bean.checkMotionEvent(ANY_MOTION_EVENT))
  {
    steps ++;

    // update scratchData[1] with step count
    Bean.setScratchNumber(1, steps);
    maybeBlink();
  }
  resetStepCount();
  setAccelerometer();
}

