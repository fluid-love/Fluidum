/*
1byte:�ύX1��index��vertex�� index->1 vertex->0
4byte:�ύX1�͉����邩
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
1byte:�ύX2��index��vertex�� index->1 vertex->0
4byte:�ύX2�͉����邩
4byte:index //����index�Ȃ��
4byte:�l
.
.
.

*/

#pragma once

#include "../common/FDtype.h"
#include "../../common/FDlogfilepath.h"

//�O���錾
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
			//����
			/*
				indices[0] 1�Ԗڂ̕ύX�͉���
				indices[1] 2�Ԗڂ̕ύX�͉���
				.
				.
				.

			*/
			static inline std::vector<std::vector<uint16_t>> num = {};

			static inline std::vector<std::vector<std::vector<uint32_t>>> changed = {};


			//index��vertex��
			//1�Ԗڂ̕ύX��index�Ȃ�true
			static inline std::vector<std::vector<bool>> indexNum = {};

			//�ꎞ�I�ɕۑ����Ă���
			/*
				tempVertexData[i]�ɑΉ�����̂�tempIndexData[i]
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
		//pair first->�ύX�̂���index second->���̒l
		//first[i]��second[i]�Ή�
		using IndexLogType = std::pair<std::vector<Object::IndexSize>, std::vector<Object::IndexSize>>;

		//pair first->�ύX�̂���index second->���̒l
		//first[i]��second[i]�Ή�
		using VertexLogType = std::pair<std::vector<Object::IndexSize>, std::vector<Object::Vertex>>;

	public:
		//�������擾����
		//std::pair<VertexLogType, IndexLogType> get(const FVK::VertexBufferKey<std::string>& key, const std::size_t at);

		//��x�͂��߂���쐬���ĕԂ�
		/*
		    at�Ԗ�(0,1,2...)��vertex��index���쐬����D
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

			const uint32_t target;//constructor�ŏ�����
		};
	}
}