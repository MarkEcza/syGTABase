#include "scanner.hpp"
#include "components.hpp"
#include <Psapi.h>
#include <sstream>
#include <optional>
#include <array>

namespace sy
{
    std::vector<std::optional<uint8_t>> parseIDASignature(const char* sig) {
        std::vector<std::optional<uint8_t>> bytes;
        std::stringstream ss(sig);

        while (!ss.eof()) {
            std::string byte_str;
            ss >> byte_str;

            if (byte_str == "?" || byte_str == "??") {
                bytes.push_back(std::nullopt);
                continue;
            }

            if (byte_str.length() != 2) {
                throw std::runtime_error("Invalid byte in IDA signature: " + byte_str);
            }

            char* end;
            long value = strtol(byte_str.c_str(), &end, 16);

            if (*end != '\0') {
                throw std::runtime_error("Invalid byte in IDA signature: " + byte_str);
            }

            bytes.push_back(static_cast<uint8_t>(value));
        }

        return bytes;
    }


    pattern::pattern(_module module) :
        m_Module(module), m_Result(0)
    {
        m_ModuleHandle = m_Module.get_handle();

        if (m_ModuleHandle == nullptr) {
            throw std::runtime_error("Could not find module!");
        }

        MODULEINFO mi = { 0 };

        if (!GetModuleInformation(GetCurrentProcess(), m_ModuleHandle, &mi, sizeof(mi))) {
            throw std::runtime_error("Could not get module information!");
        }
        m_ModuleSize = mi.SizeOfImage;
    }

    pattern::~pattern() {}

    pattern& pattern::scanAllNow(std::string sigName, std::string  IDASig)
    {
        auto signature = parseIDASignature(IDASig.c_str());

        std::uintptr_t base_address = reinterpret_cast<std::uintptr_t>(m_ModuleHandle);

        std::size_t sig_size = signature.size();
        std::uintptr_t end_address = base_address + m_ModuleSize - sig_size;

        std::array<int, 256> bad_char;
        for (int i = 0; i < 256; ++i) {
            bad_char[i] = sig_size;
        }
        for (int i = 0; i < sig_size - 1; ++i) {
            if (signature[i].has_value()) {
                bad_char[signature[i].value()] = sig_size - i - 1;
            }
            else {
                for (int j = 0; j < 256; ++j) {
                    bad_char[j] = min(bad_char[j], sig_size - i - 1);
                }
            }
        }

        std::uintptr_t i = base_address + sig_size - 1;
        while (i < end_address) {
            int j = sig_size - 1;
            while (j >= 0) {
                if (signature[j].has_value() && signature[j].value() != *reinterpret_cast<uint8_t*>(i - sig_size + j + 1))
                    break;
                --j;
            }
            if (j < 0) {
                m_Result = handle(i - sig_size + 1);
                g_Logger->logNow("Found pattern: " + sigName, logClass::GENERAL);
                m_AllResults.push_back(m_Result);
            }
            i += max(bad_char[*reinterpret_cast<uint8_t*>(i)], static_cast<int>(sig_size - j - 1));
        }

        if (!m_AllResults.empty())
            g_Logger->logNow("Failed to find pattern: " + sigName, logClass::ALERT);

        return *this;
    }


    pattern& pattern::scanNow(std::string sigName, std::string  IDASig)
    {
        auto signature = parseIDASignature(IDASig.c_str());

        std::uintptr_t base_address = reinterpret_cast<std::uintptr_t>(m_ModuleHandle);

        std::size_t sig_size = signature.size();
        std::uintptr_t end_address = base_address + m_ModuleSize - sig_size;

        std::array<int, 256> bad_char;
        for (int i = 0; i < 256; ++i) {
            bad_char[i] = sig_size;
        }
        for (int i = 0; i < sig_size - 1; ++i) {
            if (signature[i].has_value()) {
                bad_char[signature[i].value()] = sig_size - i - 1;
            }
            else {
                for (int j = 0; j < 256; ++j) {
                    bad_char[j] = min(bad_char[j], sig_size - i - 1);
                }
            }
        }

        std::uintptr_t i = base_address + sig_size - 1;
        while (i < end_address) {
            int j = sig_size - 1;
            while (j >= 0) {
                if (signature[j].has_value() && signature[j].value() != *reinterpret_cast<uint8_t*>(i - sig_size + j + 1))
                    break;
                --j;
            }
            if (j < 0) {
                m_Result = handle(i - sig_size + 1);
                g_Logger->logNow("Found pattern: " + sigName, logClass::GENERAL);
                return *this;
            }
            i += max(bad_char[*reinterpret_cast<uint8_t*>(i)], static_cast<int>(sig_size - j - 1));
        }

        g_Logger->logNow("Failed to find pattern: " + sigName, logClass::ALERT);
        m_Result = handle(0);

        return *this;
    }

    handle pattern::getResult()
    {
        return m_Result;
    }

    std::vector<handle> pattern::getAllResults()
    {
        return m_AllResults;
    }


    offset::offset(uint64_t offset, _module module) :
        m_Offset(offset), m_Module(module)
    {
        m_ModuleHandle = m_Module.get_handle();
    }

    offset::~offset() {}

    handle& offset::getResult()
    {
        auto addr = (uintptr_t)m_ModuleHandle + m_Offset;
        m_Result = handle(addr);

        return m_Result;
    }

    uintptr_t& offset::getAddress()
    {
        auto addr = (uintptr_t)m_ModuleHandle + m_Offset;
        return addr;
    }
}