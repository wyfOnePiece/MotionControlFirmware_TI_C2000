/******************************************************************************
 * Copyright (C) 2017 by Yifan Jiang                                          *
 * jiangyi@student.ethz.com                                                   *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
 ******************************************************************************/

/*
* implement the calibration process class
*/

#include "CalibrationProcess.h"

CalibrationProcess::CalibrationProcess(ControlProcessData * ControlProcessDataPtr):
  _ControlProcessData(ControlProcessDataPtr),
  _State(STATE_CAl_CURRENT_OFFSET),
  _CurrentOffsetSumA(0),
  _CurrentOffsetSumB(0),
  _CurrentOffsetSampleCnt(0),
  _CurrentOffsetCycleCnt(0)
{

}


void CalibrationProcess::Execute(void){

  switch (_State) {
    case STATE_CAl_CURRENT_OFFSET:
      for(uint16_t i=0; i<10; i++){
        _CurrentOffsetSumA += *(_ControlProcessData->_CurrentValueBufferPhaseA + i);
        _CurrentOffsetSumB += *(_ControlProcessData->_CurrentValueBufferPhaseB + i);
      }

      _CurrentOffsetSampleCnt += 10;
      _CurrentOffsetCycleCnt -= 1;
      if(_CurrentOffsetCycleCnt==0){
        _CurrentOffset_PhaseA = _CurrentOffsetSumA / (float32_t)_CurrentOffsetSampleCnt;
        _CurrentOffset_PhaseB = _CurrentOffsetSumB / (float32_t)_CurrentOffsetSampleCnt;
        _State = STATE_CAl_CURRENT_OFFSET;
      }
      break;
    case STATE_COMPLETE:
      _ProcessShouldQuit = true;
      break;
    default:
      break;
  }
}


void CalibrationProcess::Reset(void){

  _CurrentOffset_PhaseA = 0;
  _CurrentOffset_PhaseB = 0;
  _CurrentOffsetSumA = 0;
  _CurrentOffsetSumB = 0;
  _CurrentOffsetSampleCnt = 0;
  _CurrentOffsetCycleCnt = 4;

  _State = STATE_CAl_CURRENT_OFFSET;
}


void CalibrationProcess::AccessCurrentOffsetPhaseA(ObdAccessHandle * handle){
  switch (handle->AccessType) {
    case SDO_CSS_WRITE:
      break;
    case SDO_CSS_READ:
      handle->Data.DataFloat32 = _CurrentOffset_PhaseA;
      handle->AccessResult = OBD_ACCESS_SUCCESS;
      break;
    default:
      break;
  }
}

void CalibrationProcess::AccessCurrentOffsetPhaseB(ObdAccessHandle * handle){
  switch (handle->AccessType) {
    case SDO_CSS_WRITE:
      break;
    case SDO_CSS_READ:
      handle->Data.DataFloat32 = _CurrentOffset_PhaseB;
      handle->AccessResult = OBD_ACCESS_SUCCESS;
      break;
    default:
      break;
  }
}