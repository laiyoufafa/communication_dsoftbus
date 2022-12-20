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

#include <gtest/gtest.h>
#include <securec.h>

#include "lnn_local_net_ledger.h"
#include "lnn_net_builder.h"
#include "lnn_net_builder.c"
#include "lnn_net_builder_deps_mock.h"
#include "softbus_common.h"
#include "softbus_errcode.h"
#include "softbus_log.h"

namespace OHOS {
using namespace testing::ext;
constexpr int32_t LOCAL_WEIGHT = 10;
constexpr char NODE_UDID[] = "123456ABCDEF";
constexpr char NODE_NETWORK_ID[] = "235689BNHFCF";
constexpr char NODE1_NETWORK_ID[] = "345678BNHFCF";
constexpr uint8_t MSG[] = "123456BNHFCF";
constexpr int64_t AUTH_META_ID = 1;
constexpr char INVALID_UDID[] =
    "ASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJK\
    LPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJ\
    KLPASDFGHJKLPASDFGHJKLPASDFGHJKLPASDFGHJKLP";
constexpr uint32_t ADDR_LEN1 = 15;
constexpr uint32_t ADDR_LEN2 = 12;
constexpr int64_t AUTH_ID = 10;
using namespace testing;
class LnnNetBuilderMockTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void LnnNetBuilderMockTest::SetUpTestCase() {}

void LnnNetBuilderMockTest::TearDownTestCase() {}

void LnnNetBuilderMockTest::SetUp() {}

void LnnNetBuilderMockTest::TearDown() {}

/*
* @tc.name: LNN_INIT_NET_BUILDER_TEST_001
* @tc.desc: lnn init netbuilder test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, LNN_INIT_NET_BUILDER_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnInitSyncInfoManager())
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnInitTopoManager()).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, SoftbusGetConfig(_, _, _)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, RegAuthVerifyListener(_))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnRegSyncInfoHandler(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGenLocalNetworkId(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGenLocalUuid(_, _)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnSetLocalStrInfo(_, _)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_TRUE(LnnInitNetBuilder() == SOFTBUS_ERR);
    EXPECT_TRUE(LnnInitNetBuilder() == SOFTBUS_ERR);
    EXPECT_TRUE(LnnInitNetBuilder() == SOFTBUS_ERR);
    EXPECT_TRUE(LnnInitNetBuilder() == SOFTBUS_ERR);
    EXPECT_TRUE(LnnInitNetBuilder() == SOFTBUS_ERR);
}

/*
* @tc.name: CONFIG_LOCAL_LEDGER_TEST_001
* @tc.desc: config local ledger test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, CONFIG_LOCAL_LEDGER_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnGenLocalNetworkId(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGenLocalUuid(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnSetLocalStrInfo(_, _)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_TRUE(ConifgLocalLedger() == SOFTBUS_ERR);
    EXPECT_TRUE(ConifgLocalLedger() == SOFTBUS_ERR);
    EXPECT_TRUE(ConifgLocalLedger() == SOFTBUS_OK);
}

/*
* @tc.name: LNN_FILL_NODE_INFO_TEST_001
* @tc.desc: test FillNodeInfo
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, LNN_FILL_NODE_INFO_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> uuidMock;
    EXPECT_CALL(uuidMock, AuthGetDeviceUuid(_,_,_)).WillRepeatedly(Return(SOFTBUS_OK));
    MetaJoinRequestNode metaNode;
    NodeInfo info;
    info.uuid[0] = 'x';
    metaNode.addr.type = CONNECTION_ADDR_ETH;
    EXPECT_TRUE(FillNodeInfo(&metaNode, &info) == SOFTBUS_OK);
    EXPECT_CALL(uuidMock, AuthGetDeviceUuid(_,_,_)).WillRepeatedly(Return(SOFTBUS_OK));
    metaNode.addr.type = CONNECTION_ADDR_WLAN;
    EXPECT_TRUE(FillNodeInfo(&metaNode, &info) == SOFTBUS_OK);
    EXPECT_CALL(uuidMock, AuthGetDeviceUuid(_,_,_)).WillRepeatedly(Return(SOFTBUS_OK));
    metaNode.addr.type = CONNECTION_ADDR_BR;
    EXPECT_TRUE(FillNodeInfo(&metaNode, &info) == SOFTBUS_OK);
}

/*
* @tc.name: LNN_INIT_NET_BUILDER_DELAY_TEST_001
* @tc.desc: lnn init netbuilder delay test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, LNN_INIT_NET_BUILDER_DELAY_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnSetLocalStrInfo(_, _)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGetLocalWeight()).WillRepeatedly(Return(LOCAL_WEIGHT));
    EXPECT_CALL(NetBuilderMock, LnnSetLocalNumInfo(_, _)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnInitFastOffline())
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_TRUE(LnnInitNetBuilderDelay() == SOFTBUS_ERR);
    EXPECT_TRUE(LnnInitNetBuilderDelay() == SOFTBUS_OK);
    EXPECT_TRUE(LnnInitNetBuilderDelay() == SOFTBUS_OK);
}

/*
* @tc.name: LNN_LEAVE_META_TO_LEDGER_TEST_001
* @tc.desc: leave meta info to ledger
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, LNN_LEAVE_META_TO_LEDGER_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    MetaJoinRequestNode metaInfo;
    NodeInfo nodeInfo;
    EXPECT_CALL(NetBuilderMock, LnnGetNodeInfoById(_, _))
        .WillOnce(Return(nullptr))
        .WillRepeatedly(Return(&nodeInfo));
    EXPECT_CALL(NetBuilderMock, LnnGetDeviceUdid(_)).WillRepeatedly(Return(NODE_UDID));
    EXPECT_CALL(NetBuilderMock, LnnDeleteMetaInfo(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    LeaveMetaInfoToLedger(&metaInfo, nullptr);
    LeaveMetaInfoToLedger(&metaInfo, nullptr);
    LeaveMetaInfoToLedger(&metaInfo, nullptr);
}

/*
* @tc.name: POST_JOIN_REQUEST_TO_META_NODE_TEST_001
* @tc.desc: post join request to meta node test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, POST_JOIN_REQUEST_TO_META_NODE_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    MetaJoinRequestNode metaJoinNode;
    CustomData customData;
    EXPECT_CALL(NetBuilderMock, OnJoinMetaNode(_, _))
        .WillOnce(Return(SOFTBUS_OK))
        .WillRepeatedly(Return(SOFTBUS_ERR));
    EXPECT_CALL(NetBuilderMock, MetaNodeNotifyJoinResult(_, _, _)).WillRepeatedly(Return());
    EXPECT_TRUE(PostJoinRequestToMetaNode(&metaJoinNode, nullptr, nullptr, true) == SOFTBUS_OK);
    EXPECT_TRUE(PostJoinRequestToMetaNode(&metaJoinNode, nullptr, &customData, true) == SOFTBUS_ERR);
    EXPECT_TRUE(PostJoinRequestToMetaNode(&metaJoinNode, nullptr, &customData, false) == SOFTBUS_ERR);
}

/*
* @tc.name: LNN_NOTIFY_AUTH_HANDLE_LEAVE_LNN_TEST_001
* @tc.desc: lnn notify auth handle leave lnn test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, LNN_NOTIFY_AUTH_HANDLE_LEAVE_LNN_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, AuthHandleLeaveLNN(_)).WillRepeatedly(Return());
    g_netBuilder.isInit = true;
    int64_t authId = AUTH_ID;
    EXPECT_TRUE(LnnNotifyAuthHandleLeaveLNN(authId) == SOFTBUS_OK);
}

/*
* @tc.name: LNN_UPDATE_NODE_ADDR_TEST_001
* @tc.desc: lnn update node addr test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, LNN_UPDATE_NODE_ADDR_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnGetLocalStrInfo(_, _, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnSetLocalStrInfo(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGetAllOnlineNodeInfo(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnNotifyNodeAddressChanged(_)).WillRepeatedly(Return());
    EXPECT_TRUE(LnnUpdateNodeAddr(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(LnnUpdateNodeAddr(NODE_NETWORK_ID) == SOFTBUS_ERR);
    EXPECT_TRUE(LnnUpdateNodeAddr(NODE_NETWORK_ID) == SOFTBUS_OK);
    EXPECT_TRUE(LnnUpdateNodeAddr(NODE_NETWORK_ID) == SOFTBUS_OK);
    EXPECT_TRUE(LnnUpdateNodeAddr(NODE_NETWORK_ID) == SOFTBUS_OK);
}

/*
* @tc.name: NODE_INFO_SYNC_TEST_001
* @tc.desc: node info sync test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, NODE_INFO_SYNC_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnInitP2p())
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnInitNetworkInfo())
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnInitDevicename())
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnInitOffline())
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_TRUE(NodeInfoSync() == SOFTBUS_ERR);
    EXPECT_TRUE(NodeInfoSync() == SOFTBUS_ERR);
    EXPECT_TRUE(NodeInfoSync() == SOFTBUS_ERR);
    EXPECT_TRUE(NodeInfoSync() == SOFTBUS_ERR);
    EXPECT_TRUE(NodeInfoSync() == SOFTBUS_OK);
}

/*
* @tc.name: ON_RECEIVER_NODE_ADDR_CHANGED_MSG_TEST_001
* @tc.desc: on receive node addr changed msg test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, ON_RECEIVER_NODE_ADDR_CHANGED_MSG_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    OnReceiveNodeAddrChangedMsg(LNN_INFO_TYPE_NODE_ADDR, nullptr, MSG, ADDR_LEN1);
    OnReceiveNodeAddrChangedMsg(LNN_INFO_TYPE_NODE_ADDR, nullptr, MSG, ADDR_LEN2);
    OnReceiveNodeAddrChangedMsg(LNN_INFO_TYPE_NODE_ADDR, nullptr, MSG, ADDR_LEN2);
}

/*
* @tc.name: ON_DEVICE_NOT_TRUSTED_TEST_001
* @tc.desc: on device not trusted test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, ON_DEVICE_NOT_TRUSTED_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    SoftBusLooper loop;
    EXPECT_CALL(NetBuilderMock, LnnGetOnlineStateById(_, _))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(NetBuilderMock, LnnGetAllAuthSeq(_, _, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnSendNotTrustedInfo(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, GetLooper(_)).WillRepeatedly(Return(&loop));
    EXPECT_CALL(NetBuilderMock, LnnAsyncCallbackDelayHelper(_, _, _, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    OnDeviceNotTrusted(nullptr);
    OnDeviceNotTrusted(INVALID_UDID);
    OnDeviceNotTrusted(NODE_UDID);
    OnDeviceNotTrusted(NODE_UDID);
    OnDeviceNotTrusted(NODE_UDID);
    OnDeviceNotTrusted(NODE_UDID);
    OnDeviceNotTrusted(NODE_UDID);
}

/*
* @tc.name: ON_DEVICE_VERIFY_PASS_TEST_001
* @tc.desc: on device verify pass test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, ON_DEVICE_VERIFY_PASS_TEST_001, TestSize.Level1)
{
    NodeInfo info;
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, AuthGetConnInfo(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGetLocalStrInfo(_, _, _)).WillRepeatedly(Return(SOFTBUS_ERR));
    EXPECT_CALL(NetBuilderMock, LnnConvertAuthConnInfoToAddr(_, _, _))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    OnDeviceVerifyPass(AUTH_META_ID, &info);
    OnDeviceVerifyPass(AUTH_META_ID, &info);
}

/*
* @tc.name: GET_CURRENT_CONNECT_TYPE_TEST_001
* @tc.desc: get current connect type test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, GET_CURRENT_CONNECT_TYPE_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnGetLocalStrInfo(_, _, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGetAddrTypeByIfName(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_TRUE(GetCurrentConnectType() == CONNECTION_ADDR_MAX);
    EXPECT_TRUE(GetCurrentConnectType() == CONNECTION_ADDR_MAX);
    EXPECT_TRUE(GetCurrentConnectType() == CONNECTION_ADDR_MAX);
}

/*
* @tc.name: NET_BUILDER_MESSAGE_HANDLER_TEST_001
* @tc.desc: net builder message handler test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, NET_BUILDER_MESSAGE_HANDLER_TEST_001, TestSize.Level1)
{
    SoftBusMessage msg;
    msg.what = MSG_TYPE_MAX;
    NetBuilderMessageHandler(nullptr);
    NetBuilderMessageHandler(&msg);
}

/*
* @tc.name: PROCESS_ON_AUTH_META_VERIFY_TEST_001
* @tc.desc: process on auth meta verify failed and passed test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_ON_AUTH_META_VERIFY_FAILED_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, MetaNodeNotifyJoinResult(_, _, _)).WillRepeatedly(Return());
    EXPECT_TRUE(ProcessOnAuthMetaVerifyFailed(nullptr) == SOFTBUS_ERR);
    EXPECT_TRUE(ProcessOnAuthMetaVerifyPassed(nullptr) == SOFTBUS_ERR);
}

/*
* @tc.name: PROCESS_LEAVE_SPECIFIC_TEST_001
* @tc.desc: process leave specific test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_LEAVE_SPECIFIC_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(SpecificLeaveMsgPara)));
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnSendLeaveRequestToConnFsm(_)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_TRUE(ProcessLeaveSpecific(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessLeaveSpecific(para) == SOFTBUS_OK);
}

/*
* @tc.name: PROCESS_LEAVE_BY_ADDR_TYPE_TEST_001
* @tc.desc: process leave by addr type test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_LEAVE_BY_ADDR_TYPE_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(bool)));
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnSendLeaveRequestToConnFsm(_)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnNotifyAllTypeOffline(_)).WillRepeatedly(Return());
    EXPECT_TRUE(ProcessLeaveByAddrType(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessLeaveByAddrType(para) == SOFTBUS_OK);
}

/*
* @tc.name: PROCESS_ELETE_TEST_001
* @tc.desc: process elect test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_ELETE_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(ElectMsgPara)));
    EXPECT_TRUE(ProcessMasterElect(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessMasterElect(para) == SOFTBUS_ERR);
}

/*
* @tc.name: PROCESS_NODE_STATE_CHANGED_TEST_001
* @tc.desc: process node state changed test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_NODE_STATE_CHANGED_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(ConnectionAddr)));
    EXPECT_TRUE(ProcessNodeStateChanged(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessNodeStateChanged(para) == SOFTBUS_ERR);
}

/*
* @tc.name: TRY_ELECT_NODE_OFFLINE_TEST_001
* @tc.desc: try elect node offline test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, TRY_ELECT_NODE_OFFLINE_TEST_001, TestSize.Level1)
{
    LnnConnectionFsm connFsm;
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnGetLocalStrInfo(_, _, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_TRUE(TryElectMasterNodeOffline(&connFsm) == SOFTBUS_ERR);
    EXPECT_TRUE(TryElectMasterNodeOffline(&connFsm) == SOFTBUS_OK);
}

/*
* @tc.name: TRY_ELECT_NODE_ONLINE_TEST_001
* @tc.desc: try elect node online test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, TRY_ELECT_NODE_ONLINE_TEST_001, TestSize.Level1)
{
    LnnConnectionFsm connFsm;
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnGetLocalStrInfo(_, _, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGetLocalNumInfo(_, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGetRemoteStrInfo(_, _, _, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnGetRemoteNumInfo(_, _, _))
        .WillOnce(Return(SOFTBUS_ERR))
        .WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_CALL(NetBuilderMock, LnnCompareNodeWeight(_, _, _, _)).WillRepeatedly(Return(0));
    EXPECT_TRUE(TryElectMasterNodeOnline(&connFsm) == SOFTBUS_ERR);
    EXPECT_TRUE(TryElectMasterNodeOnline(&connFsm) == SOFTBUS_ERR);
    EXPECT_TRUE(TryElectMasterNodeOnline(&connFsm) == SOFTBUS_ERR);
    EXPECT_TRUE(TryElectMasterNodeOnline(&connFsm) == SOFTBUS_ERR);
    EXPECT_TRUE(TryElectMasterNodeOnline(&connFsm) == SOFTBUS_OK);
}

/*
* @tc.name: PROCESS_LEAVE_INVALID_CONN_TEST_001
* @tc.desc: process leave invalid conn test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_LEAVE_INVALID_CONN_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(LeaveInvalidConnMsgPara)));
    EXPECT_TRUE(ProcessLeaveInvalidConn(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessLeaveInvalidConn(para) == SOFTBUS_OK);
}

/*
* @tc.name: IS_INVALID_CONNECTION_FSM_TEST_001
* @tc.desc: is invalid connection fsm test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, IS_INVALID_CONNECTION_FSM_TEST_001, TestSize.Level1)
{
    LnnConnectionFsm connFsm;
    (void)memset_s(&connFsm, sizeof(LnnConnectionFsm), 0, sizeof(LnnConnectionFsm));
    (void)strncpy_s(connFsm.connInfo.peerNetworkId, NETWORK_ID_BUF_LEN, NODE_NETWORK_ID, strlen(NODE_NETWORK_ID));
    LeaveInvalidConnMsgPara msgPara;
    (void)memset_s(&msgPara, sizeof(LeaveInvalidConnMsgPara), 0, sizeof(LeaveInvalidConnMsgPara));
    (void)strncpy_s(msgPara.oldNetworkId, NETWORK_ID_BUF_LEN, NODE_NETWORK_ID, strlen(NODE1_NETWORK_ID));
    EXPECT_TRUE(IsInvalidConnectionFsm(&connFsm, &msgPara) == false);
    (void)memset_s(&msgPara, sizeof(LeaveInvalidConnMsgPara), 0, sizeof(LeaveInvalidConnMsgPara));
    (void)strncpy_s(msgPara.oldNetworkId, NETWORK_ID_BUF_LEN, NODE_NETWORK_ID, strlen(NODE_NETWORK_ID));
    connFsm.isDead = true;
    EXPECT_TRUE(IsInvalidConnectionFsm(&connFsm, &msgPara) == false);
    connFsm.isDead = false;
    msgPara.addrType = CONNECTION_ADDR_WLAN;
    connFsm.connInfo.addr.type = CONNECTION_ADDR_BR;
    EXPECT_TRUE(IsInvalidConnectionFsm(&connFsm, &msgPara) == false);
    msgPara.addrType = CONNECTION_ADDR_MAX;
    connFsm.connInfo.flag = 0;
    EXPECT_TRUE(IsInvalidConnectionFsm(&connFsm, &msgPara) == false);
    connFsm.connInfo.flag = 1;
    EXPECT_TRUE(IsInvalidConnectionFsm(&connFsm, &msgPara) == false);
}

/*
* @tc.name: PROCESS_SYNC_OFFLINE_FINISH_TEST_001
* @tc.desc: process sync offline finish test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_SYNC_OFFLINE_FINISH_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(char) * NETWORK_ID_BUF_LEN));
    EXPECT_TRUE(ProcessSyncOfflineFinish(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessSyncOfflineFinish(para) == SOFTBUS_OK);
}

/*
* @tc.name: PROCESS_LEAVE_META_NODE_REQUEST_TEST_001
* @tc.desc: process leave meta node request test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_LEAVE_META_NODE_REQUEST_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(char) * NETWORK_ID_BUF_LEN));
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, MetaNodeNotifyLeaveResult(_, _)).WillRepeatedly(Return());
    EXPECT_TRUE(ProcessLeaveMetaNodeRequest(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessLeaveMetaNodeRequest(para) == SOFTBUS_ERR);
}

/*
* @tc.name: PROCESS_LEAVE_LNN_REQUEST_TEST_001
* @tc.desc: process leave lnn request test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_LEAVE_LNN_REQUEST_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(char) * NETWORK_ID_BUF_LEN));
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnSendLeaveRequestToConnFsm(_)).WillRepeatedly(Return(SOFTBUS_ERR));
    EXPECT_CALL(NetBuilderMock, LnnNotifyLeaveResult(_, _)).WillRepeatedly(Return());
    EXPECT_TRUE(ProcessLeaveLNNRequest(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessLeaveLNNRequest(para) == SOFTBUS_ERR);
}

/*
* @tc.name: PROCESS_DEVICE_NOT_TRUSTED_TEST_001
* @tc.desc: process device not trusted test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_DEVICE_NOT_TRUSTED_TEST_001, TestSize.Level1)
{
    NiceMock<NetBuilderDepsInterfaceMock> NetBuilderMock;
    EXPECT_CALL(NetBuilderMock, LnnGetNetworkIdByUdid(_, _, _)).WillRepeatedly(Return(SOFTBUS_OK));
    EXPECT_TRUE(ProcessDeviceNotTrusted(nullptr) == SOFTBUS_INVALID_PARAM);
}

/*
* @tc.name: PROCESS_DEVICE_DISCONNECT_TEST_001
* @tc.desc: process device disconnect test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_DEVICE_DISCONNECT_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(int64_t)));
    EXPECT_TRUE(ProcessDeviceDisconnect(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessDeviceDisconnect(para) == SOFTBUS_ERR);
}

/*
* @tc.name: PROCESS_DEVICE_VERIFY_PASS_TEST_001
* @tc.desc: process device verify pass test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_DEVICE_VERIFY_PASS_TEST_001, TestSize.Level1)
{
    DeviceVerifyPassMsgPara *msgPara =
        reinterpret_cast<DeviceVerifyPassMsgPara *>(SoftBusMalloc(sizeof(DeviceVerifyPassMsgPara)));
    msgPara->nodeInfo = NULL;
    void *para = reinterpret_cast<void *>(msgPara);
    EXPECT_TRUE(ProcessDeviceVerifyPass(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessDeviceVerifyPass(para) == SOFTBUS_INVALID_PARAM);
}

/*
* @tc.name: PROCESS_VERIFY_RESULT_TEST_001
* @tc.desc: process verify result test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_VERIFY_RESULT_TEST_001, TestSize.Level1)
{
    VerifyResultMsgPara *msgPara1 = reinterpret_cast<VerifyResultMsgPara *>(SoftBusMalloc(sizeof(VerifyResultMsgPara)));
    msgPara1->nodeInfo = NULL;
    void *para1 = reinterpret_cast<void *>(msgPara1);
    EXPECT_TRUE(ProcessVerifyResult(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessVerifyResult(para1) == SOFTBUS_INVALID_PARAM);
    VerifyResultMsgPara *msgPara2 = reinterpret_cast<VerifyResultMsgPara *>(SoftBusMalloc(sizeof(VerifyResultMsgPara)));
    msgPara2->nodeInfo = reinterpret_cast<NodeInfo *>(SoftBusMalloc(sizeof(NodeInfo)));
    void *para2 = reinterpret_cast<void *>(msgPara2);
    EXPECT_TRUE(ProcessVerifyResult(para2) == SOFTBUS_ERR);
}

/*
* @tc.name: PROCESS_CLEAN_CONNECTION_FSM_TEST_001
* @tc.desc: process clean connection fsm test
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LnnNetBuilderMockTest, PROCESS_CLEAN_CONNECTION_FSM_TEST_001, TestSize.Level1)
{
    void *para = reinterpret_cast<void *>(SoftBusMalloc(sizeof(uint16_t)));
    EXPECT_TRUE(ProcessCleanConnectionFsm(nullptr) == SOFTBUS_INVALID_PARAM);
    EXPECT_TRUE(ProcessCleanConnectionFsm(para) == SOFTBUS_ERR);
}
} // namespace OHOS
