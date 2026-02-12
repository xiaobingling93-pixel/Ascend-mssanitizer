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


#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <any>
#include <mutex>

/// 暴露私有成员用于单元测试
#define private public
#include "utility/log.h"
#undef private

using namespace Sanitizer;

TEST(Log, log_get_log_expect_non_nullptr)
{
    Log *log = Log::GetLog();
    ASSERT_NE(log, nullptr);
    SAN_LOG("test log");
    SAN_LOG("test log: %d", 1);
    SAN_LOG("test log: %s", "test");
}

TEST(Log, log_set_max_debuglog_size_rate_expect_no_throw)
{
    Log *log = Log::GetLog();
    EXPECT_NO_THROW(log->SetMaxDebugLogSizeRate(100));
}

TEST(Log, singleton_pattern) {
    Log* log1 = Log::GetLog();
    Log* log2 = Log::GetLog();
    ASSERT_EQ(log1, log2) << "GetLog should return the same instance";
    
    for (int i = 0; i < 10; i++) {
        Log* log = Log::GetLog();
        ASSERT_NE(log, nullptr) << "GetLog should never return null";
    }
}


TEST(Log, log_size_measurement) {
    Log log;
    
    int64_t initialSize = log.LogSize();
    ASSERT_GE(initialSize, 0) << "Log size should be non-negative";
    
    Log emptyLog;
    emptyLog.SetMaxDebugLogSizeRate(1024);
    ASSERT_GE(emptyLog.LogSize(), -1) << "Log size should handle empty log properly";
}

TEST(Log, timestamp_generation) {
    Log log;
    
    std::string timestamp = log.GetTimeStamp();
    ASSERT_FALSE(timestamp.empty()) << "Timestamp should not be empty";
    ASSERT_GT(timestamp.length(), 10) << "Timestamp should have reasonable length";
    
    for (int i = 0; i < 5; i++) {
        std::string ts = log.GetTimeStamp();
        ASSERT_EQ(ts[0], '[') << "Timestamp should start with '['";
        ASSERT_EQ(ts[ts.length() - 1], ']') << "Timestamp should end with ']'";
    }
}

TEST(Log, running_time_calculation) {
    Log log;
    
    std::string runningTime = log.GetRunningTime();
    ASSERT_FALSE(runningTime.empty()) << "Running time should not be empty";
    
    ASSERT_EQ(runningTime[0], '[') << "Running time should start with '['";
    ASSERT_EQ(runningTime[runningTime.length() - 1], ']') << "Running time should end with ']'";
}

// 测试缓冲区追加功能
TEST(Log, buffer_append_operation) {
    Log log;
    
    // 测试单线程追加
    std::string testText = "Hello, World! ";
    for (int i = 0; i < 10; i++) {
        log.AppendBuff(testText + std::to_string(i));
    }
    
    // 测试多线程追加
    auto appendTask = [&log](int threadId) {
        for (int i = 0; i < 100; i++) {
            log.AppendBuff("Thread" + std::to_string(threadId) + "-" + std::to_string(i) + " ");
        }
    };
    
    std::thread t1(appendTask, 1);
    std::thread t2(appendTask, 2);
    std::thread t3(appendTask, 3);
    
    t1.join();
    t2.join();
    t3.join();
    
    SUCCEED() << "Buffer append operations completed without errors";
}

TEST(Log, log_file_rotation) {
    Log log;
    
    ASSERT_NO_THROW({
        log.RotateLogFile();
    }) << "Log rotation should not throw exceptions";
    
    for (int i = 0; i < 5; i++) {
        ASSERT_NO_THROW({
            log.RotateLogFile();
        }) << "Multiple log rotations should work correctly";
    }
}

TEST(Log, performance_underHeavyLoad) {
    Log log;
    
    auto start = std::chrono::high_resolution_clock::now();

    const int OPERATION_COUNT = 1000;
    for (int i = 0; i < OPERATION_COUNT; i++) {
        log.AppendBuff("Performance test message " + std::to_string(i));
        
        if (i % 100 == 0) {
            log.LogSize();
        }
        
        if (i % 500 == 0) {
            log.GetTimeStamp();
            log.GetRunningTime();
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    ASSERT_LT(duration.count(), 5000) << "Heavy load operations should complete in reasonable time";
}


TEST(Log, comprehensive_functionality) {
    Log* log = Log::GetLog();

    std::vector<std::string> testMessages = {
        "Critical system error",
        "Warning: Resource threshold exceeded",
        "Information: Process started successfully",
        "Debug: Variable value = 42",
        "Trace: Entering function processData()"
    };
    
    for (const auto& message : testMessages) {
        std::string timestamp = log->GetTimeStamp();
        std::string runningTime = log->GetRunningTime();

        log->AppendBuff(timestamp + runningTime + " " + message + "\n");

        int64_t currentSize = log->LogSize();
        ASSERT_GE(currentSize, -1) << "Log size should be valid during operations";

        static long sizeRates[] = {1024, 2048, 4096};
        for (long rate : sizeRates) {
            log->SetMaxDebugLogSizeRate(rate);
        }
    }
    
    ASSERT_NO_THROW({
        log->RotateLogFile();
    }) << "Final log rotation should complete successfully";
}

TEST(Log, long_running_stability) {
    auto testStart = std::chrono::high_resolution_clock::now();
    
    Log log;
    const int EXTENDED_OPERATION_COUNT = 5000;
    
    for (int i = 0; i < EXTENDED_OPERATION_COUNT; i++) {
        if (i % 10 == 0) {
            log.AppendBuff("Extended test iteration " + std::to_string(i));
        }
        
        if (i % 100 == 0) {
            log.GetTimeStamp();
            log.GetRunningTime();
        }
        
        if (i % 500 == 0) {
            log.LogSize();
        }
        
        if (i % 1000 == 0) {
            log.SetMaxDebugLogSizeRate(1024 * (i / 1000 + 1));
        }
        
        if (i % 2000 == 0) {
            log.RotateLogFile();
        }
    }
    
    auto testEnd = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(testEnd - testStart);
    
    ASSERT_LT(duration.count(), 30) << "Long running test should complete within time limits";
    SUCCEED() << "Long running stability test completed successfully with "
              << EXTENDED_OPERATION_COUNT << " operations";
}

TEST(Log, boundary_conditions) {
    Log log;
    
    ASSERT_NO_THROW({
        log.AppendBuff("");
    }) << "Should handle empty string appends";
    
    std::string largeString(10000, 'X');
    ASSERT_NO_THROW({
        log.AppendBuff(largeString);
    }) << "Should handle large string appends";
    
    std::string specialChars = "特殊字符测试\n\t\r\b\f\\\"'";
    ASSERT_NO_THROW({
        log.AppendBuff(specialChars);
    }) << "Should handle special characters";
}