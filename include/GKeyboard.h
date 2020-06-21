//
//   GKeyboard.h     Global Keyboard object, used to get info about the keyboard.
// 

#ifndef   __MFCX_GKEYBOARD_H
#define   __MFCX_GKEYBOARD_H

class GKeyboard
{
public:
     int CodePage() const     { return GetKBCodePage(); }
     int Type() const         { return GetKeyboardType (0); }
     int SubType() const      { return GetKeyboardType (1); }
     int NumFnKeys() const    { return GetKeyboardType (2); }
     int KeyName (LONG lparam, TCHAR *buf, int buflen) const
     {    return GetKeyNameText (lparam, buf, buflen); }                           
     
     int State(int vkey) const          { return GetKeyState (vkey); }
     void GetState (BYTE* buf) const    { GetKeyboardState (buf); }
     void SetState (BYTE* buf) const    { SetKeyboardState (buf); }
     BOOL IsArrowKey(int key) const     { return VK_LEFT<=key && key<=VK_DOWN; }
     
     BOOL IsShiftPressed() const { return IsPressed (VK_SHIFT); }
     BOOL IsCtrlPressed() const  { return IsPressed (VK_CONTROL); }

     BOOL IsPressed (int vkey) const;
};


extern GKeyboard theKeyboard;

#endif    // __MFCX_GKEYBOARD_H
