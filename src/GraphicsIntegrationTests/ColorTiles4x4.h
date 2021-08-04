#pragma once

#include <array>

#include "Graphics/Color.h"

static Graphics::Color ColorBackgroundUglyYellow(0x80, 0x80, 0x00, 0xff);

// Colors used in "colortiles4x4.png"
static Graphics::Color ColorWhite(0xff, 0xff, 0xff, 0xff);
static Graphics::Color ColorRed(0xff, 0x00, 0x00, 0xff);
static Graphics::Color ColorGreen(0x00, 0xff, 0x00, 0xff);
static Graphics::Color ColorBlue(0x00, 0x00, 0xff, 0xff);

static Graphics::Color ColorBlack(0x00, 0x00, 0x00, 0xff);
static Graphics::Color ColorYellow(0xff, 0xff, 0x00, 0xff);
static Graphics::Color ColorCyan(0x00, 0xff, 0xff, 0xff);
static Graphics::Color ColorMagenta(0xff, 0x00, 0xff, 0xff);

static Graphics::Color ColorDarkGray(0x44, 0x44, 0x44, 0xff);
static Graphics::Color ColorLightPurple(0xcc, 0xcc, 0xff, 0xff);
static Graphics::Color ColorLightGreen(0xcc, 0xff, 0xcc, 0xff);
static Graphics::Color ColorLightPeach(0xff, 0xcc, 0xcc, 0xff);

static Graphics::Color ColorLightGray(0xcc, 0xcc, 0xcc, 0xff);
static Graphics::Color ColorPurple(0xaa, 0x00, 0xff, 0xff);
static Graphics::Color ColorSkyBlue(0x00, 0xaa, 0xff, 0xff);
static Graphics::Color ColorOrange(0xff, 0xaa, 0x00, 0xff);

// Positions of colors used in "colortiles4x4.png"
static constexpr std::array<uint8_t, 4> TileWhite = { 0, 0, 0, 0 };
static constexpr std::array<uint8_t, 4> TileRed   = { 1, 0, 0, 0 };
static constexpr std::array<uint8_t, 4> TileGreen = { 2, 0, 0, 0 };
static constexpr std::array<uint8_t, 4> TileBlue  = { 3, 0, 0, 0 };

static constexpr std::array<uint8_t, 4> TileBlack   = { 0, 1, 0, 0 };
static constexpr std::array<uint8_t, 4> TileYellow  = { 1, 1, 0, 0 };
static constexpr std::array<uint8_t, 4> TileCyan    = { 2, 1, 0, 0 };
static constexpr std::array<uint8_t, 4> TileMagenta = { 3, 1, 0, 0 };

static constexpr std::array<uint8_t, 4> TileDarkGray    = { 0, 2, 0, 0 };
static constexpr std::array<uint8_t, 4> TileLightPurple = { 1, 2, 0, 0 };
static constexpr std::array<uint8_t, 4> TileLightGreen  = { 2, 2, 0, 0 };
static constexpr std::array<uint8_t, 4> TileLightPeach  = { 3, 2, 0, 0 };

static constexpr std::array<uint8_t, 4> TileLightGray = { 0, 3, 0, 0 };
static constexpr std::array<uint8_t, 4> TilePurple    = { 1, 3, 0, 0 };
static constexpr std::array<uint8_t, 4> TileSkyBlue   = { 2, 3, 0, 0 };
static constexpr std::array<uint8_t, 4> TileOrange    = { 3, 3, 0, 0 };