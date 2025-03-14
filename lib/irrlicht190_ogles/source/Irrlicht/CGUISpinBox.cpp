// Copyright (C) 2006-2012 Michael Zeilfelder
// This file uses the licence of the Irrlicht Engine.

#include "CGUISpinBox.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIEditBox.h"
#include "IGUIButton.h"
#include "IGUIEnvironment.h"
#include "IEventReceiver.h"
#include "fast_atof.h"


namespace irr
{
namespace gui
{

//! constructor
CGUISpinBox::CGUISpinBox(const wchar_t* text, bool border,IGUIEnvironment* environment,
			IGUIElement* parent, s32 id, const core::rect<s32>& rectangle, bool hasButtons)
: IGUISpinBox(environment, parent, id, rectangle),
	EditBox(0), ButtonSpinUp(0), ButtonSpinDown(0), StepSize(1.f),
	RangeMin(-FLT_MAX), RangeMax(FLT_MAX), FormatString(L"%f"),
	DecimalPlaces(-1), ValidateOn(EGUI_SBV_ENTER|EGUI_SBV_LOSE_FOCUS),
	OldValue(0.f)
{
	#ifdef _DEBUG
	setDebugName("CGUISpinBox");
	#endif

	CurrentIconColor = video::SColor(255,255,255,255);
	s32 ButtonWidth = 16;

	if ( hasButtons )
	{
		ButtonSpinDown = Environment->addButton(
			core::rect<s32>(rectangle.getWidth() - ButtonWidth, rectangle.getHeight()/2 +1,
							rectangle.getWidth(), rectangle.getHeight()), this);
		ButtonSpinDown->grab();
		ButtonSpinDown->setSubElement(true);
		ButtonSpinDown->setTabStop(false);
		ButtonSpinDown->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_LOWERRIGHT);

		ButtonSpinUp = Environment->addButton(
			core::rect<s32>(rectangle.getWidth() - ButtonWidth, 0,
							rectangle.getWidth(), rectangle.getHeight()/2), this);
		ButtonSpinUp->grab();
		ButtonSpinUp->setSubElement(true);
		ButtonSpinUp->setTabStop(false);
		ButtonSpinUp->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_CENTER);
	}
	else
	{
		ButtonWidth = 0;
	}

	const core::rect<s32> rectEdit(0, 0, rectangle.getWidth() - ButtonWidth - 1, rectangle.getHeight());
	EditBox = Environment->addEditBox(text, rectEdit, border, this, -1);
	EditBox->grab();
	EditBox->setSubElement(true);
	EditBox->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);

	refreshSprites();
}


//! destructor
CGUISpinBox::~CGUISpinBox()
{
	if (ButtonSpinUp)
		ButtonSpinUp->drop();
	if (ButtonSpinDown)
		ButtonSpinDown->drop();
	if (EditBox)
		EditBox->drop();
}

void CGUISpinBox::refreshSprites()
{
	if ( !ButtonSpinDown )
		return;

	IGUISpriteBank *sb = 0;
	if (Environment && Environment->getSkin())
	{
		sb = Environment->getSkin()->getSpriteBank();
	}

	if (sb)
	{
		IGUISkin * skin = Environment->getSkin();
		CurrentIconColor = skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL);
		ButtonSpinDown->setSpriteBank(sb);
		ButtonSpinDown->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_SMALL_CURSOR_DOWN), CurrentIconColor);
		ButtonSpinDown->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_SMALL_CURSOR_DOWN), CurrentIconColor);
		ButtonSpinUp->setSpriteBank(sb);
		ButtonSpinUp->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_SMALL_CURSOR_UP), CurrentIconColor);
		ButtonSpinUp->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_SMALL_CURSOR_UP), CurrentIconColor);
	}
	else
	{
		ButtonSpinDown->setText(L"-");
		ButtonSpinUp->setText(L"+");
	}
}

IGUIEditBox* CGUISpinBox::getEditBox() const
{
	return EditBox;
}


void CGUISpinBox::setValue(f32 val)
{
	wchar_t str[100];
	swprintf_irr(str, 99, FormatString.c_str(), val);
	EditBox->setText(str);
	verifyValueRange(getValue());
}


f32 CGUISpinBox::getValue() const
{
	return getValueFor(EditBox->getText());
}

f32 CGUISpinBox::getValueFor(const wchar_t* val) const
{
	if ( !val )
		return 0.f;
	core::stringc tmp(val);
	return core::fast_atof(tmp.c_str());
}

void CGUISpinBox::setRange(f32 min, f32 max)
{
	if (max<min)
		core::swap(min, max);
	RangeMin = min;
	RangeMax = max;

	// we have to round the range - otherwise we can get into an infinite setValue/verifyValueRange cycle.
	wchar_t str[100];
	swprintf_irr(str, 99, FormatString.c_str(), RangeMin);
	RangeMin = core::fast_atof(core::stringc(str).c_str());
	swprintf_irr(str, 99, FormatString.c_str(), RangeMax);
	RangeMax = core::fast_atof(core::stringc(str).c_str());

	verifyValueRange(getValue());
}


f32 CGUISpinBox::getMin() const
{
	return RangeMin;
}


f32 CGUISpinBox::getMax() const
{
	return RangeMax;
}


f32 CGUISpinBox::getStepSize() const
{
	return StepSize;
}

void CGUISpinBox::setStepSize(f32 step)
{
	StepSize = step;
}


//! Sets the number of decimal places to display.
void CGUISpinBox::setDecimalPlaces(s32 places)
{
	DecimalPlaces = places;
	if (places == -1)
		FormatString = "%f";
	else
	{
		FormatString = "%.";
		FormatString += places;
		FormatString += "f";
	}
	setRange( RangeMin, RangeMax );
	setValue(getValue());
}

//! Sets when the spinbox has to validate entered text.
void CGUISpinBox::setValidateOn(u32 validateOn)
{
	ValidateOn = validateOn;
}

//! Gets when the spinbox has to validate entered text.
u32 CGUISpinBox::getValidateOn() const
{
	return ValidateOn;
}

bool CGUISpinBox::OnEvent(const SEvent& event)
{
	if (IsEnabled)
	{
		bool changeEvent = false;
		bool eatEvent = false;
		switch(event.EventType)
		{
		case EET_MOUSE_INPUT_EVENT:
			switch(event.MouseInput.Event)
			{
			case EMIE_MOUSE_WHEEL:
				{
					OldValue = getValue();
					f32 val = OldValue + (StepSize * (event.MouseInput.Wheel < 0 ? -1.f : 1.f));
					setValue(val);
					changeEvent = true;
					eatEvent = true;
				}
				break;
			default:
				break;
			}
			break;

		case EET_GUI_EVENT:

			if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
			{
				if (event.GUIEvent.Caller == ButtonSpinUp)
				{
					OldValue = getValue();
					f32 val = OldValue + StepSize;
					setValue(val);
					changeEvent = true;
				}
				else if ( event.GUIEvent.Caller == ButtonSpinDown)
				{
					OldValue = getValue();
					f32 val = OldValue - StepSize;
					setValue(val);
					changeEvent = true;
				}
			}
			if (event.GUIEvent.Caller == EditBox)
			{
				if (	(event.GUIEvent.EventType == EGET_EDITBOX_CHANGED && ValidateOn & EGUI_SBV_CHANGE)
					||	(event.GUIEvent.EventType == EGET_EDITBOX_ENTER && ValidateOn & EGUI_SBV_ENTER)
					||	(event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST && ValidateOn & EGUI_SBV_LOSE_FOCUS)
					)
				{
					OldValue = getValue();	// no call to setValue when text was changed without setText call
					verifyValueRange(OldValue);
					changeEvent = true;
				}
			}
			break;
		default:
		break;
		}

		if ( changeEvent )
		{
			if ( Parent )
			{
				SEvent e;
				e.EventType = EET_GUI_EVENT;
				e.GUIEvent.Caller = this;
				e.GUIEvent.Element = 0;
				e.GUIEvent.EventType = EGET_SPINBOX_CHANGED;
			
				Parent->OnEvent(e);
			}
			if ( eatEvent )
				return true;
		}
	}

	return IGUIElement::OnEvent(event);
}


void CGUISpinBox::draw()
{
	if ( !isVisible() )
		return;

	IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;

	video::SColor iconColor = skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL);
	if ( iconColor != CurrentIconColor )
	{
		refreshSprites();
	}

	IGUISpinBox::draw();
}

void CGUISpinBox::verifyValueRange(f32 val)
{
	if ( val+core::ROUNDING_ERROR_f32 < RangeMin )
		setValue(RangeMin);
	else if ( val-core::ROUNDING_ERROR_f32 > RangeMax )
		setValue(RangeMax);
}


//! Sets the new caption of the element
void CGUISpinBox::setText(const wchar_t* text)
{
	EditBox->setText(text);
	setValue(getValue());
}


//! Returns caption of this element.
const wchar_t* CGUISpinBox::getText() const
{
	return EditBox->getText();
}


//! Writes attributes of the element.
void CGUISpinBox::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	IGUIElement::serializeAttributes(out, options);
	out->addFloat("Min", getMin());
	out->addFloat("Max", getMax());
	out->addFloat("Step", getStepSize());
	out->addInt("DecimalPlaces", DecimalPlaces);
	out->addInt("ValidateOn", (s32)ValidateOn);
}


//! Reads attributes of the element
void CGUISpinBox::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	IGUIElement::deserializeAttributes(in, options);
	setRange(in->getAttributeAsFloat("Min", RangeMin), in->getAttributeAsFloat("Max", RangeMax));
	setStepSize(in->getAttributeAsFloat("Step", StepSize));
	setDecimalPlaces(in->getAttributeAsInt("DecimalPlaces", DecimalPlaces));
	setValidateOn((u32)in->getAttributeAsInt("ValidateOn", (s32)ValidateOn) );
}


} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

