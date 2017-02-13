//
//  CATableView.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 16-4-20.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CACell.h"
#include "support/CAUIEditorParser.h"

NS_CC_BEGIN

CACell::CACell()
:m_pBackgroundView(NULL)
,m_pContentView(NULL)
,m_bControlStateEffect(true)
,m_bAllowsSelected(true)
,m_pParser(NULL)
{
    this->setHaveNextResponder(true);
    this->setDisplayRange(false);
    this->setColor(CAColor_clear);
}

CACell::~CACell()
{
    CC_SAFE_RELEASE_NULL(m_pContentView);
    CC_SAFE_RELEASE_NULL(m_pBackgroundView);
    CC_SAFE_RELEASE_NULL(m_pParser);
}

CACell* CACell::create(const std::string& reuseIdentifier)
{
    CACell* cell = new CACell();
    if (cell && cell->initWithReuseIdentifier(reuseIdentifier))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

bool CACell::initWithReuseIdentifier(const std::string& reuseIdentifier)
{
    m_pContentView = new CAView();
    m_pContentView->setLayout(DLayoutFill);
    this->addSubview(m_pContentView);
    
    this->setBackgroundView(CAView::create());
    this->setReuseIdentifier(reuseIdentifier);
    this->normalCell();
    this->parser();
    
    return true;
}

CAView* CACell::getViewWithID(const std::string &tag)
{
    return m_pParser->m_mViews.at(tag);
}

void CACell::parser()
{
    m_pParser = new CAUIEditorParser();
    
    std::string filePath = "r/cell/" + m_sReuseIdentifier + ".xib";
    
    m_pParser->initWithPath(filePath, m_pContentView);
    
}

void CACell::setBackgroundView(CrossApp::CAView *var)
{
    CC_RETURN_IF(var == m_pBackgroundView);
    m_pContentView->removeSubview(m_pBackgroundView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pBackgroundView);
    m_pBackgroundView = var;
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setLayout(DLayoutFill);
    m_pContentView->insertSubview(m_pBackgroundView, -1);
}

CAView* CACell::getBackgroundView()
{ 
    return m_pBackgroundView;
}

void CACell::setControlState(CAControl::State var)
{
    if (m_bAllowsSelected == false && var == CAControl::State::Selected)
    {
        m_eState = CAControl::State::Normal;
    }
    else
    {
        m_eState = var;
    }
    
    CC_RETURN_IF(m_bControlStateEffect == false);
    switch (m_eState)
    {
        case CAControl::State::Normal:
            this->normalCell();
            break;
        case CAControl::State::Highlighted:
            this->highlightedCell();
            break;
        case CAControl::State::Selected:
            this->selectedCell();
            break;
        case CAControl::State::Disabled:
            this->disabledCell();
            break;
        default:
            break;
    }
}

CAControl::State CACell::getControlState()
{
    return m_eState;
}

void CACell::setControlStateNormal()
{
    this->setControlState(CAControl::State::Normal);
}

void CACell::setControlStateHighlighted()
{
    this->setControlState(CAControl::State::Highlighted);
}

void CACell::setControlStateDisabled()
{
    this->setControlState(CAControl::State::Disabled);
}

void CACell::setControlStateSelected()
{
    this->setControlState(CAControl::State::Selected);
}

void CACell::normalCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(255, 255, 255, 255));
}

void CACell::highlightedCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(240, 240, 240, 255));
}


void CACell::selectedCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CACell::disabledCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(127, 127, 127, 255));
}

void CACell::resetCell()
{
    this->setVisible(true);
    this->normalCell();
    this->recoveryCell();
    m_pContentView->setLayout(DLayoutFill);
    m_pContentView->setScale(1.0f);
    m_pContentView->setRotation(0);
}

void CACell::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    CC_RETURN_IF(m_eState == CAControl::State::Disabled);
    if (m_bAllowsSelected == false)
    {
        this->setControlStateHighlighted();
    }
}

void CACell::mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent)
{
    CC_RETURN_IF(m_eState == CAControl::State::Disabled);
    if (m_bAllowsSelected == false)
    {
        setControlStateNormal();
    }
}

NS_CC_END