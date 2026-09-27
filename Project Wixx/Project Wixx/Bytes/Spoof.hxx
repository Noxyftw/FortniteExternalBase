#pragma once

#ifdef _KERNEL_MODE
#include <ntddk.h>
#include <ntdef.h>
#include <xtr1common>
#include <unordered_map>
#else
#include <Windows.h>
#include <utility>
#include <unordered_map>
#endif

#include <Intrin.h>









#define SPOOF CallSpoofer::SpoofFunction spoof( _AddressOfReturnAddress( ) );

#ifdef _KERNEL_MODE
#define SPOOF_CALL( ret_type, name ) ( CallSpoofer::SafeCall<ret_type, decltype( name )>( name ) )
#else
#define SPOOF_CALL( name ) ( CallSpoofer::SafeCall<decltype( name )>( name ) )
#endif

#define MAX_FUNC_BUFFERED 100
#define SHELLCODE_GENERATOR_SIZE 500

namespace CallSpoofer
{
#ifdef _KERNEL_MODE
    typedef unsigned __int64 uintptr_t;
    typedef unsigned __int64 size_t;
#else
    using namespace std;
#endif


    class SpoofFunction
    {
    public:
        uintptr_t temp = 0;
        const uintptr_t xor_key = 0xff00ff00ff00ff00;
        void* ret_addr_in_stack = nullptr;

        explicit SpoofFunction(void* addr) : ret_addr_in_stack(addr)
        {
            if (ret_addr_in_stack) {
                temp = *(uintptr_t*)ret_addr_in_stack;
                temp ^= xor_key;
                *(uintptr_t*)ret_addr_in_stack = 0;
            }
        }

        ~SpoofFunction()
        {
            if (ret_addr_in_stack) {
                *(uintptr_t*)ret_addr_in_stack = temp ^ xor_key;
            }
        }
    };

#ifdef _KERNEL_MODE
    __forceinline PVOID LocateShellCode(PVOID func, size_t size = SHELLCODE_GENERATOR_SIZE)
    {
        void* addr = ExAllocatePoolWithTag(NonPagedPool, size, 'File');
        if (!addr) {

            return nullptr;
        }
        return memcpy(addr, func, size);
    }
#else
    __forceinline PVOID LocateShellCode(PVOID func, size_t size = SHELLCODE_GENERATOR_SIZE)
    {
        void* addr = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!addr) {

            return nullptr;
        }
        return memcpy(addr, func, size);
    }
#endif

#ifdef _KERNEL_MODE
    template <typename RetType, typename Func, typename... Args>
    RetType __declspec(safebuffers)ShellCodeGenerator(Func f, Args&&... args)
#else
    template <typename Func, typename... Args>
    typename std::invoke_result<Func, Args...>::type __declspec(safebuffers)ShellCodeGenerator(Func f, Args&&... args)
#endif
    {
#ifdef _KERNEL_MODE
        using return_type = RetType;
#else
        using return_type = typename std::invoke_result<Func, Args...>::type;
#endif
        const uintptr_t xor_key = 0xff00ff00ff00ff00;
        void* ret_addr_in_stack = _AddressOfReturnAddress();
        uintptr_t temp = *(uintptr_t*)ret_addr_in_stack;
        temp ^= xor_key;
        *(uintptr_t*)ret_addr_in_stack = 0;

        if constexpr (std::is_same<return_type, void>::value)
        {
            f(std::forward<Args>(args)...);
            *(uintptr_t*)ret_addr_in_stack = temp ^ xor_key;
        }
        else
        {
            return_type ret = f(std::forward<Args>(args)...);
            *(uintptr_t*)ret_addr_in_stack = temp ^ xor_key;
            return ret;
        }
    }

#ifdef _KERNEL_MODE
    template<typename RetType, typename Func>
#else
    template<typename Func>
#endif
    class SafeCall
    {
        Func* funcPtr;

    public:
        explicit SafeCall(Func* func) : funcPtr(func) {}

        template<typename... Args>
        __forceinline auto operator( )(Args&&... args) -> decltype(std::declval<Func>()(std::declval<Args>()...))
        {
            SPOOF;

#ifdef _KERNEL_MODE
            using return_type = RetType;
            using p_shell_code_generator_type = decltype(&ShellCodeGenerator<RetType, Func*, Args...>);
            PVOID self_addr = static_cast<PVOID>(&ShellCodeGenerator<RetType, Func*, Args&&...>);
#else
            using return_type = typename std::invoke_result<Func, Args...>::type;
            using p_shell_code_generator_type = decltype(&ShellCodeGenerator<Func, Args...>);
            p_shell_code_generator_type self_addr = static_cast<p_shell_code_generator_type>(&ShellCodeGenerator<Func, Args...>);
#endif

            static std::unordered_map<void*, PVOID> shellcode_cache;

            auto it = shellcode_cache.find(self_addr);
            if (it != shellcode_cache.end())
            {
                return reinterpret_cast<p_shell_code_generator_type>(it->second)(funcPtr, std::forward<Args>(args)...);
            }

            PVOID p_shellcode = LocateShellCode(self_addr);
            if (!p_shellcode)
            {

                if constexpr (!std::is_void<return_type>::value)
                {
                    return {};
                }
                else
                {
                    return;
                }
            }

            shellcode_cache[self_addr] = p_shellcode;
            if constexpr (!std::is_void<return_type>::value)
            {
                return reinterpret_cast<p_shell_code_generator_type>(p_shellcode)(funcPtr, std::forward<Args>(args)...);
            }
            else
            {
                reinterpret_cast<p_shell_code_generator_type>(p_shellcode)(funcPtr, std::forward<Args>(args)...);
            }
        }
    };
}