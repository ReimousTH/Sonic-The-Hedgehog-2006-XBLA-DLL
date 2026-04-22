#include "DebugFont.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Static shader members
static LPDIRECT3DVERTEXDECLARATION9 g_pFontVertexDecl = NULL;
static LPDIRECT3DVERTEXSHADER9      g_pFontVertexShader = NULL;
static LPDIRECT3DPIXELSHADER9       g_pFontPixelShader = NULL;

//--------------------------------------------------------------------------------------
// Name: DebugFont()
// Desc: Constructor
//--------------------------------------------------------------------------------------
DebugFont::DebugFont()
{
    pDevice = NULL;
    m_pFontTexture = NULL;
    m_dwNumGlyphs = 0;
    m_Glyphs = NULL;
    m_fCursorX = 0.0f;
    m_fCursorY = 0.0f;
    m_fXScaleFactor = 1.0f;
    m_fYScaleFactor = 1.0f;
    m_fSlantFactor = 0.0f;
    m_fTexWidth = 0.0f;
    m_fTexHeight = 0.0f;
    m_dwNestedBeginCount = 0;
    m_cMaxGlyph = 0;
    m_TranslatorTable = NULL;
}

//--------------------------------------------------------------------------------------
// Name: ~DebugFont()
// Desc: Destructor
//--------------------------------------------------------------------------------------
DebugFont::~DebugFont()
{
    Destroy();
}

//--------------------------------------------------------------------------------------
// Name: CreateFontShaders()
// Desc: Creates the global font shaders
//--------------------------------------------------------------------------------------
HRESULT DebugFont::CreateFontShaders(const char* g_strFontShader)
{
    // Create vertex declaration (EXACTLY LIKE ORIGINAL!)
    if (NULL == g_pFontVertexDecl)
    {
        D3DVERTEXELEMENT9 decl[] =
        {
            { 0,  0, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0,  8, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
            D3DDECL_END()
        };

        if (FAILED(pDevice->CreateVertexDeclaration(decl, &g_pFontVertexDecl)))
            return E_FAIL;
    }
    else
    {
        g_pFontVertexDecl->AddRef();
    }

    // Create vertex shader
    if (NULL == g_pFontVertexShader)
    {
        LPD3DXBUFFER pShaderCode;
        LPD3DXBUFFER pErrorMsgs;

        if (FAILED(D3DXCompileShader(g_strFontShader, strlen(g_strFontShader),
            NULL, NULL, "FontVertexShader", "vs_2_0", 0,
            &pShaderCode, &pErrorMsgs, NULL)))
        {
            if (pErrorMsgs)
            {
                OutputDebugStringA((CHAR*)pErrorMsgs->GetBufferPointer());
                pErrorMsgs->Release();
            }
            return E_FAIL;
        }

        if (FAILED(pDevice->CreateVertexShader((DWORD*)pShaderCode->GetBufferPointer(),
            &g_pFontVertexShader)))
        {
            pShaderCode->Release();
            return E_FAIL;
        }

        pShaderCode->Release();
    }
    else
    {
        g_pFontVertexShader->AddRef();
    }

    // Create pixel shader
    if (NULL == g_pFontPixelShader)
    {
        LPD3DXBUFFER pShaderCode;
        LPD3DXBUFFER pErrorMsgs;

        if (FAILED(D3DXCompileShader(g_strFontShader, strlen(g_strFontShader),
            NULL, NULL, "FontPixelShader", "ps_2_0", 0,
            &pShaderCode, &pErrorMsgs, NULL)))
        {
            if (pErrorMsgs)
            {
                OutputDebugStringA((CHAR*)pErrorMsgs->GetBufferPointer());
                pErrorMsgs->Release();
            }
            return E_FAIL;
        }

        if (FAILED(pDevice->CreatePixelShader((DWORD*)pShaderCode->GetBufferPointer(),
            &g_pFontPixelShader)))
        {
            pShaderCode->Release();
            return E_FAIL;
        }

        pShaderCode->Release();
    }
    else
    {
        g_pFontPixelShader->AddRef();
    }

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: ParseBMFontData()
// Desc: Parses AngelCode BMFont binary data - FIXED FOR XBOX 360
//--------------------------------------------------------------------------------------
VOID DebugFont::ParseBMFontData(const BYTE* pData, DWORD dwSize)
{
    // Clear existing data
    if (m_Glyphs) { delete[] m_Glyphs; m_Glyphs = NULL; }
    if (m_TranslatorTable) { delete[] m_TranslatorTable; m_TranslatorTable = NULL; }

    const BYTE* pStart = pData;
    const BYTE* pEnd = pData + dwSize;

    // Check BMFont header
    if (dwSize < 4 || pData[0] != 'B' || pData[1] != 'M' || pData[2] != 'F')
    {
       
        return;
    }

    BYTE version = pData[3];
    pData += 4;

    std::vector<BMFontChar> chars;
    BMFontCommon common = { 0 };
    WCHAR maxCharId = 0;

    // Parse blocks
    while (pData < pEnd)
    {
        BYTE blockType = *pData++;

        // Read block size (4 bytes, LITTLE ENDIAN)
        DWORD blockSize =
            (pData[0] << 0) | (pData[1] << 8) | (pData[2] << 16) | (pData[3] << 24);
        pData += 4;

        const BYTE* blockEnd = pData + blockSize;

        switch (blockType)
        {
        case 1: // info block
            pData = blockEnd;
            break;

        case 2: // common block
            if (blockSize >= 15)
            {
                // Read LITTLE ENDIAN values
                common.lineHeight = pData[0] | (pData[1] << 8);
                common.base = pData[2] | (pData[3] << 8);
                common.scaleW = pData[4] | (pData[5] << 8);
                common.scaleH = pData[6] | (pData[7] << 8);
                common.pages = pData[8] | (pData[9] << 8);
                common.bitField = pData[10];
                common.alphaChnl = pData[11];
                common.redChnl = pData[12];
                common.greenChnl = pData[13];
                common.blueChnl = pData[14];

                m_fFontHeight = (FLOAT)common.lineHeight;
                m_fFontYAdvance = (FLOAT)common.lineHeight;
                m_fTexWidth = (FLOAT)common.scaleW;
                m_fTexHeight = (FLOAT)common.scaleH;
            }
            pData = blockEnd;
            break;

        case 3: // pages block
            pData = blockEnd;
            break;

        case 4: // chars block
        {
            DWORD charCount = blockSize / 20; // 20 bytes per char

            for (DWORD i = 0; i < charCount; i++)
            {
                BMFontChar bmChar;

                // Read ID (4 bytes LITTLE ENDIAN)
                bmChar.id = pData[0] | (pData[1] << 8) | (pData[2] << 16) | (pData[3] << 24);

                // Other values (2 bytes each, LITTLE ENDIAN)
                bmChar.x = pData[4] | (pData[5] << 8);
                bmChar.y = pData[6] | (pData[7] << 8);
                bmChar.width = pData[8] | (pData[9] << 8);
                bmChar.height = pData[10] | (pData[11] << 8);
                bmChar.xoffset = (short)(pData[12] | (pData[13] << 8));
                bmChar.yoffset = (short)(pData[14] | (pData[15] << 8));
                bmChar.xadvance = (short)(pData[16] | (pData[17] << 8));
                bmChar.page = pData[18];
                bmChar.chnl = pData[19];

                chars.push_back(bmChar);

                if (bmChar.id > maxCharId && bmChar.id < 0xFFFF)
                    maxCharId = (WCHAR)bmChar.id;

                pData += 20;
            }
            break;
        }

        case 5: // kerning block
            pData = blockEnd;
            break;

        default:
            pData = blockEnd;
            break;
        }
    }

    // Create glyphs
    m_dwNumGlyphs = (DWORD)chars.size() + 1;
    m_Glyphs = new GLYPH_ATTR[m_dwNumGlyphs];

    // Default glyph (index 0)
    m_Glyphs[0].tu1 = 0;
    m_Glyphs[0].tv1 = 0;
    m_Glyphs[0].tu2 = 0;
    m_Glyphs[0].tv2 = 0;
    m_Glyphs[0].wOffset = 0;
    m_Glyphs[0].wWidth = 0;
    m_Glyphs[0].wAdvance = (SHORT)(m_fFontHeight / 3.0f);
    m_Glyphs[0].wMask = 0x0000; // FIX: Start with no mask for default glyph

    // Set max glyph - IMPORTANT: Ensure enough space for ASCII
    m_cMaxGlyph = (maxCharId < 255) ? 255 : maxCharId;

    // Allocate translator table
    m_TranslatorTable = new SHORT[m_cMaxGlyph + 1];
    memset(m_TranslatorTable, 0, sizeof(SHORT) * (m_cMaxGlyph + 1));

    // Create glyphs and build translator table
    for (DWORD i = 0; i < chars.size(); i++)
    {
        const BMFontChar& bmChar = chars[i];
        GLYPH_ATTR& glyph = m_Glyphs[i + 1];

        // Store PIXEL coordinates (CORRECT - shader normalizes these)
        glyph.tu1 = (WORD)bmChar.x;
        glyph.tv1 = (WORD)bmChar.y;
        glyph.tu2 = (WORD)(bmChar.x + bmChar.width);
        glyph.tv2 = (WORD)(bmChar.y + bmChar.height);

        glyph.wOffset = (SHORT)bmChar.xoffset;
        glyph.wWidth = (SHORT)bmChar.width;
        glyph.wAdvance = (SHORT)bmChar.xadvance;

        // FIX: BMFont chnl is a BIT MASK, not channel number!
        // 1 = blue, 2 = green, 4 = red, 8 = alpha
        // 15 = all channels (1|2|4|8)
        // Some fonts use 1 for all data in blue channel
        WORD wMask = 0;

        if (bmChar.chnl == 1)       // Blue channel only
            wMask = 0x000F;
        else if (bmChar.chnl == 2)  // Green channel only
            wMask = 0x00F0;
        else if (bmChar.chnl == 4)  // Red channel only
            wMask = 0x0F00;
        else if (bmChar.chnl == 8)  // Alpha channel only
            wMask = 0xF000;
        else if (bmChar.chnl == 15) // All channels (most common)
            wMask = 0xFFFF;
        else                         // Default to all channels
            wMask = 0xFFFF;

        glyph.wMask = wMask;

        // Add to translator table
        if (bmChar.id <= m_cMaxGlyph)
            m_TranslatorTable[bmChar.id] = (SHORT)(i + 1);
    }
}

//--------------------------------------------------------------------------------------
// Name: Create()
// Desc: Create the font from BMFont file and texture
//--------------------------------------------------------------------------------------
HRESULT DebugFont::Create(const CHAR* strShaderFontFileName, const CHAR* strFontFileName, const CHAR* strTextureFileName)
{
    HRESULT hr = S_OK;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    HANDLE hFileShader = INVALID_HANDLE_VALUE;
    BYTE* pData = 0;
    BYTE* pDataShader = 0;

    // Load font data
    hFile = CreateFileA(strFontFileName, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        hr = E_FAIL;
        goto CLEANUP;
    }

    DWORD dwSize = GetFileSize(hFile, NULL);
    if (dwSize == INVALID_FILE_SIZE)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto CLEANUP;
    }

    pData = new (std::nothrow) BYTE[dwSize];
    if (!pData)
    {
        hr = E_OUTOFMEMORY;
        goto CLEANUP;
    }

    DWORD dwBytesRead;
    if (!ReadFile(hFile, pData, dwSize, &dwBytesRead, NULL) || dwBytesRead != dwSize)
    {
        hr = E_FAIL;
        goto CLEANUP;
    }
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;

    // Parse BMFont data
    ParseBMFontData(pData, dwSize);

    delete[] pData;
    pData = 0;

    // Load texture
    if (strTextureFileName)
    {
        hr = D3DXCreateTextureFromFileA(pDevice, strTextureFileName, &m_pFontTexture);
        if (FAILED(hr))
            goto CLEANUP;
    }

    // Initialize the window
    D3DDISPLAYMODE DisplayMode;
    if (FAILED(pDevice->GetDisplayMode(0, &DisplayMode)))
    {
        hr = E_FAIL;
        goto CLEANUP;
    }

    m_rcWindow.x1 = 0;
    m_rcWindow.y1 = 0;
    m_rcWindow.x2 = DisplayMode.Width;
    m_rcWindow.y2 = DisplayMode.Height;

    // Determine whether we should save/restore state
    m_bSaveState = TRUE;

    // Load shader file
    hFileShader = CreateFileA(strShaderFontFileName, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFileShader == INVALID_HANDLE_VALUE)
    {
        hr = E_FAIL;
        goto CLEANUP;
    }

    DWORD dwSizeShader = GetFileSize(hFileShader, NULL);
    if (dwSizeShader == INVALID_FILE_SIZE)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto CLEANUP;
    }

    pDataShader = new (std::nothrow) BYTE[dwSizeShader + 1];
    if (!pDataShader)
    {
        hr = E_OUTOFMEMORY;
        goto CLEANUP;
    }

    memset(pDataShader, 0, dwSizeShader + 1);

    if (!ReadFile(hFileShader, pDataShader, dwSizeShader, &dwBytesRead, NULL) || dwBytesRead != dwSizeShader)
    {
        hr = E_FAIL;
        goto CLEANUP;
    }
    CloseHandle(hFileShader);
    hFileShader = INVALID_HANDLE_VALUE;

    // Create shaders
    hr = CreateFontShaders((const char*)pDataShader);
    if (FAILED(hr))
        goto CLEANUP;

    delete[] pDataShader;
    pDataShader = 0;

    hr = S_OK;

CLEANUP:
    // Clean up resources in reverse order of acquisition
    if (pDataShader)
        delete[] pDataShader;

    if (hFileShader != INVALID_HANDLE_VALUE)
        CloseHandle(hFileShader);

    if (pData)
        delete[] pData;

    if (hFile != INVALID_HANDLE_VALUE)
        CloseHandle(hFile);

    return hr;
}

//--------------------------------------------------------------------------------------
// Name: Create()
// Desc: Create the font from texture and raw BMFont data
//--------------------------------------------------------------------------------------
HRESULT DebugFont::Create(const char* pShaderData,LPDIRECT3DTEXTURE9 pFontTexture, const VOID* pFontData, DWORD dwDataSize)
{
    m_pFontTexture = pFontTexture;

    // Parse BMFont data
    ParseBMFontData((const BYTE*)pFontData, dwDataSize);

    // Initialize the window
    D3DDISPLAYMODE DisplayMode;
    pDevice->GetDisplayMode(0, &DisplayMode);
    m_rcWindow.x1 = 0;
    m_rcWindow.y1 = 0;
    m_rcWindow.x2 = DisplayMode.Width;
    m_rcWindow.y2 = DisplayMode.Height;

    // Determine whether we should save/restore state
    m_bSaveState = TRUE;

    // Create shaders
    return CreateFontShaders(pShaderData);
}

//--------------------------------------------------------------------------------------
// Name: Destroy()
// Desc: Destroy the font object
//--------------------------------------------------------------------------------------
VOID DebugFont::Destroy()
{
    if (m_Glyphs)
    {
        delete[] m_Glyphs;
        m_Glyphs = NULL;
    }

    if (m_TranslatorTable)
    {
        delete[] m_TranslatorTable;
        m_TranslatorTable = NULL;
    }

    if (m_pFontTexture)
    {
        m_pFontTexture->Release();
        m_pFontTexture = NULL;
    }

    m_dwNumGlyphs = 0;
    m_dwNestedBeginCount = 0;
    m_cMaxGlyph = 0;
}

//--------------------------------------------------------------------------------------
// Name: SetWindow()
// Desc: Sets the window for text rendering
//--------------------------------------------------------------------------------------
VOID DebugFont::SetWindow(D3DRECT rcWindow)
{
    SetWindow(rcWindow.x1, rcWindow.y1, rcWindow.x2, rcWindow.y2);
}

VOID DebugFont::SetWindow(LONG x1, LONG y1, LONG x2, LONG y2)
{
    m_rcWindow.x1 = x1;
    m_rcWindow.y1 = y1;
    m_rcWindow.x2 = x2;
    m_rcWindow.y2 = y2;
    m_fCursorX = 0.0f;
    m_fCursorY = 0.0f;
}

//--------------------------------------------------------------------------------------
// Name: SetCursorPosition()
// Desc: Sets the cursor position for drawing text
//--------------------------------------------------------------------------------------
VOID DebugFont::SetCursorPosition(FLOAT fCursorX, FLOAT fCursorY)
{
    m_fCursorX = fCursorX;
    m_fCursorY = fCursorY;
}

//--------------------------------------------------------------------------------------
// Name: SetScaleFactors()
// Desc: Sets X and Y scale factors
//--------------------------------------------------------------------------------------
VOID DebugFont::SetScaleFactors(FLOAT fXScaleFactor, FLOAT fYScaleFactor)
{
    m_fXScaleFactor = fXScaleFactor;
    m_fYScaleFactor = fYScaleFactor;
}

//--------------------------------------------------------------------------------------
// Name: SetSlantFactor()
// Desc: Sets the slant factor
//--------------------------------------------------------------------------------------
VOID DebugFont::SetSlantFactor(FLOAT fSlantFactor)
{
    m_fSlantFactor = fSlantFactor;
}

//--------------------------------------------------------------------------------------
// Name: GetTextExtent()
// Desc: Get the dimensions of a text string
//--------------------------------------------------------------------------------------
VOID DebugFont::GetTextExtent(const WCHAR* strText, FLOAT* pWidth,
    FLOAT* pHeight, BOOL bFirstLineOnly) const
{
    if (!pWidth || !pHeight)
        return;

    *pWidth = 0.0f;
    *pHeight = 0.0f;

    if (!strText)
        return;

    FLOAT sx = 0.0f;
    FLOAT sy = m_fFontHeight;

    while (*strText)
    {
        WCHAR letter = *strText++;

        // Handle newline character
        if (letter == L'\n')
        {
            if (bFirstLineOnly)
                break;
            sx = 0.0f;
            sy += m_fFontYAdvance;
        }

        // Handle carriage return
        if (letter == L'\r')
            continue;

        // Translate character (LIKE ORIGINAL!)
        GLYPH_ATTR* pGlyph;
        if (letter > m_cMaxGlyph || m_TranslatorTable[letter] == 0)
            pGlyph = &m_Glyphs[0];
        else
            pGlyph = &m_Glyphs[m_TranslatorTable[letter]];

        sx += pGlyph->wOffset;
        sx += pGlyph->wAdvance;

        if (sx > (*pWidth))
            *pWidth = sx;
        if (sy > (*pHeight))
            *pHeight = sy;
    }

    *pWidth *= m_fXScaleFactor;
    *pHeight *= m_fYScaleFactor;
}

//--------------------------------------------------------------------------------------
// Name: GetTextWidth()
// Desc: Returns the width in pixels of a text string
//--------------------------------------------------------------------------------------
FLOAT DebugFont::GetTextWidth(const WCHAR* strText) const
{
    FLOAT fTextWidth = 0.0f;
    FLOAT fTextHeight = 0.0f;
    GetTextExtent(strText, &fTextWidth, &fTextHeight);
    return fTextWidth;
}

//--------------------------------------------------------------------------------------
// Name: Begin()
// Desc: Prepares the font for rendering - EXACTLY LIKE ORIGINAL!
//--------------------------------------------------------------------------------------
VOID DebugFont::Begin()
{
    if (0 == m_dwNestedBeginCount)
    {
        // Save state
        if (m_bSaveState)
        {
            pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &m_dwSavedState[SAVEDSTATE_D3DRS_ALPHABLENDENABLE]);
            pDevice->GetRenderState(D3DRS_SRCBLEND, &m_dwSavedState[SAVEDSTATE_D3DRS_SRCBLEND]);
            pDevice->GetRenderState(D3DRS_DESTBLEND, &m_dwSavedState[SAVEDSTATE_D3DRS_DESTBLEND]);
            pDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &m_dwSavedState[SAVEDSTATE_D3DRS_ALPHATESTENABLE]);
            pDevice->GetRenderState(D3DRS_ALPHAREF, &m_dwSavedState[SAVEDSTATE_D3DRS_ALPHAREF]);
            pDevice->GetRenderState(D3DRS_ALPHAFUNC, &m_dwSavedState[SAVEDSTATE_D3DRS_ALPHAFUNC]);
            pDevice->GetRenderState(D3DRS_FILLMODE, &m_dwSavedState[SAVEDSTATE_D3DRS_FILLMODE]);
            pDevice->GetRenderState(D3DRS_CULLMODE, &m_dwSavedState[SAVEDSTATE_D3DRS_CULLMODE]);
            pDevice->GetRenderState(D3DRS_ZENABLE, &m_dwSavedState[SAVEDSTATE_D3DRS_ZENABLE]);
            pDevice->GetRenderState(D3DRS_STENCILENABLE, &m_dwSavedState[SAVEDSTATE_D3DRS_STENCILENABLE]);
            pDevice->GetRenderState(D3DRS_VIEWPORTENABLE, &m_dwSavedState[SAVEDSTATE_D3DRS_VIEWPORTENABLE]);
            pDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &m_dwSavedState[SAVEDSTATE_D3DSAMP_MINFILTER]);
            pDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &m_dwSavedState[SAVEDSTATE_D3DSAMP_MAGFILTER]);
            pDevice->GetSamplerState(0, D3DSAMP_ADDRESSU, &m_dwSavedState[SAVEDSTATE_D3DSAMP_ADDRESSU]);
            pDevice->GetSamplerState(0, D3DSAMP_ADDRESSV, &m_dwSavedState[SAVEDSTATE_D3DSAMP_ADDRESSV]);
        }

        // Set render state (EXACTLY LIKE ORIGINAL!)
        pDevice->SetTexture(0, m_pFontTexture);
        pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
        pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
        pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        pDevice->SetRenderState(D3DRS_VIEWPORTENABLE, FALSE);
        pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        pDevice->SetVertexDeclaration(g_pFontVertexDecl);
        pDevice->SetVertexShader(g_pFontVertexShader);
        pDevice->SetPixelShader(g_pFontPixelShader);

        // Set texture scaling factor (LIKE ORIGINAL!)
        D3DSURFACE_DESC TextureDesc;
        m_pFontTexture->GetLevelDesc(0, &TextureDesc);
        FLOAT vTexScale[4];
        vTexScale[0] = 1.0f / TextureDesc.Width;
        vTexScale[1] = 1.0f / TextureDesc.Height;
        vTexScale[2] = 0.0f;
        vTexScale[3] = 0.0f;
        pDevice->SetVertexShaderConstantF(2, vTexScale, 1);
    }

    m_dwNestedBeginCount++;
}

//--------------------------------------------------------------------------------------
// Name: DrawText()
// Desc: Draws text using current cursor position
//--------------------------------------------------------------------------------------
VOID DebugFont::DrawText(DWORD dwColor, const WCHAR* strText, DWORD dwFlags, FLOAT fMaxPixelWidth)
{
    DrawText(m_fCursorX, m_fCursorY, dwColor, strText, dwFlags, fMaxPixelWidth);
}

//--------------------------------------------------------------------------------------
// Name: DrawText()
// Desc: Draws text at specified position - FIXED NO NAN BUG!
//--------------------------------------------------------------------------------------
VOID DebugFont::DrawText(FLOAT fOriginX, FLOAT fOriginY, DWORD dwColor,
    const WCHAR* strText, DWORD dwFlags, FLOAT fMaxPixelWidth)
{
    if (!strText || !*strText)
        return;

    // Set up for drawing
    Begin();

    // Set the color as a vertex shader constant
    FLOAT vColor[4];
    vColor[0] = ((dwColor & 0x00ff0000) >> 16L) / 255.0f;
    vColor[1] = ((dwColor & 0x0000ff00) >> 8L) / 255.0f;
    vColor[2] = ((dwColor & 0x000000ff) >> 0L) / 255.0f;
    vColor[3] = ((dwColor & 0xff000000) >> 24L) / 255.0f;
    pDevice->SetVertexShaderConstantF(1, vColor, 1);

    // Set the starting screen position
    if ((fOriginX < 0.0f) || ((dwFlags & ATGFONT_RIGHT) && (fOriginX <= 0.0f)))
        fOriginX += (m_rcWindow.x2 - m_rcWindow.x1);
    if (fOriginY < 0.0f)
        fOriginY += (m_rcWindow.y2 - m_rcWindow.y1);

    m_fCursorX = floorf(fOriginX);
    m_fCursorY = floorf(fOriginY);

    // Adjust for padding
    fOriginY -= m_fFontTopPadding;

    // Calculate ellipses width (using '.' character)
    FLOAT fEllipsesPixelWidth = 0.0f;
    if (L'.' <= m_cMaxGlyph && m_TranslatorTable[L'.'] != 0)
    {
        GLYPH_ATTR* pDotGlyph = &m_Glyphs[m_TranslatorTable[L'.']];
        fEllipsesPixelWidth = m_fXScaleFactor * 3.0f * (pDotGlyph->wOffset + pDotGlyph->wAdvance);
    }

    if (dwFlags & ATGFONT_TRUNCATED)
    {
        if (fMaxPixelWidth <= 0.0f)
            dwFlags &= (~ATGFONT_TRUNCATED);
        else
        {
            FLOAT w, h;
            GetTextExtent(strText, &w, &h, TRUE);
            if (w <= fMaxPixelWidth)
                dwFlags &= (~ATGFONT_TRUNCATED);
        }
    }

    // If vertically centered, offset the starting m_fCursorY value
    if (dwFlags & ATGFONT_CENTER_Y)
    {
        FLOAT w, h;
        GetTextExtent(strText, &w, &h);
        m_fCursorY = floorf(m_fCursorY - h / 2);
    }

    // Add window offsets
    fOriginX += m_rcWindow.x1;
    fOriginY += m_rcWindow.y1;
    m_fCursorX += m_rcWindow.x1;
    m_fCursorY += m_rcWindow.y1;

    // Set a flag so we can determine initial justification effects
    BOOL bStartingNewLine = TRUE;
    DWORD dwNumEllipsesToDraw = 0;

    // Begin drawing the vertices
    FLOAT* v;
    DWORD dwNumChars = wcslen(strText) + ((dwFlags & ATGFONT_TRUNCATED) ? 3 : 0);
    pDevice->BeginVertices(D3DPT_QUADLIST, 4 * dwNumChars, sizeof(FLOAT) * 5, (VOID**)&v);

    bStartingNewLine = TRUE;

    // Draw four vertices for each glyph
    while (*strText)
    {
        WCHAR letter;

        if (dwNumEllipsesToDraw)
            letter = L'.';
        else
        {
            // If starting text on a new line, determine justification effects
            if (bStartingNewLine)
            {
                if (dwFlags & (ATGFONT_RIGHT | ATGFONT_CENTER_X))
                {
                    // Get the extent of this line
                    FLOAT w, h;
                    GetTextExtent(strText, &w, &h, TRUE);

                    // Offset this line's starting m_fCursorX value
                    if (dwFlags & ATGFONT_RIGHT)
                        m_fCursorX = floorf(fOriginX - w);
                    if (dwFlags & ATGFONT_CENTER_X)
                        m_fCursorX = floorf(fOriginX - w / 2);
                }
                bStartingNewLine = FALSE;
            }

            // Get the current letter in the string
            letter = *strText++;

            // Handle the newline character
            if (letter == L'\n')
            {
                m_fCursorX = fOriginX;
                m_fCursorY += m_fFontYAdvance * m_fYScaleFactor;
                bStartingNewLine = TRUE;
                continue;
            }

            // Handle carriage return characters
            if (letter == L'\r')
                continue;
        }

        // Translate unprintable characters
        GLYPH_ATTR* pGlyph;
        if (letter > m_cMaxGlyph || m_TranslatorTable[letter] == 0)
            pGlyph = &m_Glyphs[0];
        else
            pGlyph = &m_Glyphs[m_TranslatorTable[letter]];

        FLOAT fOffset = m_fXScaleFactor * (FLOAT)pGlyph->wOffset;
        FLOAT fAdvance = m_fXScaleFactor * (FLOAT)pGlyph->wAdvance;
        FLOAT fWidth = m_fXScaleFactor * (FLOAT)pGlyph->wWidth;
        FLOAT fHeight = m_fYScaleFactor * m_fFontHeight;

        if (0 == dwNumEllipsesToDraw)
        {
            if (dwFlags & ATGFONT_TRUNCATED)
            {
                // Check if we will be exceeded the max allowed width
                if (m_fCursorX + fOffset + fWidth + fEllipsesPixelWidth + m_fSlantFactor >
                    fOriginX + fMaxPixelWidth)
                {
                    // Yup, draw the three ellipses dots instead
                    dwNumEllipsesToDraw = 3;
                    continue;
                }
            }
        }

        // Setup the screen coordinates
        m_fCursorX += fOffset;
        FLOAT left1 = m_fCursorX;
        FLOAT left2 = left1 + m_fSlantFactor;
        FLOAT right1 = left1 + fWidth;
        FLOAT right2 = left2 + fWidth;
        FLOAT top = m_fCursorY;
        FLOAT bottom = top + fHeight;
        m_fCursorX += fAdvance;

        // FIXED: Channel selector without NAN bug!
        // The shader expects a float4 where each channel is 0.0 or 1.0
        // But D3DDECLTYPE_D3DCOLOR packs 4 bytes into a DWORD
        // We need to create a valid float (not NAN) that when unpacked gives correct values

        // Create ARGB bytes (0x00 for off, 0xFF for on)
        BYTE a = (pGlyph->wMask & 0xf000) ? 0xFF : 0x00;
        BYTE r = (pGlyph->wMask & 0x0f00) ? 0xFF : 0x00;
        BYTE g = (pGlyph->wMask & 0x00f0) ? 0xFF : 0x00;
        BYTE b = (pGlyph->wMask & 0x000f) ? 0xFF : 0x00;

        // Pack into DWORD: ARGB format
        DWORD dwChannelSelector = (a << 24) | (r << 16) | (g << 8) | b;

        // FIX FOR XBOX 360: 0xFFFFFFFF is NAN! Use a different valid float
        // when all channels are on
        if (dwChannelSelector == 0xFFFFFFFF)
        {
            // Use 0x3F800000 which is 1.0f (perfectly valid float)
            // When unpacked as D3DCOLOR, this gives different bytes but 
            // the shader just checks if dot(ChannelSelector, 1) > 0
            // Any non-zero value works!
            dwChannelSelector = 0x3F800000; // 1.0f
        }

        FLOAT fChannelSelector = *(FLOAT*)&dwChannelSelector;

        // Add the vertices to draw this glyph
        *v++ = left2, *v++ = top, *v++ = pGlyph->tu1, *v++ = pGlyph->tv1, *v++ = fChannelSelector;
        *v++ = right2, *v++ = top, *v++ = pGlyph->tu2, *v++ = pGlyph->tv1, *v++ = fChannelSelector;
        *v++ = right1, *v++ = bottom, *v++ = pGlyph->tu2, *v++ = pGlyph->tv2, *v++ = fChannelSelector;
        *v++ = left1, *v++ = bottom, *v++ = pGlyph->tu1, *v++ = pGlyph->tv2, *v++ = fChannelSelector;

        // If drawing ellipses, exit when they're all drawn
        if (dwNumEllipsesToDraw)
        {
            if (--dwNumEllipsesToDraw == 0)
                break;
        }

        dwNumChars--;
    }

    // Since we allocated vertex data space based on the string length, we now need to
    // add some dummy verts for any skipped characters (like newlines, etc.)
    while (dwNumChars)
    {
        *v++ = 0, *v++ = 0, *v++ = 0, *v++ = 0, *v++ = 0;
        *v++ = 0, *v++ = 0, *v++ = 0, *v++ = 0, *v++ = 0;
        *v++ = 0, *v++ = 0, *v++ = 0, *v++ = 0, *v++ = 0;
        *v++ = 0, *v++ = 0, *v++ = 0, *v++ = 0, *v++ = 0;
        dwNumChars--;
    }

    // Stop drawing vertices
    pDevice->EndVertices();

    // Undo window offsets
    m_fCursorX -= m_rcWindow.x1;
    m_fCursorY -= m_rcWindow.y1;

    // Complete the begin/end pair
    End();
}

//--------------------------------------------------------------------------------------
// Name: End()
// Desc: Restores state after rendering
//--------------------------------------------------------------------------------------
VOID DebugFont::End()
{
    assert(m_dwNestedBeginCount > 0);
    if (--m_dwNestedBeginCount > 0)
        return;

    // Restore state
    if (m_bSaveState)
    {
        pDevice->SetTexture(0, NULL);
        pDevice->SetVertexDeclaration(NULL);
        pDevice->SetVertexShader(NULL);
        pDevice->SetPixelShader(NULL);
        pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, m_dwSavedState[SAVEDSTATE_D3DRS_ALPHABLENDENABLE]);
        pDevice->SetRenderState(D3DRS_SRCBLEND, m_dwSavedState[SAVEDSTATE_D3DRS_SRCBLEND]);
        pDevice->SetRenderState(D3DRS_DESTBLEND, m_dwSavedState[SAVEDSTATE_D3DRS_DESTBLEND]);
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, m_dwSavedState[SAVEDSTATE_D3DRS_ALPHATESTENABLE]);
        pDevice->SetRenderState(D3DRS_ALPHAREF, m_dwSavedState[SAVEDSTATE_D3DRS_ALPHAREF]);
        pDevice->SetRenderState(D3DRS_ALPHAFUNC, m_dwSavedState[SAVEDSTATE_D3DRS_ALPHAFUNC]);
        pDevice->SetRenderState(D3DRS_FILLMODE, m_dwSavedState[SAVEDSTATE_D3DRS_FILLMODE]);
        pDevice->SetRenderState(D3DRS_CULLMODE, m_dwSavedState[SAVEDSTATE_D3DRS_CULLMODE]);
        pDevice->SetRenderState(D3DRS_ZENABLE, m_dwSavedState[SAVEDSTATE_D3DRS_ZENABLE]);
        pDevice->SetRenderState(D3DRS_STENCILENABLE, m_dwSavedState[SAVEDSTATE_D3DRS_STENCILENABLE]);
        pDevice->SetRenderState(D3DRS_VIEWPORTENABLE, m_dwSavedState[SAVEDSTATE_D3DRS_VIEWPORTENABLE]);
        pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, m_dwSavedState[SAVEDSTATE_D3DSAMP_MINFILTER]);
        pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, m_dwSavedState[SAVEDSTATE_D3DSAMP_MAGFILTER]);
        pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, m_dwSavedState[SAVEDSTATE_D3DSAMP_ADDRESSU]);
        pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, m_dwSavedState[SAVEDSTATE_D3DSAMP_ADDRESSV]);
    }
}