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

/**
 * @file dfs_demo.c
 *
 * @brief Provides the sample code for obtaining session information.
 *
 * @since 1.0
 * @version 1.0
 */

// Device A:

#include <stdio.h>
#include "session.h"
#include "softbus_def.h"
#include "softbus_config_type.h"

const char *g_pkgNameA = "dms"; // Application bundle name of device A

// Session name of the distributed file service of device A
const char *g_sessionNameA = "ohos.distributedschedule.dms.test";

// Session name of the distributed file service of device B
const char *g_sessionNameB = "ohos.distributedschedule.dms.test";

// Network ID generated by device B after devices A and B are networked
const char *g_networkidB = "ABCDEF00ABCDEF00ABCDEF00ABCDEF00ABCDEF00ABCDEF00ABCDEF00ABCDEF00";
const char *g_groupid = "TEST_GROUP_ID";  // Group ID
static SessionAttribute g_sessionAttr = {
    .dataType = TYPE_BYTES,  // Session type
};
const char *g_testData = "TranSessionTest_GetSessionKeyTestData";

// Notify that the session is set up successfully.
static int OnSessionOpened(int sessionId, int result)
{
    printf("session opened,sesison id = %d\r\n", sessionId);
    return 0;
}

// Notify that the session is closed.
static void OnSessionClosed(int sessionId)
{
    printf("session closed, session id = %d\r\n", sessionId);
}

// Notify that the byte data is received.
static void OnBytesReceived(int sessionId, const void *data, unsigned int len)
{
    printf("session bytes received, session id = %d\r\n", sessionId);
}

// Notify that the message is received.
static void OnMessageReceived(int sessionId, const void *data, unsigned int len)
{
    printf("session msg received, session id = %d\r\n", sessionId);
}

// Notify that the stream data is received.
static void OnStreamReceived(int sessionId, const StreamData *data, const StreamData *ext,
            const StreamFrameInfo *param)
{
    printf("session stream received, session id = %d\r\n", sessionId);
}

static ISessionListener g_sessionlistenerA = {
    .OnSessionOpened = OnSessionOpened,
    .OnSessionClosed = OnSessionClosed,
    .OnBytesReceived = OnBytesReceived,
    .OnMessageReceived = OnMessageReceived,
    .OnStreamReceived = OnStreamReceived,
};

int main(void)
{
    /*
     * 1. Device A calls CreateSessionServer() to create a session server based on
     * the application bundle name and session name, and registers the callbacks for
     * session opened, session closed, byte received, and message received.
     */
    int ret = CreateSessionServer(g_pkgNameA, g_sessionNameA, &g_sessionlistenerA);
    printf("create session server result = %d\n", ret);

    /*
     * 2. Device A calls OpenSession() to open a session based on the local session name,
     * peer session name, and peer network ID and determine the session channel based on the session type.
     * When the session is open, a callback will be invoked to notify devices A and B.
     * A session ID is returned for subsequent operations.
     */
    int sessionId = OpenSession(g_sessionNameA, g_sessionNameB, g_networkidB, g_groupid, &g_sessionAttr);
    printf("open session result = %d\n", sessionId);
    
    /* 3. Device A obtains session information. */
    char sessionName[SESSION_NAME_SIZE_MAX] = {0};
    ret = GetMySessionName(sessionId, sessionName, SESSION_NAME_SIZE_MAX);
    printf("get mySessionName result = %d\n", ret);
    
    char deviceId[DEVICE_ID_SIZE_MAX] = {0};
    ret = GetPeerDeviceId(sessionId, deviceId, DEVICE_ID_SIZE_MAX);
    printf("get peerDeviceId result = %d\n", ret);
    
    char peerSessionName[SESSION_NAME_SIZE_MAX] = {0};
    ret = GetPeerSessionName(sessionId, peerSessionName, SESSION_NAME_SIZE_MAX);
    printf("get peerSessionName result = %d\n", ret);
    
    ret = GetSessionSide(sessionId);
    printf("get sessionside result = %d\n", ret);
}
