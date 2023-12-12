#ifndef WIN32WINDOWCONTEXT_P_H
#define WIN32WINDOWCONTEXT_P_H

#include <QWKCore/qwindowkit_windows.h>
#include <QWKCore/private/abstractwindowcontext_p.h>

namespace QWK {

    class Win10BorderHandler;

    class QWK_CORE_EXPORT Win32WindowContext : public AbstractWindowContext {
        Q_OBJECT
    public:
        Win32WindowContext();
        ~Win32WindowContext() override;

        enum WindowPart {
            Outside,
            ClientArea,
            ChromeButton,
            ResizeBorder,
            FixedBorder,
            TitleBar,
        };

        QString key() const override;
        void virtual_hook(int id, void *data) override;

        Q_INVOKABLE bool needWin10BorderHandler() const;
        Q_INVOKABLE void setWin10BorderHandler(Win10BorderHandler *handler);

    protected:
        bool setupHost() override;

    public:
        bool windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT *result);

        bool systemMenuHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,
                               LRESULT *result);

        // In order to perfectly apply Windows 11 Snap Layout into the Qt window, we need to
        // intercept and simulate most of the  mouse events, so that the processing logic
        // is quite complex. Simultaneously, in order to make the handling code of other
        // Windows messages clearer, we have separated them into this function.
        bool snapLayoutHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,
                               LRESULT *result);

        bool customWindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,
                                 LRESULT *result);

        bool nonClientCalcSizeHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,
                                      LRESULT *result);

        bool themeStuffHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,
                               LRESULT *resul);

    protected:
        WId windowId = 0;

        // The last hit test result, helpful to handle WM_MOUSEMOVE and WM_NCMOUSELEAVE.
        WindowPart lastHitTestResult = WindowPart::Outside;

        // Whether the last mouse leave message is blocked, mainly for handling the unexpected
        // WM_MOUSELEAVE.
        bool mouseLeaveBlocked = false;

        bool centered = false;

        std::unique_ptr<Win10BorderHandler> win10BorderHandler;
    };

}

#endif // WIN32WINDOWCONTEXT_P_H
