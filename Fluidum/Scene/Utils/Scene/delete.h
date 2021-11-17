#pragma once

#include "../../Common/common.h"

#include "../../Coding/TextEditor/texteditor.h"
#include "../../Coding/Tab/tab.h"
#include "../../Coding/Debug/debug.h"

#include "../../Flu/Node/node.h"

#include "../../Analysis/Overview/overview.h"
#include "../../Analysis/Func/function.h"
#include "../../Analysis/Plot/plot.h"

#include "../../Genome/Overview/overview.h"

#include "../../Animation/Animation/animation.h"

#include "../../Project/project.h"

#include "../../Console/Console/console.h"

namespace FS::Utils {

	template<typename... T>
	class DeleteScenes final : public Scene {
		static_assert(sizeof...(T) > 0, "DeleteScenes: SceneSize == 0");
	public:
		explicit DeleteScenes(const FD::SceneRead* const sceneRead) :
			sceneRead(sceneRead)
		{
			FluidumScene_Log_Constructor("Utils::DeleteScenes");
			this->deleteScenes<0>();
		}

		void Constructor(FD::SceneRead);

		~DeleteScenes() noexcept {
			FluidumScene_Log_Destructor_("Utils::DeleteScenes")
		}

		FluidumUtils_Class_Delete_CopyMove(DeleteScenes)

	public:
		virtual void call() override {
			assert(false && "Scene::callConstructor");
		}

	public:
		const FD::SceneRead* const sceneRead;

	private:

		template<std::size_t Index>
		void deleteScenes() {
			using namespace FU::Class;

			constexpr std::array<FU::Class::ClassCode::CodeType, sizeof...(T)> codes = { FU::Class::ClassCode::GetClassCode<T>()... };
			constexpr FU::Class::ClassCode::CodeType code = std::get<Index>(codes);

			if constexpr (code == ClassCode::GetClassCode<TextEditor>())
				this->coding();
			else if constexpr (code == ClassCode::GetClassCode<Coding::Tab>())
				this->tab();
			else if constexpr (code == ClassCode::GetClassCode<Coding::Debug>())
				this->debug();

			else if constexpr (code == ClassCode::GetClassCode<Flu::Node>())
				this->flu();

			else if constexpr (code == ClassCode::GetClassCode<Analysis::Overview>())
				this->analysis();
			else if constexpr (code == ClassCode::GetClassCode<Analysis::Plot>())
				this->analysis();
			else if constexpr (code == ClassCode::GetClassCode<Analysis::Function>())
				this->analysis();

			else if constexpr (code == ClassCode::GetClassCode<Flu::Node>())
				this->flu();
			else if constexpr (code == ClassCode::GetClassCode<Analysis::Overview>())
				this->analysis();
			else if constexpr (code == ClassCode::GetClassCode<Genome::Overview>())
				this->genome();
			else if constexpr (code == ClassCode::GetClassCode<Animation>())
				this->animation();
			else if constexpr (code == ClassCode::GetClassCode<Project>())
				this->project();
			else if constexpr (code == ClassCode::GetClassCode<Console>())
				this->console();

			if constexpr (Index < (sizeof...(T) - 1))
				this->deleteScenes<Index + 1>();
		}

	private:
		void coding() {
			if (sceneRead->isExist<Coding::Tab>()) {
				this->tab();
			}
			if (sceneRead->isExist<Coding::Debug>()) {
				this->debug();
			}

			FluidumScene_Log_RequestTryDeleteScene("Genome::Overview");
			Scene::tryDeleteScene<TextEditor>();
		}

		void tab() {
			FluidumScene_Log_RequestTryDeleteScene("Coding::Tab");
			Scene::tryDeleteScene<Coding::Tab>();
		}

		void debug() {
			FluidumScene_Log_RequestTryDeleteScene("Coding::Debug");
			Scene::tryDeleteScene<Coding::Debug>();
		}

	private:
		void flu() {
			FluidumScene_Log_RequestTryDeleteScene("Flu::Node");
			Scene::tryDeleteScene<Flu::Node>();
		}

	private:
		void analysis() {
			if (sceneRead->isExist<Analysis::Plot>()) {
				this->plot();
			}
			if (sceneRead->isExist<Analysis::Function>()) {
				this->function();
			}

			FluidumScene_Log_RequestTryDeleteScene("Analysis::Overview");
			Scene::tryDeleteScene<Analysis::Overview>();
		}

		void plot() {
			FluidumScene_Log_RequestTryDeleteScene("Analysis::Plot");
			Scene::tryDeleteScene<Analysis::Plot>();
		}

		void function() {
			FluidumScene_Log_RequestTryDeleteScene("Analysis::Function");
			Scene::tryDeleteScene<Analysis::Function>();
		}

	private:
		void genome() {
			FluidumScene_Log_RequestTryDeleteScene("Genome::Overview");
			Scene::tryDeleteScene<Genome::Overview>();
		}

	private:
		void animation() {
			FluidumScene_Log_RequestTryDeleteScene("Animation");
			Scene::tryDeleteScene<Animation>();
		}

	private:
		void project() {
			FluidumScene_Log_RequestTryDeleteScene("Project");
			Scene::tryDeleteScene<Project>();
		}

	private:
		void console() {
			FluidumScene_Log_RequestTryDeleteScene("Console");
			Scene::tryDeleteScene<Console>();
		}



	};

}