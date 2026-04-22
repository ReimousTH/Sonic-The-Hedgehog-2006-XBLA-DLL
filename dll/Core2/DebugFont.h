#pragma once
#ifndef DEBUGFONT_H
#define DEBUGFONT_H

#include <xtl.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <xgraphics.h>
#include <vector>
#include <string>

// Flags for the Font::DrawText() function
#define ATGFONT_LEFT       0x00000000
#define ATGFONT_RIGHT      0x00000001
#define ATGFONT_CENTER_X   0x00000002
#define ATGFONT_CENTER_Y   0x00000004
#define ATGFONT_TRUNCATED  0x00000008

// Structure for BMFont character info (binary format)
#pragma pack(push, 1)
struct BMFontChar
{
    DWORD id;
    WORD x;
    WORD y;
    WORD width;
    WORD height;
    short xoffset;
    short yoffset;
    short xadvance;
    BYTE page;
    BYTE chnl;
};

struct BMFontKerning
{
    DWORD first;
    DWORD second;
    short amount;
};

struct BMFontInfo
{
    WORD fontSize;
    BYTE bitField;
    BYTE charSet;
    WORD stretchH;
    BYTE aa;
    BYTE paddingUp;
    BYTE paddingRight;
    BYTE paddingDown;
    BYTE paddingLeft;
    BYTE spacingHoriz;
    BYTE spacingVert;
    BYTE outline;
};

struct BMFontCommon
{
    WORD lineHeight;
    WORD base;
    WORD scaleW;
    WORD scaleH;
    WORD pages;
    BYTE bitField;
    BYTE alphaChnl;
    BYTE redChnl;
    BYTE greenChnl;
    BYTE blueChnl;
};
#pragma pack(pop)

// Name: GLYPH_ATTR
// Desc: Structure to hold information about one glyph (font character image)
struct GLYPH_ATTR
{
    WORD  tu1, tv1, tu2, tv2; // Texture coordinates for the image
    SHORT wOffset;            // Pixel offset for glyph start
    SHORT wWidth;             // Pixel width of the glyph
    SHORT wAdvance;           // Pixels to advance after the glyph
    WORD  wMask;              // Channel mask
};

// Name: SavedStates
// Desc: List of states to save
enum SavedStates
{
    SAVEDSTATE_D3DRS_ALPHABLENDENABLE,
    SAVEDSTATE_D3DRS_SRCBLEND,
    SAVEDSTATE_D3DRS_DESTBLEND,
    SAVEDSTATE_D3DRS_ALPHATESTENABLE,
    SAVEDSTATE_D3DRS_ALPHAREF,
    SAVEDSTATE_D3DRS_ALPHAFUNC,
    SAVEDSTATE_D3DRS_FILLMODE,
    SAVEDSTATE_D3DRS_CULLMODE,
    SAVEDSTATE_D3DRS_ZENABLE,
    SAVEDSTATE_D3DRS_STENCILENABLE,
    SAVEDSTATE_D3DRS_VIEWPORTENABLE,
    SAVEDSTATE_D3DSAMP_MINFILTER,
    SAVEDSTATE_D3DSAMP_MAGFILTER,
    SAVEDSTATE_D3DSAMP_ADDRESSU,
    SAVEDSTATE_D3DSAMP_ADDRESSV,
    SAVEDSTATE_COUNT,
};

// Name: DebugFont
// Desc: Class to implement texture-based font rendering using AngelCode BMFont binary format
class DebugFont
{
public:
    // Public device pointer as requested
    LPDIRECT3DDEVICE9 pDevice;

    // Font vertical dimensions
    FLOAT         m_fFontHeight;
    FLOAT         m_fFontTopPadding;
    FLOAT         m_fFontBottomPadding;
    FLOAT         m_fFontYAdvance;
    FLOAT         m_fXScaleFactor;
    FLOAT         m_fYScaleFactor;
    FLOAT         m_fSlantFactor;

    D3DRECT       m_rcWindow;
    FLOAT         m_fCursorX;
    FLOAT         m_fCursorY;

    // Translator table for supporting unicode ranges - LIKE ORIGINAL!
    WCHAR         m_cMaxGlyph;
    SHORT* m_TranslatorTable;

    // Glyph data for the font
    DWORD         m_dwNumGlyphs;
    GLYPH_ATTR* m_Glyphs;

    // Saved state for rendering
    BOOL          m_bSaveState;
    DWORD         m_dwSavedState[SAVEDSTATE_COUNT];
    DWORD         m_dwNestedBeginCount;

    // D3D rendering objects
    LPDIRECT3DTEXTURE9   m_pFontTexture;

    // Texture dimensions
    FLOAT         m_fTexWidth;
    FLOAT         m_fTexHeight;

public:
    // Access functions
    LPDIRECT3DTEXTURE9 GetTexture() const { return m_pFontTexture; }
    FLOAT              GetFontHeight() const { return m_fFontYAdvance; }

public:
    DebugFont();
    ~DebugFont();

    // Functions to create and destroy the internal objects
    HRESULT Create(const CHAR* strShaderFontFileName, const CHAR* strFontFileName, const CHAR* strTextureFileName = NULL);
    HRESULT Create(const char* pShaderData, LPDIRECT3DTEXTURE9 pFontTexture, const VOID* pFontData, DWORD dwDataSize);
    VOID    Destroy();

    // Returns the dimensions of a text string
    VOID    GetTextExtent(const WCHAR* strText, FLOAT* pWidth,
        FLOAT* pHeight, BOOL bFirstLineOnly = FALSE) const;
    FLOAT   GetTextWidth(const WCHAR* strText) const;

    VOID    SetWindow(D3DRECT rcWindow);
    VOID    SetWindow(LONG x1, LONG y1, LONG x2, LONG y2);
    VOID    SetCursorPosition(FLOAT fCursorX, FLOAT fCursorY);
    VOID    SetScaleFactors(FLOAT fXScaleFactor, FLOAT fYScaleFactor);
    VOID    SetSlantFactor(FLOAT fSlantFactor);

    // Function to create a texture containing rendered text
    LPDIRECT3DTEXTURE9 CreateTexture(const WCHAR* strText,
        D3DCOLOR dwBackgroundColor = 0x00000000,
        D3DCOLOR dwTextColor = 0xffffffff,
        D3DFORMAT d3dFormat = D3DFMT_A8R8G8B8);

    // Public calls to render text
    VOID Begin();
    VOID DrawText(DWORD dwColor, const WCHAR* strText, DWORD dwFlags = 0L,
        FLOAT fMaxPixelWidth = 0.0f);
    VOID DrawText(FLOAT sx, FLOAT sy, DWORD dwColor, const WCHAR* strText,
        DWORD dwFlags = 0L, FLOAT fMaxPixelWidth = 0.0f);
    VOID End();

private:
    // Helper functions
    HRESULT CreateFontShaders(const char* g_strFontShader);
    VOID    ParseBMFontData(const BYTE* pData, DWORD dwSize);

    // Byteswap functions for Xbox 360 (Big Endian)
    static WORD Byteswap16(WORD value) {
        return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
    }

    static DWORD Byteswap32(DWORD value) {
        return ((value >> 24) & 0x000000FF) |
            ((value >> 8) & 0x0000FF00) |
            ((value << 8) & 0x00FF0000) |
            ((value << 24) & 0xFF000000);
    }

    static short Byteswap16(short value) {
        return (short)Byteswap16((WORD)value);
    }
};

#endif // DEBUGFONT_H