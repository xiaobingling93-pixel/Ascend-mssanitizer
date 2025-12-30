/* -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2025 Huawei Technologies Co.,Ltd.
 *
 * MindStudio is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * ------------------------------------------------------------------------- */


#include <chrono>
#include <gtest/gtest.h>
#include <thread>

#include "utility/domain_socket.h"

using namespace Sanitizer;

namespace {

// 测试基础套接字的创建和销毁
TEST(DomainSocketTest, CreateSocket) {
    std::string socketPath = "/tmp/msop_connect.202511121057.12345.sock";
    Sanitizer::DomainSocket domainSocket(socketPath);
    EXPECT_EQ(!domainSocket.CreateSocket().Fail(), true);
    EXPECT_EQ(!domainSocket.Clean().Fail(), true);
}

// 测试 DomainSocketServer 的 ListenAndBind 和 Accept 功能
TEST(DomainSocketServerTest, ListenAndAcceptClient) {
    std::string socketPath = "/tmp/msop_connect.202511121057.12345.sock";
    std::size_t maxClientNum = 1;
    Sanitizer::DomainSocketServer server(socketPath, maxClientNum);
    
    EXPECT_EQ(!server.ListenAndBind().Fail(), true);

    // 创建客户端以测试连接
    Sanitizer::DomainSocketClient client(socketPath);
    EXPECT_EQ(!client.Connect().Fail(), true);

    Sanitizer::DomainSocketServer::ClientId clientId;
    EXPECT_EQ(!server.Accept(clientId).Fail(), true);
    EXPECT_EQ(server.GetClientNum(), 1);

    EXPECT_EQ(!server.Clean().Fail(), true);
}

// 测试 DomainSocketServer 和 DomainSocketClient 的 Read 和 Write 功能
TEST(DomainSocketServerClientTest, ServerClientReadWrite) {
    std::string socketPath = "/tmp/msop_connect.202511121057.12345.sock";
    std::size_t maxClientNum = 1;
    Sanitizer::DomainSocketServer server(socketPath, maxClientNum);
    ASSERT_EQ(!server.ListenAndBind().Fail(), true);

    Sanitizer::DomainSocketClient client(socketPath);
    ASSERT_EQ(!client.Connect().Fail(), true);

    Sanitizer::DomainSocketServer::ClientId clientId;
    ASSERT_EQ(!server.Accept(clientId).Fail(), true);

    // 测试写入和读取
    std::string writeMessage = "Hello, DomainSocket!";
    size_t sentBytes;
    EXPECT_EQ(!client.Write(writeMessage, sentBytes).Fail(), true);
    EXPECT_EQ(sentBytes, writeMessage.size());

    std::string readMessage;
    size_t receivedBytes;
    EXPECT_EQ(!server.Read(clientId, readMessage, writeMessage.size(), receivedBytes).Fail(), true);
    EXPECT_EQ(receivedBytes, writeMessage.size());
    EXPECT_EQ(readMessage, writeMessage);

    // 反向通信测试
    std::string serverMessage = "Hello from server!";
    EXPECT_EQ(!server.Write(clientId, serverMessage, sentBytes).Fail(), true);
    EXPECT_EQ(sentBytes, serverMessage.size());

    EXPECT_EQ(!client.Read(readMessage, serverMessage.size(), receivedBytes).Fail(), true);
    EXPECT_EQ(receivedBytes, serverMessage.size());
    EXPECT_EQ(readMessage, serverMessage);

    EXPECT_EQ(!server.Clean().Fail(), true);
}

}  // namespace
