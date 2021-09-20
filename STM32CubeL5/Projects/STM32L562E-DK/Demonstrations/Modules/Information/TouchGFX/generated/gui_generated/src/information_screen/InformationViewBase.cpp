/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/information_screen/InformationViewBase.hpp>
#include "BitmapDatabase.hpp"

InformationViewBase::InformationViewBase() :
    buttonCallback(this, &InformationViewBase::buttonCallbackHandler)
{
    background.setXY(0, 0);
    background.setBitmap(touchgfx::Bitmap(BITMAP_INFOPAGE_ID));

    button1.setXY(156, 154);
    button1.setBitmaps(touchgfx::Bitmap(BITMAP_BACKBUTTON_ID), touchgfx::Bitmap(BITMAP_BACKBUTTONPRESSED_ID));
    button1.setAction(buttonCallback);

    add(background);
    add(button1);
}

void InformationViewBase::setupScreen()
{

}

void InformationViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &button1)
    {
        //SystemReset
        //When button1 clicked call virtual function
        //Call systemReset
        systemReset();
    }
}
