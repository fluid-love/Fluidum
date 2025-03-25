#pragma once

#include "../Manager/manager.h"

namespace FVK::Internal::Api {

	//NotCreated
	void checkManagerEmpty();

}

namespace FVK::Internal::Api {

	template<auto Enum>
	void exceptionMessage(const char* funcName) noexcept {
		constexpr auto enumName = magic_enum::enum_name<Enum>();
		GMessenger.add<FU::Log::Type::None>(nullptr, 0, "Exception({}) was thrown. Caught:{}.", funcName, enumName);
	}

	void stdExceptionMessage(const char* funcName, const char* what) noexcept;

	void seriousExceptionMessage(const char* funcName) noexcept;

}


namespace FVK::Internal::Api {

	template<FvkType Type, IsParameter Parameter>
	void createBase(const char* key, Parameter& parameter) {
		using namespace Internal;
		try {
			GManager->add<Type>(key, parameter);
		}
		catch (const ::FVK::Exception::Error& e) {
			using enum ::FVK::Exception::ErrorType;
			if (e.code() == CollisionOfKeys) {
				//There is already a key with the same name.
				Api::exceptionMessage<CollisionOfKeys>(__func__);
			}
			else if (e.code() == FailedToCreate) {
				//Failed to create it for various reasons.
				Api::exceptionMessage<FailedToCreate>(__func__);
			}
			else if (e.code() == NotSupported) {
				Api::exceptionMessage<NotSupported>(__func__);
			}
			else {
				Api::exceptionMessage<Unexpected>(__func__);
				::FVK::Internal::Exception::throwUnexpected();
			}
			FU::Exception::rethrow();
		}
		catch (const std::exception& e) {
			Api::stdExceptionMessage(__func__, e.what());
			::FVK::Internal::Exception::throwUnexpected();
		}
		catch (...) {
			GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, "Internal Error. Unknown exception was thrown.");
			Api::exceptionMessage<::FVK::Exception::ErrorType::Unknown>(__func__);
			::FVK::Internal::Exception::throwUnknown();
		}
	}

	template<FvkType Type, IsParameter Parameter>
	void createBase(const char* key, Parameter& parameter, const ::FVK::Internal::Key::RequiredDataKeys<Type>& connections) {
		using namespace Internal;
		try {
			GManager->add<Type>(key, parameter, connections);
		}
		catch (const ::FVK::Exception::Error& e) {
			using enum ::FVK::Exception::ErrorType;
			if (e.code() == CollisionOfKeys) {
				//There is already a key with the same name.
				Api::exceptionMessage<CollisionOfKeys>(__func__);
			}
			else if (e.code() == FailedToCreate) {
				//Failed to create it for various reasons.
				Api::exceptionMessage<FailedToCreate>(__func__);
			}
			else if (e.code() == NotSupported) {
				Api::exceptionMessage<NotSupported>(__func__);
			}
			else {
				Api::exceptionMessage<Unexpected>(__func__);
				::FVK::Internal::Exception::throwUnexpected();
			}
			FU::Exception::rethrow();
		}
		catch (const std::exception& e) {
			Api::stdExceptionMessage(__func__, e.what());
			::FVK::Internal::Exception::throwUnexpected();
		}
		catch (...) {
			GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, "Internal Error. Unknown exception was thrown.");
			Api::exceptionMessage<::FVK::Exception::ErrorType::Unknown>(__func__);
			::FVK::Internal::Exception::throwUnknown();
		}
	}

	template<FvkType Type, ::FVK::Internal::Key::IsKey T>
	void destroyBase(const T& key) {
		using namespace Internal;
		try {
			GManager->destroy<Type>(key);
		}
		catch (const ::FVK::Exception::Error& e) {
			using enum ::FVK::Exception::ErrorType;
			if (e.code() == NotFound) {
				Api::exceptionMessage<NotFound>(__func__);
			}
			else if (e.code() == FailedToDestroy) { //rarely
				Api::exceptionMessage<FailedToDestroy>(__func__);
			}
			else {
				Api::exceptionMessage<Unexpected>(__func__);
				::FVK::Internal::Exception::throwUnexpected();
			}
			FU::Exception::rethrow();
		}
		catch (const std::exception& e) {
			Api::stdExceptionMessage(__func__, e.what());
			::FVK::Internal::Exception::throwUnexpected();
		}
		catch (...) {
			Api::exceptionMessage<::FVK::Exception::ErrorType::Unknown>(__func__);
			::FVK::Internal::Exception::throwUnknown();
		}
	}

}
