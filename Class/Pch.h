#pragma once
//=============================================
// プリコンパイル済みヘッダー
// ここに書いたものは初回のみ解析されるため、コンパイル時間が高速になる
// 全てのcppからインクルードされる必要がある
//=============================================

//=============================================
// 基本
//=============================================
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <Windows.h>


#include <wrl/client.h>

//=============================================
// STL
//=============================================
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <sstream>
#include <functional>
#include <iostream>
#include <cassert>
#include <utility>
#include <chrono>
#include <optional>
#include <regex>
#include <ranges>
#include <numbers>
#include <filesystem>
#include <future>
#include <atomic>
#include <thread>
#include <mutex>

#define _USE_MATH_DEFINES
#include <cmath>

//============================================
// Direct3X12
//============================================
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

// DirectX Tool Kit
#pragma comment(lib,"DirectXTK12.lib")
#include <SimpleMath.h>

#pragma comment(lib,"DirectXTex.lib")
#include <DirectXTex.h>


#include "System/System.h"


