#include "editObjectPopup.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

// i hate cocos ui
// i hate rewriting cocos ui

void EditObjectPopup::applyChanges(CCObject* sender) {
    using geode::utils::numFromString;
    
    auto xPosStr = this->xPosInput->getString();
    auto scaleXStr = this->scaleXInput->getString();
    auto scaleYStr = this->scaleYInput->getString();
    auto yPosStr = this->yPosInput->getString();
    auto rotationStr = this->rotationInput->getString();
    auto zOrderStr = this->zOrderInput->getString();
    
    if (xPosStr.empty() || scaleXStr.empty() || scaleYStr.empty() || yPosStr.empty() || rotationStr.empty() || zOrderStr.empty()) {
        FLAlertLayer::create("Error", "Some of the fields are empty or null.", "Ok")->show();
        return;
    }
    
    selectedObject->setPositionX(numFromString<float>(xPosStr).unwrapOr(0.f));
    selectedObject->setRScaleX(numFromString<float>(scaleXStr).unwrapOr(1.f));
    selectedObject->m_scaleX = numFromString<float>(scaleXStr).unwrapOr(1.f);
    selectedObject->setRScaleY(numFromString<float>(scaleYStr).unwrapOr(1.f));
    selectedObject->m_scaleY = numFromString<float>(scaleYStr).unwrapOr(1.f);
    selectedObject->setPositionY(numFromString<float>(yPosStr).unwrapOr(0.f));
    selectedObject->setRotation(numFromString<float>(rotationStr).unwrapOr(0.f));
    selectedObject->m_zOrder = numFromString<int>(zOrderStr).unwrapOr(0);
}

static CCNode* makeButton(const char* inputTitle, geode::TextInput*& inputName, const std::string& value) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto inputNodes = CCNode::create();
    auto lbl = CCLabelBMFont::create(inputTitle, "goldfont.fnt");
    
    lbl->setScale(0.55f);
    lbl->setAnchorPoint({ 0.5f, 0.5f });
    lbl->setPosition(60.f, 40.f);
    
    inputName = TextInput::create(110.f, inputTitle, "bigfont.fnt");
    inputName->setPosition(60.f, 16.f);
    inputName->setFilter("0123456789.-"); // was told this would fix. I have 0 clue if it will
    // its weird how it auto filers out '.' but ig its cus geode doesnt have an NumberInput or smt (floatInput?)
    inputName->setString(value);
    
    
    inputNodes->setContentSize({ 120.f, 52.f });
    inputNodes->addChild(lbl);
    inputNodes->addChild(inputName);
    return inputNodes;
}


void EditObjectPopup::createInputs() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto rowLayout = CCNode::create();
    rowLayout->setContentSize({ 460.f, 120.f });
    rowLayout->setLayout(ColumnLayout::create()
    ->setGap(8.f)
    ->setAxisAlignment(AxisAlignment::Center));
    
    auto row1 = CCNode::create();
    auto row2 = CCNode::create();
    auto row3 = CCNode::create();
    
    row1->setContentSize({ 460.f, 52.f });
    row2->setContentSize({ 460.f, 52.f });
    // row 3 decrease by 8f
    
    row1->setLayout(RowLayout::create());
    row2->setLayout(RowLayout::create());
    
    
    
    row1->addChild(makeButton("X Pos", xPosInput, fmt::to_string(selectedObject->getPositionX())));
    row1->addChild(makeButton("Scale X", scaleXInput, fmt::to_string(selectedObject->getScaleX())));
    row1->addChild(makeButton("Rotation", rotationInput, fmt::to_string(selectedObject->getRotation())));
    row1->updateLayout();
    
    row2->addChild(makeButton("Y Pos", yPosInput, fmt::to_string(selectedObject->getPositionY())));
    row2->addChild(makeButton("Scale Y", scaleYInput, fmt::to_string(selectedObject->getScaleY())));
    row2->addChild(makeButton("Z Order", zOrderInput, fmt::to_string(selectedObject->m_zOrder)));
    row2->updateLayout();
    
    // row3->addChild(makeButton("Object ID", objectIDInput, fmt::to_string(selectedObject->getObjectID())));
    // sneak peaks ig for anyone whos in here    
    
    rowLayout->addChild(row1);
    rowLayout->addChild(row2);
    rowLayout->setAnchorPoint({ 0.5f, 0.5f });
    rowLayout->setPosition(winSize.width / 2, winSize.height / 2 + 25.f); // 25 clear button
    
    rowLayout->updateLayout();
    
    this->addChild(rowLayout);
    
}

bool EditObjectPopup::init(float width, float height) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    if (!Popup::init(width, height)) return false;
    m_noElasticity = true;
    
    this->setTitle("Edit Object Variables", "goldFont.fnt");
    
    auto vLable = CCLabelBMFont::create(
        fmt::format("v{}", geode::Mod::get()->getVersion().toNonVString()).c_str(), "bigFont.fnt"
    );
    vLable->setScale(0.25f);
    vLable->setOpacity(220);
    vLable->setPosition({ winSize.width / 2, winSize.height / 2 + 83.5f });
    this->addChild(vLable);
    
    
    auto m_applyButton = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Apply changes"), this, menu_selector(EditObjectPopup::applyChanges)
    );
    
    auto applyButtonCont = CCMenu::create();
    applyButtonCont->setPosition(winSize.width / 2, winSize.height / 2 - 80.f);
    applyButtonCont->addChild(m_applyButton);
    this->addChild(applyButtonCont);
    
    createInputs();
    return true;
}

EditObjectPopup* EditObjectPopup::create() {
    auto ret = new EditObjectPopup();
    if (ret->init(485.f, 230.f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
};