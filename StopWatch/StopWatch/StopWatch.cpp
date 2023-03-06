#include <windows.h>

// Định nghĩa hằng số ID cho các phần tử trong giao diện người dùng
#define ID_TIMER 1
#define ID_START_BUTTON 2
#define ID_STOP_BUTTON 3
#define ID_RESET_BUTTON 4

// Khai báo biến toàn cục
int seconds = 0, minutes = 0, hours = 0;

// Hàm xử lý sự kiện bấm nút Start
void OnStart(HWND hWnd) {
    SetTimer(hWnd, ID_TIMER, 1000, NULL);
}

// Hàm xử lý sự kiện bấm nút Stop
void OnStop(HWND hWnd) {
    KillTimer(hWnd, ID_TIMER);
}

// Hàm xử lý sự kiện bấm nút Reset
void OnReset(HWND hWnd) {
    seconds = 0;
    minutes = 0;
    hours = 0;
    InvalidateRect(hWnd, NULL, TRUE);
}

// Hàm xử lý sự kiện WM_PAINT để vẽ giao diện người dùng
void OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    TCHAR buffer[50];
    wsprintf(buffer, TEXT("%02d:%02d:%02d"), hours, minutes, seconds);
    TextOut(hdc, 50, 50, buffer, lstrlen(buffer));

    EndPaint(hWnd, &ps);
}

// Hàm xử lý sự kiện WM_TIMER để cập nhật thời gian
void OnTimer(HWND hWnd) {
    seconds++;
    if (seconds == 60) {
        seconds = 0;
        minutes++;
        if (minutes == 60) {
            minutes = 0;
            hours++;
        }
    }
    InvalidateRect(hWnd, NULL, TRUE);
}

// Hàm xử lý sự kiện WM_CREATE để tạo các phần tử trong giao diện người dùng
void OnCreate(HWND hWnd) {
    CreateWindow(TEXT("button"), TEXT("Start"), WS_VISIBLE | WS_CHILD, 10, 100, 100, 30, hWnd, (HMENU)ID_START_BUTTON, NULL, NULL);
    CreateWindow(TEXT("button"), TEXT("Stop"), WS_VISIBLE | WS_CHILD, 120, 100, 100, 30, hWnd, (HMENU)ID_STOP_BUTTON, NULL, NULL);
    CreateWindow(TEXT("button"), TEXT("Reset"), WS_VISIBLE | WS_CHILD, 230, 100, 100, 30, hWnd, (HMENU)ID_RESET_BUTTON, NULL, NULL);
}

// Hàm xử lý sự kiện WM_COMMAND để xử lý các sự kiện bấm nút trong giao diện người dùng
void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    switch (LOWORD(wParam)) {
    case ID_START_BUTTON:
        OnStart(hWnd);
        break;
    case ID_STOP_BUTTON:
        OnStop(hWnd);
        break;
    case ID_RESET_BUTTON:
        OnReset(hWnd);
        break;
    }
 

}

// Hàm xử lý sự kiện WM_DESTROY để kết thúc chương trình
void OnDestroy(HWND hWnd) {
    PostQuitMessage(0);
}

// Hàm xử lý sự kiện của cửa sổ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        OnCreate(hWnd);
        break;
    case WM_PAINT:
        OnPaint(hWnd);
        break;
    case WM_TIMER:
        OnTimer(hWnd);
        break;
    case WM_COMMAND:
        OnCommand(hWnd, wParam, lParam);
        break;
    case WM_DESTROY:
        OnDestroy(hWnd);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Hàm main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Đăng ký lớp cửa sổ
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("StopWatchWindowClass");
    RegisterClass(&wc);
    HWND hWnd = CreateWindow(wc.lpszClassName, TEXT("Stop Watch"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    // Vòng lặp thông điệp
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
