#pragma once

#include "../Common/common.h"

//前方宣言
namespace FD {
	class WindowWrite;
	class WindowRead;
	class ExitWrite;
	class ExitRead;
}

namespace FD {
	namespace Internal::Scene {
		class WindowData final {
		private:
			//windowの削除フラグ
			static inline bool closeFlag = false;

			//最大モードか
			static inline bool isMaximize = true;

			//最小モードか
			static inline bool isMinimize = false;

			//最大モードにする前の位置とサイズを記録しておく
			static inline std::pair<int32_t, int32_t> savedPos{};
			static inline std::pair<int32_t, int32_t> savedSize{};

			static inline std::mutex mtx{};
		private:
			friend class WindowWrite;
			friend class WindowRead;
		};
	}
}

namespace FD {

	class WindowWrite final {
	public:
		explicit WindowWrite(Internal::PassKey) noexcept {};
		~WindowWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(WindowWrite);

	public:
		//true -> exit
		bool* getCloseFlag() const noexcept;

		void setIsMaximise(const bool flag) const;
		void setIsMinimise(const bool flag) const;

		void setSize(const int32_t width, const int32_t heigth) const;
		void setPos(const int32_t x, const int32_t y) const;
	};

	class WindowRead final {
	public:
		explicit WindowRead(Internal::PassKey) noexcept {};
		~WindowRead() = default;
		FluidumUtils_Class_Delete_CopyMove(WindowRead);

	public:
		bool getIsMaximise() const;
		bool getIsMinimise() const;

		std::pair<int32_t, int32_t> getSavedSize() const;
		std::pair<int32_t, int32_t> getSavedPos() const;

	};

}

namespace FD {

	class ExitWrite final {
	public:
		explicit ExitWrite(Internal::PassKey) noexcept {};
		~ExitWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ExitWrite);

	public:
		void saveAsAndExit() const;
	};

	class ExitRead final {
	public:
		explicit ExitRead(Internal::PassKey) noexcept {};
		~ExitRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ExitRead);

	public:
		_NODISCARD bool saveAsAndExit() const;

	};
}
