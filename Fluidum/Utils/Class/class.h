#pragma once

#define FluidumUtils_Class_Delete_Copy(Class)\
	Class(const Class&) = delete;\
	Class& operator=(const Class&) = delete;

#define FluidumUtils_Class_Delete_Move(Class)\
	Class(Class&&) = delete;\
	Class& operator=(Class&&) = delete

#define FluidumUtils_Class_Delete_CopyMove(Class)\
	Class(const Class&) = delete;\
	Class& operator=(const Class&) = delete;\
	Class(Class&&) = delete;\
	Class& operator=(Class&&) = delete

#define FluidumUtils_Class_Delete_ConDestructor(Class)\
	Class() = delete;\
	~Class() = delete

#define FluidumUtils_Class_Default_CopyMove(Class)\
	Class(Class&&) = default;\
	Class& operator=(Class&&) = default;\
    Class(const Class&) = default; \
    Class& operator=(const Class&) = default

#define FluidumUtils_Class_Default_CopyMove_Noexcept(Class)\
	Class(Class&&) noexcept = default;\
	Class& operator=(Class&&) noexcept = default;\
    Class(const Class&) noexcept = default; \
    Class& operator=(const Class&) noexcept = default

#define FluidumUtils_Class_Default_ConDestructor(Class)\
	Class() = default;\
	~Class() = default

#define FluidumUtils_Class_Default_ConDestructor_Noexcept(Class)\
	Class() noexcept = default;\
	~Class() noexcept = default

#define FluidumUtils_Class_Default_Move(Class)\
	Class(Class&&) = default;\
	Class& operator=(Class&&) = default

#define FluidumUtils_Class_Default_Move_Noexcept(Class)\
	Class(Class&&) noexcept = default;\
	Class& operator=(Class&&) noexcept = default

#define FluidumUtils_Class_Default_Copy(Class)\
    Class(const Class&) = default; \
    Class& operator=(const Class&) = default 

#define FluidumUtils_Class_Default_Copy_Noexcept(Class)\
    Class(const Class&) noexcept = default; \
    Class& operator=(const Class&) noexcept = default