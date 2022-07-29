/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * miscservices under the License is miscservices on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "securec.h"
#include "softbus_error_code.h"
#include "softbus_hisysevt_common.h"
#include "softbus_hisysevt_transreporter.h"

#define STATISTIC_EVT_TRANS_OPEN_SESSION_CNT "TRANS_OPEN_SESSION_CNT"
#define STATISTIC_EVT_TRANS_OPEN_SESSION_TIME_COST "TRANS_OPEN_SESSION_TIME_COST"

#define FAULT_EVT_TRANS_FAULT "TRANS_FAULT"

#define TRANS_PARAM_SUCCESS_CNT "SUCCESS_CNT"
#define TRANS_PARAM_FAIL_CNT "FAIL_CNT"
#define TRANS_PARAM_SUCCESS_RATE "SUCCESS_RATE"

#define TRANS_PARAM_MAX_TIME_COST "MAX_TIME_COST"
#define TRANS_PARAM_MIN_TIME_COST "MIN_TIME_COST"
#define TRANS_PARAM_AVE_TIME_COST "AVE_TIME_COST"
#define TRANS_PARAM_TIMES_UNDER_500MS "TIMES_UNDER_500MS"
#define TRANS_PARAM_TIMES_BETWEEN_500MS_1S "TIMES_BETWEEN_500MS_1S"
#define TRANS_PARAM_TIMES_BETWEEN_1S_2S "TIMES_BETWEEN_1S_2S"
#define TRANS_PARAM_TIMES_ABOVE_2S "TIMES_ABOVE_2S"

#define TRANS_PARAM_ERRCODE "ERROR_CODE"

#define TIME_COST_500MS (500)
#define TIME_COST_1S (1000)
#define TIME_COST_2S (2000)

typedef struct {
    uint32_t failCnt;
    uint32_t successCnt;
    float successRate;
}OpenSessionCntStruct;

typedef struct {
    uint32_t maxTimeCost;
    uint32_t minTimeCost;
    uint32_t aveTimeCost;
    uint32_t timesIn500ms;
    uint32_t timesIn500and1s;
    uint32_t timesIn1and2s;
    uint32_t timesOn2s;
}OpenSessionTimeStruct;

static OpenSessionCntStruct g_openSessionCnt;
static OpenSessionTimeStruct g_openSessionTime;

void SoftbusRecordOpenSession(SoftBusOpenSessionStatus isSucc, uint32_t time)
{
    g_openSessionCnt.failCnt += (isSucc != SOFTBUS_EVT_OPEN_SESSION_SUCC);
    g_openSessionCnt.successCnt += (isSucc == SOFTBUS_EVT_OPEN_SESSION_SUCC);
    int totalCnt = g_openSessionCnt.failCnt + g_openSessionCnt.successCnt;
    g_openSessionCnt.successRate = (float)(g_openSessionCnt.successCnt)/(float)(totalCnt);

    if (isSucc != SOFTBUS_EVT_OPEN_SESSION_SUCC) {
        return;
    }
    
    if (time > g_openSessionTime.maxTimeCost) {
        g_openSessionTime.maxTimeCost = time;
    } else if (time < g_openSessionTime.minTimeCost) {
        g_openSessionTime.minTimeCost = time;
    }

    uint64_t totalTimeCost = (g_openSessionTime.aveTimeCost) * (g_openSessionCnt.successCnt - 1) + time;
    g_openSessionTime.aveTimeCost = (uint32_t)(totalTimeCost / g_openSessionCnt.successCnt);

    if (time < TIME_COST_500MS) {
        g_openSessionTime.timesIn500ms++;
    } else if (time < TIME_COST_1S) {
        g_openSessionTime.timesIn500and1s++;
    } else if (time < TIME_COST_2S) {
        g_openSessionTime.timesIn1and2s++;
    } else {
        g_openSessionTime.timesOn2s++;
    }
}

static inline void clearOpenSessionCnt(void)
{
    memset_s(&g_openSessionCnt, sizeof(OpenSessionCntStruct), 0, sizeof(OpenSessionCntStruct));
}

static inline void clearOpenSessionTime(void)
{
    memset_s(&g_openSessionTime, sizeof(OpenSessionTimeStruct), 0, sizeof(OpenSessionTimeStruct));
}

static void CreateOpenSessionCntMsg(SoftBusEvtReportMsg* msg)
{
    // event
    strcpy_s(msg->evtName, SOFTBUS_HISYSEVT_NAME_LEN + 1, STATISTIC_EVT_TRANS_OPEN_SESSION_CNT);
    msg->evtType = SOFTBUS_EVT_TYPE_STATISTIC;
    msg->paramNum = SOFTBUS_EVT_PARAM_THREE;

    // param 0
    SoftBusEvtParam* param = &msg->paramArray[SOFTBUS_EVT_PARAM_ZERO];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_SUCCESS_CNT);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = g_openSessionCnt.successCnt;

    // param 1
    param = &msg->paramArray[SOFTBUS_EVT_PARAM_ONE];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_FAIL_CNT);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = (g_openSessionCnt.failCnt);

    // param 2
    param = &msg->paramArray[SOFTBUS_EVT_PARAM_TWO];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_SUCCESS_RATE);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_FLOAT;
    param->paramValue.f = g_openSessionCnt.successRate;
}


static int32_t SoftbusReportOpenSessionCntEvt()
{
    SoftBusEvtReportMsg* msg = SoftbusCreateEvtReportMsg(SOFTBUS_EVT_PARAM_THREE);
    if (msg == NULL) {
        return SOFTBUS_ERR;
    }
    CreateOpenSessionCntMsg(msg);
    int ret = SoftbusWriteHisEvt(msg);
    SoftbusFreeEvtReporMsg(msg);

    clearOpenSessionCnt();

    return ret;
}

static void CreateOpenSessionTimeMsg(SoftBusEvtReportMsg* msg)
{
    // event
    strcpy_s(msg->evtName, SOFTBUS_HISYSEVT_NAME_LEN + 1, STATISTIC_EVT_TRANS_OPEN_SESSION_TIME_COST);
    msg->evtType = SOFTBUS_EVT_TYPE_STATISTIC;
    msg->paramNum = SOFTBUS_EVT_PARAM_SEVEN;

    // param 0
    SoftBusEvtParam* param = &msg->paramArray[SOFTBUS_EVT_PARAM_ZERO];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_MAX_TIME_COST);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = g_openSessionTime.maxTimeCost;

    // param 1
    param = &msg->paramArray[SOFTBUS_EVT_PARAM_ONE];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_MIN_TIME_COST);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = g_openSessionTime.minTimeCost;

    // param 2
    param = &msg->paramArray[SOFTBUS_EVT_PARAM_TWO];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_AVE_TIME_COST);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = g_openSessionTime.aveTimeCost;

    // param 3
    param = &msg->paramArray[SOFTBUS_EVT_PARAM_THREE];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_TIMES_UNDER_500MS);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = g_openSessionTime.timesIn500ms;

    // param 4
    param = &msg->paramArray[SOFTBUS_EVT_PARAM_FOUR];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_TIMES_BETWEEN_500MS_1S);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = g_openSessionTime.timesIn500and1s;

    // param 5
    param = &msg->paramArray[SOFTBUS_EVT_PARAM_FIVE];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_TIMES_BETWEEN_1S_2S);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = g_openSessionTime.timesIn1and2s;

    // param 6
    param = &msg->paramArray[SOFTBUS_EVT_PARAM_SIX];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_TIMES_ABOVE_2S);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_UINT32;
    param->paramValue.u32v = g_openSessionTime.timesOn2s;
}

static int32_t SoftbusReportOpenSessionTimeEvt()
{
    SoftBusEvtReportMsg* msg = SoftbusCreateEvtReportMsg(SOFTBUS_EVT_PARAM_SEVEN);
    if (msg == NULL) {
        return SOFTBUS_ERR;
    }
    CreateOpenSessionTimeMsg(msg);
    int ret = SoftbusWriteHisEvt(msg);
    SoftbusFreeEvtReporMsg(msg);

    clearOpenSessionTime();

    return ret;
}

static inline void CreateTransErrMsg(SoftBusEvtReportMsg* msg, int32_t errcode)
{
    // event
    strcpy_s(msg->evtName, SOFTBUS_HISYSEVT_NAME_LEN + 1, FAULT_EVT_TRANS_FAULT);
    msg->evtType = SOFTBUS_EVT_TYPE_FAULT;
    msg->paramNum = SOFTBUS_EVT_PARAM_ONE;

    // param 0
    SoftBusEvtParam* param = &msg->paramArray[SOFTBUS_EVT_PARAM_ZERO];
    strcpy_s(param->paramName, SOFTBUS_HISYSEVT_NAME_LEN + 1, TRANS_PARAM_ERRCODE);
    param->paramType = SOFTBUS_EVT_PARAMTYPE_INT32;
    param->paramValue.i32v = errcode;
}

int32_t SoftbusReportTransErrorEvt(int32_t errcode)
{
    SoftBusEvtReportMsg* msg = SoftbusCreateEvtReportMsg(SOFTBUS_EVT_PARAM_ONE);
    if (msg == NULL) {
        return SOFTBUS_ERR;
    }
    CreateTransErrMsg(msg, errcode);
    int ret = SoftbusWriteHisEvt(msg);
    SoftbusFreeEvtReporMsg(msg);

    return ret;
}

void InitTransStatisticSysEvt(void)
{
    clearOpenSessionCnt();
    clearOpenSessionTime();

    SetStatisticEvtReportFunc(SOFTBUS_STATISTIC_EVT_TRANS_OPEN_SESSION_CNT, SoftbusReportOpenSessionCntEvt);
    SetStatisticEvtReportFunc(SOFTBUS_STATISTIC_EVT_TRANS_OPEN_SESSION_TIME_COST, SoftbusReportOpenSessionTimeEvt);
}