#include "DebugUI.h"
#include <xtl.h>
#include <Core2/Debug.h>
#include <Sox/Input/Manager.h>

namespace Patch
{
    namespace DebugUI
    {
        DebugFont* Font = 0;

        void RenderDebugUI(Sonicteam::SoX::ApplicationXenon* app, LPDIRECT3DDEVICE9 pDevice, double delta)
        {
            Sonicteam::DocMarathonImp* pDoc = (Sonicteam::DocMarathonImp*)(app->AppDoc);
            Sonicteam::SoX::Input::Manager* pManager = pDoc->m_vspInputManager[0].get();

            if (delta <= 0.0)
            {
                delta = 1.0 / 60.0;
            }

            static float ScrollOffset = 0.0f;
            static float ScrollVelocity = 0.0f;
            static bool ScrollUser = false;
            static DebugFont* Font = NULL;
            static float AutoScrollTimer = 0.0f;
            static bool AutoScrollEnabled = true; // Auto-scroll by default
            static float LastContentHeight = 0.0f;

            const float lineHeight = 20.0f;
            const float maxScrollSpeed = 500.0f; // Максимальная скорость скролла
            const float acceleration = 800.0f;   // Ускорение при удерживании
            const float friction = 5.0f;         // Трение при отпускании
            const float autoScrollDelay = 3.0f;  // Seconds before auto-scroll activates
            const float autoScrollSpeed = 100.0f; // Auto-scroll speed

            // Initialize font if needed
            if (!Font)
            {
                Font = new DebugFont();
                Font->pDevice = pDevice;

                HRESULT hr = Font->Create(
                    "game:\\media\\FontShader.hlsl",
                    "game:\\media\\font_atlas.fnt",
                    "game:\\media\\font_atlas_0.dds"
                );

                if (FAILED(hr))
                {
                    delete Font;
                    Font = NULL;
                    return;
                }

                Font->SetScaleFactors(1.0f, 1.0f);
            }

            // Get viewport dimensions
            D3DVIEWPORT9 vp;
            pDevice->GetViewport(&vp);
            int viewportTop = vp.Y;
            int viewportBottom = vp.Y + vp.Height;
            int viewportLeft = vp.X;
            int viewportRight = vp.X + vp.Width;

            // Set font window to entire screen
            Font->SetWindow(viewportLeft, viewportTop, viewportRight, viewportBottom);
            Font->Begin();

            if (Debug::ms_EnableDebugUI_LOG)
            {
                // Handle input for scrolling
                float stickInput = pManager->m_PadState.RightStickVertical.get();

                // Check if user is providing input
                bool userInput = false;

                if (pManager->m_PadState.IsPressed(Sonicteam::SoX::Input::KeyState_DpadDown))
                {
                    // Reset scroll
                    ScrollOffset = 0.0f;
                    ScrollVelocity = 0.0f;
                    ScrollUser = false;
                    AutoScrollEnabled = false; // User took control, disable auto-scroll
                    AutoScrollTimer = 0.0f; // Reset auto-scroll timer
                }
                else if (fabs(stickInput) > 0.1f) // Dead zone
                {
                    // User is controlling scroll
                    userInput = true;
                    ScrollUser = true;
                    AutoScrollEnabled = false; // User is manually scrolling
                    AutoScrollTimer = 0.0f; // Reset auto-scroll timer

                    // Apply acceleration based on stick input
                    float targetVelocity = -stickInput * maxScrollSpeed; // Negative because down = scroll down

                    // Smoothly approach target velocity
                    if (fabs(ScrollVelocity - targetVelocity) > 10.0f)
                    {
                        float direction = (targetVelocity > ScrollVelocity) ? 1.0f : -1.0f;
                        ScrollVelocity += direction * acceleration * (float)delta;

                        // Clamp to target velocity
                        if ((direction > 0 && ScrollVelocity > targetVelocity) ||
                            (direction < 0 && ScrollVelocity < targetVelocity))
                        {
                            ScrollVelocity = targetVelocity;
                        }
                    }
                    else
                    {
                        ScrollVelocity = targetVelocity;
                    }
                }

                // If user stopped input, apply friction
                if (!userInput && ScrollUser)
                {
                    // No user input - apply friction
                    if (fabs(ScrollVelocity) > 1.0f)
                    {
                        ScrollVelocity -= ScrollVelocity * friction * (float)delta;
                    }
                    else
                    {
                        ScrollVelocity = 0.0f;
                        ScrollUser = false;
                    }
                }

                // Handle auto-scroll
                float maxScroll = std::max(0.0f, (float)(Debug::ms_Messages.size() * lineHeight - (viewportBottom - viewportTop))) + 100;

                if (!userInput && !ScrollUser && AutoScrollEnabled)
                {
                    // Check if we need to auto-scroll
                    if (Debug::ms_Messages.size() > 0 && maxScroll > 0.0f)
                    {
                        // Check if new content was added
                        float currentContentHeight = (float)(Debug::ms_Messages.size() * lineHeight);
                        bool newContentAdded = (currentContentHeight > LastContentHeight);
                        LastContentHeight = currentContentHeight;

                        if (newContentAdded)
                        {
                            // New content was added, scroll to show it
                            ScrollOffset = maxScroll;
                            ScrollVelocity = 0.0f;
                        }
                        else
                        {
                            // No new content, check if we're at the bottom
                            if (ScrollOffset < maxScroll - 1.0f)
                            {
                                // Not at bottom, start auto-scroll timer
                                AutoScrollTimer += (float)delta;

                                if (AutoScrollTimer >= autoScrollDelay)
                                {
                                    // Auto-scroll down to bottom
                                    ScrollVelocity = autoScrollSpeed;
                                }
                            }
                            else
                            {
                                // At bottom, reset timer
                                AutoScrollTimer = 0.0f;
                                ScrollVelocity = 0.0f;
                            }
                        }
                    }
                }
                else if (userInput || ScrollUser)
                {
                    // User is controlling, reset auto-scroll timer
                    AutoScrollTimer = 0.0f;

                    // Check if user scrolled to bottom
                    if (ScrollOffset >= maxScroll - 1.0f)
                    {
                        // User scrolled to bottom, re-enable auto-scroll
                        AutoScrollEnabled = true;
                    }
                }

                // Apply velocity to scroll offset
                ScrollOffset += ScrollVelocity * (float)delta;

                // Clamp scroll to prevent going too far
                if (ScrollOffset > maxScroll) ScrollOffset = maxScroll;
                if (ScrollOffset < 0.0f) ScrollOffset = 0.0f;

                // Calculate starting Y position with scroll offset
                float y_position = viewportTop - ScrollOffset; // Negative because we want to move content up when scrolling down

                // Calculate visible range
                int firstVisible = 0;
                int lastVisible = (int)Debug::ms_Messages.size() - 1;

                // Only render visible messages
                if (Debug::ms_Messages.size() > 0)
                {
                    firstVisible = std::max(0, (int)(-y_position / lineHeight));
                    lastVisible = std::min((int)Debug::ms_Messages.size() - 1,
                        firstVisible + (int)((viewportBottom - viewportTop) / lineHeight) + 1);

                    // Make sure we don't go out of bounds
                    firstVisible = std::max(0, firstVisible - 1); // Render one extra above
                    lastVisible = std::min((int)Debug::ms_Messages.size() - 1, lastVisible + 1); // Render one extra below
                }

                // Render visible messages
                for (int i = firstVisible; i <= lastVisible && i < (int)Debug::ms_Messages.size(); ++i)
                {
                    CMessage& msg = Debug::ms_Messages[i];
                    const float currentY = y_position + (i * lineHeight);

                    // Skip if outside viewport
                    if (currentY + lineHeight < viewportTop || currentY > viewportBottom)
                        continue;

                    // Update animation
                    msg.Lerp(static_cast<float>(delta));

                    // Convert string to wide string
                    std::wstring wmsg;
                    if (!msg.Data.empty())
                    {
                        // Simple ASCII conversion for Xbox (no UTF-8 needed for debug text)
                        wmsg.assign(msg.Data.begin(), msg.Data.end());
                    }

                    // Append count if needed
                    wchar_t finalMsg[1024];
                    if (msg.Count > 0)
                    {
                        wsprintfW(finalMsg, L"%s | %d", wmsg.c_str(), msg.Count);
                    }
                    else
                    {
                        wcsncpy(finalMsg, wmsg.c_str(), min(wmsg.length(), (size_t)1023));
                        finalMsg[min(wmsg.length(), (size_t)1023)] = L'\0';
                    }

                    // Draw the text
                    Font->DrawText(5.0f, currentY, msg.CurrentColor, finalMsg, ATGFONT_LEFT);
                }

                // Draw scroll indicators and info
                if (maxScroll > 0.0f)
                {
                    if (ScrollOffset > 0.0f)
                    {
                        Font->DrawText(viewportRight - 100.0f, viewportTop + 10.0f,
                            D3DCOLOR_ARGB(255, 255, 255, 255),
                            L"↑", ATGFONT_LEFT);
                    }

                    if (ScrollOffset < maxScroll - 1.0f)
                    {
                        Font->DrawText(viewportRight - 100.0f, viewportBottom - 30.0f,
                            D3DCOLOR_ARGB(255, 255, 255, 255),
                            L"↓", ATGFONT_LEFT);
                    }

                    // Draw scroll info with auto-scroll status
                    wchar_t scrollInfo[64];
                    if (AutoScrollEnabled && !userInput && !ScrollUser)
                    {
                        wsprintfW(scrollInfo, L"Auto: %.1fs", autoScrollDelay - AutoScrollTimer);
                    }
                    else
                    {
                        wsprintfW(scrollInfo, L"Scroll: %.0f/%.0f", ScrollOffset, maxScroll);
                    }

                    Font->DrawText(viewportRight - 200.0f, viewportTop + 10.0f,
                        AutoScrollEnabled ? D3DCOLOR_ARGB(255, 0, 255, 0) : D3DCOLOR_ARGB(255, 255, 255, 0),
                        scrollInfo, ATGFONT_LEFT);
                }
            }

            // Draw Meta Inforomation
            wchar_t _out_message[256];
            memset(_out_message, 0, sizeof(_out_message));
            MEMORYSTATUS statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatus(&statex);
            wsprintfW(_out_message, L"FPS: %03d, PhysMem: %d\\%d, VirtualMem: %d\\%d,  PageMem: %d\\%d \1\2\3\4\5\6\7",
                (int)(1.0 / delta),
                statex.dwAvailPhys / 1024, statex.dwTotalPhys / 1024,
                statex.dwAvailVirtual / 1024, statex.dwTotalVirtual / 1024,
                statex.dwAvailPageFile / 1024, statex.dwTotalPageFile / 1024);
            Font->DrawText(5.0f, 10.0f, D3DCOLOR_ARGB(255, 0, 255, 0), _out_message, ATGFONT_LEFT);

            Font->End();

            // Custom Drawings

            for (std::vector<void(*)()>::iterator it = Debug::ms_DebugUIDrawFunctions.begin(); it != Debug::ms_DebugUIDrawFunctions.end(); it++)
            {
                (*it)();
            }
        }

        HOOKV3(0x82523AF8, int, D3DRenderFrameAndSwapBuffersDebug,
            (Sonicteam::SoX::ApplicationXenon*), (app), Sonicteam::SoX::ApplicationXenon* app)
        {
            Sonicteam::DocMarathonImp* pDoc = (Sonicteam::DocMarathonImp*)(app->AppDoc);
            D3DDevice* pDevice = app->_D3DDevice;
            RenderDebugUI(app,pDevice, *(float*)&pDoc->Docuint0xF0);
            return 0;
        }

        void INSTALL()
        {
            INSTALL_HOOKV3EX(D3DRenderFrameAndSwapBuffersDebug, -1, false, 9);
        }

    }
}