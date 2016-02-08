#pragma once
#include <cstdint>
#include "cpu/state.h"
#include "kernelexport.h"
#include "ppcinvoke.h"
#include "utils/type_list.h"

// Kernel Function Export
struct KernelFunction : KernelExport
{
   KernelFunction() :
      KernelExport(KernelExport::Function)
   {
   }

   virtual ~KernelFunction() override = default;

   virtual void call(ThreadState *state) = 0;

   bool valid = false;
   uint32_t syscallID = 0;
   uint32_t vaddr = 0;
};

namespace kernel
{

namespace functions
{

template<typename ReturnType, typename... Args>
struct KernelFunctionImpl : KernelFunction
{
   ReturnType (*wrapped_function)(Args...);

   virtual void call(ThreadState *thread) override
   {
      ppctypes::invoke(thread, wrapped_function, name);
   }
};

template<typename ReturnType, typename ObjectType, typename... Args>
struct KernelMemberFunctionImpl : KernelFunction
{
   ReturnType (ObjectType::*wrapped_function)(Args...);

   virtual void call(ThreadState *thread) override
   {
      ppctypes::invokeMemberFn(thread, wrapped_function, name);
   }
};

template<typename ObjectType>
struct KernelConstructorFunctionImpl : KernelFunction
{
   static void trampFunction(ObjectType *object)
   {
      new (object) ObjectType();
   }

   virtual void call(ThreadState *thread) override
   {
      ppctypes::invoke(thread, &trampFunction, name);
   }
};

template<typename ObjectType>
struct KernelDestructorFunctionImpl : KernelFunction
{
   static void trampFunction(ObjectType *object)
   {
      object->~ObjectType();
   }

   virtual void call(ThreadState *thread) override
   {
      ppctypes::invoke(thread, &trampFunction, name);
   }
};

} // namespace functions

// Regular Function
template<typename ReturnType, typename... Args>
inline KernelFunction *
makeFunction(ReturnType (*fptr)(Args...))
{
   auto func = new kernel::functions::KernelFunctionImpl<ReturnType, Args...>();
   func->valid = true;
   func->wrapped_function = fptr;
   return func;
}

// Member Function
template<typename ReturnType, typename Class, typename... Args>
inline KernelFunction *
makeFunction(ReturnType (Class::*fptr)(Args...))
{
   auto func = new kernel::functions::KernelMemberFunctionImpl<ReturnType, Class, Args...>();
   func->valid = true;
   func->wrapped_function = fptr;
   return func;
}

// Constructor
template<typename Class>
inline KernelFunction *
makeConstructor()
{
   auto func = new kernel::functions::KernelConstructorFunctionImpl<Class>();
   func->valid = true;
   return func;
}

// Destructor
template<typename Class>
inline KernelFunction *
makeDestructor()
{
   auto func = new kernel::functions::KernelDestructorFunctionImpl<Class>();
   func->valid = true;
   return func;
}

}  // namespace kernel
