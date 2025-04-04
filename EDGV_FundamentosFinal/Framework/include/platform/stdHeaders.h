#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <random>


using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

using Byte = U8;
using SIZE = std::size_t;


using String = std::string;
using WString = std::wstring;
template<typename T, typename A = std::allocator<T>>
using Vector = std::vector<T, A>;
template<typename T, typename U>
using Map = std::map<T, U>;
template<typename T, typename U>
using Pair = std::pair<T, U>;

template<typename T>
using UPtr = std::unique_ptr<T>;
template<typename T>
using SPtr = std::shared_ptr<T>;
template<typename T>
using WPtr = std::weak_ptr<T>;
/**
 * @brief For parenting on smart pointer use. Enable getting a SPtr from 'this'.
 */
template<typename T>
using EnableSPtrFromThis = std::enable_shared_from_this<T>;

using FStream = std::fstream;
using IFStream = std::ifstream;
using OFStream = std::ofstream;

using Path = std::filesystem::path;