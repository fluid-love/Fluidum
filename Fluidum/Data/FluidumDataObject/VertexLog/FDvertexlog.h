/*
1byte:変更1はindexかvertexか index->1 vertex->0
4byte:変更1は何個あるか
4byte:index
4byte:index
4byte:index
.
.
.
4byte:index
28byte:xyzrgba
28byte:xyzrgba
28byte:xyzrgba
.
.
.
28byte:xyzrgba
1byte:変更2はindexかvertexか index->1 vertex->0
4byte:変更2は何個あるか
4byte:index //もしindexならば
4byte:値
.
.
.

*/

#pragma once

#include "../common/FDtype.h"
#include "../../common/FDlogfilepath.h"

//前方宣言
namespace FD {
	class VertexLogWrite;
	class VertexLogRead;
	namespace Internal {
		class VertexLogOutFile;
	}
}

namespace FD {

	namespace Internal {

		class LogData final {
			//順番
			/*
				indices[0] 1番目の変更は何個か
				indices[1] 2番目の変更は何個か
				.
				.
				.

			*/
			static inline std::vector<std::vector<uint16_t>> num = {};

			static inline std::vector<std::vector<std::vector<uint32_t>>> changed = {};


			//indexかvertexか
			//1番目の変更がindexならtrue
			static inline std::vector<std::vector<bool>> indexNum = {};

			//一時的に保存しておく
			/*
				tempVertexData[i]に対応するのはtempIndexData[i]
			*/
			static inline std::vector<std::vector<Object::Vertex>> tempVertexData = {};
			static inline std::vector<std::vector<Object::IndexSize>> tempIndexData = {};

			static inline std::map<std::string, uint32_t> keys = {};

			static inline std::string currentKey = {};

		private:
			friend ::FD::Internal::VertexLogOutFile;
			friend ::FD::VertexLogWrite;
			friend ::FD::VertexLogRead;
		};

	}

	class VertexLogWrite final {
	public:
		VertexLogWrite(Internal::ManagerKey);
		~VertexLogWrite() = default;
		FluidumUtils_Class_NoCopyNoMove(VertexLogWrite)
	
	public:
		void add(const std::vector<uint32_t>& changedIndices, const Object::Vertices* const data);
		void add(const std::vector<uint32_t>& changedIndices, const Object::Indices* const data);

		void setTargetKey(const FVK::VertexBufferKey<std::string>& key);

	private:
		bool shouldOutFile(const std::size_t target) const noexcept;
		void outFile() const;



	};

	class VertexLogRead final {
	public:
		VertexLogRead(Internal::ManagerKey) {};
		~VertexLogRead() = default;
		FluidumUtils_Class_NoCopyNoMove(VertexLogRead)

	public:
		//pair first->変更のあるindex second->その値
		//first[i]はsecond[i]対応
		using IndexLogType = std::pair<std::vector<Object::IndexSize>, std::vector<Object::IndexSize>>;

		//pair first->変更のあるindex second->その値
		//first[i]はsecond[i]対応
		using VertexLogType = std::pair<std::vector<Object::IndexSize>, std::vector<Object::Vertex>>;

	public:
		//差分を取得する
		//std::pair<VertexLogType, IndexLogType> get(const FVK::VertexBufferKey<std::string>& key, const std::size_t at);

		//一度はじめから作成して返す
		/*
		    at番目(0,1,2...)のvertexとindexを作成する．
		*/
		_NODISCARD std::pair<Object::Vertices, Object::Indices> create(const FVK::VertexBufferKey<std::string>& key, const std::size_t at) const;
	};



	namespace Internal {

		class VertexLogOutFile final {
		public:
			VertexLogOutFile(const std::string &key);

			void out();

		private:
			void write(const uint32_t i);

			void reset();

		private:

			std::ofstream ofs;
			uint32_t vPos = 0;
			uint32_t iPos = 0;

			const uint32_t target;//constructorで初期化
		};
	}
}