#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <set>
#include <utility>

using namespace geode::prelude;

class $modify(SelectSameUI, EditorUI) {
	bool init(LevelEditorLayer* layer) {
		if (!EditorUI::init(layer)) return false;

		auto sprite = ButtonSprite::create("Same ID", "bigFont.fnt", "GJ_button_04.png", .6f);
		auto button = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(SelectSameUI::onSelectSame));

		auto mod = Mod::get();
		auto corner = mod->getSettingValue<std::string>("corner");
		auto winSize = CCDirector::get()->getWinSize();
		auto size = button->getContentSize();
		constexpr float margin = 8.f;

		float x = corner.ends_with("right") ? winSize.width - size.width / 2 - margin : size.width / 2 + margin;
		float y = corner.starts_with("Top") ? winSize.height - size.height / 2 - margin : size.height / 2 + margin;
		button->setPosition({
			x + static_cast<float>(mod->getSettingValue<int64_t>("offset-x")),
			y + static_cast<float>(mod->getSettingValue<int64_t>("offset-y"))
		});

		auto menu = CCMenu::create();
		menu->setID("select-same-menu"_spr);
		menu->setPosition({0.f, 0.f});
		menu->addChild(button);
		this->addChild(menu, 10);

		return true;
	}

	void onSelectSame(CCObject*) {
		std::set<std::pair<int, int>> keys;
		auto collect = [&](GameObject* obj) {
			if (obj) keys.emplace(obj->m_objectID, obj->m_editorLayer);
		};

		collect(m_selectedObject);
		if (m_selectedObjects) {
			for (auto obj : CCArrayExt<GameObject*>(m_selectedObjects)) collect(obj);
		}
		if (keys.empty()) return;

		auto matches = CCArray::create();
		for (auto obj : CCArrayExt<GameObject*>(m_editorLayer->getAllObjects())) {
			if (keys.contains({obj->m_objectID, obj->m_editorLayer})) matches->addObject(obj);
		}

		// keys are collected first, deselectAll clears the selection they came from
		this->deselectAll();
		this->selectObjects(matches, true);
	}
};
