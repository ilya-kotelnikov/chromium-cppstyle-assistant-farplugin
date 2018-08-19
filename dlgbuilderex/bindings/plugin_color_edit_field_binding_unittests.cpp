////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#define ALERT(msg) do { ::MessageBoxA(nullptr, #msg, "Test Failed", \
                                     MB_OK | MB_ICONSTOP); } while(0)
#define EXPECT(expr) do { if (!(expr)) ALERT(expr); } while(0)

void TestGetColorValueFromHexString() {
  const COLORREF initial_color = RGB(0xFF, 0xAB, 0xBC);

  COLORREF result_color = initial_color;
  EXPECT(GetColorValueFromHexString(&result_color, L"AB35E7"));
  EXPECT(result_color == RGB(0xAB, 0x35, 0xE7));

  result_color = initial_color;
  EXPECT(GetColorValueFromHexString(&result_color, L"ab35e7"));
  EXPECT(result_color == RGB(0xAB, 0x35, 0xE7));

  result_color = initial_color;
  EXPECT(GetColorValueFromHexString(&result_color, L"Ab35Ef"));
  EXPECT(result_color == RGB(0xAB, 0x35, 0xEF));

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"GB35E7"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"AB3_E7"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"AB35E "));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"AB35E"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"AB35G"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"AB_5E"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"AB35"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"AB3"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"AB"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"A "));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"?"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L""));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"ab35e70"));
  EXPECT(result_color == initial_color);

  result_color = initial_color;
  EXPECT(!GetColorValueFromHexString(&result_color, L"ab35e700"));
  EXPECT(result_color == initial_color);
}
