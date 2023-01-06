/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <atomic>
#include <mutex>
#include <gmock/gmock.h>
#include "p2plink_negotiation.h"
#include "p2plink_common.h"
#include "p2plink_json_payload.h"
#include "softbus_json_utils.h"
#include "p2plink_state_machine.h"
#include "p2plink_adapter.h"

class NegotiationInterface {
public:
    virtual bool P2pLinkIsEnable() = 0;
    virtual bool GetJsonObjectStringItem(const cJSON *json, const char * const string, char *target,
                                         uint32_t targetLen) = 0;
    virtual bool GetJsonObjectNumberItem(const cJSON *json, const char * const string, int *target) = 0;
    virtual bool P2pLinkIsDisconnectState() = 0;
    virtual void P2pLinkFsmMsgProc(const FsmStateMachine *fsm, int32_t msgType, void *param) = 0;
    virtual int32_t P2plinkUnpackRepsonseMsg(const cJSON *data, P2pContentType type, P2pRespMsg *response) = 0;
    virtual void P2pLinkFsmTransactState(FsmStateMachine *fsm, FsmState *state) = 0;
    virtual void P2pLinkFsmMsgProcDelayDel(int32_t msgType) = 0;
    virtual int32_t P2pLinkSetPeerWifiCfgInfo(const char *cfgData) = 0;
    virtual void P2pLinkRemoveGroup() = 0;
    virtual bool P2pLinkGetDhcpState() = 0;
    virtual void P2pLinkFsmMsgProcDelay(const FsmStateMachine *fsm, int32_t msgType, void *param,
                                         uint64_t delayMs) = 0;
};

class NegotiationMock : public NegotiationInterface {
public:
    static NegotiationMock* GetMock()
    {
        return mock.load();
    }

    NegotiationMock();
    ~NegotiationMock();

    void SetupSuccessStub();

    MOCK_METHOD(bool, P2pLinkIsEnable, (), (override));
    MOCK_METHOD(bool, GetJsonObjectStringItem, (const cJSON *json, const char * const string, char *target,
                uint32_t targetLen), (override));
    static bool ActionOfGetJsonObjectStringItem(const cJSON *json, const char * const string, char *target,
                                                uint32_t targetLen);
    MOCK_METHOD(bool, P2pLinkIsDisconnectState, (), (override));
    MOCK_METHOD(void, P2pLinkFsmMsgProc, (const FsmStateMachine *fsm, int32_t msgType, void *param), (override));
    static void ActionOfP2pLinkFsmMsgProc(const FsmStateMachine *fsm, int32_t msgType, void *param);
    MOCK_METHOD(bool, GetJsonObjectNumberItem, (const cJSON *json, const char * const string, int *target), (override));
    static bool ActionOfGetJsonObjectNumberItem(const cJSON *json, const char * const string, int *target);
    MOCK_METHOD(int32_t, P2plinkUnpackRepsonseMsg, (const cJSON *data, P2pContentType type, P2pRespMsg *response),
                (override));
    MOCK_METHOD(void, P2pLinkFsmTransactState, (FsmStateMachine *fsm, FsmState *state), (override));
    static void ActionOfP2pLinkFsmTransactState(FsmStateMachine *fsm, FsmState *state);
    MOCK_METHOD(void, P2pLinkFsmMsgProcDelayDel, (int32_t msgType), (override));
    static void ActionOfP2pLinkFsmMsgProcDelayDel(int32_t msgType);
    MOCK_METHOD(int32_t, P2pLinkSetPeerWifiCfgInfo, (const char *cfgData), (override));
    static int32_t ActionOfP2pLinkSetPeerWifiCfgInfo(const char *cfgData);
    MOCK_METHOD(void, P2pLinkRemoveGroup, (), (override));
    static void ActionOfP2pLinkRemoveGroup();
    MOCK_METHOD(bool, P2pLinkGetDhcpState, (), (override));
    static bool ActionOfP2pLinkGetDhcpState();
    MOCK_METHOD(void, P2pLinkFsmMsgProcDelay, (const FsmStateMachine *fsm, int32_t msgType, void *param,
                uint64_t delayMs), (override));
    static void ActionOfP2pLinkFsmMsgProcDelay(const FsmStateMachine *fsm, int32_t msgType, void *param,
                                               uint64_t delayMs);

private:
    static inline std::atomic<NegotiationMock*> mock = nullptr;
};