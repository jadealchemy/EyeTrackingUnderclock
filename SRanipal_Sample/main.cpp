#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <windows.h>
#include "SRanipal.h"
#include "SRanipal_Eye.h"
#include "SRanipal_Enums.h"
#include "SRanipal_NotRelease.h"
#pragma comment (lib, "SRanipal.lib")
using namespace ViveSR;

#define EnableEyeTracking 1
#define DisableEyeTracking 0

int howlong;
int clockspeedy;

bool GetBitMaskValidation(uint64_t mask, ViveSR::anipal::Eye::SingleEyeDataValidity SingleEyeDataType);
std::string CovertErrorCode(int error);

std::thread *t = nullptr;
bool EnableEyeV2 = true;
bool looping = true;
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
                if (gaze[2] == 0) {
                    if (howlong == 100) {
                    }
                    else {
                        howlong = howlong + 1;
                    }

                }
                if (gaze[2] != 0) {
                    if (howlong == 0) {
                    }
                    else {
                        if (howlong >= 10) {
                            howlong = howlong - 10;
                        }
                        else {
                            howlong = howlong - howlong;
                        }
                    }
                }

                if (howlong == 100) {
                    if (clockspeedy != 1) {
                        system("CLS");
                        

                        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 50");
                        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 50");
                        system("powercfg.exe -setactive SCHEME_MAX");


                        printf("Status: Low Speed\n\n");
                        printf("Press 0 then enter to exit safely.\n\n");
                        printf("Eyetracking underclocking proof of concept.\nYour PC should underclock after 10 seconds of your left eye being closed or untrackable\nUse included 'Fix clock speed.bat' if your clockspeeds get stuck underclocked.\n\n");
                        printf("\nWait for key event :");
                        clockspeedy = 1;
                    }
                }
                if (howlong == 0) {
                    if (clockspeedy != 0) {
                        system("CLS");


                        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMIN 100");
                        system("powercfg -setacvalueindex SCHEME_MAX SUB_PROCESSOR PROCTHROTTLEMAX 100");
                        system("powercfg.exe -setactive SCHEME_MAX");

                        printf("Status: High Speed\n\n");
                        printf("Press 0 then enter to exit safely.\n\n");
                        printf("Eyetracking underclocking proof of concept.\nYour PC should underclock after 10 seconds of your left eye being closed or untrackable\nUse included 'Fix clock speed.bat' if your clockspeeds get stuck underclocked.\n\n");
                        printf("\nWait for key event :");
                        clockspeedy = 0;
                    }
                }
            }
        }
	}
}

int main() {
    int error, id = NULL;
    printf("Initializing eye tracking...\n");
    error = ViveSR::anipal::Initial(ViveSR::anipal::Eye::ANIPAL_TYPE_EYE_V2, NULL);
    if (error == ViveSR::Error::WORK) {
        EnableEyeV2 = true;
        printf("Successfully initialized eye tracking.\n");
    }
    else {
        printf("Fail to initialize version2 Eye engine. please refer the code %d %s.\n", error, CovertErrorCode(error).c_str());
        return -1;
    }

//    if (t == nullptr) {
        t = new std::thread(streaming);
        if (t)   looping = true;
//    }

    Sleep(1500);
    system("CLS");

    printf("Status: No interaction yet.\n\n");
	printf("Press 0 then enter to exit safely.\n\n");
    printf("Eyetracking underclocking proof of concept.\nYour PC should underclock after 10 seconds of your left eye being closed or untrackable\nUse included 'Fix clock speed.bat' if your clockspeeds get stuck underclocked.\n\n");
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