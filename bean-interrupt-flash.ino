uint32_t steps;
uint8_t clearit[2];

void setup() 
{
  steps = 0;
  clearit[0] = ' ';
  clearit[1] = ' ';
  
  Bean.setAccelerationRange(2);
  Bean.enableMotionEvent(ANY_MOTION_EVENT);
//  Bean.setAccelerometerPowerMode(0x00);
  Bean.accelRegisterWrite(0x27, 0x0b);
  Bean.accelRegisterWrite(0x28, 0x14);
}

void loop() {
  Bean.sleep(0xFFFFffff);
  steps ++;

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

  // update scratchData[1] with step count
  Bean.setScratchNumber(1, steps);

  // little blink
  Bean.setLed(0, 0, 255);
  Bean.sleep(10);
  Bean.setLed(0, 0, 0); 
}

