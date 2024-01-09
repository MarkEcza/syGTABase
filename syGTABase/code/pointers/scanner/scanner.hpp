#pragma once
#include <windows.h>
#include <cstdint>
#include <vector>
#include <string>

namespace sy
{
    class handle {
    public:
        handle() = default;
        explicit handle(uintptr_t address) : m_Address(address) {}

        template <typename T>
        T as() const {
            return reinterpret_cast<T>(m_Address);
        }

        handle add(uintptr_t offset) const {
            if (m_Address != 0)
            {
                return handle(m_Address + offset);
            }

            return *this;
        }

        handle sub(uintptr_t offset) const {
            if (m_Address != 0)
            {
                return handle(m_Address - offset);
            }

            return *this;
        }

        handle rip() const {
            if (m_Address != 0)
            {
                auto offset = *as<int32_t*>();
                return add(offset + sizeof(int32_t));
            }

            return *this;
        }

        uintptr_t get_address()
        {
            return m_Address;
        }

    private:
        uintptr_t m_Address = 0;
    };


    class _module {
    public:
        _module(const char* module) : m_Module(module)
        {
            m_ModuleHandle = GetModuleHandleA(m_Module);
        }

        handle get_export(const char* func)
        {
            return handle((std::uintptr_t)GetProcAddress(m_ModuleHandle, func));
        }

        HMODULE get_handle()
        {
            return m_ModuleHandle;
        }

    private:
        const char* m_Module;
        HMODULE m_ModuleHandle;
    };

    class pattern {
    public:
        explicit pattern(_module module);
        ~pattern() noexcept;

        pattern& scanNow(std::string sigName, std::string  IDASig);
        pattern& scanAllNow(std::string sigName, std::string  IDASig);

        handle getResult();
        std::vector<handle> getAllResults();
    private:
        _module m_Module;
        size_t m_ModuleSize;
        HMODULE m_ModuleHandle;
        handle m_Result;
        std::vector<handle> m_AllResults;
    };

    class offset {
    public:
        explicit offset(uint64_t offset, _module module = "GTA5.exe");
        ~offset() noexcept;

        handle& getResult();
        uintptr_t& getAddress();
    private:
        uint64_t m_Offset;
        _module m_Module;
        handle m_Result;
        HMODULE m_ModuleHandle;
        uintptr_t m_Address;
    };
}