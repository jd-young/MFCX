/*!
     \file     CryptTest.cpp
     \author   John Young
     \date     28 February 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include "../include/Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


TEST(CCryptTest, TestEncrypt)
{
     CString sOrig = "This is the text to encrypt";

     CCrypt crypt ("The key");

     CString sEncrypted = crypt.Encrypt (sOrig);
     ASSERT_EQ (37, sEncrypted.GetLength());
     
     ASSERT_EQ (sOrig, crypt.Decrypt (sEncrypted));
}
