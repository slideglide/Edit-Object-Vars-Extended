#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "editObjectPopup.hpp"
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
	//code stolen from circletool by matcool
	// code restolen by me lmfao
	void createMoveMenu() {
		EditorUI::createMoveMenu();
		
		// auto* btn = this->getSpriteButton("editSprite2.png"_spr, menu_selector(MyEditorUI::onMyButton), nullptr, 0.9f);
		// removed since uhh idk how to make it not be in the green frame thing lol

		auto* btn = this->getSpriteButton("editSprite.png"_spr, menu_selector(MyEditorUI::onMyButton), nullptr, 0.9f);
		m_editButtonBar->m_buttonArray->addObject(btn);
		auto rows = GameManager::sharedState()->getIntGameVariable("0049");
		auto cols = GameManager::sharedState()->getIntGameVariable("0050");
		// TODO: replace with reloadItemsInNormalSize
		// TODO: Figure out what jojo meant 
		m_editButtonBar->reloadItems(rows, cols);
	}

	void onMyButton(CCObject*) {
		if (!m_selectedObject || m_selectedObjects->count() >= 2) {
			FLAlertLayer::create("Error", "You can currently only edit one object at a time", "Ok")->show();
			return;
		}
		EditObjectPopup::create()->show();
	}
};
