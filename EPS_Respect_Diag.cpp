#include "EPS_Respect_Diag.h"//delshadi
#include "MSG_EPS_Respect_Diag.h"
#include "ArrayTools.h"
#include "OtherHeader\EPS_Respect_Diag_OH.h"
EPS_Respect_Diag::EPS_Respect_Diag(void)
{
}
EPS_Respect_Diag::~EPS_Respect_Diag(void)
{
}
bool EPS_Respect_Diag::Sware_End_Stop_Reset(CommandAdorner* Command)
{
    byte response[255]={};
	OBDii.Security_Pass(Command, 1);
		OnShowMessage_Messages(EPSRespectDiag_AreYouSure, Yes_No, true);
		byte Routine1[4] = { 0x31, 0x01, 0x32, 0x0C };
		if ((OBDii.Send_Receive(sizeof(Routine1), Routine1, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				return false; 
			  }
			Sleep(500);
			byte Routine2[] = { 0x31, 0x03, 0x32, 0x0C };
			if ((OBDii.Send_Receive(sizeof(Routine2), Routine2, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
			{
				return false; 
			} 
			if(!OnShowMessage_Messages(EPSRespectDiag_ExitAndCloseSwitch, Yes_No, true))
				return false;
			if (response[4]!=0x02)
			{return false;}
			byte Routine3[4] = { 0x31, 0x02, 0x33, 0x0C };
			if ((OBDii.Send_Receive(sizeof(Routine3), Routine3, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				return false; 
			  }
			byte Routine4[4] = { 0x14, 0xFF, 0xFF, 0xFF };
			if ((OBDii.Send_Receive(sizeof(Routine4), Routine4, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				return false; 
			  }
			byte Routine5[2] = { 0x10, 0x01 };
			if ((OBDii.Send_Receive(sizeof(Routine5), Routine5, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				return false; 
			  }
			Sleep(500);
			return true; 
  }
	
bool EPS_Respect_Diag::Motor_Position_Sensor(CommandAdorner* Command)
{
    byte response[255]={};
	OBDii.Security_Pass(Command, 1);

		if(!OnShowMessage_Messages(EPSRespectDiag_AreYouSure, Yes_No, true))
			return false;
		byte Routine1[4] = { 0x31, 0x01, 0x32, 0x0B };
		if ((OBDii.Send_Receive(sizeof(Routine1), Routine1, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
			{	
			return false; 
			}
			byte Routine2[4] = { 0x31, 0x03, 0x32, 0x0B };
			if ((OBDii.Send_Receive(sizeof(Routine2), Routine2, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				return false; 
				}
					
			Sleep(500);
				return true; 
			if(!OnShowMessage_Messages(EPSRespectDiag_ExitAndCloseSwitch, Yes_No, true))
				return false;
			if (response[4]!=0x02)
			{return false;}
			byte Routine3[4] = { 0x31, 0x02, 0x33, 0x0B };
			if ((OBDii.Send_Receive(sizeof(Routine3), Routine3, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				return false; 
			  }
			byte Routine4[4] = { 0x14, 0xFF, 0xFF, 0xFF };
			if ((OBDii.Send_Receive(sizeof(Routine4), Routine4, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				return false; 
			  }
			byte Routine5[2] = { 0x10, 0x01 };
			if ((OBDii.Send_Receive(sizeof(Routine5), Routine5, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				return false; 
			  }
			Sleep(500);
			return true; 
}
bool EPS_Respect_Diag::EPSDTC(CommandAdorner* Command)
{
    byte res[1424];
	  OBDii.Security_Pass(Command, 1);
    int res_len = 0;
    memset(res, 0, sizeof(res));
    u32 code = 0;
    byte send_buff[3] = { 0x19, 0x02, 0xFF };
    FaultCodeAdorner _FaultCodeAdorner;

    res_len =  (int)OBDii.Send_Receive(sizeof(send_buff), send_buff, res, sizeof(res));

    if (res_len < 7 || res[0] == 0x7F)
    {
        Manager.BreakFlag = true;
        Command->State.IsFaulty = true;
        return true;
    }

    for (int i = 3; (i + 3) < res_len; i += 4)
    {
      code = 0;
			code |= (res[i] << 16);
			code |= (res[i + 1] << 8);
			code |= res[i + 2];

        if (code == 0xFFFF || code == 0x0000)
            continue;

        byte state = res[i + 3];

        if (_FaultCodeAdorner.CheckFault(code, state, Command))
        {
           if ((state & 0x0F) == 0x09)
            {
                _FaultCodeAdorner.AddFault(code,null,GetDTCCategory(code, Command->DB->FaultCodeLength),GetDTCinCategoryMode(code,Command->DB->FaultCodeLength));
            }
        }
    }

    _FaultCodeAdorner.ShowFaults(Command, true);
    Manager.BreakFlag = true;
    return true;
}	
