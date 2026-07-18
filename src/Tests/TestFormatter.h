#pragma once

/******************************************************************************
 *
 * Project : NAS Controller Firmware
 * Module  : Tests
 * File    : TestFormatter.h
 *
 * Description:
 * Centralized formatting helper for test output.
 * Provides consistent formatting for PASS, FAIL, SKIPPED, and section headers.
 * All component names are dot-padded to fixed width for alignment.
 *
 ******************************************************************************/

#include "../Core/Logger.h"

namespace NAS::Tests
{

class TestFormatter
{
public:

    static void PrintHeader(const char* layerName) noexcept
    {
        NAS::Core::Logger logger;
        logger.Initialize();
        logger.Info("--------------------------------------------------");
        logger.Info(layerName);
        logger.Info("--------------------------------------------------");
        logger.Info("");
    }

    static void PrintPass(const char* componentName) noexcept
    {
        NAS::Core::Logger logger;
        logger.Initialize();
        char buffer[64];
        FormatComponentLine(componentName, "PASS", buffer, sizeof(buffer));
        logger.Info(buffer);
    }

    static void PrintFail(const char* componentName) noexcept
    {
        NAS::Core::Logger logger;
        logger.Initialize();
        char buffer[64];
        FormatComponentLine(componentName, "FAIL", buffer, sizeof(buffer));
        logger.Error(buffer);
    }

    static void PrintSkipped(const char* componentName,
                             const char* reason = nullptr) noexcept
    {
        NAS::Core::Logger logger;
        logger.Initialize();

        if (reason == nullptr)
        {
            char buffer[64];
            FormatComponentLine(componentName, "SKIPPED", buffer,
                sizeof(buffer));
            logger.Warning(buffer);
        } else {
            char buffer[128];
            FormatComponentLineWithReason(componentName, "SKIPPED", reason,
                buffer, sizeof(buffer));
            logger.Warning(buffer);
        }
    }

    static void PrintFooter(int passCount, int failCount,
                           int skippedCount) noexcept
    {
        NAS::Core::Logger logger;
        logger.Initialize();
        logger.Info("");

        if (passCount > 0)
        {
            char buffer[32];
            FormatCount("PASS", passCount, buffer, sizeof(buffer));
            logger.Info(buffer);
        }

        if (failCount > 0)
        {
            char buffer[32];
            FormatCount("FAIL", failCount, buffer, sizeof(buffer));
            logger.Error(buffer);
        }

        if (skippedCount > 0)
        {
            char buffer[32];
            FormatCount("SKIPPED", skippedCount, buffer, sizeof(buffer));
            logger.Warning(buffer);
        }

        logger.Info("");
    }

    static void PrintSectionBreak() noexcept
    {
        NAS::Core::Logger logger;
        logger.Initialize();
        logger.Info("");
    }

private:

    static void FormatComponentLine(const char* name, const char* status,
                                   char* buffer, int bufferSize) noexcept
    {
        int needed = 0;
        for (int i = 0; i < 30; ++i)
        {
            if (name[i] == '\0')
            {
                needed = i;
                break;
            }
        }

        int paddingNeeded = 31 - needed;
        if (paddingNeeded < 1) paddingNeeded = 1;

        int pos = 0;
        for (int i = 0; i < needed && pos < bufferSize - 1; ++i)
        {
            buffer[pos++] = name[i];
        }

        for (int i = 0; i < paddingNeeded && pos < bufferSize - 1; ++i)
        {
            buffer[pos++] = '.';
        }

        for (int i = 0; status[i] != '\0' && pos < bufferSize - 1; ++i)
        {
            buffer[pos++] = status[i];
        }

        buffer[pos] = '\0';
    }

    static void FormatComponentLineWithReason(const char* name,
                                             const char* status,
                                             const char* reason,
                                             char* buffer,
                                             int bufferSize) noexcept
    {
        int needed = 0;
        for (int i = 0; i < 30; ++i)
        {
            if (name[i] == '\0')
            {
                needed = i;
                break;
            }
        }

        int paddingNeeded = 31 - needed;
        if (paddingNeeded < 1) paddingNeeded = 1;

        int pos = 0;
        for (int i = 0; i < needed && pos < bufferSize - 1; ++i)
        {
            buffer[pos++] = name[i];
        }

        for (int i = 0; i < paddingNeeded && pos < bufferSize - 1; ++i)
        {
            buffer[pos++] = '.';
        }

        for (int i = 0; status[i] != '\0' && pos < bufferSize - 1; ++i)
        {
            buffer[pos++] = status[i];
        }

        if (pos < bufferSize - 1)
        {
            buffer[pos++] = ' ';
        }

        buffer[pos++] = '(';
        for (int i = 0; reason[i] != '\0' && pos < bufferSize - 2; ++i)
        {
            buffer[pos++] = reason[i];
        }
        if (pos < bufferSize - 1)
        {
            buffer[pos++] = ')';
        }

        buffer[pos] = '\0';
    }

    static void FormatCount(const char* label, int count, char* buffer,
                           int bufferSize) noexcept
    {
        int pos = 0;
        for (int i = 0; label[i] != '\0' && pos < bufferSize - 1; ++i)
        {
            buffer[pos++] = label[i];
        }

        if (pos < bufferSize - 1)
        {
            buffer[pos++] = ' ';
        }

        if (count >= 10)
        {
            buffer[pos++] = ('0' + (count / 10));
        }
        if (pos < bufferSize - 1)
        {
            buffer[pos++] = ('0' + (count % 10));
        }

        buffer[pos] = '\0';
    }
};

} // namespace NAS::Tests
