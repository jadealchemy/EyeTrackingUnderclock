#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <windows.h>
#include <fstream>
#include <string>
#include "SRanipal.h"
#include "SRanipal_Eye.h"
#include "SRanipal_Enums.h"
#include "SRanipal_NotRelease.h"
#include <iostream>
#pragma comment (lib, "SRanipal.lib")
using namespace ViveSR;

#define EnableEyeTracking 1
#define DisableEyeTracking 0



int howlong;
int clockstate;
int changestate;

int cpuspeedmin;
int fasttime;
int slowtime;

bool looping = true;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
        looping = false;
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 100");
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 100");
        system("powercfg.exe -setactive SCHEME_MAX");
        ViveSR::anipal::Release(ViveSR::anipal::Eye::ANIPAL_TYPE_EYE_V2);
        return 0;

        // CTRL-CLOSE: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
        looping = false;
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 100");
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 100");
        system("powercfg.exe -setactive SCHEME_MAX");
        ViveSR::anipal::Release(ViveSR::anipal::Eye::ANIPAL_TYPE_EYE_V2);
        return 0;

        // Pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
        looping = false;
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 100");
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 100");
        system("powercfg.exe -setactive SCHEME_MAX");
        ViveSR::anipal::Release(ViveSR::anipal::Eye::ANIPAL_TYPE_EYE_V2);
        return 0;

    case CTRL_LOGOFF_EVENT:
        looping = false;
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 100");
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 100");
        system("powercfg.exe -setactive SCHEME_MAX");
        ViveSR::anipal::Release(ViveSR::anipal::Eye::ANIPAL_TYPE_EYE_V2);
        return 0;

    case CTRL_SHUTDOWN_EVENT:
        looping = false;
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 100");
        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 100");
        system("powercfg.exe -setactive SCHEME_MAX");
        ViveSR::anipal::Release(ViveSR::anipal::Eye::ANIPAL_TYPE_EYE_V2);
        return 0;

    default:
        return FALSE;
    }
}



using namespace std;

string cmd1;
string cmd2;

bool GetBitMaskValidation(uint64_t mask, ViveSR::anipal::Eye::SingleEyeDataValidity SingleEyeDataType);
std::string CovertErrorCode(int error);

std::thread *t = nullptr;
bool EnableEyeV2 = true;

void streaming() {
    // Eye 
    ViveSR::anipal::Eye::EyeData_v2 eye_data_v2;

	int result = ViveSR::Error::WORK;
	while (looping) {
        if (EnableEyeV2) {
            int result = ViveSR::anipal::Eye::GetEyeData_v2(&eye_data_v2);
            if (result == ViveSR::Error::WORK) {
                float *gaze = eye_data_v2.verbose_data.left.gaze_direction_normalized.elem_;
                //printf("[Eye v2] Gaze: %.2f %.2f %.2f\n", gaze[0], gaze[1], gaze[2]);

                Sleep(100);
                changestate = 0;

                if (clockstate == 0) {
                    if (gaze[2] == 0) {
                        howlong = howlong + 1;
                    }

                    if (gaze[2] != 0) {
                        howlong = howlong - 1;
                    }
                }




                if (clockstate == 1) {
                    if (gaze[2] != 0) {
                        howlong = howlong + 1;
                    }

                    if (gaze[2] == 0) {
                        howlong = howlong - 1;
                    }
                }


                if ((howlong == slowtime) && (clockstate == 0)) {
                    howlong = 0;
                    changestate = 1;
                    clockstate = 1;
                }

                if ((howlong == fasttime) && (clockstate == 1)) {
                    howlong = 0;
                    changestate = 1;
                    clockstate = 0;
                }

                
                if (changestate == 1) {
                    if (clockstate == 1) {
                        system("CLS");
                        

                        system(cmd1.c_str());
                        system(cmd2.c_str());
                        system("powercfg.exe -setactive SCHEME_MAX");


                        printf("Status: Low Speed\n\n");
                        printf("Press 0 then enter to exit safely.\n\n");
                        printf("Eyetracking underclocking proof of concept.\nUse included 'Fix clock speed.bat' if your clockspeeds get stuck underclocked.\n\n");
                        printf("\nWait for key event :");
                        
                    }

                    if (clockstate == 0) {
                        system("CLS");


                        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 100");
                        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 100");
                        system("powercfg.exe -setactive SCHEME_MAX");

                        printf("Status: High Speed\n\n");
                        printf("Press 0 then enter to exit safely.\n\n");
                        printf("Eyetracking underclocking proof of concept.\nUse included 'Fix clock speed.bat' if your clockspeeds get stuck underclocked.\n\n");
                        printf("\nWait for key event :");
                        
                    }
                    
                }
                
            }
        }
	}
}



int main() {
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    printf("Reading config files...\n");
    fstream cpumin;
    cpumin.open("config/cpuspeedmin.txt", ios::in);

    if (cpumin.is_open() == false) {
        printf("Error reading 'cpuspeedmin.txt'!! Does the file exist?\n\n");
        Sleep(10000);
        return 2;
    }

    cpumin >> cpuspeedmin;

    if (cpuspeedmin == 0) {
        printf("Something is wrong with 'cpuspeedmin.txt'!! Did you accidentally put in something other than a number?\n'0' is also invalid.\n\n");
        Sleep(10000);
        return 3;
    }

    cmd1 = "powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN ";
    cmd2 = "powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX ";

    cmd1 += std::to_string(cpuspeedmin);
    cmd2 += std::to_string(cpuspeedmin);




    fstream hightime;
    hightime.open("config/hightime.txt", ios::in);

    if (hightime.is_open() == false) {
        printf("Error reading 'hightime.txt'!! Does the file exist?\n\n");
        Sleep(10000);
        return 4;
    }

    hightime >> fasttime;

    if (fasttime == 0) {
        printf("Something is wrong with 'hightime.txt'!! Did you accidentally put in something other than a number?\n'0' is also invalid.\n\n");
        Sleep(10000);
        return 5;
    }





    fstream lowtime;
    lowtime.open("config/lowtime.txt", ios::in);

    if (lowtime.is_open() == false) {
        printf("Error reading 'lowtime.txt'!! Does the file exist?\n\n");
        Sleep(10000);
        return 6;
    }

    lowtime >> slowtime;

    if (slowtime == 0) {
        printf("Something is wrong with 'slowtime.txt'!! Did you accidentally put in something other than a number?\n'0' is also invalid.\n\n");
        Sleep(10000);
        return 7;
    }

    Sleep(1000);
    printf("Config variables set.\n");
    Sleep(100);

    int error, id = NULL;
    printf("Initializing eye tracking...\n");
    error = ViveSR::anipal::Initial(ViveSR::anipal::Eye::ANIPAL_TYPE_EYE_V2, NULL);
    if (error == ViveSR::Error::WORK) {
        EnableEyeV2 = true;
        printf("Successfully initialized eye tracking.\n");
    }
    else {
        printf("Fail to initialize version2 Eye engine. please refer the code %d %s.\n", error, CovertErrorCode(error).c_str());
        Sleep(10000);
        return 1;
    }

    t = new std::thread(streaming);

    Sleep(1500);
    system("CLS");
    printf("Status: No interaction yet.\n\n");
	printf("Press 0 then enter to exit safely.\n\n");
    printf("Eyetracking underclocking proof of concept.\nUse included 'Fix clock speed.bat' if your clockspeeds get stuck underclocked.\n\n");
	char str = 0;  
	while (true) {
		if (str != '\n' && str != EOF) { printf("\nWait for key event :"); }
		
        str = getchar();

        if (str == '0') break;
        
        

		else if (str == '1') {
            if (t == nullptr) {
                t = new std::thread(streaming);
                if(t)   looping = true;
            }
		}
		
        else if (str == '2') {
            looping = false;
			if (t == nullptr) continue;
			t->join();
			delete t;
			t = nullptr;
		}

	}
    looping = false;
    ViveSR::anipal::Release(ViveSR::anipal::Eye::ANIPAL_TYPE_EYE_V2);
    system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 100");
    system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 100");
    system("powercfg.exe -setactive SCHEME_MAX");
    system("CLS");
    printf("Clock speeds should be restored and program will now exit.");


    return 0;
}




std::string CovertErrorCode(int error) {
    std::string result = "";
    switch (error) {
    case(RUNTIME_NOT_FOUND):     result = "RUNTIME_NOT_FOUND"; break;
    case(NOT_INITIAL):           result = "NOT_INITIAL"; break;
    case(FAILED):                result = "FAILED"; break;
    case(WORK):                  result = "WORK"; break;
    case(INVALID_INPUT):         result = "INVALID_INPUT"; break;
    case(FILE_NOT_FOUND):        result = "FILE_NOT_FOUND"; break;
    case(DATA_NOT_FOUND):        result = "DATA_NOT_FOUND"; break;
    case(UNDEFINED):             result = "UNDEFINED"; break;
    case(INITIAL_FAILED):        result = "INITIAL_FAILED"; break;
    case(NOT_IMPLEMENTED):       result = "NOT_IMPLEMENTED"; break;
    case(NULL_POINTER):          result = "NULL_POINTER"; break;
    case(OVER_MAX_LENGTH):       result = "OVER_MAX_LENGTH"; break;
    case(FILE_INVALID):          result = "FILE_INVALID"; break;
    case(UNINSTALL_STEAM):       result = "UNINSTALL_STEAM"; break;
    case(MEMCPY_FAIL):           result = "MEMCPY_FAIL"; break;
    case(NOT_MATCH):             result = "NOT_MATCH"; break;
    case(NODE_NOT_EXIST):        result = "NODE_NOT_EXIST"; break;
    case(UNKONW_MODULE):         result = "UNKONW_MODULE"; break;
    case(MODULE_FULL):           result = "MODULE_FULL"; break;
    case(UNKNOW_TYPE):           result = "UNKNOW_TYPE"; break;
    case(INVALID_MODULE):        result = "INVALID_MODULE"; break;
    case(INVALID_TYPE):          result = "INVALID_TYPE"; break;
    case(MEMORY_NOT_ENOUGH):     result = "MEMORY_NOT_ENOUGH"; break;
    case(BUSY):                  result = "BUSY"; break;
    case(NOT_SUPPORTED):         result = "NOT_SUPPORTED"; break;
    case(INVALID_VALUE):         result = "INVALID_VALUE"; break;
    case(COMING_SOON):           result = "COMING_SOON"; break;
    case(INVALID_CHANGE):        result = "INVALID_CHANGE"; break;
    case(TIMEOUT):               result = "TIMEOUT"; break;
    case(DEVICE_NOT_FOUND):      result = "DEVICE_NOT_FOUND"; break;
    case(INVALID_DEVICE):        result = "INVALID_DEVICE"; break;
    case(NOT_AUTHORIZED):        result = "NOT_AUTHORIZED"; break;
    case(ALREADY):               result = "ALREADY"; break;
    case(INTERNAL):              result = "INTERNAL"; break;
    case(CONNECTION_FAILED):     result = "CONNECTION_FAILED"; break;
    case(ALLOCATION_FAILED):     result = "ALLOCATION_FAILED"; break;
    case(OPERATION_FAILED):      result = "OPERATION_FAILED"; break;
    case(NOT_AVAILABLE):         result = "NOT_AVAILABLE"; break;
    case(CALLBACK_IN_PROGRESS):  result = "CALLBACK_IN_PROGRESS"; break;
    case(SERVICE_NOT_FOUND):     result = "SERVICE_NOT_FOUND"; break;
    case(DISABLED_BY_USER):      result = "DISABLED_BY_USER"; break;
    case(EULA_NOT_ACCEPT):       result = "EULA_NOT_ACCEPT"; break;
    case(RUNTIME_NO_RESPONSE):   result = "RUNTIME_NO_RESPONSE"; break;
    case(OPENCL_NOT_SUPPORT):    result = "OPENCL_NOT_SUPPORT"; break;
    case(NOT_SUPPORT_EYE_TRACKING): result = "NOT_SUPPORT_EYE_TRACKING"; break;
    case(LIP_NOT_SUPPORT):       result = "LIP_NOT_SUPPORT"; break;
    default:
        result = "No such error code";
    }
    return result;
}